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
            mTuple(std::get<0>(inFirst), std::get<1>(inFirst), std::get<0>(inSecond), std::get<1>(inSecond))
        {
            // Do nothing
        }

        template<std::size_t Index>
        constexpr T Get() const
        {
            static_assert(Index < std::tuple_size_v<decltype(mTuple)>, "Provided index out of bounds.");
            return std::get<Index>(mTuple);
        }

        template<std::size_t Index>
        constexpr void Set(T inT)
        {
            static_assert(Index < std::tuple_size_v<decltype(mTuple)>, "Provided index out of bounds.");
            std::get<Index>(mTuple) = inT;
        }

        constexpr Scalar& operator+=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) += std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) += std::get<1>(inRHS.mTuple);
			std::get<2>(mTuple) += std::get<2>(inRHS.mTuple);
            std::get<3>(mTuple) += std::get<3>(inRHS.mTuple);
            return *this;
        }

        constexpr Scalar& operator-=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) -= std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) -= std::get<1>(inRHS.mTuple);
			std::get<2>(mTuple) -= std::get<2>(inRHS.mTuple);
            std::get<3>(mTuple) -= std::get<3>(inRHS.mTuple);
            return *this;
        }

        constexpr Scalar& operator*=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) *= std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) *= std::get<1>(inRHS.mTuple);
			std::get<2>(mTuple) *= std::get<2>(inRHS.mTuple);
            std::get<3>(mTuple) *= std::get<3>(inRHS.mTuple);
            return *this;
        }

        constexpr Scalar& operator/=(const Scalar& inRHS)
        {
            std::get<0>(mTuple) /= std::get<0>(inRHS.mTuple);
            std::get<1>(mTuple) /= std::get<1>(inRHS.mTuple);
			std::get<2>(mTuple) /= std::get<2>(inRHS.mTuple);
            std::get<3>(mTuple) /= std::get<3>(inRHS.mTuple);
            return *this;
        }

        constexpr bool IsEqual(const Scalar& inRHS) const
        {
            // Point said to copy/paste this section into the scalar portion
            bool result = false;
            if constexpr (std::is_floating_point_v<T>)
            {   
                // Floating point comparisons are always inexact within an epsilon
                result = Inexact::IsEq(std::get<0>(mTuple), std::get<0>(inRHS.mTuple)) 
							&& Inexact::IsEq(std::get<1>(mTuple), std::get<1>(inRHS.mTuple))
							&& Inexact::IsEq(std::get<2>(mTuple), std::get<2>(inRHS.mTuple))
							&& Inexact::IsEq(std::get<3>(mTuple), std::get<3>(inRHS.mTuple));
            }
            else
            {
                // Integer comparisons are always exact
                result = (std::get<0>(mTuple) == std::get<0>(inRHS.mTuple)) 
							&& (std::get<1>(mTuple) == std::get<1>(inRHS.mTuple))
							&& (std::get<2>(mTuple) == std::get<2>(inRHS.mTuple))
							&& (std::get<3>(mTuple) == std::get<3>(inRHS.mTuple));
            }
            return result;
        }

        constexpr void RoundNearest()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundNearest() only supports floating point types");
            
            std::get<0>(mTuple) = std::round(std::get<0>(mTuple));
            std::get<1>(mTuple) = std::round(std::get<1>(mTuple));
			std::get<2>(mTuple) = std::round(std::get<2>(mTuple));
            std::get<3>(mTuple) = std::round(std::get<3>(mTuple));
        }

        constexpr void RoundCeil()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundCeil() only supports floating point types");
            
            std::get<0>(mTuple) = std::ceil(std::get<0>(mTuple));
            std::get<1>(mTuple) = std::ceil(std::get<1>(mTuple));
			std::get<2>(mTuple) = std::ceil(std::get<2>(mTuple));
            std::get<3>(mTuple) = std::ceil(std::get<3>(mTuple));
        }

        constexpr void RoundFloor()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundFloor() only supports floating point types");
            
            std::get<0>(mTuple) = std::floor(std::get<0>(mTuple));
            std::get<1>(mTuple) = std::floor(std::get<1>(mTuple));
			std::get<2>(mTuple) = std::floor(std::get<2>(mTuple));
            std::get<3>(mTuple) = std::floor(std::get<3>(mTuple));
        }

        constexpr void RoundTrunc()
        {
            constexpr bool isFloatingPoint = std::is_floating_point_v<T>;
            static_assert(isFloatingPoint, "RoundTrunc() only supports floating point types");
            
            std::get<0>(mTuple) = std::trunc(std::get<0>(mTuple)); // 0 = Origin.X
            std::get<1>(mTuple) = std::trunc(std::get<1>(mTuple)); // 1 = Origin.Y
			std::get<2>(mTuple) = std::trunc(std::get<2>(mTuple)); // 2 = Size.Width
            std::get<3>(mTuple) = std::trunc(std::get<3>(mTuple)); // 3 = Size.Height
        }

        constexpr Scalar& Union(const Scalar& inImpl4)
        {
            // By default Scalar is in X,Y,Width,Height format
            // Convert to L,T,R,B format
            auto lhs = ToLTRB(*this);
            auto rhs = ToLTRB(inImpl4);

            // Figure out the top left of the union rectangle
            std::get<0>(lhs.mTuple) = std::min(std::get<0>(lhs.mTuple), std::get<0>(rhs.mTuple)); // Min Left
            std::get<1>(lhs.mTuple) = std::min(std::get<1>(lhs.mTuple), std::get<1>(rhs.mTuple)); // Min Top

            // Figure out the bottom right of the union rectangle
            std::get<2>(lhs.mTuple) = std::max(std::get<2>(lhs.mTuple), std::get<2>(rhs.mTuple)); // Max Right
            std::get<3>(lhs.mTuple) = std::max(std::get<3>(lhs.mTuple), std::get<3>(rhs.mTuple)); // Max Bottom

            // Remember to revert back to XYWH format
            *this = ToXYWH(lhs);
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
            std::get<0>(lhs.mTuple) = std::max(std::get<0>(lhs.mTuple), std::get<0>(rhs.mTuple));
            std::get<1>(lhs.mTuple) = std::max(std::get<1>(lhs.mTuple), std::get<1>(rhs.mTuple));

            // Figure out the top left of the intersect rectangle
            std::get<2>(lhs.mTuple) = std::min(std::get<2>(lhs.mTuple), std::get<2>(rhs.mTuple));
            std::get<3>(lhs.mTuple) = std::min(std::get<3>(lhs.mTuple), std::get<2>(rhs.mTuple));

            // Remember to revert back to XYWH format
            *this = ToXYWH(lhs);

            // Convert the bottom right to width and height, ensuring we do not end up with negative values
            std::get<2>(mTuple) = max(std::get<2>(mTuple), 0);
            std::get<3>(mTuple) = max(std::get<3>(mTuple), 0);
            return *this;
        }

        constexpr bool IsOverlapping(const typename Impl2<T>::Scalar& inImpl2)
        {
            // TODO: Change all std::get() to use Get() and remove .mTuple

            Scalar ltrb = ToLTRB(mTuple);
            bool isOverlapping = false;
            do 
            {
                // Test the x and left component
                if (Get<0>(inImpl2) < Get<0>(ltrb)) // Not greater than or equal
                {
                    // Extra step needed for floating point equality
                    if constexpr(std::is_floating_point_v<T>)
                    {
                        // Approximately equal values are contained within the rectangle
                        // Eg. x = 2.99999_, left = 3.0
                        if (!Inexact::Eq(Get<0>(inImpl2), Get<0>(ltrb)))
                        {
                            break;
                        }
                        // Continue checking, since point is approximately contained within the rectangle
                    }
                    // Point is left of the rectangle
                    else
                    {
                        break;
                    }
                }

                // Test the y and top component
                if (Get<1>(inImpl2) < Get<1>(ltrb)) // Not greater than or equal
                {
                    // Extra step needed for floating point equality
                    if constexpr(std::is_floating_point_v<T>)
                    {
                        // Approximately equal values are contained within the rectangle
                        // Eg. y = 2.99999_, top = 3.0
                        if (!Inexact::Eq(Get<1>(inImpl2), Get<1>(ltrb)))
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

                // Test the x and right component
                if (Get<0>(inImpl2) >= Get<2>(ltrb)) // Not less than
                {
                    // Extra step needed for floating point equality
                    if constexpr(std::is_floating_point_v<T>)
                    {
                        // Approximately equal values are contained within the rectangle
                        // Eg. x = 2.99999_, right = 3.0
                        if (Inexact::Eq(Get<0>(inImpl2), Get<2>(ltrb)))
                        {
                            break;
                        }
                        // Continue checking, since point is approximately contained within the rectangle
                    }
                    // Point is right of the rectangle
                    else
                    {
                        break;
                    }
                }

                // Test the y and bottom component
                if (Get<1>(inImpl2) >= Get<3>(ltrb)) // Not less than 
                {
                    // Extra step needed for floating point equality
                    if constexpr(std::is_floating_point_v<T>)
                    {
                        // Approximately equal values are contained within the rectangle
                        // Eg. y = 2.99999_, bottom = 3.0
                        if (Inexact::Eq(Get<1>(inImpl2), Get<3>(ltrb)))
                        {
                            break;
                        }
                        // Continue checking, since point is approximately contained within the rectangle
                    }
                    // Point is below the rectangle
                    else
                    {
                        break;
                    }
                }

                // Point is contained within the rectangle
                isOverlapping = true;
            } while (false);

            return isOverlapping;
        }

        constexpr bool IsOverlapping(const Scalar& inImpl4)
        {
            auto intersection = Intersect(mTuple, inImpl4);
            bool result = !IsEmpty(intersection);
           
            return result;
        }

    private:
        // TRICKY: There is no way to discern whether a Scalar is in LTRB or XYWH format, therefore methods are kept private
        static constexpr Scalar ToLTRB(const Scalar& inXYWH)
        {
            Scalar ltrb = inXYWH;
            std::get<2>(ltrb.mTuple) += std::get<0>(ltrb.mTuple);
            std::get<3>(ltrb.mTuple) += std::get<1>(ltrb.mTuple);
            return ltrb;
        }

        static constexpr Scalar ToXYWH(const Scalar& inLTRB)
        {
            Scalar xywh = inLTRB;
            std::get<2>(xywh.mTuple) -= std::get<0>(xywh.mTuple);
            std::get<3>(xywh.mTuple) -= std::get<1>(xywh.mTuple);
            return xywh;
        }

    private:
        friend class Simd; // Permit Simd class to provide constexpr api

    private:
        std::tuple<T,T,T,T> mTuple{}; // Impl4: so 4 elements are assumed
    };

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
            Simd128<T>::Store2(&mArray[0], inFirst.GetSimdType());
            Simd128<T>::Store2(&mArray[2], inSecond.GetSimdType());
        }

        template<std::size_t Index>
        constexpr T Get() const
        {
            static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
            return mArray[Index];
        }

        template<std::size_t Index>
        constexpr void Set(T inT)
        {
            static_assert(Index < std::tuple_size_v<decltype(mArray)>, "Provided index out of bounds.");
            mArray[Index] = inT;
        }

        constexpr Simd& operator+=(const Simd& inRHS)
        {
            do 
            {
#if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    Scalar lhs{mArray[0], mArray[1], mArray[2], mArray[3]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1], inRHS.mArray[2], inRHS.mArray[3]};
                    lhs += rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1]; 
					mArray[2] = lhs.mArray[2];
                    mArray[3] = lhs.mArray[3]; 
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&mArray[0]);
					auto rhs = Simd128<T>::Load4(&inRHS.mArray[0]);
					auto result = Simd128<T>::Add(lhs, rhs);
                	Simd128<T>::Store4(&mArray[0], result);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&mArray[0]);
					auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
					auto result = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&mArray[0], result);

					lhs = Simd128<T>::Load2(&mArray[2]);
					rhs = Simd128<T>::Load2(&inRHS.mArray[2]);
					result = Simd128<T>::Add(lhs, rhs);
					Simd128<T>::Store2(&mArray[2], result); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
                    Scalar lhs{mArray[0], mArray[1], mArray[2], mArray[3]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1], inRHS.mArray[2], inRHS.mArray[3]};
                    lhs -= rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1];
					mArray[2] = lhs.mArray[2];
                    mArray[3] = lhs.mArray[3];
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&mArray[0]);
					auto rhs = Simd128<T>::Load4(&inRHS.mArray[0]);
					auto result = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store4(&mArray[0], result);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&mArray[0]);
					auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
					auto result = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&mArray[0], result);

					lhs = Simd128<T>::Load2(&mArray[2]);
					rhs = Simd128<T>::Load2(&inRHS.mArray[2]);
					result = Simd128<T>::Sub(lhs, rhs);
					Simd128<T>::Store2(&mArray[2], result); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
                    Scalar lhs{mArray[0], mArray[1], mArray[2], mArray[3]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1], inRHS.mArray[2], inRHS.mArray[3]};
                    lhs *= rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1]; 
					mArray[2] = lhs.mArray[2];
                    mArray[3] = lhs.mArray[3]; 
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&mArray[0]);
					auto rhs = Simd128<T>::Load4(&inRHS.mArray[0]);
					auto result = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store4(&mArray[0], result);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&mArray[0]);
					auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
					auto result = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&mArray[0], result);

					lhs = Simd128<T>::Load2(&mArray[2]);
					rhs = Simd128<T>::Load2(&inRHS.mArray[2]);
					result = Simd128<T>::Mul(lhs, rhs);
					Simd128<T>::Store2(&mArray[2], result); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
                    Scalar lhs{mArray[0], mArray[1], mArray[2], mArray[3]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1], inRHS.mArray[2], inRHS.mArray[3]};
                    lhs /= rhs;
                    mArray[0] = lhs.mArray[0];
                    mArray[1] = lhs.mArray[1]; 
					mArray[2] = lhs.mArray[2];
                    mArray[3] = lhs.mArray[3]; 
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&mArray[0]);
					auto rhs = Simd128<T>::Load4(&inRHS.mArray[0]);
					auto result = Simd128<T>::Div(lhs, rhs);
					Simd128<T>::Store4(&mArray[0], result);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&mArray[0]);
					auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
					auto result = Simd128<T>::Div(lhs, rhs);
					Simd128<T>::Store2(&mArray[0], result);

					lhs = Simd128<T>::Load2(&mArray[2]);
					rhs = Simd128<T>::Load2(&inRHS.mArray[2]);
					result = Simd128<T>::Div(lhs, rhs);
					Simd128<T>::Store2(&mArray[2], result); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
            bool result = false;
            // protect our interface so it can remain constexpr
            do 
            {
        #if __cpp_lib_is_constant_evaluated
                if (std::is_constant_evaluated())
                {
                    // Delegate to Scalar Impl which is constexpr capable
                    Scalar lhs{mArray[0], mArray[1], mArray[2], mArray[3]};
                    const Scalar rhs{inRHS.mArray[0], inRHS.mArray[1], inRHS.mArray[2], inRHS.mArray[3]};
                    result = lhs == rhs;
                    break;
                }
        #endif // __cpp_lib_is_constant_evaluated


				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto lhs = Simd128<T>::Load4(&mArray[0]);
					auto rhs = Simd128<T>::Load4(&inRHS.mArray[0]);
					result = Simd128<T>::IsEQ(lhs, rhs);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto lhs = Simd128<T>::Load2(&mArray[0]);
					auto rhs = Simd128<T>::Load2(&inRHS.mArray[0]);
					result = Simd128<T>::IsEQ(lhs, rhs);

					if (result)
					{
						lhs = Simd128<T>::Load2(&mArray[2]);
						rhs = Simd128<T>::Load2(&inRHS.mArray[2]);
						result = result && Simd128<T>::IsEQ(lhs, rhs);
					}
				}
				else
				{
					static_assert("Unsupported T");
				}

            } while (false);

            return result;
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
                    const Scalar round{mArray[0], mArray[1], mArray[2], mArray[3]};
                    round.RoundNearest();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);
					mArray[2] = std::get<2>(round.mTuple);
                    mArray[3] = std::get<3>(round.mTuple);
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated
 
				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&mArray[0]);
					round = Simd128<T>::RoundNearest(round);
					Simd128<T>::Store4(&mArray[0], round);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&mArray[0]);
					round1 = Simd128<T>::RoundNearest(round1);
					Simd128<T>::Store2(&mArray[0], round1);

					auto round2 = Simd128<T>::Load2(&mArray[2]);
					round2 = Simd128<T>::RoundNearest(round2);
					Simd128<T>::Store2(&mArray[2], round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
                    const Scalar round{mArray[0], mArray[1], mArray[2], mArray[3]};
                    round.RoundCeil();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);
					mArray[2] = std::get<2>(round.mTuple);
                    mArray[3] = std::get<3>(round.mTuple);
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&mArray[0]);
					round = Simd128<T>::RoundCeil(round);
					Simd128<T>::Store4(&mArray[0], round);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&mArray[0]);
					round1 = Simd128<T>::RoundCeil(round1);
					Simd128<T>::Store2(&mArray[0], round1);

					auto round2 = Simd128<T>::Load2(&mArray[2]);
					round2 = Simd128<T>::RoundCeil(round2);
					Simd128<T>::Store2(&mArray[2], round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
                    const Scalar round{mArray[0], mArray[1], mArray[2], mArray[3]};
                    round.RoundFloor();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);
					mArray[2] = std::get<2>(round.mTuple);
                    mArray[3] = std::get<3>(round.mTuple);
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated

				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&mArray[0]);
					round = Simd128<T>::RoundFloor(round);
					Simd128<T>::Store4(&mArray[0], round);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&mArray[0]);
					round1 = Simd128<T>::RoundFloor(round1);
					Simd128<T>::Store2(&mArray[0], round1);

					auto round2 = Simd128<T>::Load2(&mArray[2]);
					round2 = Simd128<T>::RoundFloor(round2);
					Simd128<T>::Store2(&mArray[2], round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
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
                    const Scalar round{mArray[0], mArray[1], mArray[2], mArray[3]};
                    round.RoundTrunc();
                    mArray[0] = std::get<0>(round.mTuple);
                    mArray[1] = std::get<1>(round.mTuple);
					mArray[2] = std::get<2>(round.mTuple);
                    mArray[3] = std::get<3>(round.mTuple);
                    break;
                }
