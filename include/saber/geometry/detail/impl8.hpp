#ifndef SABER_GEOMETRY_DETAIL_IMPL8_HPP
#define SABER_GEOMETRY_DETAIL_IMPL8_HPP

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/config.hpp"
#include "saber/geometry/detail/impl2.hpp"
#include "saber/geometry/detail/impl4.hpp"

// std
#include <array>
#include <type_traits>

namespace saber::geometry::detail {

template<typename T>
struct Impl8 final
{
	class Scalar
	{
	public:
		// default ctor
		constexpr Scalar() = default;
		~Scalar() = default;

		// alt ctor
		constexpr Scalar(T inFirst, T inSecond, T inThird, T inFourth, T inFifth, T inSixth, T inSeventh, T inEigth) :
			mArray{inFirst, inSecond, inThird, inFourth, inFifth, inSixth, inSeventh, inEigth}
		{
			// Do nothing
		}

		constexpr Scalar(const typename Impl2<T>::Scalar& inFirst, const typename Impl2<T>::Scalar& inSecond, const typename Impl2<T>::Scalar& inThird, const typename Impl2<T>::Scalar& inFourth) :
			mArray{inFirst.template Get<0>(), inFirst.template Get<1>(), inSecond.template Get<0>(), inSecond.template Get<1>(), inThird.template Get<0>(), inThird.template Get<1>(), inFourth.template Get<0>(), inFourth.template Get<1>()}
		{
			// Do nothing
		}

		constexpr Scalar(const typename Impl4<T>::Scalar& inFirst, const typename Impl4<T>::Scalar& inSecond) :
			mArray{inFirst.template Get<0>(), inFirst.template Get<1>(), inFirst.template Get<2>(), inFirst.template Get<3>(), inSecond.template Get<0>(), inSecond.template Get<1>(), inSecond.template Get<2>(), inSecond.template Get<3>()}
		{
			// Do nothing
		}

		template<std::size_t Index>
		constexpr const T& Get() const
		{
			static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
			return mArray.at(Index);
		}

		template<std::size_t Index>
		constexpr T& Get()
		{
			static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
			return mArray.at(Index);
		}

		constexpr Scalar& operator+=(const Scalar& inRHS)
		{
			Get<0>() += inRHS.Get<0>();
			Get<1>() += inRHS.Get<1>();
			Get<2>() += inRHS.Get<2>();
			Get<3>() += inRHS.Get<3>();
			Get<4>() += inRHS.Get<4>();
			Get<5>() += inRHS.Get<5>();
			Get<6>() += inRHS.Get<6>();
			Get<7>() += inRHS.Get<7>();
			return *this;
		}

		constexpr Scalar& operator-=(const Scalar& inRHS)
		{
			Get<0>() -= inRHS.Get<0>();
			Get<1>() -= inRHS.Get<1>();
			Get<2>() -= inRHS.Get<2>();
			Get<3>() -= inRHS.Get<3>();
			Get<4>() -= inRHS.Get<4>();
			Get<5>() -= inRHS.Get<5>();
			Get<6>() -= inRHS.Get<6>();
			Get<7>() -= inRHS.Get<7>();
			return *this;
		}

		constexpr Scalar& operator*=(const Scalar& inRHS)
		{
			Get<0>() *= inRHS.Get<0>();
			Get<1>() *= inRHS.Get<1>();
			Get<2>() *= inRHS.Get<2>();
			Get<3>() *= inRHS.Get<3>();
			Get<4>() *= inRHS.Get<4>();
			Get<5>() *= inRHS.Get<5>();
			Get<6>() *= inRHS.Get<6>();
			Get<7>() *= inRHS.Get<7>();
			return *this;
		}

