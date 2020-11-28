//
// Created by binabik on 28/11/2020.
//

#ifndef TD_UTIL_COMMON_INL
#define TD_UTIL_COMMON_INL

#include <functional>
#include <numeric>
#include <type_traits>

namespace td::util
{
    template <typename T>
    constexpr T
      maths<T>::pi()
    {
        // see:
        // https://www.piday.org/million/
        // 36 significant digits should be enough for anyone.
        return static_cast<T>(3.1415926535897932384626433832795028);
    }
    template <typename T>
    template <typename Ret_T, int normOrder>
    Ret_T
      maths<T>::standardDeviation(const std::vector<T> & vec)
    {
        static_assert(normOrder > 0);
        //
        Ret_T avg = average<Ret_T>(vec);

        std::size_t count = 0;
        Ret_T       res = std::accumulate(vec.begin(),
                                    vec.end(),
                                    static_cast<Ret_T>(0.),
                                    [&count, avg](Ret_T sigma, T el)
                                    {
                                        // error propagation much?  but probably not as inaccurate
                                        // as adding powers.
                                        // probably.
                                        sigma = (static_cast<Ret_T>(count) * std::abs(power<normOrder>(sigma))
                                                   + std::abs(power<normOrder>(avg - el))
                                                 ) / static_cast<Ret_T>(count + 1);
                                        sigma = std::pow(sigma, 1. / static_cast<Ret_T>(normOrder));
                                        ++count;
                                        return sigma;
                                    }
                                    );
        return res;
    }
    template <typename T>
    template <typename Ret_T>
    Ret_T
      maths<T>::average(const std::vector<T> & vec)
    {
        // constraints
        static_assert(std::is_convertible_v<T, Ret_T>);
        static_assert(std::is_floating_point_v<Ret_T>);

        //
        std::size_t count   = 0;
        Ret_T       res = std::accumulate(
          vec.begin(),
          vec.end(),
          static_cast<Ret_T>(0.),
          [&count](Ret_T avg, T val) {
            avg = (avg * static_cast<Ret_T>(count) + static_cast<Ret_T>(val)) / static_cast<Ret_T>(count + 1);
            ++count;
            return avg;
        });
        return res;
    }
    template <typename T>
    template <typename Ret_T, int normOrder>
    Ret_T
      maths<T>::norm(const std::vector<T> & vec)
    {
        Ret_T dot = std::inner_product(vec.begin(),
                                       vec.end(),
                                       vec.begin(),
                                       static_cast<Ret_T>(0),
                                       std::plus<Ret_T>(),
                                       [](T a, [[maybe_unused]] T b) {
                                           // here b == a, so we can ignore it.
                                           return power<normOrder>(a);
                                       });
        return std::pow(dot, 1 / static_cast<Ret_T>(normOrder));
    }
    template <typename T>
    template <int exponent>
    constexpr T
      maths<T>::power(T val)
    {
        static_assert(exponent >= 0);
        if constexpr (exponent == 0)
        {
            // for base instantiation val is not used.
            // writing this is a better hack than adding
            // a [[maybe_unused]] for val, because
            // this only affects this branch.
            (void)val;
            return static_cast<T>(1);
        }
        else
        {
            return power<exponent - 1>(val) * val;
        }
    }
}  // namespace td::util

#endif  // TD_UTIL_COMMON_INL
