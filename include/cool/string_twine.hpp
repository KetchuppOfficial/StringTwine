/*
 * This is a plain daylight stealing of code from LLVM. This disgusting fact will be fixed in the
 * future.
 */

#ifndef COOL_STRING_TWINE_HPP
#define COOL_STRING_TWINE_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

namespace cool {

class Twine {
  public:
    /// NodeKind - Represent the type of an argument.
    enum NodeKind : unsigned char {
        /// An empty string; the result of concatenating anything with it is also
        /// empty.
        NullKind,

        /// The empty string.
        EmptyKind,

        /// A pointer to a Twine instance.
        TwineKind,

        /// A pointer to a C string instance.
        CStringKind,

        /// A pointer to an std::string instance.
        StdStringKind,

        /// A Pointer and Length representation. Used for std::string_view,
        /// StringRef, and SmallString.  Can't use a StringRef here
        /// because they are not trivally constructible.
        PtrAndLengthKind,

        /// A char value, to render as a character.
        CharKind,

        /// An unsigned int value, to render as an unsigned decimal integer.
        DecUIKind,

        /// An int value, to render as a signed decimal integer.
        DecIKind,

        /// A pointer to an unsigned long value, to render as an unsigned decimal
        /// integer.
        DecULKind,

        /// A pointer to a long value, to render as a signed decimal integer.
        DecLKind,

        /// A pointer to an unsigned long long value, to render as an unsigned
        /// decimal integer.
        DecULLKind,

        /// A pointer to a long long value, to render as a signed decimal integer.
        DecLLKind,

        /// A pointer to a std::uint64_t value, to render as an unsigned hexadecimal
        /// integer.
        UHexKind
    };

    union Child {
        const Twine *twine;
        const char *cString;
        const std::string *stdString;
        struct {
            const char *ptr;
            std::size_t length;
        } ptrAndLength;
        char character;
        unsigned int decUI;
        int decI;
        const unsigned long *decUL;
        const long *decL;
        const unsigned long long *decULL;
        const long long *decLL;
        const std::uint64_t *uHex;
    };

    /// LHS - The prefix in the concatenation, which may be uninitialized for
    /// Null or Empty kinds.
    Child LHS;

    /// RHS - The suffix in the concatenation, which may be uninitialized for
    /// Null or Empty kinds.
    Child RHS;

    /// LHSKind - The NodeKind of the left hand side, \see getLHSKind().
    NodeKind LHSKind = EmptyKind;

    /// RHSKind - The NodeKind of the right hand side, \see getRHSKind().
    NodeKind RHSKind = EmptyKind;

    /// Construct a nullary twine; the kind must be NullKind or EmptyKind.
    explicit Twine(NodeKind Kind) : LHSKind(Kind) { assert(isNullary() && "Invalid kind!"); }

    /// Construct a binary twine.
    explicit Twine(const Twine &LHS, const Twine &RHS) : LHSKind(TwineKind), RHSKind(TwineKind) {
        this->LHS.twine = &LHS;
        this->RHS.twine = &RHS;
        assert(isValid() && "Invalid twine!");
    }

    /// Construct a twine from explicit values.
    explicit Twine(Child LHS, NodeKind LHSKind, Child RHS, NodeKind RHSKind)
        : LHS(LHS), RHS(RHS), LHSKind(LHSKind), RHSKind(RHSKind) {
        assert(isValid() && "Invalid twine!");
    }

    /// Check for the null twine.
    bool isNull() const { return getLHSKind() == NullKind; }

    /// Check for the empty twine.
    bool isEmpty() const { return getLHSKind() == EmptyKind; }

    /// Check if this is a nullary twine (null or empty).
    bool isNullary() const { return isNull() || isEmpty(); }

    /// Check if this is a unary twine.
    bool isUnary() const { return getRHSKind() == EmptyKind && !isNullary(); }

    /// Check if this is a binary twine.
    bool isBinary() const { return getLHSKind() != NullKind && getRHSKind() != EmptyKind; }

