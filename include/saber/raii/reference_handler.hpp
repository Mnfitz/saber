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

/// @brief ReferenceHandler provides RAII lifetime management for a reference.
/// Use this by providing a specialization of class deleter for a custom type.
/// You can use it to give RAII lifetime management to types that are incpmpatible with std::unique_ptr.
/// For instance, invoking fclose() on the destrucyion of type std::FILE
template<typename T, typename DeleterType = detail::Deleter<T>>
class ReferenceHandler
{
private:
    using UniqueType = std::unique_ptr<T, DeleterType>;

public:
    /// @brief The "handle" type that this ReferenceHandler stores and hands to its deleter.
    ///
    /// This is plain `T*` for ordinary pointer-managed types. However, if DeleterType
    /// provides a nested `pointer` typedef, that type is stored instead. This is the
    /// std::unique_ptr extension point that lets ReferenceHandler RAII manage
    /// *non-pointer* types (e.g. `EventManager::Token`) with no heap allocation:
    /// the deleter simply declares a small handle type that wraps the value.
    ///
    /// A DeleterType::pointer must satisfy the standard NullablePointer requirements:
    /// default constructible, constructible/assignable/comparable with std::nullptr_t,
    /// copyable, equality comparable, and contextually convertible to bool.
    /// See saber::raii::detail::Deleter<saber::events::EventToken> for an example.
    using pointer = typename UniqueType::pointer;

    // ctor stores a copy of the variable's stored data
    explicit ReferenceHandler(pointer inReference) :
        mReference{inReference}
    {
        // Do nothing
    }

    /// @brief Construct a ReferenceHandler using a *stateful* deleter.
    /// Needed whenever DeleterType carries data (e.g. the EventManager which must be
    /// told to Unregister()), since such deleters cannot be default constructed.
    /// @param inReference Handle to be managed
    /// @param inDeleter Deleter instance to be stored alongside the handle
    ReferenceHandler(pointer inReference, DeleterType inDeleter) :
        mReference{inReference, std::move(inDeleter)}
    {
        // Do nothing
    }

    ReferenceHandler() = default;

    ~ReferenceHandler() = default;

    // Move Ctor
    /// @brief Construct this with data moved from inputted ReferenceHandler, nullifying it
    /// @param ioMove ReferenceHandler which will have its data moved into this
    // TRICKY mnfitz 02aug2026: Defaulted, rather than hand written using std::swap().
    // A hand written move must first default construct mReference, which is impossible
    // for a stateful DeleterType. std::unique_ptr's own move is already noexcept and
    // additionally releases any resource this was holding, instead of parking it in
    // ioMove and delaying its destruction.
    ReferenceHandler(ReferenceHandler&& ioMove) noexcept = default;

    // Move Operator
    /// @brief Assign this with data moved from inputted ReferenceHandler
    /// @param ioMove ReferenceHandler which will have its data moved into this
    /// @return this
    ReferenceHandler& operator=(ReferenceHandler&& ioMove) noexcept = default;

    // Copying should not be done, as only one ReferenceHandler should handle a reference
    ReferenceHandler(const ReferenceHandler& inCopy) = delete;
    ReferenceHandler& operator=(const ReferenceHandler& inCopy) = delete;

    /// @brief Return boolean sense of this ReferenceHandler
    /// @return true, if this is managing a reference; otherwise false
    explicit operator bool() const noexcept
    {
        return static_cast<bool>(mReference);
    }

    /// @brief Accessor for the ReferenceHandler's underlying mReference data member.
    /// @return handle of underlying mReference; may be null
    pointer Get() const noexcept
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
    UniqueType mReference{};
}; // class ReferenceHandler

} // namespace saber::raii

#endif // SABER_RAII_REFERENCEHANDLER_HPP