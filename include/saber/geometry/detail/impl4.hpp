#ifndef SABER_GEOMETRY_DETAIL_IMPL4_HPP
#define SABER_GEOMETRY_DETAIL_IMPL4_HPP

// std
#include <array>
#include <tuple>
#include <type_traits>

// saber
#include "saber/inexact.hpp"
#include "saber/geometry/detail/impl2.hpp"
#include "saber/geometry/detail/simd.hpp"

namespace saber::geometry::detail {

// Helpers
#pragma region

template<typename T>
constexpr bool Is32BitDataType() 
{
	constexpr bool kIs32Bit = (sizeof(T)*8 <= 32);
	return kIs32Bit;
}

template<typename T>
constexpr bool Is64BitDataType() 
{
	constexpr bool kIs64Bit = (sizeof(T)*8 == 64);
	return kIs64Bit;
}

#pragma endregion

template<typename T>
struct Impl4 final
{
	class Simd;

	class Scalar
	{
	public:
		// default ctor
		constexpr Scalar() = default;
		~Scalar() = default;

		// alt ctor
		constexpr Scalar(T inFirst, T inSecond, T inThird, T inFourth) :
			mTuple(inFirst, inSecond, inThird, inFourth)
		{
			// Do nothing
		}

		constexpr Scalar(const typename Impl2<T>::Scalar& inFirst, const typename Impl2<T>::Scalar& inSecond) :
			mTuple(Get<0>(inFirst), Get<1>(inFirst), Get<0>(inSecond), Get<1>(inSecond))
		{
			// Do nothing
		}

		template<std::size_t Index>
		constexpr const T& Get() const
		{
			static_assert(Index < std::tuple_size_v<decltype(mTuple)>, "Provided index out of bounds.");
			return std::get<Index>(mTuple);
		}

		template<std::size_t Index>
		constexpr T& Get()
		{
			static_assert(Index < std::tuple_size_v<decltype(mTuple)>, "Provided index out of bounds.");
			return std::get<Index>(mTuple);
		}

		constexpr void SetLo(const typename Impl2<T>::Scalar& inImpl2)
		{
			Get<0>() = inImpl2.Get<0>();
			Get<1>() = inImpl2.Get<1>();
		}

		constexpr void SetHi(const typename Impl2<T>::Scalar& inImpl2)
		{
			Get<2>() = inImpl2.Get<0>();
			Get<3>() = inImpl2.Get<1>();
		}

		constexpr Scalar& operator+=(const Scalar& inRHS)
		{
			Get<0>(mTuple) += Get<0>(inRHS.mTuple);
			Get<1>(mTuple) += Get<1>(inRHS.mTuple);
			Get<2>(mTuple) += Get<2>(inRHS.mTuple);
			Get<3>(mTuple) += Get<3>(inRHS.mTuple);
			return *this;
		}

		constexpr Scalar& operator-=(const Scalar& inRHS)
		{
			Get<0>(mTuple) -= Get<0>(inRHS.mTuple);
			Get<1>(mTuple) -= Get<1>(inRHS.mTuple);
			Get<2>(mTuple) -= Get<2>(inRHS.mTuple);
			Get<3>(mTuple) -= Get<3>(inRHS.mTuple);
			return *this;
		}

		constexpr Scalar& operator*=(const Scalar& inRHS)
		{
			Get<0>(mTuple) *= Get<0>(inRHS.mTuple);
			Get<1>(mTuple) *= Get<1>(inRHS.mTuple);
			Get<2>(mTuple) *= Get<2>(inRHS.mTuple);
			Get<3>(mTuple) *= Get<3>(inRHS.mTuple);
			return *this;
		}

		constexpr Scalar& operator/=(const Scalar& inRHS)
		{
			Get<0>(mTuple) /= Get<0>(inRHS.mTuple);
			Get<1>(mTuple) /= Get<1>(inRHS.mTuple);
			Get<2>(mTuple) /= Get<2>(inRHS.mTuple);
			Get<3>(mTuple) /= Get<3>(inRHS.mTuple);
			return *this;
		}

