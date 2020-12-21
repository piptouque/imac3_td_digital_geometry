//
// Created by binabik on 27/11/2020.
//

#ifndef TD_UTIL_DIGITALCOMPONENT_INL
#define TD_UTIL_DIGITALCOMPONENT_INL

// cloning mechanism (deep copy)
#include <DGtal/base/Clone.h>

#include <util/common.hpp>

namespace td::util
{
    template <int dimension, class Topology_T>
    inline DigitalComponent<dimension, Topology_T>::DigitalComponent(const DigitalComponent::Object & a_object)
        : m_object(a_object), m_isSet(false)
    {}

    template <int dimension, class Topology_T>
    inline DigitalComponent<dimension, Topology_T>::DigitalComponent(DigitalComponent const & component)
      : m_object(DGtal::Clone(component.m_object)),
          m_boundary(DGtal::Clone(component.m_boundary)),
          m_segmentation(),
        m_omega(DGtal::Clone(component.m_omega)),
        m_isSet(component.m_isSet)
    {
        m_segmentation.setSubRange(m_boundary.getPointsRange().begin(), m_boundary.getPointsRange().end());
    }


    template <int dimension, class Topology_T>
    inline DigitalComponent<dimension, Topology_T>::DigitalComponent(DigitalComponent && component) noexcept
      : m_object(std::move(component.m_object)),
          m_boundary(std::move(component.m_boundary)),
          m_segmentation(),
          m_omega(std::move(component.m_omega)),
          m_isSet(component.m_isSet)
    {
        m_segmentation.setSubRange(m_boundary.getPointsRange().begin(), m_boundary.getPointsRange().end());
    }

    template <int dimension, class Topology_T>
    inline DigitalComponent<dimension, Topology_T> &
    DigitalComponent<dimension, Topology_T>::operator=(const DigitalComponent & other)
    {
        m_object   = other.m_object;
        m_boundary = other.m_boundary;
        m_convexHull = other.m_convexHull;
        m_segmentation.setSubRange(m_boundary.getPointsRange().begin(), m_boundary.getPointsRange().end());
        m_omega = other.m_omega;
        m_isSet = other.m_isSet;
        return *this;
    }


    template <int dimension, class Topology_T>
    inline void
    DigitalComponent<dimension, Topology_T>::computeGeometry() const
    {
        m_boundary = computeBoundary(m_object);
        m_convexHull = computeConvexHull(m_boundary);
        m_segmentation.setSubRange(m_boundary.getPointsRange().begin(), m_boundary.getPointsRange().end());
        m_omega = computeOmega(m_convexHull);

        // The object is now set!
        m_isSet = true;
    }

