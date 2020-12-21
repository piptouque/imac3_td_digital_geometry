//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_COMPOSITEDIGITALOBJECT_HPP
#define TD_UTIL_COMPOSITEDIGITALOBJECT_HPP

#include <util/DigitalComponent.hpp>

#include <DGtal/images/RigidTransformation2D.h>
#include <DGtal/images/ConstImageAdapter.h>
#include <DGtal/images/IntervalForegroundPredicate.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>

#include <optional>

namespace td::util
{
    /// A class for any digital object.
    /// \tparam dimension
    template <int dimension, class Topology_T>
    class CompositeDigitalObject
    {
      public:
        /** --------- typedefs ------------- **/
        typedef Topology_T                              DigitalTopology;
        typedef DigitalComponent<dimension, Topology_T> Component;

        typedef typename Component::Space  Space;
        typedef typename Component::Domain Domain;
        typedef typename Component::Object Object;

        typedef typename Space::Point          Point;
        typedef typename Component::Vector     Vector;
        typedef typename Space::RealPoint      RealPoint;
        typedef typename Space::RealVector     RealVector;
        typedef typename Component::PointSet   DigitalSet;

        typedef typename Component::AngleRadian AngleRadian;

        // Image type
        typedef typename DGtal::ImageSelector<Domain, unsigned char>::Type Image;

        // Rigid transformations
        typedef DGtal::functors::ForwardRigidTransformation2D<Space> ForwardTransform;
        typedef DGtal::functors::BackwardRigidTransformation2D<Space> BackwardTransform;
        typedef DGtal::ConstImageAdapter<Image, Domain, BackwardTransform , typename Image::Value, DGtal::functors::Identity> ImageBackwardAdapter;
        typedef DGtal::functors::DomainRigidTransformation2D<Domain, ForwardTransform > DomainTransformer;
        typedef typename DomainTransformer::Bounds Bounds;

        // Distance transformations
        int static constexpr c_metricOrder = 2; // L2 metric
        typedef DGtal::ExactPredicateLpSeparableMetric<Space, c_metricOrder> Metric;
        typedef DGtal::functors::IntervalForegroundPredicate<Image> Binariser;

        typedef DGtal::DistanceTransformation<Space, Binariser, Metric> DistanceTransform;

        // other
        typedef DGtal::Color Colour;

        /** --------- methods ------------- **/
        // c-tor
        inline explicit CompositeDigitalObject(Image const & image);

        inline CompositeDigitalObject(CompositeDigitalObject const & wrapper);
        // And for some reason it does not compile if I mark this as noexcept..
        inline CompositeDigitalObject(CompositeDigitalObject && wrapper) = default;

        /// Set custom interest point. Will be transformed with the object.
        /// \param interestPoint
        inline void setInterestPoint(Point interestPoint);

        std::optional<Point> getInterestPoint() const;

        void cullAllButLargestComponent();

        void
          transformRigidForward(RealPoint const & rotCentre, AngleRadian angle, RealVector const & translation);
        void
          transformRigidBackward(RealPoint const & rotCentre, AngleRadian angle, RealVector const & translation);



        inline void
        drawObjectComponents(DGtal::Board2D & board,
             Colour const &   objectColour        = Colour::None,
             Colour const &   interestPointColour = Colour::Magenta) const;

        /** --------- data ------------- **/
        std::vector<Component> components;

       private:

        /** --------- methods ------------- **/
        [[nodiscard]] inline static std::vector<Object>
          computeObjectComponents(Object const & object);
        [[nodiscard]] inline static Object
          computeObject(Image const & image);
        [[nodiscard]] inline static DistanceTransform
          computeDistanceTransform(Image const & image);
        /// Removes the components whose set of points include a border point.
        /// \param components
        void
          cullBorderComponents();
        void reset(Image const & image);

        /** --------- data ------------- **/
        Image m_image;
        Object m_object;
        // Point of interest (optional)
        std::optional<Point> m_interestPoint;
        // Topology object
        inline static DigitalTopology const s_topology = DGtal::Z2i::dt4_8;
        // Metric object
        inline static Metric const s_metric = DGtal::Z2i::l2Metric;

    };
}  // namespace td::util

#include "CompositeDigitalObject.inl"

#endif  // TD_UTIL_COMPOSITEDIGITALOBJECT_HPP
