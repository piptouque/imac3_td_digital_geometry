#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageSelector.h>
#include <DGtal/images/imagesSetsUtils/SetFromImage.h>
#include <DGtal/io/Color.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/ColorBrightnessColorMap.h>
#include <DGtal/topology/SurfelAdjacency.h>
#include <DGtal/topology/helpers/Surfaces.h>
#include <DGtal/io/readers/PGMReader.h>
#include <DGtal/io/writers/GenericWriter.h>

#include <filesystem>

namespace E = DGtal::Z2i;

// Topology
typedef E::DT4_8 DigitalTopology;
// Topology object
static DigitalTopology const s_topology = E::dt4_8;
// Adjacency object.
// Interior to exterior only for adjacency pairs.
DGtal::SurfelAdjacency<E::KSpace::dimension> s_adjacency (true);

// number of tries for stochastic search of a border cell.
static int constexpr s_numberTries = 20000;

// Digital object type
typedef DGtal::Object<DigitalTopology, E::DigitalSet>        Object;
// type of image
typedef DGtal::ImageSelector<E::Domain, unsigned char>::Type Image;

static constexpr char const * outputDirName = "res";
static constexpr char const * inputDirName = "assets/rice_grains";

E::Curve
  getBoundary(Object const & component)
{
    // Khalimsky space
    E::KSpace kSpace;
    // we need to add a margin to prevent situations such that an
    // object touch the border of the domain
    // Point::diagonal is more general than subtracting Point(1, 1)
    // it works in any dimension.
    kSpace.init(component.domain().lowerBound(),
                component.domain().upperBound(),
      true);

    // 1) Call Surfaces::findABel() to find a cell which belongs to the border

    // findABell can't seem to find a boundary point with an acceptable
    // number of tries:
    // at s_numberTries == 1000, it fails on the first component
    // at s_numberTries == 10000, it seems okay, actually.

    // On second thought, using random.
    // find the boundary cell.
    E::SCell boundaryCell = DGtal::Surfaces<E::KSpace>::findABel(
      kSpace,
      component.pointSet(),
      s_numberTries
      );

    std::vector<E::Point> boundaryPoints;  // boundary points are going to be stored here

    // 2) Call Surfaces::track2DBoundaryPoints to extract the boundary of the object
    DGtal::Surfaces<E::KSpace>::track2DBoundaryPoints(
      boundaryPoints,
      kSpace,
      s_adjacency,
      // DigitalSets are models of PointPredicate, no worry.
      component.pointSet(),
      boundaryCell
      );
    E::Curve boundaryCurve;
    // 3) Create a curve from a vector
    boundaryCurve.initFromPointsVector(boundaryPoints);
    return boundaryCurve;
}

template <class T>
void
  sendToBoard(DGtal::Board2D & board, T const & p_Object, DGtal::Color const & p_Color)
{
    board << DGtal::CustomStyle(p_Object.className(), new DGtal::CustomFillColor(p_Color));
    board << p_Object;
}

void saveComponent(
    Object const & component,
    E::Curve const & boundary,
    std::filesystem::path const & outputPath,
    std::string const & fileName,
    DGtal::Color const & colour = DGtal::Color::Red,
    DGtal::Color const & boundaryColour = DGtal::Color::Lime
    )
{
    DGtal::Board2D board;  // we will use this object to save output
    sendToBoard(board, component, colour);
    sendToBoard(board, boundary, boundaryColour);
    board.saveEPS(
      std::filesystem::path(outputPath)
        .append(fileName + ".eps").c_str()
      );
}

void saveObjectComponents(
  std::vector<Object> const & objectComponents,
  std::filesystem::path const & outputPath,
  std::string const & fileName,
  DGtal::Color const & colour = DGtal::Color::Red)
{
    DGtal::Board2D board;  // we will use this object to save output
    for (Object const & component : objectComponents)
    {
        sendToBoard(board, component, colour);
    }
    board.saveEPS(
      std::filesystem::path(outputPath)
        .append(fileName + ".eps").c_str()
                 );
}