    template <int dimension, class Topology_T>
    inline void
      DigitalComponent<dimension, Topology_T>::computeGeometryIfNotSet() const
    {
        if (!m_isSet)
        {
           computeGeometry();
        }
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Point
    DigitalComponent<dimension, Topology_T>::computeOmega(ConvexHull const & convexHull)
    {
        // IMPORTANT:
        // Here the coordinates of the objects are relative to the origin of the composite object,
        // which means that the (0, 0) is NOT in the object,
        // and therefore can't be used as the third point in the triangle decomposition
        // we need to arbitrarily define another one.
        // see the report for more details.
        // a lot hinges on the choice for this middle point.

        // Since the shape is convex, we know that I will be in P.
        auto begin = convexHull.begin();
        auto middle = begin + convexHull.size() / 2;
        return *middle + *begin;
    }

    template <int dimension, class Topology_T>
    inline typename DigitalComponent<dimension, Topology_T>::Curve
    DigitalComponent<dimension, Topology_T>::computeBoundary(Object const & objectComponent)
    {
        // IMPORTANT:
        // objectComponent() returns a value,
        // so the result can't be stored by reference!
        // Object border = objectComponent.border();
        std::vector<Point> boundaryPoints;

        // Khalimsky space
        KSpace kSpace;
        // we need to add a margin to prevent situations such that an
        // object touch the border of the domain
        // nope.
        // Point::diagonal is more general than subtracting Point(1, 1)
        // it works in any dimension.
        kSpace.init(objectComponent.domain().lowerBound(), objectComponent.domain().upperBound(), true);

        // 1) Call Surfaces::findABel() to find a cell which belongs to the border

        // TD2:
        // findABell can't seem to find a boundary point with an acceptable
        // number of tries:
        // at s_numberTries == 1000, it fails on the first component
        // at s_numberTries == 10000, still fails for japanese and camargue rice
        // at s_numberTries == 100_000, it finally goes through.

        // TD3:
        // Even 10_000_000 isn't enough.

        // number of tries for stochastic search of a border cell.
        int constexpr s_numberTries = 10000000;
        // On second thought, using random.
        // find the boundary cell.
        SCell boundaryCell = DGtal::Surfaces<KSpace>::findABel(kSpace, objectComponent.pointSet(), s_numberTries);

        // 2) Call Surfaces::track2DBoundaryPoints to extract the boundary of the object
        DGtal::template Surfaces<KSpace>::track2DBoundaryPoints(boundaryPoints,
                                                                kSpace,
                                                                s_adjacency,
          // DigitalSets are models of PointPredicate, no worries.
                                                                objectComponent.pointSet(),
                                                                boundaryCell);
        Curve boundaryCurve;
        // 3) Create a curve from a vector
        boundaryCurve.initFromPointsVector(boundaryPoints);
        return boundaryCurve;
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::ConvexHull
      DigitalComponent<dimension, Topology_T>::computeConvexHull(DigitalComponent::Curve const & boundary)
    {
        OrientationFunctor f;
        ConvexHull         convexHull{f};
        for (auto const & p : boundary.getPointsRange())
        {
            convexHull.add(p);
        }

        return convexHull;
    }

    template <int dimension, class Topology_T>
    inline bool
    DigitalComponent<dimension, Topology_T>::isBorderingRim(Domain const & compositeDomain) const
    {
        // compute domain without the borders.
        // Point::diagonal is more general than subtracting Point(1, 1)
        Domain borderless  = Domain(compositeDomain.lowerBound() + Point::diagonal(),
                                    compositeDomain.upperBound() - Point::diagonal());
        bool   isBordering = false;
        for (auto const & point : m_object.pointSet())
        {
            // if the point is part of the domain's border, discard the whole component.
            if (!borderless.isInside(point))
            {
                isBordering = true;
                break;
            }
        }
        return isBordering;
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Area
      DigitalComponent<dimension, Topology_T>::getCountArea() const
    {
        computeGeometryIfNotSet();
        return static_cast<Area>(m_object.pointSet().size());
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Area
      DigitalComponent<dimension, Topology_T>::getConvexHullArea() const
    {
        computeGeometryIfNotSet();
        // see report for first assignment for details.
        // answer_sheets/td1.md
        auto a = static_cast<Area>(0.);

        auto begin = m_convexHull.begin();
        auto end   = m_convexHull.end();

        for (auto it = begin; it < end; ++it)
        {
            bool const    shouldLoop = std::next(it) == end;
            Point const & p          = *it;
            Point const & q          = *(shouldLoop ? begin : std::next(it));

            a += (p - q).norm() * (p + q - m_omega).norm();
        }
        a /= static_cast<Area>(4.);
        return a;
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Area
    DigitalComponent<dimension, Topology_T>::getSegmentationArea() const
    {
        computeGeometryIfNotSet();
        auto a = static_cast<Area>(0.);
        for (auto const & segment : m_segmentation)
        {
            Point const & p          = segment.front();
            Point const & q          = segment.back();

            a += (p - q).norm() * (p + q - m_omega).norm();
        }
        a /= static_cast<Area>(4.);
        return a;
    }



    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Perimeter
      DigitalComponent<dimension, Topology_T>::getCountPerimeter() const
    {
        computeGeometryIfNotSet();
        return static_cast<Perimeter>(m_boundary.size());
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Perimeter
      DigitalComponent<dimension, Topology_T>::getConvexHullPerimeter() const
    {
        computeGeometryIfNotSet();
        // multiply by the surface of each cell.
        auto l  = static_cast<Perimeter>(0.);
        for (auto it = m_convexHull.begin(); it < m_convexHull.end(); ++it)
        {
            bool const shouldLoop = std::next(it) == m_convexHull.end();
            // Will use L2 norm.
            Point const & p = *it;
            Point const & q = *(shouldLoop ? m_convexHull.begin() : std::next(it));
            // Homography to set the scale of the diff in Real space.
            l += (p - q).norm();
        }
        return l;
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Area
    DigitalComponent<dimension, Topology_T>::getSegmentationPerimeter() const
    {
        computeGeometryIfNotSet();
        auto l  = static_cast<Perimeter>(0.);
        for (auto const & segment : m_segmentation)
        {
            // Will use L2 norm.
            Point const & p = segment.front();
            Point const & q = segment.back();
            // Homography to set the scale of the diff in Real space.
            l += (p - q).norm();
        }
        return l;
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::FloatScalar
      DigitalComponent<dimension, Topology_T>::getCircularity() const
    {
        computeGeometryIfNotSet();
        // We should use the segmentation to compute area and perimeter,
        // since it is more accurate.
        // See report for the rationale behind this definition.
        Perimeter l = getSegmentationPerimeter();
        Perimeter lSquared = maths<Perimeter>::power<2>(l);
        Area      a = getSegmentationArea();
        FloatScalar pi = maths<FloatScalar>::pi();
        FloatScalar circularity =  (static_cast<FloatScalar>(4.) * pi * a) / lSquared;
        return circularity;
    }

    template <int dimension, class Topology_T>
    inline
    typename DigitalComponent<dimension, Topology_T>::Point
    DigitalComponent<dimension, Topology_T>::getGeometricCentre() const
    {
        // No need to compute an average on the whole shape.
        // The boundary would give us a good enough approximate.
        // But it would require to compute it,
        // and it takes so long it's not worth it..
        Point sum = Point::zero;
        // Will most probably overflow here.
        // Should use dichotomy instead.
        // We would get an exact result since the coordinates are integers.
        for (auto const& point : m_object.pointSet())
        {
            sum += point;
        }
        return sum / static_cast<Integer>(m_object.size());
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::AngleRadian
    DigitalComponent<dimension, Topology_T>::computeRotationAngle(
        std::vector<Point> const & points1,
        std::vector<Point> const & points2)
    {
        // Will use Kabsch for the most accurate result.
        Matrix rotationMatrix = algorithms<FloatScalar>::computeRotationKabsch<Point>(points1, points2);
        // will only work in 2D.
        return std::acos(rotationMatrix(0, 0));
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Perimeter
    DigitalComponent<dimension, Topology_T>::computeHausdorffDistance(const DigitalComponent & other) const
    {
        //
        return std::max(
          this->computeFarthestDistance(other),
          other.computeFarthestDistance(*this)
        );
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Perimeter
    DigitalComponent<dimension, Topology_T>::computeDubuissonJainDissimilarity(const DigitalComponent & other) const
    {
        //
        return std::max(
          this->computeAverageDistance(other),
          other.computeAverageDistance(*this)
       );
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Perimeter
    DigitalComponent<dimension, Topology_T>::computeFarthestDistance(const DigitalComponent<dimension, Topology_T> & other) const
    {
        auto it = std::max_element(
          m_object.pointSet().begin(),
          m_object.pointSet().end(),
          [&other](Point const & first, Point const & second) -> bool
          {

              return other.computeClosestPointDistance(first)
                     <= other.computeClosestPointDistance(second);
          }
          );
        return other.computeClosestPointDistance(*it);
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Perimeter
    DigitalComponent<dimension, Topology_T>::computeAverageDistance(const DigitalComponent<dimension, Topology_T> & other) const
    {
        int count = 0;
        return std::accumulate(
          m_object.pointSet().begin(),
          m_object.pointSet().end(),
          static_cast<Perimeter>(0.),
          [&other, &count](Perimeter average, Point const & point)
          {
              average = (average * (count + 1) + other.computeClosestPointDistance(point)) / static_cast<Perimeter>(count);
              ++count;
              return average;
          });
    }

    template <int dimension, class Topology_T>
    typename DigitalComponent<dimension, Topology_T>::Perimeter
    DigitalComponent<dimension, Topology_T>::computeClosestPointDistance(Point const & from) const
    {
        // find the min element.
        auto it = std::max_element(
          m_object.pointSet().begin(),
          m_object.pointSet().end(),
          [&from](Point const & first, Point const & second) -> bool
          {
              // return true if first is greater,
              // in order to have get the minimum element.
              return (first - from).norm() >= (second - from).norm();
          }
          );
        return (*it - from).norm();
    }

    template <int dimension, class Topology_T>
    inline typename DigitalComponent<dimension, Topology_T>::Point
    DigitalComponent<dimension, Topology_T>::getPositionFromCentre(Point const & point) const
    {
        return point - getGeometricCentre();
    }

    template <int dimension, class Topology_T>
    inline typename DigitalComponent<dimension, Topology_T>::Vector
    DigitalComponent<dimension, Topology_T>::getTranslationTo(DigitalComponent const &other) const
    {
        return other.getGeometricCentre() - getGeometricCentre();
    }

    template <int dimension, class Topology_T>
    void
    DigitalComponent<dimension, Topology_T>::drawObject(DGtal::Board2D & board, Colour const & objectColour) const
    {
        // no need to compute geometry here.
        board << DGtal::CustomStyle(m_object.className(), new DGtal::CustomFillColor(objectColour));
        board << m_object;
    }


        template <int dimension, class Topology_T>
    void
      DigitalComponent<dimension, Topology_T>::draw(DGtal::Board2D & board,
                                                    Colour const &   objectColour,
                                                    Colour const &   segmentColour,
                                                    Colour const &   convexHullColour,
                                                    Colour const &   boundaryColour) const
    {
        computeGeometryIfNotSet();

        // draw object and boundary
        draw(board, objectColour);
        board << DGtal::CustomStyle(m_boundary.className(), new DGtal::CustomFillColor(boundaryColour));
        board << m_boundary;

        // draw Convex Hull (with segmentation, actually.)
        board.setPenColor(convexHullColour);
        board.setFillColor(Colour::None);
        for (auto const & segment : m_segmentation)
        {
            Point const & p          = segment.front();
            Point const & q          = segment.back();

            // there is a little +1/2 shift in the board exporter
            double offset = 0.5;
            board.drawTriangle(
              p[0] - offset,           p[1] - offset,
              q[0] - offset,           q[1] - offset,
              m_omega[0] / 2 - offset, m_omega[1] / 2 - offset
              );
        }
        // save segmentation
        for (auto const & segment : m_segmentation)
        {
            board << DGtal::SetMode("ArithmeticalDSS", "BoundingBox");
            board << DGtal::CustomStyle("ArithmeticalDSS/BoundingBox", new DGtal::CustomPenColor(segmentColour));
            board << segment.primitive();
        }
    }

}  // namespace td::util

#endif  // TD_UTIL_DIGITALCOMPONENT_INL
