#ifndef SABER_RAII_REFERENCEHANDLER_HPP
#define SABER_RAII_REFERENCEHANDLER_HPP

//std
#include <cstdio>
#include <memory>
#include <type_traits>
#include <utility>

namespace saber::raii {
namespace detail {

template<typename T>
struct Deleter
{
    void operator()(T* inDelete) const noexcept
    {
        // REVISIT mnfitz 19may2024: Provide default_delete or not? 
        // Should users be forced to enter their own specialization deleter,
        // or will it be safe to allow them to use the default_delete case?
#if 1
        // We don't use raw delete, since unique_ptr defaults to 
        // using std::default_delete. We should follow this pattern as well
        //delete inDelete;
        // Specify constexpr since we never modify deleter
        constexpr std::default_delete<T> deleter{};
        deleter(inDelete);
#else 
        // TRICKY mnfitz 18may2024: Lambda wrapper for static_assert()
        // If we want to prevent use of a primary template definition, 
        // so that only specializations will compile, we must wrap the
        // static assert in a lambda. Otherwise, the static_assert()
        // will always give us compile errors at every usage of our template
        // class, whether it's specialized or not.
		static_assert(std::is_same_v<T, void> && false, "Must specialize MyTrait!");
        auto mustBeSpecialized = []()
        {
            static_assert(false, "You must provide your own Deleter<T> specialization when using ReferenceHandler");
        };
#endif
    }
}; // struct Deleter<>

// Template specialization of deleter for type std::FILE
template<> // NOTE: no typename T
struct Deleter<std::FILE> // specialization!
{
    void operator()(std::FILE* inFile) const noexcept
    {
        std::fclose(inFile);
    }
};

} // namespace detail

template<typename T>
class ReferenceHandler
{
public:
    /// @brief ReferenceHandler provides RAII lifetime management for a reference. 
    /// Use this by providing a specialization of class deleter for a custom type.
    /// You can use it to give RAII lifetime management to types that are incpmpatible with std::unique_ptr.
    /// For instance, invoking fclose() on the destrucyion of type std::FILE
    // ctor stores a copy of the variable's stored data
    ReferenceHandler(T* inReference) :
        mReference{inReference}
    {
        // Do nothing
    }

    ReferenceHandler() = default;

    ~ReferenceHandler() = default;

    // Move Ctor
    /// @brief Construct this with data moved from inputted ReferenceHandler
    /// @param ioMove ReferenceHandler which will have its data swapped with this, nullifying it 
    ReferenceHandler(ReferenceHandler&& ioMove) noexcept
    {
        if (ioMove)
        {
            // swap allows for noexcept move ctor
            std::swap(mReference, ioMove.mReference);
        }
    }

    // Move Operator
    /// @brief Assign this with data moved from inputted ReferenceHandler
    /// @param ioMove ReferenceHandler which will have its data swapped with this
    /// @return this
    ReferenceHandler& operator=(ReferenceHandler&& ioMove) noexcept
    {
        // Moving to self should be a NOP
        if (this != &ioMove)
        {
            // swap allows for noexcept move assign
            std::swap(mReference, ioMove.mReference);
        }
        return *this;
    }

    operator bool()
    {
        return (mReference != nullptr);
    }

    /// @brief Accessor for the ReferenceHandler's underlying mReference data member.
    /// @return raw pointer of underlying mReference; may be nullptr
    T* Get()
    {
        return mReference.get();
    }

    /// @brief Resets the ReferenceHandler's underlying mReference data member. 
    /// Effectively a manual delete for ReferenceHandler's underlying mReference.
    void Reset() noexcept
    {
        mReference.reset();
    }

private:
    std::unique_ptr<T, detail::Deleter<T>> mReference{};
}; // class ReferenceHandler

} // namespace saber::raii

#endif // SABER_RAII_REFERENCEHANDLER_HPP