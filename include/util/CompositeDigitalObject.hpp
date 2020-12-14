//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_COMPOSITEDIGITALOBJECT_HPP
#define TD_UTIL_COMPOSITEDIGITALOBJECT_HPP

#include <util/DigitalComponent.hpp>

namespace td::util
{
    /// A class for any digital object.
    /// \tparam dimension
    template <int dimension, class Topology_T>
    struct CompositeDigitalObject
    {
        /** --------- typedefs ------------- **/
        typedef Topology_T                              DigitalTopology;
        typedef DigitalComponent<dimension, Topology_T> Component;

        typedef typename Component::Domain Domain;
        typedef typename Component::Object Object;

        typedef typename Component::Point      Point;
        typedef typename Component::PointSet   DigitalSet;

        // type of image
        typedef typename DGtal::ImageSelector<Domain, unsigned char>::Type Image;


        /** --------- methods ------------- **/
        // c-tor
        inline explicit CompositeDigitalObject(Image const & image);

        inline CompositeDigitalObject(CompositeDigitalObject const & wrapper);
        // And for some reason it does not compile if I mark this as noexcept..
        inline CompositeDigitalObject(CompositeDigitalObject && wrapper) = default;

        /** --------- data ------------- **/
        std::vector<Component> components;

       private:
        /** --------- data ------------- **/

        Object m_object;
        // Topology object
        inline static DigitalTopology const s_topology = DGtal::Z2i::dt4_8;

        /** --------- methods ------------- **/
        [[nodiscard]] inline static std::vector<Object>
          computeObjectComponents(Object const & object);
        [[nodiscard]] inline static Object
          computeObject(Image const & image);
        /// Pops the components whose set of points include a border point.
        /// \param components
        void
          cullBorderComponents();

    };
}  // namespace td::util

#include "CompositeDigitalObject.inl"

#endif  // TD_UTIL_COMPOSITEDIGITALOBJECT_HPP
