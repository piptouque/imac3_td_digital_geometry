//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_DIGITALOBJECTWRAPPER_INL
#define TD_UTIL_DIGITALOBJECTWRAPPER_INL

#include <DGtal/images/imagesSetsUtils/SetFromImage.h>
// cloning mechanism (deep copy)
#include <DGtal/base/Clone.h>

#include <algorithm>

namespace td::util
{
    template <int dimension, class Topology_T>
    inline CompositeDigitalObject<dimension, Topology_T>::CompositeDigitalObject(Image const & image)
        : components(),
          m_image(DGtal::Clone(image)),
          m_backgroundDistanceTransform(computeBackgroundDistanceTransform(m_image))
    {
        // assigning m_image twice, otherwise we get a compilation error.
        reset(image);
    }

    template <int dimension, class Topology_T>
    void
    CompositeDigitalObject<dimension, Topology_T>::reset(Image const & image)
    {
        m_image = DGtal::Clone(image);
        m_object = computeObject(m_image);
        std::vector<Object> objectComponents = computeObjectComponents(m_object);
        m_backgroundDistanceTransform        = computeBackgroundDistanceTransform(image);
        components.clear();
        components.reserve(objectComponents.size());
        for (auto const & objectComponent : objectComponents)
        {
            components.emplace_back(objectComponent);
        }
        // we remove the components too close to the domain's rim.
        cullBorderComponents();
    }