		constexpr bool IsEqual(const Scalar& inRHS) const
		{
			// Point said to copy/paste this section into the scalar portion
			bool result = false;
			if constexpr (std::is_floating_point_v<T>)
			{   
				// Floating point comparisons are always inexact within an epsilon
				result = Inexact::IsEq(Get<0>(), inRHS.Get<0>()) 
							&& Inexact::IsEq(Get<1>(), inRHS.Get<1>())
							&& Inexact::IsEq(Get<2>(), inRHS.Get<2>())
							&& Inexact::IsEq(Get<3>(), inRHS.Get<3>())
							&& Inexact::IsEq(Get<4>(), inRHS.Get<4>())
							&& Inexact::IsEq(Get<5>(), inRHS.Get<5>())
							&& Inexact::IsEq(Get<6>(), inRHS.Get<6>())
							&& Inexact::IsEq(Get<7>(), inRHS.Get<7>());
			}
			else
			{
				// Integer comparisons are always exact
				result = (Get<0>() == inRHS.Get<0>()) 
							&& (Get<1>() == inRHS.Get<1>())
							&& (Get<2>() == inRHS.Get<2>())
							&& (Get<3>() == inRHS.Get<3>())
							&& (Get<4>() == inRHS.Get<4>())
							&& (Get<5>() == inRHS.Get<5>())
							&& (Get<6>() == inRHS.Get<6>())
							&& (Get<7>() == inRHS.Get<7>());
			}
			return result;
		}

	private:
		//friend class Simd; // Permit Simd class to provide constexpr api

	private:
		std::array<T, 8> mArray; // Impl8: so 8 elements are assumed
	}; // struct Scalar

	class Simd
	{
	public:
		// default ctor
		constexpr Simd() = default;
		~Simd() = default;

		constexpr Simd(T inFirst, T inSecond, T inThird, T inFourth, T inFifth, T inSixth, T inSeventh, T inEigth) :
			mArray{{inFirst, inSecond, inThird, inFourth, inFifth, inSixth, inSeventh, inEigth}}
		{
			// Do nothing
		}

		constexpr Simd(const typename Impl2<T>::Simd& a, const typename Impl2<T>::Simd& b, const typename Impl2<T>::Simd& c, const typename Impl2<T>::Simd& d)
		{
			// Each Impl2::Simd contains 2 elements. Store them at 0,2,4,6
			Simd128<T>::Store2(&Get<0>(), a.GetSimdType());
			Simd128<T>::Store2(&Get<2>(), b.GetSimdType());
			Simd128<T>::Store2(&Get<4>(), c.GetSimdType());
			Simd128<T>::Store2(&Get<6>(), d.GetSimdType());
		}

		constexpr Simd(const typename Impl4<T>::Simd& lo, const typename Impl4<T>::Simd& hi)
		{
			// Each Impl4::Simd contains 4 elements. Store lo at 0..3 and hi at 4..7
			Simd128<T>::Store4(&Get<0>(), lo.GetSimdType());
			Simd128<T>::Store4(&Get<4>(), hi.GetSimdType());
		}

		template<std::size_t Index>
		constexpr const T& Get() const
		{
			static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
			return mArray[Index];
		}

		template<std::size_t Index>
		constexpr T& Get()
		{
			static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
			return mArray[Index];
		}

		constexpr Simd& operator+=(const Simd& inRHS)
		{
			do
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>(), Get<4>(), Get<5>(), Get<6>(), Get<7>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>(), inRHS.Get<4>(), inRHS.Get<5>(), inRHS.Get<6>(), inRHS.Get<7>()};
					lhs += rhs;
					Get<0>() = lhs.Get<0>(); Get<1>() = lhs.Get<1>(); Get<2>() = lhs.Get<2>(); Get<3>() = lhs.Get<3>();
					Get<4>() = lhs.Get<4>(); Get<5>() = lhs.Get<5>(); Get<6>() = lhs.Get<6>(); Get<7>() = lhs.Get<7>();
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>())
				{
					auto lhs1 = Simd128<T>::Load4(&Get<0>());
					auto rhs1 = Simd128<T>::Load4(&inRHS.Get<0>());
					auto res1 = Simd128<T>::Add(lhs1, rhs1);
					Simd128<T>::Store4(&Get<0>(), res1);

					auto lhs2 = Simd128<T>::Load4(&Get<4>());
					auto rhs2 = Simd128<T>::Load4(&inRHS.Get<4>());
					auto res2 = Simd128<T>::Add(lhs2, rhs2);
					Simd128<T>::Store4(&Get<4>(), res2);
				}
				else if constexpr (Is64BitDataType<T>())
				{
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					auto res = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&Get<0>(), res);

