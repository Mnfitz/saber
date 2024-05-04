#pragma once
#ifndef SABER_HANDLER_HPP
#define SABER_HANDLER_HPP

//std
#include <optional>

namespace saber{

template <typename T>
class ValueHandler
{
public:
    // ctor stores a copy of the variable's stored data
    ValueHandler(T& inValue, T inNewValue) :
        mValue{inValue},
        mSaved{inValue}
    {
        inValue = inNewValue;
    }

    ~ValueHandler()
    {
        Reset();
    }

    // Copying should not be done
    ValueHandler(const ValueHandler& inCopy) = delete;
    ValueHandler& operator=(const ValueHandler& inCopy) = delete;

    void Reset()
    {
        if (mSaved)
        {
            mValue = *mSaved;
            mSaved.reset();
        }
    }

private:
    T& mValue;
    std::optional<T> mSaved;

}; // class ValueHandler

}// namespace saber

#endif // SABER_HANDLER_HPP