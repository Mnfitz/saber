/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2025 Matthew Fitzgerald
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
/////////////////////////////////////////////////////////////////////

#pragma once
#ifndef SABER_HASH_HPP
#define SABER_HASH_HPP

// saber
#include "saber/config.hpp"

// std
#include <assert.h>
#include <string_view>
#if SABER_DEBUG
#include <variant>
#endif // SABER_DEBUG

namespace saber {

/// @name Hashing
/// `HashValue<>`: "constexpr-capable" string hashing class that
/// takes an input string and computes a "unique" integer hash
/// value. Useful in cases where you want to define a common set
/// of constants across multiple components... but then you
/// don't want to get into the business of reserving "ranges"
/// of constant values for each component. Instead, each component
/// can merely define their own values hashed from a component-
/// specific string format of their choosing. It's easier to ensure
/// everyone defines their own unique strings, than ensuring
/// everyone uses their own unique numbers.
/// See: https://en.wikipedia.org/wiki/Hash_function
///
/// NOTE: This is not a "secure hash". So, the result hash
/// values are expected, but not gauranteed, to be: "unique".
/// Collisions can happen in rare circumstances. Beware!
///
/// This class is "constexpr" so it's computed hash values
/// can be used statically at compile-time, just like other
/// numeric constants.
///
/// Use it ala:
/// @code
/// #include "saber/hash.hpp"
/// main()
/// {
/// 	constexpr auto kMyConstant1 = saber::Hash{"my.component: Some String"};
/// 	constexpr auto kMyConstant2 = saber::Hash{"some.service: Another String"};
/// 	switch (Hash{})
/// 	{
/// 	case kMyConstant1:
/// 	case kMyConstant2:
/// 	default:
/// 		break;
/// 	}
///
/// 	enum class MyEnum : saber::Hash64::ValueType // underlying_type as Hash64::ValueType
/// 	{
/// 		// Beware: upcasting to enum loses `mKey` debug info in DEBUG builds
/// 		kSomeEnum = saber::Hash64{"A descriptive string"};
/// 	};
/// }
/// @endcode
/// @{

#if 0
//constexpr Hash operator"" _hash(const char* inString)
//{
//  return Hash{inString};
//}
#endif

// ------------------------------------------------------------------
#pragma region struct detail::Fnv1a<>

namespace detail {

/// @brief Type traits describing variants of the FNV1A hash algorithm
///
/// Currently, the hash algorithm is implemented as: "fnv1a"
/// See: https://en.wikipedia.org/wiki/Fowler-Noll-Vo_hash_function
///
/// But we hide the implementation from the HashValue interface,
/// so that we can change it (should we need to) in the future.
/// Like maybe switching to: crc32, or murmur3?
/// @tparam BitLength: Hash value size in bits
template<int BitLength>
struct Fnv1aTraits;

/// @brief Specialization describing 32bit version of FNV1A
template<>
struct Fnv1aTraits<32> // Support for: 32bit hash
{
	using ValueType = std::uint32_t;
	enum : ValueType
	{
		// Magic numbers from FNV1A(32bit) spec
		kOffset = 0x811c9dc5UL,
		kPrime = 0x1000193UL
	};
};

/// @brief Specialization describing 64bit version of FNV1A
template<>
struct Fnv1aTraits<64> // Support for: 64bit hash
{
	using ValueType = std::uint64_t;
	enum : ValueType
	{
		// Magic numbers from FNV1A(64bit) spec
		kOffset = 0xcbf29ce484222325ULL,
		kPrime = 0x100000001b3ULL
	};
};

/// @brief Functor that implements FNV1A has algorithm
/// @tparam BitLength: Hash value size in bits
template<int BitLength>
struct Fnv1a :
	private Fnv1aTraits<BitLength> // is-impl-in-terms-of: Fnv1aTraits<>
{
public:
	using typename Fnv1aTraits<BitLength>::ValueType; // Bring trait's ValueType into scope

	// Functor ref() operator
	template<typename T>
	constexpr ValueType operator()(const T* inBuffer, std::size_t inSize) const noexcept
	{
		static_assert(std::is_integral_v<T>, "Only hashing of integral types are supported");
		static_assert(sizeof(*inBuffer) <= 4, "Only hashing of integral types of: 8, 16, and 32 bit sizes are supported");

		ValueType basis = kOffset;
		for (std::size_t i = 0; i < inSize; i++)
		{
			const auto bytes = inBuffer[i];
			switch (sizeof(*inBuffer)) // Support 8/16 *and* 32 bit variants of "T"
			{
			case 4: // 32bits
				basis = Hash(basis, (bytes >> 24) & 0xff);
				basis = Hash(basis, (bytes >> 16) & 0xff);
			case 2: // 16bits
				basis = Hash(basis, (bytes >> 8) & 0xff);
			case 1: // 8bits
				basis = Hash(basis, (bytes >> 0) & 0xff);
				break;
			default:
				assert(!"Unsupported type. Only 8, 16, and 32 bit types are supported");
				break;
			}
		}
		return basis;
	}

private:
	// fnv1a hash algorithm
	static constexpr ValueType Hash(ValueType inBasis, char inByte) noexcept
	{
		const auto hash = static_cast<ValueType>(kPrime * (inBasis ^ static_cast<ValueType>(inByte)));
		return hash;
	}
}; // struct Fnv1a<>

} // namespace detail

#pragma endregion {}

#if SABER_DEBUG
namespace detail::debug {

/// @brief Debug hint of source string "key" used to a generate result hash "value".
///
/// Only enabled in DEBUG builds, because it adds significant runtime
/// overhead that's only useful when debugging.
struct HashKey
{
	std::variant<std::string_view, std::wstring_view> mKey{}; // debug hint! for humans only!
};

} // namespace detail::debug
#endif // SABER_DEBUG

// ------------------------------------------------------------------
#pragma region struct HashValue<>

/// @brief Traits class that encapsulates implementation details of HashValue<>.
///
/// If you ever want to change the hash algorithm to something else,
/// this is the spot where you'd make the change.
/// @tparam BitLength: Size of the hashed value in bits (typically 32 or 64)
template<int BitLength>
struct HashTraits
{
	using ImplType = detail::Fnv1a<BitLength>; // fnv1a!
	using ValueType = typename ImplType::ValueType;
};

/// @brief Wraps the hashed value result of an input buffer.
///
/// By wrapping the hashed result in a class, we can also include
/// as a debug-hint, a reference to the original input string.
/// Note that this is a debug-hint for humans only, and appears
/// only in debug builds.
/// @tparam BitLength: Size of the hashed value in bits (typically 32 or 64)
template<int BitLength>
class HashValue :
	public HashTraits<BitLength>::ValueType // is-a: hashed value type
{
public:
	using ValueType = typename HashTraits<BitLength>::ValueType;

public:
	/// @brief Construct an empty `HashValue`
	constexpr HashValue() noexcept :
		HashTraits<BitLength>::ValueType{0}
	{
		// This space intentionally blank
	}

	/// @brief Construct a `HashValue` from a 8bit string literal
	/// @tparam N: Character count of the string literal (including 0x0 terminator)
	/// @param inLiteral: Reference& to string literal to hash
	template<int N>
	constexpr HashValue(const char(&inLiteral)[N]) noexcept :
		HashValue{std::string_view{inLiteral, N-1}} // N-1: exclude trailing '\0' from Hash
	{
		static_assert(N > 1, "HashValue(): inLiteral[] cannot be empty");
		assert(inLiteral[N-1] == '\0' && "HashValue(): inLiteral[] must be '\0' terminated");
	}

	/// @brief Construct a `HashValue` from a 8bit string
	/// @param inString: View to string literal to hash
	constexpr HashValue(std::string_view inString) noexcept :
		HashValue{inString.data(), inString.size()}
	{
#if SABER_DEBUG
		mDebug.mKey = inString;
#endif // SABER_DEBUG
	}

	/// @brief Construct a `HashValue` from a 16bit wstring literal
	/// @tparam N: Character count of the wstring literal (including 0x0 terminator)
	/// @param inLiteral: Reference& to the wstring literal to hash
	template<int N>
	constexpr HashValue(const wchar_t(&inLiteral)[N]) noexcept :
		HashValue{std::wstring_view{inLiteral, N-1}} // N-1: exclude trailing L'\0' from Hash
	{
		static_assert(N > 1, "HashValue(): inLiteral[] cannot be empty");
		assert(inLiteral[N-1] == L'\0' && "HashValue(): inLiteral[] must be L'\0' terminated");
	}

	/// @brief Construct a `HashValue` from a 16bit string
	/// @param inString: View to wstring literal to hash
	constexpr HashValue(std::wstring_view inString) noexcept :
		HashValue{inString.data(), inString.size()}
	{
#if SABER_DEBUG
		mDebug.mKey = inString;
#endif // SABER_DEBUG
	}

	/// @brief Construct a `HashValue` from a buffer and element count
	/// @tparam T: value type of the buffer
	/// @param inBuffer: Pointer* to buffer to hash
	/// @param inSize: Count of elements in buffer to hash
	template<typename T>
	constexpr HashValue(const T* inBuffer, std::size_t inSize) noexcept :
		mValue{Hash(inBuffer, inSize)}
	{
		// This space intentionally blank
	}

private:
	/// @brief Compute the hash of the provided buffer and element count
	/// @tparam T: value type of the buffer
	/// @param inBuffer: Pointer* to buffer to hash
	/// @param inSize: Count of elements in buffer to hash
	/// @return Hash value result
	template<typename T>
	static constexpr ValueType Hash(const T* inBuffer, std::size_t inSize) noexcept
	{
		const ValueType value = sImpl(inBuffer, inSize);
		assert(kNull != value && "HashValue(): hash collision with 0! Choose a different string");
		return value;
	}

private:
	using ImplType = typename HashTraits<BitLength>::ImplType;
	static constexpr ImplType sImpl{}; // static fnv1a{} functor instance
#if SABER_DEBUG
	detail::debug::HashKey mDebug{};
#endif // SABER_DEBUG
}; // struct HashValue<>

#pragma endregion {}

using Hash = HashValue<8 * sizeof(std::size_t)>;
using Hash32 = HashValue<32>;
using Hash64 = HashValue<64>;

/// @}

} // namespace saber

#endif // SABER_HASH_HPP
