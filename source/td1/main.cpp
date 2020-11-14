///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <fstream>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"

// shape and digitizer
#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/shapes/Shapes.h"

// tracking grid curve
#include "DGtal/geometry/curves/GridCurve.h"
#include "DGtal/topology/helpers/Surfaces.h"

#include "DGtal/io/boards/Board2D.h"
///////////////////////////////////////////////////////////////////////////////

using namespace DGtal;

// define an Euclidean shapes
typedef ImplicitBall<Z2i::Space>      Disc;
typedef ImplicitHyperCube<Z2i::Space> Square;
// Digitalised shapes
template <class Shape>
using Digital = GaussDigitizer<Z2i::Space, Shape>;
// set of points in digital shape
typedef Z2i::DigitalSet DigitalSet;
// Khalimsky Space
typedef Z2i::Domain Domain;
typedef Z2i::KSpace KSpace;
// Point
typedef Z2i::Point DigitalPoint;
// Boundary
typedef Z2i::Curve Boundary;
// Surfaces
typedef std::vector<std::vector<Z2i::SCell>> Surface;
// Area and perimeter
typedef double Area;
typedef double Perimetre;

// Functor class for computing relative orientation of three points in space.
typedef InHalfPlaneBySimple3x3Matrix<DigitalPoint, DGtal::int64_t> OrientationFunctor;
// Convex Hull
typedef MelkmanConvexHull<DigitalPoint, OrientationFunctor> ConvexHull;

/// STEP 1 ////////////////////////////////////////////////////////////////////////////
template <class Shape>
Digital<Shape>
  createDigitalShape(Shape const & shape, double h)
{
    // Gauss discretisation
    // note: h is the gridStep
    Digital<Shape> dig;
    {
        DigitalPoint digSize(1, 1);
        dig.attach(shape);
        dig.init(shape.getLowerBound() - digSize, shape.getUpperBound() + digSize, h);
    }
    return dig;
}

template <class Shape, class Adjacency>
Boundary
  getDigitalShapeBoundary(Digital<Shape> const & dig, Adjacency const & adj)
{
    // make a Kovalevsky-Khalimsky space
    Z2i::KSpace ks;
    ks.init(dig.getLowerBound(),
            dig.getUpperBound(),
            true  // isClosed
    );

    // search for one boundary element
    Z2i::SCell bel = Surfaces<Z2i::KSpace>::findABel(ks, dig, 1000);
    // boundary tracking
    std::vector<Z2i::Point> boundaryPoints;
    Surfaces<Z2i::KSpace>::track2DBoundaryPoints(boundaryPoints, ks, adj, dig, bel);

    Z2i::Curve c;
    c.initFromVector(boundaryPoints);

    return c;
}

void
  saveThisOrElse(Board2D const & board, std::string const & outFilePath)
{
    board.saveCairo((outFilePath + ".pdf").c_str(), Board2D::CairoPDF);
    board.saveCairo((outFilePath + ".png").c_str(), Board2D::CairoPNG);
}

void
  drawDigitalShapeBoundaries(std::vector<Boundary> const & boundaries, std::string const & outFilePath)
{
    // draw a boundaries and make PNG and PDF files
    Board2D board;
    for (auto const & boundary : boundaries)
    {
        board << boundary;
    }
    saveThisOrElse(board, outFilePath);
}

/// STEP 2 ////////////////////////////////////////////////////////////////////////////

template <class Shape>
DigitalSet
  getDigitalShapeSet(Digital<Shape> const & dig)
{
    Domain     domain = dig.getDomain();
    DigitalSet set(domain);
    // I'm sure there's a much better way of doing this, but I can't find it.
    // Create the set from the digital shape
    for (auto const & point : domain)
    {
        if (dig(point))
        {
            set.insertNew(point);
        }
    }
    return set;
}

template <class Shape>
Area
  getDigitalShapeArea(Digital<Shape> const & dig)
{
    // Get the number of (2-cells) of the digital shape
    DigitalSet const                                set       = getDigitalShapeSet(dig);
    typename Digital<Shape>::Space::RealPoint const gridSteps = dig.gridSteps();
    std::size_t                                     n         = set.size();
    Area                                            a         = static_cast<Area>(n);
    // multiply by the surface of each cell.
    for (auto coord : gridSteps)
    {
        a *= coord;
    }
    return a;
}

Area
  getDiscArea(double radius)
{
    return boost::math::constants::pi<double>() * radius * radius;
}

Area
  getSquareArea(double halfWidth)
{
    return halfWidth * halfWidth * 4;
}

