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
/// NOTE: Not a "secure hash"! So, the result hash values
/// are expected, but not gauranteed, to be: "unique".
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
///		struct Color
///		{
///			unsigned char mRed{}, mGreen{} mBlue{};
///		};
///		constexpr Color kColor = {0x00, 0xff, 0x00};
///
///		// 1. Create using "string literal"
/// 	constexpr auto kMyID_1 = saber::Hash{"my.component: Some locally unique string"};
///		// 2. Create using L"string literal"
/// 	constexpr auto kMyID_2 = saber::Hash{L"some.service: Another string"};
///		// 3. Create using buffer + size
/// 	constexpr auto kMyID_3 = saber::Hash{&kColor.mRed, sizeof(Color)};
///
///		constexpr auto kMyID = saber::Hash{"this.is.my.componenet"};
/// 	switch (kMyID.Value())
/// 	{
///		// 1. Use with accessor
/// 	case kMyID_1().Value:
///			break;
///		// 2. Use with operator ref()
/// 	case kMyID_2():
///			break;
///		// 3. Use with static_cast<>()
/// 	case static_cast<saber::Hash::ValueType>(kMyID_3):
///			break;
/// 	}
///
/// 	enum class MyEnum : saber::Hash::ValueType
/// 	{
///			// 1. Use with accessor
/// 		kSomeEnum1 = saber::Hash{"A unique string"}.Value();
///			// 2. Use with operator ref()
/// 		kSomeEnum2 = saber::Hash{"Second unique string"}();
///			// 3. Use with static_cast<>()
/// 		kSomeEnum3 = static_cast<saber::Hash::ValueType>(saber::Hash{"Third unique string"});
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
}; // struct Fnv1aTraits<32>

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
}; // struct Fnv1aTraits<64>

/// @brief Functor that implements FNV1A has algorithm
/// @tparam BitLength: Hash value size in bits
template<int BitLength>
struct Fnv1a :
	private Fnv1aTraits<BitLength> // is-impl-in-terms-of: Fnv1aTraits<>
{
public:
	using typename Fnv1aTraits<BitLength>::ValueType; // Bring trait's ValueType into public scope

	// Functor "operator ref()"
	template<typename T>
	constexpr ValueType operator()(const T* inBuffer, std::size_t inSize) const noexcept
	{
		static_assert(std::is_integral_v<T>, "Only hashing of integral types are supported");
		static_assert(sizeof(T) <= 4, "Only hashing of integral types of: 8, 16, and 32 bit sizes are supported");

		auto fnv1a = [](ValueType inBasis, unsigned char inByte)
		{
			const auto hash = Fnv1aTraits<BitLength>::kPrime
								* (inBasis ^ static_cast<ValueType>(inByte));
			return hash;
		};

		auto basis = Fnv1aTraits<BitLength>::kOffset;
		for (std::size_t i = 0; i < inSize; i++)
		{
			const auto bytes = inBuffer[i];
			switch (sizeof(T)) // Support 8/16 *and* 32 bit variants of "T"
			{
			case 4: // 32bits
				basis = fnv1a(basis, (bytes >> 24) & 0xff);
			case 3: // ??
				basis = fnv1a(basis, (bytes >> 16) & 0xff);
			case 2: // 16bits
				basis = fnv1a(basis, (bytes >> 8) & 0xff);
			case 1: // 8bits
				basis = fnv1a(basis, (bytes >> 0) & 0xff);
				break;
			default:
				assert(!"Unsupported type. Only 8, 16, and 32 bit types are supported");
				break;
			} // switch (sizeof(T))
		}
		return basis;
	}
}; // struct Fnv1a<>

} // namespace detail

#pragma endregion {}

#if SABER_DEBUG
namespace detail {
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
}; // struct HashTraits<>

