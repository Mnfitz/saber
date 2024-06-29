#ifndef SABER_UTILITY_HPP
#define SABER_UTILITY_HPP
#pragma once

namespace saber {

namespace detail {

// "primary" template class declaration (2 type args)
template<typename ToType, typename FromType>
struct ConvertTraits
{
    // Return a |ToType| as converted from |FromType|
	ToType operator()(const FromType& inValue) const;
};

} // namespace detail

// "public" template function
template<typename ToType, typename FromType>
inline ToType ConvertTo(const FromType& inValue)
{
	constexpr auto convert = detail::ConvertTraits<ToType, FromType>{}; // "handoff" functor
	return convert(inValue); // operator()
}

// ------
/*
// All clients of conversions must provide
// "full specialization" of template class definition.
// Like this:
template<>
struct ConvertTraits<std::tuple<int, int>, std::array<float, 2>>
{
    std::tuple<int, int> operator()(const std::array<float, 2>& inValue) const
    {
        auto tuple = std::make_tuple(
            static_cast<int>(inValue.at(0)),
            static_cast<int>(inValue.at(1)));
        return tuple;
    }
};

// Then, use it:
const std::array<float, 2> array{3, 4};

// technically, you call it like this:
const auto tuple = saber::ConvertTo<std::tuple<int, int>, std::array<float, 2>>(array);

// but c++ is smart, and knows how to deduce
// the FromType because of the typeof(parameter)...
// so you can do this:
const auto tuple2 = saber::ConvertTo<std::tuple<int, int>>(array);
*/

} // namepsace saber

#endif // SABER_UTILITY_HPP