template <class Shape, class Adjacency>
Perimetre
  getDigitalShapePerimetre(Digital<Shape> const & dig, Adjacency const & adj)
{
    // Get the boundary of the digital shape.
    Boundary const                                  boundary  = getDigitalShapeBoundary(dig, adj);
    typename Digital<Shape>::Space::RealPoint const gridSteps = dig.gridSteps();
    Perimetre                                       l         = 0;
    for (auto const & [point, vector] : boundary.getArrowsRange())
    {
        // vector here gives the topology of the 1-cell
        // -> norm is the length.
        // However, we the manhattan norm here to compute this per-axis
        unsigned int i = 0;
        for (auto coord : vector)
        {
            // length depends on the axis, in general.
            // Not an issue for us though, since we have even-sized grid elements.
            l += gridSteps[i++] * std::abs(coord);
        }
    }
    // multiply by the surface of each cell.
    return l;
}

Perimetre
  getDiscPerimetre(double radius)
{
    return 2 * boost::math::constants::pi<double>() * radius;
}

Perimetre
  getSquarePerimetre(double halfWidth)
{
    return 8 * halfWidth;
}

/// STEP 4 ////////////////////////////////////////////////////////////////////////////

template <class Shape, class Adjacency>
ConvexHull
  getDigitalShapeConvexHull(Digital<Shape> const & dig, Adjacency const & adj)
{
    // make a convex hull
    OrientationFunctor functor;
    ConvexHull         convexHull(functor);
    Boundary           boundary = getDigitalShapeBoundary(dig, adj);

    // Iterate over the boundary points
    for (auto const & p : boundary.getPointsRange())
    {
        convexHull.add(p);
    }

    return convexHull;
}

template <class Shape, class Adjacency>
void
  drawDigitalShapeConvexHull(Digital<Shape> const & dig, Adjacency const & adj, std::string const & outFilePath)
{
    ConvexHull convexHull = getDigitalShapeConvexHull(dig, adj);
    // scan the CVX points and draw the edges
    Board2D      board;
    auto         it    = convexHull.begin();
    DigitalPoint first = *it;
    board.setPenColor(Color::Red);
    for (; it != convexHull.end() && std::next(it) != convexHull.end(); ++it)
    {
        DigitalPoint const & p = *it;
        DigitalPoint const & q = *std::next(it);

        board.drawArrow(p[0] - 0.5, p[1] - 0.5, q[0] - 0.5, q[1] - 0.5);
    }
    {
        DigitalPoint last = *it;
        // there is a little +1/2 shift in the board exporter
        board.drawArrow(last[0] - 0.5, last[1] - 0.5, first[0] - 0.5, first[1] - 0.5);
    }

    // make PNG and PDF files
    saveThisOrElse(board, outFilePath);
}

/// STEP 5 ////////////////////////////////////////////////////////////////////////////
template <class Shape, class Adjacency>
Area
  getConvexHullArea(Digital<Shape> const & dig, Adjacency adj)
{
    // Get the convex hull
    ConvexHull                                      convexHull = getDigitalShapeConvexHull(dig, adj);
    typename Digital<Shape>::Space::RealPoint const gridSteps  = dig.gridSteps();
    Area                                            a          = 0;
    auto                                            it         = convexHull.begin();
    DigitalPoint                                    first      = *it;
    for (; it != convexHull.end() && std::next(it) != convexHull.end(); ++it)
    {
        // Will use L2 norm.
        DigitalPoint const & p       = *it;
        DigitalPoint const & q       = *std::next(it);
        Perimetre            dist    = (p - q).norm();
        Perimetre            sumNorm = (p + q).norm();
        a += dist * sumNorm;
    }
    {
        DigitalPoint last    = *it;
        Perimetre    dist    = (first - last).norm();
        Perimetre    sumNorm = (first - last).norm();
        a += dist * sumNorm;
    }
    a /= 4.;
    // Don't forget to multiply by the cell size!
    for (auto coord : gridSteps)
    {
        a *= coord;
    }
    return a;
}

template <class Shape, class Adjacency>
Perimetre
  getConvexHullPerimetre(Digital<Shape> const & dig, Adjacency const & adj)
{
    // Get the boundary of the digital shape.
    ConvexHull convexHull = getDigitalShapeConvexHull(dig, adj);
    // multiply by the surface of each cell.
    typename Digital<Shape>::Space::RealPoint gridSteps = dig.gridSteps();
    Perimetre                                 l         = 0;
    auto                                      it        = convexHull.begin();
    DigitalPoint                              first     = *it;
    for (; it != convexHull.end() && std::next(it) != convexHull.end(); ++it)
    {
        // Will use L2 norm.
        DigitalPoint const & p    = *it;
        DigitalPoint const & q    = *std::next(it);
        unsigned int         i    = 0;
        // Homography to set the scale of the diff in Real space.
        typename Digital<Shape>::Space::RealPoint diff = q - p;
        for (auto & coord : diff)
        {
            coord *= gridSteps[i++];
        }
        l += diff.norm();
    }
    {
        DigitalPoint last = *it;
        typename Digital<Shape>::Space::RealPoint diff = first - last;
        unsigned int         i    = 0;
        for (auto & coord : diff)
        {
            coord *= gridSteps[i++];
        }
        l += diff.norm();
    }
    return l;
}

