#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/readers/PGMReader.h>

#include <util/CompositeDigitalObject.hpp>
#include <util/DigitalComponent.hpp>

#include <fstream>
#include <filesystem>
#include <tuple>
#include <algorithm>

// Topology
// TD3: we choose (4, 8) for an adjacency pair for now.
// see report for details.
typedef DGtal::Z2i::DT4_8     DigitalTopology;
static constexpr int dimension = 2;

// Utility Classes.
typedef td::util::DigitalComponent<dimension, DigitalTopology>       Component;
typedef td::util::CompositeDigitalObject<dimension, DigitalTopology> CompositeObject;
typedef typename CompositeObject::Image                              Image;
template <typename T>
using Maths = td::util::maths<T>;

typedef typename Component::Area        Area;
typedef typename Component::Perimeter   Perimeter;
typedef typename Component::FloatScalar FloatScalar;
typedef typename Component::AngleRadian AngleRadian;
typedef typename Component::Colour Colour;

typedef typename Component::Point Point;
typedef typename Component::Vector Vector;
typedef typename CompositeObject::RealPoint RealPoint;
typedef typename Component::Matrix  Matrix;

static constexpr char const * outputDirName = "res/td3/";
static constexpr char const * inputDirName  = "assets/td3/binary";

int
  main(int argc, char ** argv)
{
    if (argc < 2)
    {
        std::cout << "usage: programme_name [path_to_image] *([path_to_images] )" << std::endl;
        return 0;
    }
    setlocale(LC_NUMERIC, "us_US");  // To prevent French local settings

    // paths of files
    std::filesystem::path const outputPath = std::filesystem::current_path().parent_path().append(outputDirName);
    std::filesystem::path const inputPath  = std::filesystem::current_path().parent_path().append(inputDirName);

    auto size = static_cast<std::size_t>(argc);

    // all objects
    std::vector<std::vector<CompositeObject>> matCompositeObjects;
    matCompositeObjects.reserve(size);

    /// ---------------- STEP 2, 3 ------------------------ //
    // IMPORTANT
    // Assumes that:
    // - the file containing the coordinates of the POI does not have an extension.
    // - the image file and the file containing the coordinates have the same name.
    // May reorder images depending on iteration order.
    for (int i = 1; i < argc; ++i)
    {
        matCompositeObjects.emplace_back();
        std::filesystem::path directory  = inputPath / argv[i];
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                std::filesystem::path const & path = entry.path();
                // only open image files (file with an extension.)
                if (path.has_extension())
                {
                    Image image = DGtal::PGMReader<Image>::importPGM(entry.path().c_str());
                    matCompositeObjects.back().emplace_back(std::move(image));

                    // look for the corresponding position: same file name, without extension.
                    std::filesystem::path pointPath = path.parent_path() / path.stem();
                    // add interest point.
                    std::fstream fs;
                    fs.open(pointPath.c_str(), std::fstream::in);
                    int x, y;
                    fs >> x; fs >> y;
                    fs.close();
                    // remember this interest point.
                    matCompositeObjects.back().back().setInterestPoint(Point(x, y));
                }
            }
        }

    }

    for (auto &  compositeObjects : matCompositeObjects)
    {
        for (auto & composite : compositeObjects)
        {
            // TD3: we know there is only one object per image,
            // we can then discard all be the the largest one.
            // for this, we can compute the area by the count method.
            composite.cullAllButLargestComponent();
        }
    }

    // saving before transformations
    {
        int i = 0;
        for (auto const & compositeObjects : matCompositeObjects)
        {
            int j = 0;
            for (auto const & composite : compositeObjects)
            {
                DGtal::Board2D board;
                composite.drawObjectComponents(board, Colour::Gray, Colour::Yellow);

                board.saveEPS(
                  std::filesystem::path(outputPath)
                    .append(std::string(argv[i + 1]) + "_" + std::to_string(j + 1) + ".eps")
                    .c_str()
                  );
                ++j;
            }
            ++i;
        }
    }

    //
    for (std::size_t i = 0; i < matCompositeObjects.size(); ++i)
    {
        std::cout << "Object " << i << std::endl;
        for (std::size_t j = 0; j < matCompositeObjects.at(i).size() - 1; ++j)
        {
            CompositeObject & firstObject = matCompositeObjects.at(i).at(j);
            CompositeObject & secondObject = matCompositeObjects.at(i).at(j + 1);

            AngleRadian angle;
            Vector translation;
            {
                Component const & firstComponent = firstObject.components.front();
                Component const & secondComponent = secondObject.components.front();
                Point firstPoint = firstObject.getInterestPoint().value();
                Point secondPoint = secondObject.getInterestPoint().value();
                Point firstPointFromCentre = firstComponent.getPositionFromCentre(firstPoint);
                Point secondPointFromCentre = secondComponent.getPositionFromCentre(secondPoint);

                std::cout << "Points: " << firstPoint << ", " << secondPoint << std::endl;
                std::cout << "Centres: " << firstComponent.getGeometricCentre() << ", "
                          << secondComponent.getGeometricCentre() << std::endl;

                angle = Component::computeRotationAngle(
                  { firstPointFromCentre },
                  { secondPointFromCentre });
                translation = firstComponent.getTranslationTo(secondComponent);

                // Now transform the second object to align it with the first.
                secondObject.transformRigidBackward(
                  firstComponent.getGeometricCentre(),
                  - angle, - translation
                 );
                // We should cull the components again,
                // because the rigid transformation may have introduced artifacts.
                secondObject.cullAllButLargestComponent();
            }

            std::cout << "Rotation angle: " << angle * (180. / M_PI) << "°" << std::endl;
            std::cout << "Translation:    " << translation << std::endl;

            Component const & firstComponent = firstObject.components.front();
            Component const & secondComponent = secondObject.components.front();

            std::cout << "Hausdorff distance:           " << secondComponent.computeHausdorffDistance(firstComponent) << std::endl;
            std::cout << "Dubuisson-Jain dissimilarity: " << secondComponent.computeDubuissonJainDissimilarity(firstComponent) << std::endl;

            DGtal::Board2D board;
            secondObject.drawObjectComponents(board, Colour::Black, Colour::Yellow);

            board.saveEPS(
              std::filesystem::path(outputPath)
                .append(std::string(argv[i + 1]) + "_" + std::to_string(j + 2) + "_transformed_backward.eps")
                .c_str()
                         );
        }
    }

    return 0;
}
