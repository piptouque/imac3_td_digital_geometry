//
// Created by binabik on 28/11/2020.
//

#ifndef TD_UTIL_COMMON_HPP
#define TD_UTIL_COMMON_HPP

namespace td::util
{
    template <typename T>
    class maths
    {
       private:
        static constexpr int c_defaultNormOrder = 2;

       public:
        // constraints
        static_assert(std::is_arithmetic_v<T>);

        /// pi
        /// \return
        static constexpr T
        pi();

        /// norm
        /// \tparam Ret_T
        /// \tparam normOrder
        /// \param vec
        /// \return
        template <typename Ret_T = T, int normOrder = c_defaultNormOrder>
        static Ret_T norm(std::vector<T> const & vec);

        /// average
        /// \tparam Ret_T
        /// \param vec
        /// \return
        template <typename Ret_T = T>
        static Ret_T average(std::vector<T> const & vec);

        /// standard deviation (with a specific norm)
        /// \tparam Ret_T
        /// \tparam normOrder
        /// \param vec
        /// \return
        template <typename Ret_T = T, int normOrder = c_defaultNormOrder>
        static Ret_T standardDeviation(std::vector<T> const & vec);

        /// positive integer power.
        /// \tparam exponent
        /// \return
        template <int exponent>
        static constexpr T power(T val);

    };
}

#include "common.inl"

#endif  // TD_UTIL_COMMON_HPP