		constexpr bool IsEqual(const Scalar& inRHS) const
		{
			// Point said to copy/paste this section into the scalar portion
			bool result = false;
			if constexpr (std::is_floating_point_v<T>)
			{   
				// Floating point comparisons are always inexact within an epsilon
				result = Inexact::IsEq(Get<0>(mTuple), Get<0>(inRHS.mTuple)) 
							&& Inexact::IsEq(Get<1>(mTuple), Get<1>(inRHS.mTuple))
							&& Inexact::IsEq(Get<2>(mTuple), Get<2>(inRHS.mTuple))
							&& Inexact::IsEq(Get<3>(mTuple), Get<3>(inRHS.mTuple));
			}
			else
			{
				// Integer comparisons are always exact
				result = (Get<0>(mTuple) == Get<0>(inRHS.mTuple)) 
							&& (Get<1>(mTuple) == Get<1>(inRHS.mTuple))
							&& (Get<2>(mTuple) == Get<2>(inRHS.mTuple))
							&& (Get<3>(mTuple) == Get<3>(inRHS.mTuple));
			}
			return result;
		}

		constexpr void RoundNearest()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundNearest() only supports floating point types");
			
			Get<0>(mTuple) = std::round(Get<0>(mTuple));
			Get<1>(mTuple) = std::round(Get<1>(mTuple));
			Get<2>(mTuple) = std::round(Get<2>(mTuple));
			Get<3>(mTuple) = std::round(Get<3>(mTuple));
		}

		constexpr void RoundCeil()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundCeil() only supports floating point types");
			
			Get<0>(mTuple) = std::ceil(Get<0>(mTuple));
			Get<1>(mTuple) = std::ceil(Get<1>(mTuple));
			Get<2>(mTuple) = std::ceil(Get<2>(mTuple));
			Get<3>(mTuple) = std::ceil(Get<3>(mTuple));
		}

		constexpr void RoundFloor()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundFloor() only supports floating point types");
			
