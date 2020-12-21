//
// Created by binabik on 19/12/2020.
//

#ifndef IMAC3_DG_INCLUDE_UTIL_EIGEN_INL
#define IMAC3_DG_INCLUDE_UTIL_EIGEN_INL

namespace td::util
{

    template <class Point_T>
    Eigen::Matrix<typename Point_T::Scalar, Point_T::dimension, 1>
    EigenUtility::dgtalPointToColumnVector(Point_T const & point)
    {
        Eigen::Matrix<typename Point_T::Scalar, Point_T::dimension, 1> colMatrix;
        int i = 0;
        for (auto const& coord : point)
        {
            colMatrix[i++] = coord;
        }
        return colMatrix;
    }

}
#endif  // IMAC3_DG_INCLUDE_UTIL_EIGEN_INL
