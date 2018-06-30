#ifndef     _RAW_TEXT_H_
#include    "RawText.H"
#endif  //  _RAW_TEXT_H_

#ifndef     _TEXT_H_
#define     _TEXT_H_

class Text {
    private:
        PtrToRawText ptr;

        Text (PtrToRawText rawText) {
            ptr = rawText;
        }

    public:
        Text () : ptr (RawText::getEmpty ()) {
        }

        Text (const Text& text) : ptr (text.ptr) {
        }

        Text (const char* source, uint sourceLength) : ptr (RawText::make (source, sourceLength)) {
        }

        Text (const char* source) : ptr (source ? RawText::make (source, strlen (source)) : RawText::getEmpty ()) {
        }

        // do not subclass this type
        ~Text () {
            ptr = 0;
        }

        uint length () const {
            return ptr->getLength();
        }

        uint capacity () const {
            return ptr->getCapacity();
        }

        const char* get () const {
            return ptr->get ();
        }

        char* get () {
            return ptr->get ();
        }

        operator const char* () const {
            return get ();
        }

        operator char* () {
            return get ();
        }

        Text& set (const Text& text) {
            ptr = text.ptr;
            return *this;
        }

        Text& set (const char* source, uint sourceLength) {
            ptr = RawText::make (source, sourceLength);
            return *this;
        }

        Text& set (const char* source) {
            ptr = source ? RawText::make (source, strlen (source)) : RawText::getEmpty ();
            return *this;
        }

        Text& operator = (const Text& text) {
            return set (text);
        }

        Text& operator = (const char* source) {
            return set (source);
        }

        Text& clear () {
            ptr = RawText::getEmpty ();
            return *this;
        }

        bool operator == (const Text& text) const {
            return ((ptr == text.ptr) || (compare (text) == 0));
        }

        bool operator == (const char* source) const {
            return (compare (source) == 0);
        }

        bool operator != (const Text& text) const {
            return ((ptr != text.ptr) && (compare (text) != 0));
        }

        bool operator != (const char* source) const {
            return (compare (source) != 0);
        }

        bool operator < (const Text& text) const {
            return (compare (text) < 0);
        }

        bool operator < (const char* source) const {
            return (compare (source) < 0);
        }

        bool operator <= (const Text& text) const {
            return (compare (text) <= 0);
        }

        bool operator <= (const char* source) const {
            return (compare (source) <= 0);
        }

        bool operator >= (const Text& text) const {
            return (compare (text) >= 0);
        }

        bool operator >= (const char* source) const {
            return (compare (source) >= 0);
        }

        bool operator > (const Text& text) const {
            return (compare (text) > 0);
        }

        bool operator > (const char* source) const {
            return (compare (source) > 0);
        }

        Text makeUnique () const {
            return Text (RawText::make(get (), length ()));
        }

        // compare
        int compare (const char* source, uint sourceLength) const {
            return ptr->compare(source, sourceLength);
        }

        int compare (const char* source) const {
            return ptr->compare(source, source ? strlen (source) : 0);
        }

        int compare (const Text& text) const {
            return compare (text.get (), text.length());
        }

        // append
        Text& append (const char* source, uint sourceLength) {
            if (! ptr->append(source, sourceLength)) {
                PtrToRawText newPtr = RawText::make (length () + sourceLength, ptr->get(), ptr->getLength ());
                newPtr->append (source, sourceLength);
                ptr = newPtr;
            }
            return *this;
        }

        Text& append (const char* source) {
            return append (source, source? strlen (source) : 0);
        }

        Text& append (const Text& text) {
            return append (text.get (), text.length());
        }

        // find
        // substr
        // format

        // operators
        Text& operator += (const Text& text) {
            return append (text);
        }

        Text& operator += (const char* source) {
            return append (source);
        }

        Text& operator << (const Text& text) {
            return append (text);
        }

        Text& operator << (const char* source) {
            return append (source);
        }

        Text& operator << (bool value) {
            return append (value ? "true" : "false");
        }

        // XXX TODO temporary hack, should do something smarter here
        template<typename ValueType>
        Text&  operator << (const ValueType& value) {
            ostringstream out;
            out << fixed << setprecision(8) << value;
            return append (out.str ().c_str ());
        }

        // Text& operator >> (to a value type)
};

inline
bool operator == (const char* lhs, const Text& rhs) {
    return rhs == lhs;
}

inline
bool operator != (const char* lhs, const Text& rhs) {
    return rhs != lhs;
}

inline
Text operator + (const Text& lhs, const Text& rhs) {
    return lhs.makeUnique().append (rhs);
}

inline
Text operator + (const char* lhs, const Text& rhs) {
    return Text (lhs).append (rhs);
}

inline
Text operator + (const Text& lhs, const char* rhs) {
    return lhs.makeUnique().append (rhs);
}

inline
ostream& operator << (ostream& ostr, const Text& text) {
    return ostr << (const char*) text;
}

#define END_LINE    "\n";

#endif  //  _TEXT_H_
