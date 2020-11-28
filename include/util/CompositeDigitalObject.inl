//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_DIGITALOBJECTWRAPPER_INL
#define TD_UTIL_DIGITALOBJECTWRAPPER_INL

#include <DGtal/images/imagesSetsUtils/SetFromImage.h>

#include <algorithm>

namespace td::util
{
    template <int dimension, class Topology_T>
    inline CompositeDigitalObject<dimension, Topology_T>::CompositeDigitalObject(Image const & image)
        : components(), m_object(computeObject(image))
    {
        std::vector<Object> objectComponents = computeObjectComponents(m_object);
        components.reserve(objectComponents.size());
        for (auto const & objectComponent : objectComponents)
        {
            components.emplace_back(objectComponent);
        }
        // we remove the components too close to the domain's rim.
        cullBorderComponents();
    }
    template <int dimension, class Topology_T>
    void
      CompositeDigitalObject<dimension, Topology_T>::cullBorderComponents()
    {
        Domain const & compositeDomain = m_object.domain();
        // IMPORTANT:
        // be mindful to use it < components.end()
        // as an end condition,
        // because of erasure, the iterator index gets messed up,
        // and can jump the end such that it > components.end()
        for (auto it = components.begin(); it < components.end(); ++it)
        {
            if (it->isBorderingRim(compositeDomain))
            {
                it = components.erase(it);
            }
        }
        /* for some other idiotic reason I can't use this.
         * but I like it, so it stays.
        std::remove_if(
         components.begin(),
          components.end(),
          [&compositeDomain](Component const & component) -> bool
          {
            return component.isBorderingRim(compositeDomain);
          }
          );
          */
    }

    template <int dimension, class Topology_T>
    typename CompositeDigitalObject<dimension, Topology_T>::Object
      CompositeDigitalObject<dimension, Topology_T>::computeObject(CompositeDigitalObject::Image const & image)
    {
        // 1) Create a digital set of proper size
        DigitalSet set2d(image.domain());
        // 2) Use SetFromImage::append() to populate a digital set from the input image
        DGtal::SetFromImage<DigitalSet>::template append<Image>(set2d, image, 1, 255);

        // 3) Create a digital object from the digital set
        DigitalSet objectSet(set2d);
        return Object(s_topology, objectSet);
    }

    template <int dimension, class Topology_T>
    inline std::vector<typename CompositeDigitalObject<dimension, Topology_T>::Object>
      CompositeDigitalObject<dimension, Topology_T>::computeObjectComponents(
        const CompositeDigitalObject::Object & object)
    {
        // create output
        std::vector<Object> components;
        // create output iterator
        std::back_insert_iterator<std::vector<Object>> inserter(components);
        object.writeComponents(inserter);
        return components;
    }

}  // namespace td::util

#endif  // TD_UTIL_DIGITALOBJECTWRAPPER_INL
