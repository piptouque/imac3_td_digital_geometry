//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_DIGITALCOMPONENT_HPP
#define TD_UTIL_DIGITALCOMPONENT_HPP

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/topology/SurfelAdjacency.h>
#include <DGtal/topology/helpers/Surfaces.h>

// added in order to access GreedySegmentation
#include <DGtal/geometry/curves/GreedySegmentation.h>

namespace td::util
{
    /// A class for closed digital objects.
    /// That is, digital objects which are CONNECTED.
    /// \tparam dimension
    /// \tparam Topology_T
    template <int dimension, class Topology_T>
    struct DigitalComponent
    {
        /** --------- typedefs ------------- **/
        // Topology
        typedef Topology_T                                  DigitalTopology;
        typedef DGtal::Z2i::Integer                         Integer;
        typedef DGtal::KhalimskySpaceND<dimension, Integer> KSpace;
        typedef DGtal::SpaceND<dimension, Integer>          Space;
        typedef DGtal::HyperRectDomain<Space>               Domain;

        typedef typename DGtal::DigitalSetSelector<Domain, DGtal::HIGH_BEL_DS + 2>::Type PointSet;

        typedef typename KSpace::SCell   SCell;
        typedef typename Space::Point    Point;
        typedef DGtal::GridCurve<KSpace> Curve;
        // Digital object type
        typedef DGtal::Object<DigitalTopology, PointSet> Object;
        // Convex Hull stuff
        typedef DGtal::InHalfPlaneBySimple3x3Matrix<Point, DGtal::int64_t> OrientationFunctor;
        typedef DGtal::MelkmanConvexHull<Point, OrientationFunctor>        ConvexHull;
        typedef DGtal::SurfelAdjacency<KSpace::dimension>                  Adjacency;

        // segmentation
        // Careful to specify the adjacency, it defaults to 8 otherwise.
        // Second TParam must be of a range larger than TCoordinate (signed!!).
        static constexpr unsigned short c_kappa = 4;

        typedef DGtal::ArithmeticalDSSComputer<typename Curve::PointsRange::ConstIterator, DGtal::int64_t, c_kappa>
                                                           SegmentComputer;
        typedef DGtal::GreedySegmentation<SegmentComputer> Segmentation;

        // Circularity is without dimension.
        typedef double FloatScalar;
        // Area and Perimeter (this time, they are normalised relative to the scale of the grid.)
        typedef double Area;
        typedef Area   Perimeter;

        // things
        typedef DGtal::Color Colour;


        /** --------- methods ------------- **/

        inline explicit DigitalComponent(Object const & object);

        // copy constructor
        inline  DigitalComponent(DigitalComponent const & component);
        // need to explicitly define a move constructor...
        inline DigitalComponent(DigitalComponent && component) noexcept;

        // and a copy operator...
        inline DigitalComponent &
          operator=(DigitalComponent const & other);

        inline void compute();

        [[nodiscard]] inline DGtal::Board2D
          draw(Colour const & objectColour = Colour::None,
               Colour const & boundaryColour = Colour::Black,
               Colour const & segmentColour = Colour::Aqua,
               Colour const & convexHullColour = Colour::Red) const;

        // Area computations.
        [[nodiscard]] inline Area
          getCountArea() const;
        [[nodiscard]] inline Area
          getConvexHullArea() const;
        [[nodiscard]] inline Area
          getSegmentationArea() const;

        // Perimeter computations.
        [[nodiscard]] inline Perimeter
          getCountPerimeter() const;
        [[nodiscard]] inline Perimeter
          getConvexHullPerimeter() const;
        [[nodiscard]] inline Perimeter
          getSegmentationPerimeter() const;

        [[nodiscard]] inline FloatScalar
          getCircularity() const;

        [[nodiscard]] inline bool
          isBorderingRim(Domain const & compositeDomain) const;

       private:

        /** --------- methods ------------- **/
        [[nodiscard]] inline static Curve
          computeBoundary(Object const & objectComponent);
        [[nodiscard]] inline static ConvexHull
          computeConvexHull(Curve const & boundary);
        [[nodiscard]] inline static Point
          computeOmega(ConvexHull const & convexHull);

        /** --------- data ------------- **/
        Object       m_object;
        Curve        m_boundary;
        ConvexHull   m_convexHull;
        Segmentation m_segmentation;

        // we store twice I (omega) for the computations.
        // see report for details.
        Point m_omega;

        // Adjacency object.
        // Interior to exterior only for adjacency pairs.
        inline static Adjacency const s_adjacency = {true};

    };

}  // namespace td::util
#include "DigitalComponent.inl"

#endif  // TD_UTIL_DIGITALCOMPONENT_HPP
