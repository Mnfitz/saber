#pragma once
#ifndef SABER_HANDLER_HPP
#define SABER_HANDLER_HPP

//std
#include <cstdio>
#include <memory>
#include <optional>

namespace saber {
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
        // using default_delete. We should follow this pattern as well
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
        auto mustBeSpecialized = []()
        {
            static_assert(false, "You must provide your own Deleter<T> specialization when using ReferenceHandler");
        };
#endif
    }
};

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

/// @brief ValueHandler takes in a reference to a variable and a value which the variable will be assigned.
/// When the ValueHandler is destructed, it will revert the variable back to it's original state
/// This can be used while performing thread operations.
/// @param inValue The variable which the Value Handler will manage
/// @param inNewValue The value which will be assigned to inValue for the duration of the ValueHandler's lifetime
template<typename T>
class ValueHandler
{
public:
    /// @brief Constructs a ValueHandler by saving the value of a reference and assigning a new value to it
    /// @param inValue The reference value which will be saved and restored after the ValueHandler destructs or resets
    /// @param inNewValue The new value which will be assigned to the reference inValue for the duration of the ValueHandler's lifetime
    // ctor stores a copy of the variable's stored data
    ValueHandler(T& inValue, T inNewValue) :
        mValue{inValue}
    {
        //std::swap<T>(mValue, mSaved);
        mSaved = std::move(mValue);
        mValue = std::move(inNewValue);
    }

    ~ValueHandler()
    {
        Reset(); // called by dtor: Must be noexcept
    }

    // RO5 Methods
    // Move Ctor
    /// @brief Construct this with data moved from inputted ValueHandler
    /// @param ioMove ValueHandler which will have its data swapped with this, nullifying it 
    ValueHandler(ValueHandler&& ioMove) noexcept
    {
        if (ioMove)
        {
            // swap allows for noexcept move ctor
            std::swap(mValue, ioMove.mValue);
            std::swap(mSaved, ioMove.mSaved);
        }
    }

    // Move Operator
    /// @brief Assign this with data moved from inputted shared_ptr
    /// @param ioMove shared_ptr which will have its data swapped with this
    /// @return this
    ValueHandler& operator=(ValueHandler&& ioMove) noexcept
    {
        // Moving to self should be a NOP
        if (this != &ioMove)
        {
            std::swap(mValue, ioMove.mValue);
            std::swap(mSaved, ioMove.mSaved);
        }
        return *this;
    }

    // Copying should not be done, as only one ValueHandler should handle a data
    ValueHandler(const ValueHandler& inCopy) = delete;
    ValueHandler& operator=(const ValueHandler& inCopy) = delete;

    /// @brief Revert the handled value back to its original state. 
    /// This operation can only be performed once
    // Must be noexcept, since it is called by dtor
    // Otherwise, the program will crash
    void Reset() noexcept
    {
        if (mSaved)
        {
            mValue = std::move(*mSaved);
            mSaved.reset();
        }
    }

private:
    T& mValue;
    std::optional<T> mSaved;

}; // class ValueHandler

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
    //std::unique_ptr<T> mReference{nullptr};
    std::unique_ptr<T, detail::Deleter<T>> mReference{};
}; // class ReferenceHandler

}// namespace saber

#endif // SABER_HANDLER_HPP