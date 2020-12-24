//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_COMPOSITEDIGITALOBJECT_HPP
#define TD_UTIL_COMPOSITEDIGITALOBJECT_HPP

#include <util/DigitalComponent.hpp>

#include <DGtal/images/RigidTransformation2D.h>
#include <DGtal/images/ConstImageAdapter.h>

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

        typedef typename Component::Perimeter Perimeter;
        // Image type
        typedef typename Component::Image Image;

        // Rigid transformations
        typedef DGtal::functors::ForwardRigidTransformation2D<Space> ForwardTransform;
        typedef DGtal::functors::BackwardRigidTransformation2D<Space> BackwardTransform;
        typedef DGtal::ConstImageAdapter<Image, Domain, BackwardTransform , typename Image::Value, DGtal::functors::Identity> ImageBackwardAdapter;
        typedef DGtal::functors::DomainRigidTransformation2D<Domain, ForwardTransform > DomainTransformer;
        typedef typename DomainTransformer::Bounds Bounds;


        // Distance transformations
        typedef typename Component::Metric Metric;
        typedef typename Component::Binariser Binariser;

        typedef typename Component::DistanceTransform DistanceTransform;

        // other
        typedef DGtal::Color Colour;

        /** --------- methods ------------- **/
        // c-tor
        inline explicit CompositeDigitalObject(Image const & image);

        inline CompositeDigitalObject(CompositeDigitalObject const &);
        // And for some reason it does not compile if I mark this as noexcept..
        inline CompositeDigitalObject(CompositeDigitalObject &&) = default;

        /// Set custom interest point. Will be transformed with the object.
        /// \param interestPoint
        inline void setInterestPoint(Point interestPoint);

        std::optional<Point> getInterestPoint() const;

        void cullAllButLargestComponent();

        void
          transformRigidForward(RealPoint const & rotCentre, AngleRadian angle, RealVector const & translation);
        void
          transformRigidBackward(RealPoint const & rotCentre, AngleRadian angle, RealVector const & translation);


        [[nodiscard]] Perimeter computeHausdorffDistance(CompositeDigitalObject const & other) const;
        [[nodiscard]] Perimeter computeDubuissonJainDissimilarity(CompositeDigitalObject const & other) const;

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
          computeBackgroundDistanceTransform(Image const & image);
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
        DistanceTransform    m_backgroundDistanceTransform;
        // Topology object
        inline static DigitalTopology const s_topology = DGtal::Z2i::dt4_8;
        // Metric object
        inline static Metric const s_metric = DGtal::Z2i::l2Metric;

    };
}  // namespace td::util

#include "CompositeDigitalObject.inl"

#endif  // TD_UTIL_COMPOSITEDIGITALOBJECT_HPP
