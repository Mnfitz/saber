/////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2026 Matthew Fitzgerald
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

#ifndef SABER_RAII_RESOURCEHANDLER_HPP
#define SABER_RAII_RESOURCEHANDLER_HPP

// saber
#include "saber/config.hpp"
#include "saber/exception.hpp"

// std
#include <optional>
#include <type_traits>
#include <utility>

namespace saber::raii {

/// @brief Primary deleter template used by `ResourceHandler`
///
/// The default implementation intentionally triggers a static_assert
/// so that users must provide a specialization for each resource type
/// they wish to manage.  This ensures the deletion logic is explicit.
///
template<typename T>
struct ResourceDeleter
{
	/// @brief Invokes the deleter on `inDelete`.
	///
	/// This primary template should never be instantiated; attempting to do so
	/// will result in a compile‑time error, forcing the user to supply
	/// a specialization.
	///
	/// @param inDelete Reference to the resource to delete.
	void operator()(T& inDelete) const noexcept
	{
		constexpr bool kIsSpecialization =
			(std::is_same_v<T, void> && !std::is_same_v<T, void>);
		static_assert(kIsSpecialization,
					  "You must provide your own Deleter<T> specialization "
					  "when using ResourceHandler");
	}
}; // struct ResourceDeleter

// #include <cstdio>
//
// Template specialization of deleter for type std::FILE
// template<> // NOTE: no typename T
// struct ResourceDeleter<std::FILE*> // specialization!
// {
//       void operator()(std::FILE* inFile) const noexcept
//       {
//           std::fclose(inFile);
//       }
// }

/// @brief RAII handler that manages the lifetime of a resource.
///
/// `ResourceHandler` owns an optional resource of type `T`.  When the
/// handler goes out of scope or is reset, it automatically invokes a
/// deleter (defaulting to `ResourceDeleter<T>`) to release the resource.
/// The class is move‑constructible and move‑assignable but not copyable.
///
/// @tparam T            Type of the managed resource.
/// @tparam DeleterType  Callable type that implements `operator()(T&)`
///                      Defaults to `ResourceDeleter<T>`.
//------------------------------------------------------------------------------
template<typename T, typename DeleterType = ResourceDeleter<T>>
class ResourceHandler
{
public:
	/// @brief Default constructs an empty handler.
	///
	/// The resulting object holds no resource and is considered invalid.
	ResourceHandler() = default;

	/// @brief Destructor that releases the owned resource if present.
	~ResourceHandler();

	// Copy operations are deleted to enforce unique ownership.
	ResourceHandler(const ResourceHandler& inCopy) = delete;
	ResourceHandler& operator=(const ResourceHandler& inCopy) = delete;

	/// @brief Move constructor that transfers ownership from another handler.
	///
	/// The source handler becomes empty after the move.
	/// @param ioMove Source handler to move from.
	ResourceHandler(ResourceHandler&& ioMove) noexcept;

	/// @brief Move assignment operator.
	///
	/// Releases any currently owned resource before acquiring the new one.
	/// @param ioMove Source handler to assign from.
	/// @return Reference to this handler.
	ResourceHandler& operator=(ResourceHandler&& ioMove) noexcept;

	/// @brief Constructs a handler that takes ownership of `ioResource`.
	///
	/// The default deleter (`DeleterType{}`) is used.
	/// @param ioResource Resource to manage.
	ResourceHandler(T&& ioResource);

	/// @brief Constructs a handler with an explicit deleter.
	///
	/// @param ioResource Resource to manage.
	/// @param ioDeleter  Deleter responsible for destroying the resource.
	ResourceHandler(T&& ioResource, DeleterType&& ioDeleter);

	/// @brief Assigns a new resource to the handler.
	///
	/// Any previously owned resource is destroyed before taking ownership
	/// of `ioResource` and resetting the deleter to the default.
	/// @param ioResource Resource to assign.
	/// @return Reference to this handler.
	ResourceHandler& operator=(T&& ioResource) noexcept;

	/// @brief Boolean conversion indicating if a resource is currently owned.
	///
	/// @return `true` if the handler holds a value; otherwise `false`.
	operator bool() const noexcept;

	/// @brief Provides mutable access to the underlying resource.
	///
	/// This allows callers to inspect or modify the held value directly.
	/// @return Mutable reference to the resource.
	/// @throws saber::RequireException if the handler does not own a resource.
	T& Get();

	/// @brief Const overload of `Get()` providing read‑only access.
	///
	/// @return Const reference to the resource.
	/// @throws saber::RequireException if the handler does not own a resource.
	const T& Get() const;

	/// @brief Releases the owned resource, if any.
	///
	/// The deleter is invoked on the stored value and the optional is reset.
	void Reset() noexcept;

private:
	DeleterType mDeleter{};
	std::optional<T> mResource{};
}; // class ResourceHandler

/// inlines

template<typename T, typename DeleterType>
inline ResourceHandler<T, DeleterType>::~ResourceHandler()
{
	Reset();
}

template<typename T, typename DeleterType>
inline ResourceHandler<T, DeleterType>::ResourceHandler(ResourceHandler&& ioMove) noexcept :
	mDeleter{std::move(ioMove.mDeleter)},
	mResource{std::move(ioMove.mResource)}
{
	// This space intentionally blank
}

template<typename T, typename DeleterType>
inline ResourceHandler<T, DeleterType>&
	ResourceHandler<T, DeleterType>::operator=(ResourceHandler&& ioMove) noexcept
{
	// Moving to self is a NOP
	if (this != &ioMove)
	{
		Reset();
		mResource = std::move(ioMove.mResource);
		mDeleter = std::move(ioMove.mDeleter);
	}
	return *this;
}

template<typename T, typename DeleterType>
inline ResourceHandler<T, DeleterType>::ResourceHandler(T&& ioResource) :
	ResourceHandler{std::move(ioResource), DeleterType{}}
{
	// This space intentionally blank
}

template<typename T, typename DeleterType>
inline ResourceHandler<T, DeleterType>::ResourceHandler(T&& ioResource, DeleterType&& ioDeleter) :
	mDeleter{std::move(ioDeleter)}
{
	Reset(); // Reset to ensure no resource is currently held before taking ownership of the new resource
	mResource = std::move(ioResource);
}

template<typename T, typename DeleterType>
inline ResourceHandler<T, DeleterType>& ResourceHandler<T, DeleterType>::operator=(T&& ioResource) noexcept
{
	Reset(); // Reset to ensure no resource is currently held before taking ownership of the new resource
	mResource = std::move(ioResource);
	mDeleter = DeleterType{};
	return *this;
}

template<typename T, typename DeleterType>
inline ResourceHandler<T, DeleterType>::operator bool() const noexcept
{
	return mResource.has_value();
}

template<typename T, typename DeleterType>
inline T& ResourceHandler<T, DeleterType>::Get()
{
	SABER_REQUIRE(mResource.has_value(), "ResourceHandler does not hold a value");
	return mResource.value();
}

template<typename T, typename DeleterType>
inline const T& ResourceHandler<T, DeleterType>::Get() const
{
	SABER_REQUIRE(mResource.has_value(), "ResourceHandler does not hold a value");
	return mResource.value();
}

template<typename T, typename DeleterType>
inline void ResourceHandler<T, DeleterType>::Reset() noexcept
{
	if (mResource)
	{
		mDeleter(*mResource);
		mResource.reset();
	}
}

} // namespace saber::raii

#endif // SABER_RAII_RESOURCEHANDLER_HPP