template <class Image_T>
Object
  computeDigitalObject(Image_T const & image)
{
    // 1) Create a digital set of proper size
    E::DigitalSet set2d(image.domain());
    // 2) Use SetFromImage::append() to populate a digital set from the input image
    DGtal::SetFromImage<E::DigitalSet>::append<Image>(set2d, image, 1, 255);

    // 3) Create a digital object from the digital set
    E::DigitalSet objectSet(set2d);
    return Object(s_topology, objectSet);

}


/// Pops the components whose set of points include a border point.
/// \param components
void cullBorderComponents(std::vector<Object> & components, E::Domain const & objectDomain)
{
    if (components.empty())
    {
        return;
    }
    // domain should be the same for every components anyway.

    // compute domain without the borders.
    // Point::diagonal is more general than subtracting Point(1, 1)
    E::Domain borderless = E::Domain(
      objectDomain.lowerBound() + E::Point::diagonal(),
      objectDomain.upperBound() - E::Point::diagonal()
      );
    for (auto it = components.begin(); it < components.end(); ++it)
    {
        E::DigitalSet const & pointSet = it->pointSet();
        bool shouldPop = false;
        for (auto const & point : pointSet)
        {
            // if the point is part of the domain's border, discard the whole component.
            if (!borderless.isInside(point))
            {
                shouldPop = true;
                break;
            }
        }
        if (shouldPop)
        {
            components.erase(it);
        }
    }
}

std::vector<Object>
  computeObjectComponents(Object const & object)
{
    // create output
    std::vector<Object> components;
    // create output iterator
    std::back_insert_iterator<std::vector<Object>> inserter(components);
    object.writeComponents(inserter);
    return components;
}

std::vector<E::Curve>
  computeObjectComponentBoundaries(std::vector<Object> const & components)
{
    std::vector<E::Curve> curves;
    curves.reserve(components.size());
    for (std::size_t i = 0; i < components.size() - 1; ++i)
    {
       curves.push_back(getBoundary(components.at(i)));
    }
    /*
    for (auto const & component : components)
    {
        curves.push_back(getBoundary(component));
    }
     */
    return curves;
}

int
  main(int argc, char ** argv)
{
    if (argc < 2)
    {
        std::cout << "usage: programme_name [path_to_image] *([path_to_images] )" << std::endl;
        return 0;
    }
    setlocale(LC_NUMERIC, "us_US");  // To prevent French local settings


    // all objects
    std::vector<Object> objects;
    // All connected components for each object.
    std::vector<std::vector<Object>> objectComponents;
    // Curves of all connected components for each object.
    std::vector<std::vector<E::Curve>> objectComponentBoundaries;
    // paths of files
    std::filesystem::path outputPath = std::filesystem::current_path()
      .parent_path()
      .append(outputDirName);
    std::filesystem::path inputPath = std::filesystem::current_path()
      .parent_path()
      .append(inputDirName);

    auto size = static_cast<std::size_t>(argc);

    objects.reserve(size);
    objectComponents.reserve(size);
    objectComponentBoundaries.reserve(size);

    for (int i = 1; i < argc; ++i)
    {
        std::filesystem::path path = inputPath.append(argv[i]);

        Image image = DGtal::PGMReader<Image>::importPGM(path);

        objects.push_back(computeDigitalObject(image));
        objectComponents.push_back(computeObjectComponents(objects.back()));
        // optionally, cull the border components.
        cullBorderComponents(objectComponents.back(), objects.back().domain());
        objectComponentBoundaries.push_back(computeObjectComponentBoundaries(objectComponents.back()));

    }

    for (auto const & components : objectComponents)
    {
        // Right now size of "components" is the number of connected components
        std::cout << " number of components : " << components.size() << std::endl;
    }

    // This is an example how to save a file
    for (std::size_t i = 20; i < 40; ++i)
    {
        saveComponent(
          objectComponents.at(0).at(i),
          objectComponentBoundaries.at(0).at(i),
          outputPath,
          std::string("component_") + std::to_string(i),
          DGtal::Color::Aqua);
    }
    /*
    saveObjectComponents(objectComponents.at(0),
                         outputPath,
                           std::string("the_whole_thing_yo"),
                           DGtal::Color::Green
                         );
     */

    return 0;
}