#endif // __cpp_lib_is_constant_evaluated
 
				if constexpr (sizeof(T) <= 4) // Int/Float up to 32 bit data type
				{
					// 32 bits means 4 elements at a time
					auto round = Simd128<T>::Load4(&mArray[0]);
					round = Simd128<T>::RoundTrunc(round);
					Simd128<T>::Store4(&mArray[0], round);
				}
				else if constexpr (sizeof(T) <= 8) // Double up to 64 bit data type
				{
					// 64 bits means 2 elements at a time
					auto round1 = Simd128<T>::Load2(&mArray[0]);
					round1 = Simd128<T>::RoundTrunc(round1);
					Simd128<T>::Store2(&mArray[0], round1);

					auto round2 = Simd128<T>::Load2(&mArray[2]);
					round2 = Simd128<T>::RoundTrunc(round2);
					Simd128<T>::Store2(&mArray[2], round2); // Store2 only sets the initial address and the one after; does not overwrite entire object
				}
				else
				{
					static_assert("Unsupported T");
				}
            } while (false);
        }

    private:
        std::array<T,4> mArray{}; // Impl4: so 4 elements are assumed
    }; // class Simd
}; // struct Impl4<>

template<typename T>
inline constexpr bool IsEmpty(const typename Impl4<T>::Scalar& inScalar)
{
    bool result = (std::get<2>(inScalar) <= 0) || (std::get<3>(inScalar) <= 0);
    return result;
}

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

} // namespace saber::geometry::detail

#endif // SABER_GEOMETRY_DETAIL_IMPL4_HPP