/// @brief Hashed value result of an input buffer.
/// @tparam BitLength: Size of the hashed value in bits (typically 32 or 64)
template<int BitLength>
class HashValue :
	private HashTraits<BitLength> // is-impl-in-terms-of: HashTraits<>
{
public:
	using typename HashTraits<BitLength>::ValueType;

public:
	/// @brief Construct an empty `HashValue`
	constexpr HashValue() noexcept :
		mValue{0}
	{
		// This space intentionally blank
	}

	/// @brief Construct a `HashValue` from a 8bit string
	/// @param inString: View to string literal to hash
	constexpr HashValue(std::string_view inString) noexcept :
		HashValue{inString.data(), inString.size()}
	{
		// This space intentionally blank
	}

	/// @brief Construct a `HashValue` from a 16bit string
	/// @param inString: View to wstring literal to hash
	constexpr HashValue(std::wstring_view inString) noexcept :
		HashValue{inString.data(), inString.size()}
	{
		// This space intentionally blank
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

	/// @brief Return underlying `HashValue::ValueType`
	/// @return Result hashed value
	constexpr auto Value() const noexcept { return mValue; }

	/// @brief "operator reference()" to `HashValue::ValueType`.
	constexpr auto operator()() const noexcept { return Value(); }

	/// @brief "operator cast" to `HashValue::ValueType`.
	constexpr operator auto() const noexcept { return Value(); }

private:
	/// @brief Compute the hash of the provided buffer and element count
	/// @tparam T: value type of the buffer
	/// @param inBuffer: Pointer* to buffer to hash
	/// @param inSize: Count of elements in buffer to hash
	/// @return Hashed value result
	template<typename T>
	static constexpr ValueType Hash(const T* inBuffer, std::size_t inSize) noexcept
	{
		const ValueType value = sImpl(inBuffer, inSize);
		assert(!!value && "HashValue(): hash collision with 0! Choose a different string");
		return value;
	}

private:
	using ImplType = typename HashTraits<BitLength>::ImplType;
	static constexpr ImplType sImpl{}; // static hash-algoritm{} functor instance

	ValueType mValue{};
}; // class HashValue<>

#pragma endregion {}

#if SABER_DEBUG
} // namespace detail
#endif // SABER_DEBUG

#if SABER_DEBUG
// ------------------------------------------------------------------
#pragma region struct debug::HashValue<>

inline namespace debug { // "inline", meaning: also "using namespace debug;"

/// @brief `debug::HashValue<>` hashed result of an input buffer.
///
/// During a debug build, this class extends the actual `HashValue<>`
/// class to include as debug-hint: reference to original input key
/// string. Note this key is a debug-hint for humans using a debugger,
/// and is not available via any API.
/// @tparam BitLength: Size of the hashed value in bits (typically 32 or 64)
template<int BitLength>
class HashValue :
	public detail::HashValue<BitLength> // is-a: "the actual" HashValue<>
{
public:
	using typename detail::HashValue<BitLength>::ValueType;

public:
	/// @brief Construct an empty `HashValue`
	constexpr HashValue() noexcept :
		detail::HashValue<BitLength>{},
		mKey{"|empty|"}
	{
		// This space intentionally blank
	}

	/// @brief Construct a `HashValue` from a 8bit string
	/// @param inString: View to string literal to hash
	constexpr HashValue(std::string_view inString) noexcept :
		detail::HashValue<BitLength>{inString},
		mKey{inString}
	{
		// This space intentionally blank
	}

	/// @brief Construct a `HashValue` from a 16bit string
	/// @param inString: View to wstring literal to hash
	constexpr HashValue(std::wstring_view inString) noexcept :
		detail::HashValue<BitLength>{inString},
		mKey{inString}
	{
		// This space intentionally blank
	}

	/// @brief Construct a `HashValue` from a buffer and element count
	/// @tparam T: value type of the buffer
	/// @param inBuffer: Pointer* to buffer to hash
	/// @param inSize: Count of elements in buffer to hash
	template<typename T>
	constexpr HashValue(const T* inBuffer, std::size_t inSize) noexcept :
		detail::HashValue<BitLength>{inBuffer, inSize},
		mKey{}
	{
		// This space intentionally blank
	}

	/// @brief Return underlying `HashValue::ValueType`
	/// @return Result hashed value
	constexpr auto Value() const noexcept { return detail::HashValue<BitLength>::Value(); }

	/// @brief "operator reference()" to `HashValue::ValueType`.
	constexpr auto operator()() const noexcept { return Value(); }

	/// @brief "operator cast" to `HashValue::ValueType`.
	constexpr operator auto() const noexcept { return Value(); }

private:
	/// @brief Debug hint: source string "key" used to generate result hashed "value".
	///
	/// Debug hint! For humans only! Enabled only in DEBUG builds, because it
	/// adds significant runtime overhead that's only useful when debugging.
	std::variant<std::string_view, std::wstring_view> mKey{};
}; // class debug::HashValue<>

} // namespace debug
#endif // SABER_DEBUG

#pragma endregion {}

using Hash   = HashValue<8*sizeof(std::size_t)>;
using Hash32 = HashValue<32>;
using Hash64 = HashValue<64>;

/// @}

} // namespace saber

#endif // SABER_HASH_HPP
