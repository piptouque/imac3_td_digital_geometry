#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/readers/PGMReader.h>

#include <util/CompositeDigitalObject.hpp>
#include <util/DigitalComponent.hpp>

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
    for (int i = 1; i < argc; ++i)
    {
        matCompositeObjects.emplace_back();
        std::filesystem::path directory  = inputPath / argv[i];
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                Image image = DGtal::PGMReader<Image>::importPGM(entry.path().c_str());
                matCompositeObjects.back().emplace_back(std::move(image));
            }
        }

    }

    for (auto &  compositeObjects : matCompositeObjects)
    {
        for (auto & composite : compositeObjects)
        {
            std::cout << " number of components : " << composite.components.size() << std::endl;
            // Right now size of "components" is the number of connected components
            // TD3: we know there is only one object per image,
            // we can then discard all be the the largest one.
            // for this, we can compute the area by the count method.
            auto const itMax = std::max_element(
              composite.components.begin(),
              composite.components.end(),
              [](Component const& c1, Component const& c2) -> bool
              {
                return c1.getCountArea() <= c2.getCountArea();
              }
                                               );
            // pretty bad.
            Component const maxComponent = std::move(*itMax);
            composite.components.clear();
            composite.components.push_back(maxComponent);
        }
    }

    {
        int i = 0;
        for (auto const & compositeObjects : matCompositeObjects)
        {
            for (auto const & composite : compositeObjects)
            {
                DGtal::Board2D board = composite.components.front().draw();

                board.saveEPS(std::filesystem::path(outputPath).append("object_" + std::to_string(i) + ".eps").c_str());
            }
        }
        ++i;

    }

    return 0;
}