/// MAIN  ////////////////////////////////////////////////////////////////////////////
int
  main()
{
    static constexpr Perimetre discRadius      = 10;
    static constexpr Perimetre squareHalfWidth = 5;
    // Shapes
    DigitalPoint centre(0, 0);
    Disc         disc(centre, discRadius);
    Square       square(centre, squareHalfWidth);

    Area discArea   = getDiscArea(discRadius);
    Area squareArea = getSquareArea(squareHalfWidth);

    Perimetre discPerimetre   = getDiscPerimetre(discRadius);
    Perimetre squarePerimetre = getSquarePerimetre(squareHalfWidth);

    // Gauss discretization
    double          h              = 20.;  // gridStep
    double          hHighRes       = 0.1;
    Digital<Disc>   discDig        = createDigitalShape(disc, h);
    Digital<Disc>   discDigHighRes = createDigitalShape(disc, hHighRes);
    Digital<Square> squareDig      = createDigitalShape(square, h);

    // Adjacency (4-connectivity)
    SurfelAdjacency<2> adj(true);

    /// STEP 2 ////////////////////////////////////////////////////////////////////////////
    std::vector<Boundary> boundaries;
    // disc
    boundaries.push_back(getDigitalShapeBoundary(discDig, adj));
    // high-resolution disc
    boundaries.push_back(getDigitalShapeBoundary(discDigHighRes, adj));
    // square
    boundaries.push_back(getDigitalShapeBoundary(squareDig, adj));

    drawDigitalShapeBoundaries(boundaries, "../res/Boundaries");

    /// STEP 3 ////////////////////////////////////////////////////////////////////////////
    std::cout << "/// STEP 3 ///" << std::endl;

    std::vector<std::pair<Area, Area>>           areas;
    std::vector<std::pair<Perimetre, Perimetre>> perimetres;

    // discs of increasing resolutions
    static constexpr int numberSteps = 20;
    for (int i = 0; i < numberSteps; ++i)
    {
        double        gridStep = numberSteps / static_cast<double>(i + 1);
        Digital<Disc> dig      = createDigitalShape(disc, gridStep);
        areas.emplace_back(getDigitalShapeArea(dig), discArea);
        perimetres.emplace_back(getDigitalShapePerimetre(dig, adj), discPerimetre);
    }

    // then with squares
    for (int i = 0; i < numberSteps; ++i)
    {
        double          gridStep = numberSteps / static_cast<double>(i + 1);
        Digital<Square> dig      = createDigitalShape(square, gridStep);
        areas.emplace_back(getDigitalShapeArea(dig), squareArea);
        perimetres.emplace_back(getDigitalShapePerimetre(dig, adj), squarePerimetre);
    }

    std::cout << "--- Area ---" << std::endl;
    for (auto const & areaPair : areas)
    {
        std::cout << "Areas (Digital|Real) " << areaPair << std::endl;
    }

    std::cout << "--- Perimetre ---" << std::endl;
    for (auto const & perimetrePair : perimetres)
    {
        std::cout << "Perimetres (Digital|Real) " << perimetrePair << std::endl;
    }

    /// STEP 4 ////////////////////////////////////////////////////////////////////////////
    drawDigitalShapeConvexHull(discDig, adj, "../res/ConvexHullDisc");
    drawDigitalShapeConvexHull(discDigHighRes, adj, "../res/ConvexHullDiscHighRes");
    drawDigitalShapeConvexHull(squareDig, adj, "../res/ConvexHullSquare");

    /// STEP 5 ////////////////////////////////////////////////////////////////////////////
    std::cout << "/// STEP 5 ///" << std::endl;

    areas.clear();
    perimetres.clear();

    // discs of increasing resolutions
    for (int i = 0; i < numberSteps; ++i)
    {
        double        gridStep = numberSteps / static_cast<double>(i + 1);
        Digital<Disc> dig      = createDigitalShape(disc, gridStep);
        areas.emplace_back(getConvexHullArea(dig, adj), discArea);
        perimetres.emplace_back(getConvexHullPerimetre(dig, adj), discPerimetre);
    }

    // then with squares
    for (int i = 0; i < numberSteps; ++i)
    {
        double          gridStep = numberSteps / static_cast<double>(i + 1);
        Digital<Square> dig      = createDigitalShape(square, gridStep);
        areas.emplace_back(getConvexHullArea(dig, adj), squareArea);
        perimetres.emplace_back(getConvexHullPerimetre(dig, adj), squarePerimetre);
    }

    std::cout << "--- Area ---" << std::endl;
    for (auto const & areaPair : areas)
    {
        std::cout << "Areas (ConvexHull|Real) " << areaPair << std::endl;
    }

    std::cout << "--- Perimetre ---" << std::endl;
    for (auto const & perimetrePair : perimetres)
    {
        std::cout << "Perimetres (ConvexHull|Real) " << perimetrePair << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