    /// Check if this is a valid twine (satisfying the invariants on
    /// order and number of arguments).
    bool isValid() const {
        // Nullary twines always have Empty on the RHS.
        if (isNullary() && getRHSKind() != EmptyKind)
            return false;

        // Null should never appear on the RHS.
        if (getRHSKind() == NullKind)
            return false;

        // The RHS cannot be non-empty if the LHS is empty.
        if (getRHSKind() != EmptyKind && getLHSKind() == EmptyKind)
            return false;

        // A twine child should always be binary.
        if (getLHSKind() == TwineKind && !LHS.twine->isBinary())
            return false;
        if (getRHSKind() == TwineKind && !RHS.twine->isBinary())
            return false;

        return true;
    }

    /// Get the NodeKind of the left-hand side.
    NodeKind getLHSKind() const { return LHSKind; }

    /// Get the NodeKind of the right-hand side.
    NodeKind getRHSKind() const { return RHSKind; }

  public:
    /// @name Constructors
    /// @{

    /// Construct from an empty string.
    /*implicit*/ Twine() { assert(isValid() && "Invalid twine!"); }

    Twine(const Twine &) = default;

    /// Construct from a C string.
    ///
    /// We take care here to optimize "" into the empty twine -- this will be
    /// optimized out for string constants. This allows Twine arguments have
    /// default "" values, without introducing unnecessary string constants.
    /*implicit*/ Twine(const char *Str) {
        if (Str[0] != '\0') {
            LHS.cString = Str;
            LHSKind = CStringKind;
        } else {
            LHSKind = EmptyKind;
        }

        assert(isValid() && "Invalid twine!");
    }
    /// Delete the implicit conversion from nullptr as Twine(const char *)
    /// cannot take nullptr.
    /*implicit*/ Twine(std::nullptr_t) = delete;

    /// Construct from an std::string.
    /*implicit*/ Twine(const std::string &Str) : LHSKind(StdStringKind) {
        LHS.stdString = &Str;
        assert(isValid() && "Invalid twine!");
    }

    /// Construct from an std::string_view by converting it to a pointer and
    /// length.  This handles string_views on a pure API basis, and avoids
    /// storing one (or a pointer to one) inside a Twine, which avoids problems
    /// when mixing code compiled under various C++ standards.
    /*implicit*/ Twine(std::string_view Str) : LHSKind(PtrAndLengthKind) {
        LHS.ptrAndLength.ptr = Str.data();
        LHS.ptrAndLength.length = Str.length();
        assert(isValid() && "Invalid twine!");
    }

    /// Construct from a char.
    explicit Twine(char Val) : LHSKind(CharKind) { LHS.character = Val; }

    /// Construct from a signed char.
    explicit Twine(signed char Val) : LHSKind(CharKind) { LHS.character = static_cast<char>(Val); }

    /// Construct from an unsigned char.
    explicit Twine(unsigned char Val) : LHSKind(CharKind) {
        LHS.character = static_cast<char>(Val);
    }

    /// Construct a twine to print \p Val as an unsigned decimal integer.
    explicit Twine(unsigned Val) : LHSKind(DecUIKind) { LHS.decUI = Val; }

    /// Construct a twine to print \p Val as a signed decimal integer.
    explicit Twine(int Val) : LHSKind(DecIKind) { LHS.decI = Val; }

    /// Construct a twine to print \p Val as an unsigned decimal integer.
    explicit Twine(const unsigned long &Val) : LHSKind(DecULKind) { LHS.decUL = &Val; }

    /// Construct a twine to print \p Val as a signed decimal integer.
    explicit Twine(const long &Val) : LHSKind(DecLKind) { LHS.decL = &Val; }

    /// Construct a twine to print \p Val as an unsigned decimal integer.
    explicit Twine(const unsigned long long &Val) : LHSKind(DecULLKind) { LHS.decULL = &Val; }

    /// Construct a twine to print \p Val as a signed decimal integer.
    explicit Twine(const long long &Val) : LHSKind(DecLLKind) { LHS.decLL = &Val; }

    // FIXME: Unfortunately, to make sure this is as efficient as possible we
    // need extra binary constructors from particular types. We can't rely on
    // the compiler to be smart enough to fold operator+()/concat() down to the
    // right thing. Yet.

    /// Construct as the concatenation of a C string and a std::string_view.
    /*implicit*/ Twine(const char *LHS, std::string_view RHS)
        : LHSKind(CStringKind), RHSKind(PtrAndLengthKind) {
        this->LHS.cString = LHS;
        this->RHS.ptrAndLength.ptr = RHS.data();
        this->RHS.ptrAndLength.length = RHS.size();
        assert(isValid() && "Invalid twine!");
    }