			Get<0>(mTuple) = std::floor(Get<0>(mTuple));
			Get<1>(mTuple) = std::floor(Get<1>(mTuple));
			Get<2>(mTuple) = std::floor(Get<2>(mTuple));
			Get<3>(mTuple) = std::floor(Get<3>(mTuple));
		}

		constexpr void RoundTrunc()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundTrunc() only supports floating point types");
			
			Get<0>(mTuple) = std::trunc(Get<0>(mTuple)); // 0 = Origin.X
			Get<1>(mTuple) = std::trunc(Get<1>(mTuple)); // 1 = Origin.Y
			Get<2>(mTuple) = std::trunc(Get<2>(mTuple)); // 2 = Size.Width
			Get<3>(mTuple) = std::trunc(Get<3>(mTuple)); // 3 = Size.Height
		}

		constexpr Scalar& Union(const Scalar& inImpl4)
		{
			// By default Scalar is in X,Y,Width,Height format
			// Convert to L,T,R,B format
			ToLTRB(*this);
			Simd ltrb = inImpl4;
			ToLTRB(ltrb);

			// Figure out the top left of the union rectangle
			Get<0>() = std::min(Get<0>(), ltrb.Get<0>()); // Min Left
			Get<1>() = std::min(Get<1>(), ltrb.Get<1>()); // Min Top

			// Figure out the bottom right of the union rectangle
			Get<2>() = std::min(Get<2>(), ltrb.Get<2>()); // Max Right
			Get<3>() = std::min(Get<3>(), ltrb.Get<3>()); // Max Bottom

			// Remember to revert back to XYWH format
			FromLTRB(*this);
			return *this;
		}

		constexpr Scalar& Intersect(const Scalar& inImpl4)
		{
			// Reverse of Union operation
			// Note: some intersection rectangles may be empty (size = 0)

			// By default Scalar is in X,Y,Width,Height format
			// Convert to L,T,R,B format
			auto lhs = ToLTRB(*this);
			auto rhs = ToLTRB(inImpl4);

			// Figure out the bottom right of the intersect rectangle
			Get<0>(lhs.mTuple) = std::max(Get<0>(lhs.mTuple), Get<0>(rhs.mTuple));
			Get<1>(lhs.mTuple) = std::max(Get<1>(lhs.mTuple), Get<1>(rhs.mTuple));

			// Figure out the top left of the intersect rectangle
			Get<2>(lhs.mTuple) = std::min(Get<2>(lhs.mTuple), Get<2>(rhs.mTuple));
			Get<3>(lhs.mTuple) = std::min(Get<3>(lhs.mTuple), Get<2>(rhs.mTuple));

			// Remember to revert back to XYWH format
			*this = FromLTRB(lhs);

			// Convert the bottom right to width and height, ensuring we do not end up with negative values
			Get<2>(mTuple) = max(Get<2>(mTuple), 0);
			Get<3>(mTuple) = max(Get<3>(mTuple), 0);
			return *this;
		}

		constexpr bool IsOverlapping(const typename Impl2<T>::Scalar& inImpl2) const
		{
			Scalar ltrb = ToLTRB(mTuple);
			bool isOverlapping = false;
			do 
			{
				// Test the x and left component
				if (inImpl2.Get<0>() < ltrb.Get<0>()) // Not greater than or equal
				{
					// Extra step needed for floating point equality
					if constexpr(std::is_floating_point_v<T>)
					{
						// Approximately equal values are contained within the rectangle
						// Eg. x = 2.99999..., left = 3.0
						if (!Inexact::Eq(inImpl2.Get<0>(), ltrb.Get<0>()))
						{
							break;
						}
						// Continue checking, since point is approximately contained within the rectangle
					}
					else
					{
						// Point is left of the rectangle
						break;
					}
				}

				// Test the y and top component
				if (inImpl2.Get<1>() < ltrb.Get<1>()) // Not greater than or equal
				{
					// Extra step needed for floating point equality
					if constexpr(std::is_floating_point_v<T>)
					{
						// Approximately equal values are contained within the rectangle
						// Eg. y = 2.99999..., top = 3.0
						if (!Inexact::Eq(inImpl2.Get<1>(), ltrb.Get<1>()))
						{
							break;
						}
						// Continue checking, since point is approximately contained within the rectangle
					}
					// Point is above the rectangle
					else
					{
						break;
					}
				}

				{
					// Test the x and right component
					if (inImpl2.Get<0>() >= ltrb.Get<2>()) // Not less than
					{
						break;
					}

					// Extra step needed for floating point equality
					if constexpr(std::is_floating_point_v<T>)
					{
						// Approximately equal values are not contained within the rectangle
						// Eg. y = 2.99999..., right = 3.0
						if (Inexact::Eq(inImpl2.Get<0>(), ltrb.Get<2>()))
						{
							break;
						}
					}
				}

				{
					// Test the y and bottom component
					if (inImpl2.Get<1>() >= ltrb.Get<3>()) // Not less than 
					{
						break;
					}

					// Extra step needed for floating point equality
					if constexpr(std::is_floating_point_v<T>)
					{
						// Approximately equal values are not contained within the rectangle
						// Eg. y = 2.99999..., bottom = 3.0
						if (Inexact::Eq(inImpl2.Get<1>(), ltrb.Get<3>()))
						{
							break;
						}
					}
				}

				// Point is contained within the rectangle
				isOverlapping = true;
			} while (false);

			return isOverlapping;
		}

		constexpr bool IsOverlapping(const Scalar& inImpl4) const
		{
			auto copy = *this;
			auto intersection = copy.Intersect(inImpl4);
			const bool isOverlapping = !IsEmpty(intersection);
		   
			return isOverlapping;
		}

	private:
		// TRICKY: There is no way to discern whether a Scalar is in LTRB or XYWH format, therefore methods are kept private
		static constexpr void ToLTRB(Scalar inXYWH)
		{
			Get<2>(inXYWH.mTuple) += Get<0>(inXYWH.mTuple);
			Get<3>(inXYWH.mTuple) += Get<1>(inXYWH.mTuple);
		}

		static constexpr void FromLTRB(Scalar inLTRB)
		{
			Get<2>(inLTRB.mTuple) -= Get<0>(inLTRB.mTuple);
			Get<3>(inLTRB.mTuple) -= Get<1>(inLTRB.mTuple);
		}

		// Friend meaning free function (and always public)
		friend constexpr bool IsEmpty(const Scalar& inScalar)
		{
			bool isEmpty = false;
			do
			{
				// check for "exact" width and height being zero or negative
				isEmpty = (inScalar.Get<2>() <= 0) || (inScalar.Get<3>() <= 0);
				if (isEmpty)
				{
					break;
				}

				if constexpr(std::is_floating_point_v<T>)
				{
					// check for "inexact" width and height being zero
					isEmpty = (Inexact::Eq(inScalar.Get<2>(), 0) || Inexact::Eq(inScalar.Get<3>(), 0));
				}

			} while (false);
		
			return isEmpty;
		}

	private:
		friend class Simd; // Permit Simd class to provide constexpr api

	private:
		std::tuple<T,T,T,T> mTuple{}; // Impl4: so 4 elements are assumed
	}; // struct Scalar

	class Simd
	{
	public:
		// default ctor
		constexpr Simd() = default;
		~Simd() = default;

		constexpr Simd(T inFirst, T inSecond, T inThird, T inFourth) :
			mArray{{inFirst, inSecond, inThird, inFourth}} 
		{
			// Do nothing
		}

		constexpr Simd(const typename Impl2<T>::Simd& inFirst, const typename Impl2<T>::Simd& inSecond)
		{
			Simd128<T>::Store2(&Get<0>(), inFirst.GetSimdType());
			Simd128<T>::Store2(&Get<2>(), inSecond.GetSimdType());
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

		constexpr void SetLo(const typename Impl2<T>::Simd& inImpl2)
		{
			Simd128<T>::Store2(&Get<0>(), inImpl2.GetSimdType());
		}

		constexpr void SetHi(const typename Impl2<T>::Simd& inImpl2)
		{
			Simd128<T>::Store2(&Get<2>(), inImpl2.GetSimdType());
		}

		// TRICKY mnfitz 04oct2025: GetSimdType() doesn't make sense for Impl4, 
		// because it is incompatible when using datatype T = double
		// constexpr auto GetSimdType() const
		// {
		// 	return Simd128<T>::Load4(&Get<0>());
		// }

		constexpr Simd& operator+=(const Simd& inRHS)
		{
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>()};
					lhs += rhs;
					Get<0>() = lhs.Get<0>();
					Get<1>() = lhs.Get<1>(); 
					Get<2>() = lhs.Get<2>();
					Get<3>() = lhs.Get<3>(); 
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&Get<0>());
					auto rhs = Simd128<T>::Load4(&inRHS.Get<0>());
					auto result = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store4(&Get<0>(), result);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					auto result = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&Get<0>(), result);

					lhs = Simd128<T>::Load2(&Get<2>());
					rhs = Simd128<T>::Load2(&inRHS.Get<2>());
					result = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&Get<2>(), result); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>()};
					lhs -= rhs;
					Get<0>() = lhs.Get<0>();
					Get<1>() = lhs.Get<1>();
					Get<2>() = lhs.Get<2>();
					Get<3>() = lhs.Get<3>();
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&Get<0>());
					auto rhs = Simd128<T>::Load4(&inRHS.Get<0>());
					auto result = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store4(&Get<0>(), result);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					auto result = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&Get<0>(), result);

					lhs = Simd128<T>::Load2(&Get<2>());
					rhs = Simd128<T>::Load2(&inRHS.Get<2>());
					result = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&Get<2>(), result); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>()};
					lhs *= rhs;
					Get<0>() = lhs.Get<0>();
					Get<1>() = lhs.Get<1>(); 
					Get<2>() = lhs.Get<2>();
					Get<3>() = lhs.Get<3>(); 
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&Get<0>());
					auto rhs = Simd128<T>::Load4(&inRHS.Get<0>());
					auto result = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store4(&Get<0>(), result);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					auto result = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&Get<0>(), result);

					lhs = Simd128<T>::Load2(&Get<2>());
					rhs = Simd128<T>::Load2(&inRHS.Get<2>());
					result = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&Get<2>(), result); // Store2 only sets the initial address and the one after; does not overwrite entire object
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);

			return *this;
		}

		constexpr Simd& operator/=(const Simd& inRHS)
		{
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>()};
					lhs /= rhs;
					Get<0>() = lhs.Get<0>();
					Get<1>() = lhs.Get<1>();
					Get<2>() = lhs.Get<2>();
					Get<3>() = lhs.Get<3>();
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&Get<0>());
					auto rhs = Simd128<T>::Load4(&inRHS.Get<0>());
					auto result = Simd128<T>::Div(lhs, rhs);
					Simd128<T>::Store4(&Get<0>(), result);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					auto result = Simd128<T>::Div(lhs, rhs);
					Simd128<T>::Store2(&Get<0>(), result);

					lhs = Simd128<T>::Load2(&Get<2>());
					rhs = Simd128<T>::Load2(&inRHS.Get<2>());
					result = Simd128<T>::Div(lhs, rhs);
					Simd128<T>::Store2(&Get<2>(), result); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					Scalar lhs{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					const Scalar rhs{inRHS.Get<0>(), inRHS.Get<1>(), inRHS.Get<2>(), inRHS.Get<3>()};
					isEqual = lhs == rhs;
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&Get<0>());
					auto rhs = Simd128<T>::Load4(&inRHS.Get<0>());
					isEqual = Simd128<T>::IsEq(lhs, rhs);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&Get<0>());
					auto rhs = Simd128<T>::Load2(&inRHS.Get<0>());
					isEqual = Simd128<T>::IsEq(lhs, rhs);

					if (isEqual)
					{
						lhs = Simd128<T>::Load2(&Get<2>());
						rhs = Simd128<T>::Load2(&inRHS.Get<2>());
						//isEqual = isEqual && Simd128<T>::IsEq(lhs, rhs);
						isEqual = isEqual && (Simd128<T>::IsEq2(lhs, rhs) == 15);
					}
				}
				else
				{
					static_assert("Unsupported T");
				}

			} while (false);

			return isEqual;
		}

		constexpr void RoundNearest()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundNearest() only supports floating point types");
			
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					const Scalar round{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					round.RoundNearest();
					Get<0>() = Get<0>(round.mTuple);
					Get<1>() = Get<1>(round.mTuple);
					Get<2>() = Get<2>(round.mTuple);
					Get<3>() = Get<3>(round.mTuple);
					break;
				}
