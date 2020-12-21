#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/readers/PGMReader.h>

#include <util/CompositeDigitalObject.hpp>
#include <util/DigitalComponent.hpp>

#include <filesystem>
#include <tuple>

// Topology
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

static constexpr char const * outputDirName = "res/td2/";
static constexpr char const * inputDirName  = "assets/td2/";

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
    std::vector<CompositeObject> compositeObjects;
    compositeObjects.reserve(size);

    /// ---------------- STEP 2, 3 ------------------------ //
    for (int i = 1; i < argc; ++i)
    {
        std::filesystem::path path  = inputPath / argv[i];
        Image const           image = DGtal::PGMReader<Image>::importPGM(path);

        compositeObjects.emplace_back(image);
    }

    for (auto const & composite : compositeObjects)
    {
        // Right now size of "components" is the number of connected components
        std::cout << " number of components : " << composite.components.size() << std::endl;
    }


    /// ---------------- STEP 4, 5, 6 ------------------------ //
    std::vector<std::vector<Area>>      countAreas;
    std::vector<std::vector<Perimeter>> countPerimeters;
    std::vector<std::vector<Area>>      convexHullAreas;
    std::vector<std::vector<Perimeter>> convexHullPerimeters;
    std::vector<std::vector<Area>>      segmentationAreas;
    std::vector<std::vector<Perimeter>> segmentationPerimeters;


    // The circularities are computed with the *segmentation* only.
    std::vector<std::vector<FloatScalar>> circularities;

    // first is average, second is standard deviation
    std::vector<std::pair<Area, Area>>               statCountAreas;
    std::vector<std::pair<Perimeter, Perimeter>>     statCountPerimeters;
    std::vector<std::pair<Area, Area>>               statConvexHullAreas;
    std::vector<std::pair<Perimeter, Perimeter>>     statConvexHullPerimeters;
    std::vector<std::pair<Area, Area>>               statSegmentationAreas;
    std::vector<std::pair<Perimeter, Perimeter>>     statSegmentationPerimeters;

    std::vector<std::pair<FloatScalar, FloatScalar>> statCircularities;

    countAreas.reserve(compositeObjects.size());
    countPerimeters.reserve(compositeObjects.size());
    convexHullAreas.reserve(compositeObjects.size());
    convexHullPerimeters.reserve(compositeObjects.size());
    segmentationAreas.reserve(compositeObjects.size());
    segmentationPerimeters.reserve(compositeObjects.size());

    circularities.reserve(compositeObjects.size());

    statCountAreas.reserve(compositeObjects.size());
    statCountPerimeters.reserve(compositeObjects.size());
    statConvexHullAreas.reserve(compositeObjects.size());
    statConvexHullPerimeters.reserve(compositeObjects.size());
    statSegmentationAreas.reserve(compositeObjects.size());
    statSegmentationPerimeters.reserve(compositeObjects.size());

    statCircularities.reserve(compositeObjects.size());

    for (auto const & composite : compositeObjects)
    {
        countAreas.emplace_back();
        countPerimeters.emplace_back();
        convexHullAreas.emplace_back();
        convexHullPerimeters.emplace_back();
        segmentationAreas.emplace_back();
        segmentationPerimeters.emplace_back();

        circularities.emplace_back();

        countAreas.back().reserve(composite.components.size());
        countPerimeters.back().reserve(composite.components.size());
        convexHullAreas.back().reserve(composite.components.size());
        convexHullPerimeters.back().reserve(composite.components.size());
        segmentationAreas.back().reserve(composite.components.size());
        segmentationPerimeters.back().reserve(composite.components.size());

        circularities.reserve(composite.components.size());

        for (auto const & component : composite.components)
        {
            countAreas.back().push_back(component.getCountArea());
            countPerimeters.back().push_back(component.getCountPerimeter());

            convexHullAreas.back().push_back(component.getConvexHullArea());
            convexHullPerimeters.back().push_back(component.getConvexHullPerimeter());

            segmentationAreas.back().push_back(component.getSegmentationArea());
            segmentationPerimeters.back().push_back(component.getSegmentationPerimeter());

            circularities.back().emplace_back(component.getCircularity());
        }

        statCountAreas.emplace_back(Maths<Area>::average(countAreas.back()),
                               Maths<Area>::standardDeviation(countAreas.back()));
        statCountPerimeters.emplace_back(Maths<Area>::average(countPerimeters.back()),
                                    Maths<Area>::standardDeviation(countPerimeters.back()));

        statConvexHullAreas.emplace_back(Maths<Area>::average(convexHullAreas.back()),
                               Maths<Area>::standardDeviation(convexHullAreas.back()));
        statConvexHullPerimeters.emplace_back(Maths<Area>::average(convexHullPerimeters.back()),
                                    Maths<Area>::standardDeviation(convexHullPerimeters.back()));

        statSegmentationAreas.emplace_back(Maths<Area>::average(segmentationAreas.back()),
                                         Maths<Area>::standardDeviation(segmentationAreas.back()));
        statSegmentationPerimeters.emplace_back(Maths<Area>::average(segmentationPerimeters.back()),
                                              Maths<Area>::standardDeviation(segmentationPerimeters.back()));

        statCircularities.emplace_back(Maths<Area>::average(circularities.back()),
                                    Maths<Area>::standardDeviation(circularities.back()));
    }

    ///
    std::cout << std::endl;
    std::cout << "---- STATISTICS -----" << std::endl;
    for (std::size_t i = 0; i < compositeObjects.size(); ++i)
    {
        std::cout << "-- " << argv[i+1] << " --" << std::endl;
        std::cout << "[Area (Count)]      avg: " << statCountAreas.at(i).first
                  << "  deviation: " << statCountAreas.at(i).second << std::endl;
        std::cout << "[Area (CH)]         avg: " << statConvexHullAreas.at(i).first
                  << "  deviation: " << statConvexHullAreas.at(i).second << std::endl;
        std::cout << "[Area (Seg)]         avg: " << statSegmentationAreas.at(i).first
                  << "  deviation: " << statSegmentationAreas.at(i).second << std::endl;
        std::cout << "[Perimeter (Count)] avg: " << statCountPerimeters.at(i).first
                  << "  deviation: " << statCountPerimeters.at(i).second << std::endl;
        std::cout << "[Perimeter (CH)]    avg: " << statConvexHullPerimeters.at(i).first
                  << "  deviation: " << statConvexHullPerimeters.at(i).second << std::endl;
        std::cout << "[Perimeter (Seg)]    avg: " << statSegmentationPerimeters.at(i).first
                  << "  deviation: " << statSegmentationPerimeters.at(i).second << std::endl;
    }

    ///
    std::cout << std::endl;
    std::cout << "---- MORE STATISTICS -----" << std::endl;
    for (std::size_t i = 0; i < compositeObjects.size(); ++i)
    {
        std::cout << "-- " << argv[i+1] << " --" << std::endl;
        std::cout << "[Area (Seg)]         avg: " << statSegmentationAreas.at(i).first
                  << "  deviation: " << statSegmentationAreas.at(i).second << std::endl;
        std::cout << "[Perimeter (Seg)]    avg: " << statSegmentationPerimeters.at(i).first
                  << "  deviation: " << statSegmentationPerimeters.at(i).second << std::endl;
        std::cout << "[Circularity]    avg: " << statCircularities.at(i).first
                  << "  deviation:  " << statCircularities.at(i).second << std::endl;
    }

    for (std::size_t i = 10; i < 20; ++i)
    {
        DGtal::Board2D board;
        compositeObjects.at(1).components.at(i).draw(board);

        board.saveEPS(std::filesystem::path(outputPath).append("component_" + std::to_string(i) + ".eps").c_str());
    }

    return 0;
}
