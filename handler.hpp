#pragma once
#ifndef SABER_HANDLER_HPP
#define SABER_HANDLER_HPP

//std
#include <optional>

namespace saber {

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
        Reset();
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

    // Move Operand
    /// @brief Assign this with data moved from inputted shared_ptr
    /// @param ioMove shared_ptr which will have its data swapped with this
    /// @return this
    ValueHandler& operator=(ValueHandler&& ioMove) noexcept
    {
        // Moving to self should be a NOP
        if (this != &ioMove)
        {
            // Reset/delete the old value which was moved
            //reset(); // noexcept requires us to leave as a turd
            // swap allows for noexcept move assign
            std::swap(mValue, ioMove.mValue);
            std::swap(mSaved, ioMove.mSaved);
        }
        return *this;
    }

    // Copying should not be done, as only one ValueHandler should handle a data
    ValueHandler(const ValueHandler& inCopy) = delete;
    ValueHandler& operator=(const ValueHandler& inCopy) = delete;

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

}// namespace saber


#endif // SABER_HANDLER_HPP