					lhs = Simd128<T>::Load2(&Get<2>());
					rhs = Simd128<T>::Load2(&inRHS.Get<2>());
					res = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&Get<2>(), res);

					lhs = Simd128<T>::Load2(&Get<4>());
					rhs = Simd128<T>::Load2(&inRHS.Get<4>());
					res = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&Get<4>(), res);

					lhs = Simd128<T>::Load2(&Get<6>());
					rhs = Simd128<T>::Load2(&inRHS.Get<6>());
					res = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&Get<6>(), res);
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);

			return *this;
		}

		constexpr Simd& operator-=(const Simd& inRHS)
		{
			do
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>(), Get<4>(), Get<5>(), Get<6>(), Get<7>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>(), inRHS.Get<4>(), inRHS.Get<5>(), inRHS.Get<6>(), inRHS.Get<7>()};
					lhs -= rhs;
					Get<0>() = lhs.Get<0>(); Get<1>() = lhs.Get<1>(); Get<2>() = lhs.Get<2>(); Get<3>() = lhs.Get<3>();
					Get<4>() = lhs.Get<4>(); Get<5>() = lhs.Get<5>(); Get<6>() = lhs.Get<6>(); Get<7>() = lhs.Get<7>();
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>())
				{
					auto lhs1 = Simd128<T>::Load4(&Get<0>());
					auto rhs1 = Simd128<T>::Load4(&inRHS.Get<0>());
					auto res1 = Simd128<T>::Sub(lhs1, rhs1);
					Simd128<T>::Store4(&Get<0>(), res1);

					auto lhs2 = Simd128<T>::Load4(&Get<4>());
					auto rhs2 = Simd128<T>::Load4(&inRHS.Get<4>());
					auto res2 = Simd128<T>::Sub(lhs2, rhs2);
					Simd128<T>::Store4(&Get<4>(), res2);
				}
				else if constexpr (Is64BitDataType<T>())
				{
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					auto res = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&Get<0>(), res);

					lhs = Simd128<T>::Load2(&Get<2>());
					rhs = Simd128<T>::Load2(&inRHS.Get<2>());
					res = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&Get<2>(), res);

					lhs = Simd128<T>::Load2(&Get<4>());
					rhs = Simd128<T>::Load2(&inRHS.Get<4>());
					res = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&Get<4>(), res);

					lhs = Simd128<T>::Load2(&Get<6>());
					rhs = Simd128<T>::Load2(&inRHS.Get<6>());
					res = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&Get<6>(), res);
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);

			return *this;
		}

		constexpr Simd& operator*=(const Simd& inRHS)
		{
			do
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>(), Get<4>(), Get<5>(), Get<6>(), Get<7>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>(), inRHS.Get<4>(), inRHS.Get<5>(), inRHS.Get<6>(), inRHS.Get<7>()};
					lhs *= rhs;
					Get<0>() = lhs.Get<0>(); Get<1>() = lhs.Get<1>(); Get<2>() = lhs.Get<2>(); Get<3>() = lhs.Get<3>();
					Get<4>() = lhs.Get<4>(); Get<5>() = lhs.Get<5>(); Get<6>() = lhs.Get<6>(); Get<7>() = lhs.Get<7>();
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>())
				{
					auto lhs1 = Simd128<T>::Load4(&Get<0>());
					auto rhs1 = Simd128<T>::Load4(&inRHS.Get<0>());
					auto res1 = Simd128<T>::Mul(lhs1, rhs1);
					Simd128<T>::Store4(&Get<0>(), res1);

					auto lhs2 = Simd128<T>::Load4(&Get<4>());
					auto rhs2 = Simd128<T>::Load4(&inRHS.Get<4>());
					auto res2 = Simd128<T>::Mul(lhs2, rhs2);
					Simd128<T>::Store4(&Get<4>(), res2);
				}
				else if constexpr (Is64BitDataType<T>())
				{
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					auto res = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&Get<0>(), res);

					lhs = Simd128<T>::Load2(&Get<2>());
					rhs = Simd128<T>::Load2(&inRHS.Get<2>());
					res = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&Get<2>(), res);

					lhs = Simd128<T>::Load2(&Get<4>());
					rhs = Simd128<T>::Load2(&inRHS.Get<4>());
					res = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&Get<4>(), res);

					lhs = Simd128<T>::Load2(&Get<6>());
					rhs = Simd128<T>::Load2(&inRHS.Get<6>());
					res = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&Get<6>(), res);
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);

			return *this;
		}

		constexpr bool IsEqual(const Simd& inRHS) const
		{
			bool isEqual = false;
			do
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar for constexpr
					const Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>(), Get<4>(), Get<5>(), Get<6>(), Get<7>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>(), inRHS.Get<4>(), inRHS.Get<5>(), inRHS.Get<6>(), inRHS.Get<7>()};
					isEqual = lhs.IsEqual(rhs);
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>())
				{
					auto lhs1 = Simd128<T>::Load4(&Get<0>());
					auto rhs1 = Simd128<T>::Load4(&inRHS.Get<0>());
					isEqual = Simd128<T>::IsEq(lhs1, rhs1);
					if (isEqual)
					{
						auto lhs2 = Simd128<T>::Load4(&Get<4>());
						auto rhs2 = Simd128<T>::Load4(&inRHS.Get<4>());
						isEqual = Simd128<T>::IsEq(lhs2, rhs2);
					}
				}
				else if constexpr (Is64BitDataType<T>())
				{
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					isEqual = Simd128<T>::IsEq(lhs, rhs);
					if (isEqual)
					{
						lhs = Simd128<T>::Load2(&Get<2>());
						rhs = Simd128<T>::Load2(&inRHS.Get<2>());
						isEqual = Simd128<T>::IsEq(lhs, rhs);
					}
					if (isEqual)
					{
						lhs = Simd128<T>::Load2(&Get<4>());
						rhs = Simd128<T>::Load2(&inRHS.Get<4>());
						isEqual = Simd128<T>::IsEq(lhs, rhs);
					}
					if (isEqual)
					{
						lhs = Simd128<T>::Load2(&Get<6>());
						rhs = Simd128<T>::Load2(&inRHS.Get<6>());
						isEqual = Simd128<T>::IsEq(lhs, rhs);
					}
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);

			return isEqual;
		}

	private:
		std::array<T,8> mArray{}; // Impl8: so 8 elements are assumed
	};

}; // struct Impl8<>

#pragma region struct Impl8Traits
template<typename T, ImplKind Impl> // Primary template declaration
struct Impl8Traits;

template<typename T> // Partial template specialization
struct Impl8Traits<T, ImplKind::kScalar>
{
	using ImplType = typename Impl8<T>::Scalar; // VOODOO: Nested template type requires `typename` prefix
};

template<typename T> // Partial template specialization
struct Impl8Traits<T, ImplKind::kSimd>
{
	using ImplType = typename Impl8<T>::Simd; // VOODOO: Nested template type requires `typename` prefix
};

#pragma endregion

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL8_HPP#endif // SABER_GEOMETRY_DETAIL_IMPL8_HPP