    template <int dimension, class Topology_T>
    CompositeDigitalObject<dimension, Topology_T>::CompositeDigitalObject(CompositeDigitalObject const & other)
        : components(other.components),
          m_image(DGtal::Clone(other.m_image)),
          m_object(DGtal::Clone(other.m_object)),
          m_interestPoint(other.m_interestPoint),
          m_backgroundDistanceTransform(other.m_backgroundDistanceTransform)
    {

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
        for (auto it = components.begin(); it < components.end();)
        {
            if (it->isBorderingRim(compositeDomain))
            {
                it = components.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    template <int dimension, class Topology_T>
    inline void
      CompositeDigitalObject<dimension, Topology_T>::setInterestPoint(Point interestPoint)
    {
        m_interestPoint = std::move(interestPoint);
    }

    template <int dimension, class Topology_T>
    std::optional<typename CompositeDigitalObject<dimension, Topology_T>::Point>
    CompositeDigitalObject<dimension, Topology_T>::getInterestPoint() const
    {
        return m_interestPoint;
    }

    template <int dimension, class Topology_T>
    void
    CompositeDigitalObject<dimension, Topology_T>::transformRigidForward(
      RealPoint const & rotCentre,
      AngleRadian       angle,
      RealVector const & translation)
    {
        ForwardTransform forwardTransform (rotCentre, angle, translation);
        DomainTransformer megatron (forwardTransform);
        Bounds bounds = megatron (m_image.domain());
        Domain transformedDomain (bounds.first, bounds.second);

        Image transformedImage (transformedDomain);
        // Compute the resulting point from each point in the original image.
        for (auto const & point : m_image.domain())
        {
            transformedImage.setValue(forwardTransform(point), m_image(point));
        }
        // Also transform the interest point if set.
        if (m_interestPoint.has_value())
        {
            setInterestPoint(forwardTransform(m_interestPoint.value()));
        }
        // reset the object with the transformed image.
        reset(transformedImage);
    }

    template <int dimension, class Topology_T>
    void
    CompositeDigitalObject<dimension, Topology_T>::transformRigidBackward(
      RealPoint const & rotCentre,
      AngleRadian       angle,
      RealVector const & translation)
    {
        BackwardTransform backwardTransform (rotCentre, angle, translation);
        // Compute transformed domain from the forward transform.
        ForwardTransform forwardTransform (rotCentre, angle, translation);
        DomainTransformer megatron (forwardTransform);
        Bounds bounds = megatron (m_image.domain());
        Domain transformedDomain (bounds.first, bounds.second);
        DGtal::functors::Identity id {};
        //

        Image transformedImage (transformedDomain);

        ImageBackwardAdapter  imageBackwardAdapter (
          m_image,
          transformedDomain,
          backwardTransform,
          id
          );
        // Get the value for each point in the transformed image.
        for (auto const & point : transformedImage.domain())
        {
            transformedImage.setValue(point, imageBackwardAdapter(point));
        }
        // Also transform the interest point if it is set.
        // Have to use the forward transformation for that.
        if (m_interestPoint.has_value())
        {
            setInterestPoint(forwardTransform(m_interestPoint.value()));
        }

        // reset the object with the transformed image.
        reset(transformedImage);
    }

    template <int dimension, class Topology_T>
    typename CompositeDigitalObject<dimension, Topology_T>::Object
      CompositeDigitalObject<dimension, Topology_T>::computeObject(Image const & image)
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
    inline typename CompositeDigitalObject<dimension, Topology_T>::DistanceTransform
    CompositeDigitalObject<dimension, Topology_T>::computeBackgroundDistanceTransform(Image const & image)
    {
        Binariser bin (image, -1, 0);
        return DistanceTransform(&image.domain(), &bin, &s_metric);
    }


    template <int dimension, class Topology_T>
    inline std::vector<typename CompositeDigitalObject<dimension, Topology_T>::Object>
      CompositeDigitalObject<dimension, Topology_T>::computeObjectComponents(
        Object const & object)
    {
        // create output
        std::vector<Object> components;
        // create output iterator
        std::back_insert_iterator<std::vector<Object>> inserter(components);
        object.writeComponents(inserter);
        return components;
    }


    template <int dimension, class Topology_T>
    typename CompositeDigitalObject<dimension, Topology_T>::Perimeter
    CompositeDigitalObject<dimension, Topology_T>::computeHausdorffDistance(
      CompositeDigitalObject const & other
    ) const
    {
        //
        return std::max(
        this->components.front().computeLargestDistance(other.m_backgroundDistanceTransform),
          other.components.front().computeLargestDistance(this->m_backgroundDistanceTransform)
        );
    }

    template <int dimension, class Topology_T>
    typename CompositeDigitalObject<dimension, Topology_T>::Perimeter
    CompositeDigitalObject<dimension, Topology_T>::computeDubuissonJainDissimilarity(
        CompositeDigitalObject const & other) const
    {
        //
        return std::max(
          components.front().computeAverageDistance(other.m_backgroundDistanceTransform),
          other.components.front().computeAverageDistance(this->m_backgroundDistanceTransform)
        );
    }

    template <int dimension, class Topology_T>
    void
      CompositeDigitalObject<dimension, Topology_T>::cullAllButLargestComponent()
    {
        auto const itMax = std::max_element(
          components.begin(),
          components.end(),
          [](Component const & c1, Component const & c2) -> bool
          {
            return c1.getCountArea() <= c2.getCountArea();
          }
        );
        // keep only the max.
        Component const maxComponent = std::move(*itMax);
        components.clear();
        components.push_back(maxComponent);
    }

    template <int dimension, class Topology_T>
    void
    CompositeDigitalObject<dimension, Topology_T>::drawObjectComponents(DGtal::Board2D & board,
                                                                        Colour const &   objectColour,
                                                                        Colour const &   interestPointColour) const
    {
        for (auto const & component : components)
        {
            component.drawObject(board, objectColour);
        }
        // Can't get this to work, too bad.
        if (m_interestPoint.has_value())
        {
            board << DGtal::CustomStyle(m_interestPoint.value().className(), new DGtal::CustomFillColor(interestPointColour));
            board << m_interestPoint.value();
        }
    }

}  // namespace td::util

#endif  // TD_UTIL_DIGITALOBJECTWRAPPER_INL
