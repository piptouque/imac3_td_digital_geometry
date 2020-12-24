//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_DIGITALCOMPONENT_HPP
#define TD_UTIL_DIGITALCOMPONENT_HPP

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/topology/SurfelAdjacency.h>
#include <DGtal/topology/helpers/Surfaces.h>

#include <DGtal/images/IntervalForegroundPredicate.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>

#include <DGtal/geometry/curves/GreedySegmentation.h>


#include <util/eigen.hpp>

namespace td::util
{
    /// A class for closed digital objects.
    /// That is, digital objects which are CONNECTED.
    /// \tparam dimension
    /// \tparam Topology_T
    template <int dimension, class Topology_T>
    class DigitalComponent
    {
    public:
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
        typedef typename Space::Vector   Vector;
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
        //
        typedef double AngleRadian;

        // matrix and vector types
        typedef Eigen::Matrix<FloatScalar, dimension, dimension> Matrix;

        // Image type
        typedef int ImageValue;
        typedef typename DGtal::ImageSelector<Domain, ImageValue>::Type Image;

        // Distance transformations
        int static constexpr c_metricOrder = 2; // L2 metric
        typedef DGtal::ExactPredicateLpSeparableMetric<Space, c_metricOrder> Metric;
        typedef DGtal::functors::IntervalForegroundPredicate<Image> Binariser;

        typedef DGtal::DistanceTransformation<Space, Binariser, Metric> DistanceTransform;

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



        void
        drawObject(DGtal::Board2D & board,
             const Colour &   objectColour     = Colour::None) const;

             void
          draw(DGtal::Board2D & board,
               const Colour &   objectColour     = Colour::None,
               const Colour &   segmentColour    = Colour::Aqua,
               const Colour &   convexHullColour = Colour::Red,
               const Colour &   boundaryColour   = Colour::Black) const;

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

        [[nodiscard]] inline Point
          getPositionFromCentre(Point const & point) const;

        [[nodiscard]] inline Vector
        getTranslationTo(DigitalComponent const & other) const;

        /// Computes geometric centre of the shape.
        /// Called centre of mass in the assigment.
        /// \return geometric centre.
        [[nodiscard]] inline Point
          getGeometricCentre() const;

        [[nodiscard]] Perimeter computeLargestDistance(DistanceTransform const & otherBackgroundDistance) const;
        [[nodiscard]] Perimeter computeAverageDistance(DistanceTransform const & otherBackgroundDistance) const;

        [[nodiscard]] inline bool
          isBorderingRim(Domain const & compositeDomain) const;

        [[nodiscard]] inline static AngleRadian computeRotationAngle(
          std::vector<Point> const & points1,
          std::vector<Point> const & points2);

       private:

        /** --------- methods ------------- **/
        [[nodiscard]] inline static Curve
          computeBoundary(Object const & objectComponent);
        [[nodiscard]] inline static ConvexHull
          computeConvexHull(Curve const & boundary);
        [[nodiscard]] inline static Point
          computeOmega(ConvexHull const & convexHull);

        /// Computes Boundary, Convex Hull and Segmentation.
        inline void
        computeGeometry() const;

        [[nodiscard]] Perimeter computeClosestPointDistance(Point  const & from) const;

        inline void computeGeometryIfNotSet() const;

        /** --------- data ------------- **/
        Object m_object;
        // Computed from the digital object.
        Curve  mutable       m_boundary;
        ConvexHull mutable   m_convexHull;
        Segmentation mutable m_segmentation;

        // we store twice I (omega) for the computations.
        // see report for details.
        Point mutable m_omega;

        // Computing geometry only if needed.
        bool mutable m_isSet;

        // Adjacency object.
        // Interior to exterior only for adjacency pairs.
        inline static Adjacency const s_adjacency = {true};

    };

}  // namespace td::util
#include "DigitalComponent.inl"

#endif  // TD_UTIL_DIGITALCOMPONENT_HPP