#endif // __cpp_lib_is_constant_evaluated
 
				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&Get<0>());
					round = Simd128<T>::RoundNearest(round);
					Simd128<T>::Store4(&Get<0>(), round);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&Get<0>());
					round1 = Simd128<T>::RoundNearest(round1);
					Simd128<T>::Store2(&Get<0>(), round1);

					auto round2 = Simd128<T>::Load2(&Get<2>());
					round2 = Simd128<T>::RoundNearest(round2);
					Simd128<T>::Store2(&Get<2>(), round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);
		}

		constexpr void RoundCeil()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundCeil() only supports floating point types");
			
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					const Scalar round{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					round.RoundCeil();
					Get<0>() = Get<0>(round.mTuple);
					Get<1>() = Get<1>(round.mTuple);
					Get<2>() = Get<2>(round.mTuple);
					Get<3>() = Get<3>(round.mTuple);
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&Get<0>());
					round = Simd128<T>::RoundCeil(round);
					Simd128<T>::Store4(&Get<0>(), round);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&Get<0>());
					round1 = Simd128<T>::RoundCeil(round1);
					Simd128<T>::Store2(&Get<0>(), round1);

					auto round2 = Simd128<T>::Load2(&Get<2>());
					round2 = Simd128<T>::RoundCeil(round2);
					Simd128<T>::Store2(&Get<2>(), round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);
		}

		constexpr void RoundFloor()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundFloor() only supports floating point types");
			
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					const Scalar round{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					round.RoundFloor();
					Get<0>() = Get<0>(round.mTuple);
					Get<1>() = Get<1>(round.mTuple);
					Get<2>() = Get<2>(round.mTuple);
					Get<3>() = Get<3>(round.mTuple);
					break;
				}
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&Get<0>());
					round = Simd128<T>::RoundFloor(round);
					Simd128<T>::Store4(&Get<0>(), round);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&Get<0>());
					round1 = Simd128<T>::RoundFloor(round1);
					Simd128<T>::Store2(&Get<0>(), round1);

					auto round2 = Simd128<T>::Load2(&Get<2>());
					round2 = Simd128<T>::RoundFloor(round2);
					Simd128<T>::Store2(&Get<2>(), round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);
		}

		constexpr void RoundTrunc()
		{
			constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
			static_assert(isFloatingPoint, "RoundTrunc() only supports floating point types");
			
			// protect our interface so it can remain constexpr
			do 
			{
#if __cpp_lib_is_constant_evaluated
				if (std::is_constant_evaluated())
				{
					// Delegate to Scalar Impl which is constexpr capable
					const Scalar round{Get<0>(), Get<1>(), Get<2>(), Get<3>()};
					round.RoundTrunc();
					Get<0>() = Get<0>(round.mTuple);
					Get<1>() = Get<1>(round.mTuple);
					Get<2>() = Get<2>(round.mTuple);
					Get<3>() = Get<3>(round.mTuple);
					break;
				}
#endif // __cpp_lib_is_constant_evaluated
 
				if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&Get<0>());
					round = Simd128<T>::RoundTrunc(round);
					Simd128<T>::Store4(&Get<0>(), round);
				}
				else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&Get<0>());
					round1 = Simd128<T>::RoundTrunc(round1);
					Simd128<T>::Store2(&Get<0>(), round1);

					auto round2 = Simd128<T>::Load2(&Get<2>());
					round2 = Simd128<T>::RoundTrunc(round2);
					Simd128<T>::Store2(&Get<2>(), round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
				}
				else
				{
					static_assert("Unsupported T");
				}
			} while (false);
		}

		constexpr Simd& Union(const Simd& inImpl4)
		{
			if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
			{
				ToLTRB(*this);
				Simd ltrb = inImpl4;
				ToLTRB(ltrb);

				auto lhs = Simd128<T>::Load4(&Get<0>());
				auto rhs = Simd128<T>::Load4(&ltrb.Get<0>());
				auto result = Simd128<T>::MinMax(lhs, rhs);
				Simd128<T>::Store4(&Get<0>(), result);
				FromLTRB(*this);
			}
			else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
			{
				// Find the minimum of the left and top values
				Simd ltrb = inImpl4;

				auto lt1 = Simd128<T>::Load2(&Get<0>());
				auto lt2 = Simd128<T>::Load2(&ltrb.Get<0>());
				auto result = Simd128<T>::Min(lt1, lt2);
				Simd128<T>::Store2(&Get<0>(), result);

				// Find the maximum of the right and bottom values
				// NOTE: Requires ToLTRB() conversion of width/height to right/bottom
				ToLTRB(*this);
				ToLTRB(ltrb);
				auto rb1 = Simd128<T>::Load2(&Get<2>());
				auto rb2 = Simd128<T>::Load2(&ltrb.Get<2>());
				result = Simd128<T>::Max(rb1, rb2);

				Simd128<T>::Store2(&Get<2>(), result);
				// Now convert right and bottom to width and height
				FromLTRB(*this);
			}
			else
			{
				static_assert("Unsupported T");
			}
			
			return *this;
		}

		constexpr Simd& Intersect(const Simd& inImpl4)
		{
			if constexpr (Is32BitDataType<T>()) // Int/Float up to 32 bit data type
			{
				ToLTRB(*this);
				Simd ltrb = inImpl4;
				ToLTRB(ltrb);

				auto lhs = Simd128<T>::Load4(&Get<0>());
				auto rhs = Simd128<T>::Load4(&ltrb.Get<0>());
				auto intersection = Simd128<T>::MaxMin(lhs, rhs);
				Simd128<T>::Store4(&Get<0>(), intersection);
				FromLTRB(*this);
			}
			else if constexpr (Is64BitDataType<T>()) // Double up to 64 bit data type
			{
				Simd ltrb = inImpl4;
				// Find the maximum of the left and top values
				auto lt1 = Simd128<T>::Load2(&Get<0>());
				auto lt2 = Simd128<T>::Load2(&ltrb.Get<0>());
				auto intersectionLT = Simd128<T>::Max(lt1, lt2);

				// Find the minimum of the right and bottom values
				// NOTE: Requires ToLTRB() conversion of width/height to right/bottom
				ToLTRB(*this);
				ToLTRB(ltrb);
				auto rb1 = Simd128<T>::Load2(&Get<2>());
				auto rb2 = Simd128<T>::Load2(&ltrb.Get<2>());
				auto intersectionRB = Simd128<T>::Min(rb1, rb2);

				Simd128<T>::Store2(&Get<0>(), intersectionLT);
				Simd128<T>::Store2(&Get<2>(), intersectionRB);
				// Now convert right and bottom to width and height
				FromLTRB(*this);
			}
			else
			{
				static_assert("Unsupported T");
			}
			
			return *this;
		}

		constexpr bool IsOverlapping(const typename Impl2<T>::Simd& inImpl2) const
		{
			bool isOverlapping = false;
			Simd ltrb = *this;
			ToLTRB(ltrb);

			auto lt = Simd128<T>::Load2(&ltrb.Get<0>());
			auto rb = Simd128<T>::Load2(&ltrb.Get<2>());
			auto xy = inImpl2.GetSimdType(); // Get the underlying Simd value

			if constexpr (Is32BitDataType<T>()) // int/float up to 32 bit data type
			{
				// Fix high 2 elements, since they're zeroed and can screw up comparison
				lt = Simd128<T>::DupLo(lt);
				rb = Simd128<T>::DupLo(rb);
				xy = Simd128<T>::DupLo(xy);
			}
			
			do
			{
				if (!Simd128<T>::IsGe(xy, lt)) // Fancy way of saying xy < lt
				{
					// Impl2 is too far to the left/above the Impl4
					break;
				}

				//if (Simd128<T>::IsGe(xy, rb)) 
				if (Simd128<T>::IsGe2(xy, rb) == 15) 
				{
					// Impl2 is too far to the right/below/touching the Impl4
					break;
				}
				isOverlapping = true;

			} while (false);
			return isOverlapping;
		}

		constexpr bool IsOverlapping(const Simd& inImpl4) const
		{
			Simd copy = *this;
			auto intersection = copy.Intersect(inImpl4);
			const bool isOverlapping = !IsEmpty(intersection);
		   
			return isOverlapping;
		}

	private:
		static constexpr void ToLTRB(Simd& inXYWH)
		{
			auto lt = Simd128<T>::Load2(&inXYWH.Get<0>());
			auto wh = Simd128<T>::Load2(&inXYWH.Get<2>());
			auto rb = Simd128<T>::Add(lt, wh);
			Simd128<T>::Store2(&inXYWH.Get<2>(), rb);
		}

		static constexpr void FromLTRB(Simd& inLTRB)
		{
			auto lt = Simd128<T>::Load2(&inLTRB.Get<0>());
			auto rb = Simd128<T>::Load2(&inLTRB.Get<2>());
			auto wh = Simd128<T>::Sub(rb, lt);
			Simd128<T>::Store2(&inLTRB.Get<2>(), wh);
		}

		friend constexpr bool IsEmpty(const Simd& inSimd)
		{
			constexpr Simd kZero{};
			auto width = Simd128<T>::Load1(&inSimd.Get<2>());
			auto height = Simd128<T>::Load1(&inSimd.Get<3>());
			auto min = Simd128<T>::Min(width, height);
			auto zero = Simd128<T>::Load2(&kZero.Get<0>());
			// TRICKY: IsLe() expects to compare Impl4 elements, but we pass in Impl2
			// The logic works, since the empty zeroes will not affect the Le check
			//bool isEmpty = Simd128<T>::IsLe(min, zero);
			bool isEmpty = (Simd128<T>::IsLe2(min, zero) == 15);
			return isEmpty;
		}

	private:
		std::array<T,4> mArray{}; // Impl4: so 4 elements are assumed
	}; // class Simd
}; // struct Impl4<>

#pragma region struct Impl4Traits
template<typename T, ImplKind Impl> // Primary template declaration
struct Impl4Traits;

template<typename T> // Partial template specialization
struct Impl4Traits<T, ImplKind::kScalar>
{
	using ImplType = typename Impl4<T>::Scalar; // VOODOO: Nested template type requires `typename` prefix
};

template<typename T> // Partial template specialization
struct Impl4Traits<T, ImplKind::kSimd>
{
	using ImplType = typename Impl4<T>::Simd; // VOODOO: Nested template type requires `typename` prefix
};

#pragma endregion

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL4_HPP