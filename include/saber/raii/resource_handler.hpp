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

/// @brief RAII handler that manages the lifetime of a resource.
///
/// `saber::raii::ResourceHandler<T, DeleterType>` stores an optional `T` and
/// invokes its deleter when a stored value is reset or the handler is destroyed.
/// It is movable but not copyable. Resource construction and assignment take
/// `T&&`, so pass an rvalue (for example, `std::move(resource)`).
///
/// The default `DeleterType` is `ResourceDeleter<T>`, whose primary template
/// intentionally fails to compile; provide a `ResourceDeleter<T>` specialization
/// or supply an explicit deleter. The deleter must be callable with `T&` and
/// must not throw, because it is invoked by `Reset()` and destruction, which
/// are `noexcept`.
///
/// The handler's boolean conversion reports whether its optional contains a
/// value. For pointer-valued `T`, this does not test whether the stored pointer
/// is non-null; check both the handler and the pointer when null is invalid.
///
/// @tparam T The stored resource type. If `T` is a pointer, the handler's
///           engagement is independent of whether that pointer is null.
/// @tparam DeleterType Callable accepting `T&`; defaults to `ResourceDeleter<T>`.
///
/// @warning A moved-from handler remains engaged. Its resource and deleter
///          are moved-from, so the deleter must safely accept that state when
///          the source handler is reset or destroyed.
///
/// @par Move usage example
/// This example uses `std::unique_ptr`, whose moved-from state is empty, and a
/// deleter that safely resets either an owning or empty pointer:
/// @code{.cpp}
/// #include <memory>
/// #include <utility>
/// #include "saber/raii/resource_handler.hpp"
///
/// struct ResetDeleter
/// {
///     void operator()(std::unique_ptr<int>& resource) const noexcept
///     {
///         resource.reset();
///     }
/// };
///
/// using Handler = saber::raii::ResourceHandler<
///     std::unique_ptr<int>, ResetDeleter>;
///
/// Handler source{std::make_unique<int>(42), ResetDeleter{}};
/// Handler moved{std::move(source)}; // source remains engaged, but its pointer is empty
///
/// Handler destination{std::make_unique<int>(7), ResetDeleter{}};
/// destination = std::move(moved);   // moved remains engaged with an empty pointer
/// @endcode
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

	/// @brief Move-constructs this handler from another handler.
	///
	/// The resource and deleter are move-constructed from `ioMove`, but the
	/// source handler remains engaged with a moved-from resource. Its later
	/// reset or destruction will invoke its moved-from deleter on that resource.
	/// @param ioMove Source handler to move from.
	ResourceHandler(ResourceHandler&& ioMove) noexcept;

	/// @brief Move assignment operator.
	///
	/// Releases this handler's current resource, then moves the resource and
	/// deleter from `ioMove`. The source remains engaged with a moved-from
	/// resource; its later reset or destruction will invoke its moved-from
	/// deleter on that resource.
	/// @param ioMove Source handler to assign from.
	/// @return Reference to this handler.
	ResourceHandler& operator=(ResourceHandler&& ioMove) noexcept;

	/// @brief Constructs a handler from an rvalue resource.
	///
	/// Moves `ioResource` into the handler and default-constructs its deleter.
	/// Use `std::move(resource)` when transferring a named resource.
	/// @param ioResource Rvalue resource to manage.
	ResourceHandler(T&& ioResource);

	/// @brief Constructs a handler from an rvalue resource and an explicit deleter.
	///
	/// Moves both arguments into the handler. The deleter must accept `T&` and
	/// must not throw when invoked by `Reset()` or destruction.
	/// @param ioResource Rvalue resource to manage; use `std::move` for a named value.
	/// @param ioDeleter Rvalue deleter to invoke when releasing the resource.
	ResourceHandler(T&& ioResource, DeleterType&& ioDeleter);

	/// @brief Replaces the resource with an rvalue resource.
	///
	/// Releases the current resource, moves `ioResource` into the handler, and
	/// replaces the current deleter with a default-constructed `DeleterType`.
	/// This operation therefore requires `DeleterType` to be default-constructible
	/// and move-assignable. Use `std::move(resource)` for a named resource.
	/// @param ioResource Rvalue resource to take ownership of.
	/// @return Reference to this handler.
	ResourceHandler& operator=(T&& ioResource) noexcept;

	/// @brief Tests whether the handler's optional contains a value.
	///
	/// For pointer-valued `T`, `true` only means the optional is engaged; the
	/// stored pointer may still be null. Check `Get() != nullptr` separately when
	/// a non-null pointer is required, after confirming the handler is engaged.
	/// @return `true` if a `T` value is stored; otherwise `false`.
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