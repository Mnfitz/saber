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

//std
#include <optional>
#include <type_traits>
#include <utility>

namespace saber::raii {

template<typename T>
struct ResourceDeleter
{
	void operator()(T& inDelete) const noexcept
	{
		// TRICKY mnfitz 18may2024: Lambda wrapper for static_assert()
		// Disallow use of this primary template definition... 
		// and enforce use of "specializations only".
		// Note dummy use of `std::is_same_v<>` which causes evaulation of the
		// static_assert() only when the primary template is instanciated/used.
		//
		// as opposed to: static_assert(false, ...) which triggers when merely
		// encountered by the compiler even if not instanciated/used.
		constexpr bool kIsSpecialization = (std::is_same_v<T, void> && !std::is_same_v<T, void>);
		static_assert(kIsSpecialization, "You must provide your own Deleter<T> specialization when using ResourceHandler");
	}
}; // struct ResourceDeleter<>

// #include <cstdio>
//
// Template specialization of deleter for type std::FILE
// template<> // NOTE: no typename T
// struct ResourceDeleter<std::FILE*> // specialization!
// {
// 	  void operator()(std::FILE* inFile) const noexcept
// 	  {
// 	      std::fclose(inFile);
// 	  }
// }

template<typename T, typename DeleterType = ResourceDeleter<T>>
class ResourceHandler
{
public:
	ResourceHandler() = default;
	~ResourceHandler();

	ResourceHandler(const ResourceHandler& inCopy) = delete;
	ResourceHandler& operator=(const ResourceHandler& inCopy) = delete;

	ResourceHandler(ResourceHandler&& ioMove) noexcept;
	ResourceHandler& operator=(ResourceHandler&& ioMove) noexcept;

	ResourceHandler(T&& ioResource);
	ResourceHandler(T&& ioResource, DeleterType&& ioDeleter);
	ResourceHandler& operator=(T&& ioResource) noexcept;

	operator bool() const noexcept;

	std::optional<T>& Get();
	const std::optional<T>& Get() const;

	void Reset() noexcept;

private:
	DeleterType mDeleter{};
	std::optional<T> mResource{};
}; // class ResourceHandler<>

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
inline std::optional<T>& ResourceHandler<T, DeleterType>::Get()
{
	return mResource;
}

template<typename T, typename DeleterType>
inline const std::optional<T>& ResourceHandler<T, DeleterType>::Get() const
{
	return mResource;
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