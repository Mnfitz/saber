#pragma once
#ifndef SABER_HANDLER_HPP
#define SABER_HANDLER_HPP

//std
#include <memory>
#include <optional>

namespace saber {

/// @brief ValueHandler takes in a reference to a variable and a value which the variable will be assigned.
/// When the ValueHandler is destructed, it will revert the variable back to it's original state
/// This can be used while performing thread operations.
/// @param inValue The variable which the Value Handler will manage
/// @param inNewValue The value which will be assigned to inValue for the duration of the ValueHandler's lifetime
template<typename T>
class ValueHandler
{
public:
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
    // ctor stores a copy of the variable's stored data
    ReferenceHandler(T* inReference) :
        mReference{inReference};
    {
        // Do nothing
    }

    ~ReferenceHandler()
    {
        Reset(); // called by dtor: Must be noexcept
    }

private:
    std::unique_ptr<T> mReference;
}; // class ReferenceHandler

}// namespace saber


#endif // SABER_HANDLER_HPP