    /// Construct as the concatenation of a std::string_view and a C string.
    /*implicit*/ Twine(std::string_view LHS, const char *RHS)
        : LHSKind(PtrAndLengthKind), RHSKind(CStringKind) {
        this->LHS.ptrAndLength.ptr = LHS.data();
        this->LHS.ptrAndLength.length = LHS.size();
        this->RHS.cString = RHS;
        assert(isValid() && "Invalid twine!");
    }

    /// Since the intended use of twines is as temporary objects, assignments
    /// when concatenating might cause undefined behavior or stack corruptions
    Twine &operator=(const Twine &) = delete;

    /// Create a 'null' string, which is an empty string that always
    /// concatenates to form another empty string.
    static Twine createNull() { return Twine(NullKind); }

    /// @}
    /// @name Numeric Conversions
    /// @{

    // Construct a twine to print \p Val as an unsigned hexadecimal integer.
    static Twine utohexstr(const std::uint64_t &Val) {
        Child LHS, RHS;
        LHS.uHex = &Val;
        RHS.twine = nullptr;
        return Twine(LHS, UHexKind, RHS, EmptyKind);
    }

    /// @}
    /// @name Predicate Operations
    /// @{

    /// Check if this twine is trivially empty; a false return value does not
    /// necessarily mean the twine is empty.
    bool isTriviallyEmpty() const { return isNullary(); }

    /// Return true if this twine can be dynamically accessed as a single
    /// StringRef value with getSingleStringRef().
    bool isSingleStringRef() const {
        if (getRHSKind() != EmptyKind)
            return false;

        switch (getLHSKind()) {
        case EmptyKind:
        case CStringKind:
        case StdStringKind:
        case PtrAndLengthKind:
            return true;
        default:
            return false;
        }
    }

    /// @}
    /// @name String Operations
    /// @{

    Twine concat(const Twine &Suffix) const;

    /// @}
    /// @name Output & Conversion.
    /// @{

    /// Return the twine contents as a std::string.
    std::string str() const;

    /// This returns the twine as a single StringRef.  This method is only valid
    /// if isSingleStringRef() is true.
    std::string_view getSingleStringRef() const {
        assert(isSingleStringRef() && "This cannot be had as a single stringref!");
        switch (getLHSKind()) {
        default:
            std::unreachable();
        case EmptyKind:
            return {};
        case CStringKind:
            return LHS.cString;
        case StdStringKind:
            return *LHS.stdString;
        case PtrAndLengthKind:
            return {LHS.ptrAndLength.ptr, LHS.ptrAndLength.length};
        }
    }
};

/// @name Twine Inline Implementations
/// @{
inline Twine Twine::concat(const Twine &Suffix) const {
    // Concatenation with null is null.
    if (isNull() || Suffix.isNull())
        return Twine(NullKind);

    // Concatenation with empty yields the other side.
    if (isEmpty())
        return Suffix;
    if (Suffix.isEmpty())
        return *this;

    // Otherwise we need to create a new node, taking care to fold in unary
    // twines.
    Child NewLHS, NewRHS;
    NewLHS.twine = this;
    NewRHS.twine = &Suffix;
    NodeKind NewLHSKind = TwineKind, NewRHSKind = TwineKind;
    if (isUnary()) {
        NewLHS = LHS;
        NewLHSKind = getLHSKind();
    }
    if (Suffix.isUnary()) {
        NewRHS = Suffix.LHS;
        NewRHSKind = Suffix.getLHSKind();
    }

    return Twine(NewLHS, NewLHSKind, NewRHS, NewRHSKind);
}

inline Twine operator+(const Twine &LHS, const Twine &RHS) { return LHS.concat(RHS); }

/// Additional overload to guarantee simplified codegen; this is equivalent to
/// concat().

inline Twine operator+(const char *LHS, std::string_view RHS) { return Twine(LHS, RHS); }

/// Additional overload to guarantee simplified codegen; this is equivalent to
/// concat().

inline Twine operator+(std::string_view LHS, const char *RHS) { return Twine(LHS, RHS); }

/// @}

} // end namespace cool

#endif // COOL_STRING_TWINE_HPP
