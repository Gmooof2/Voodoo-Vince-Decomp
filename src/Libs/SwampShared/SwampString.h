// HEADER: named for PDB class SwampString; no file recorded
// Partial: declares only what matched units use so far.
#ifndef SWAMPSTRING_H
#define SWAMPSTRING_H

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    ~basic_string() { _Tidy(true); }
    const E* c_str() const { return (_Myptr()); }
    const E* _Myptr() const { return (16 <= _Myres ? _Bx._Ptr : _Bx._Buf); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _Alval;                       // 0x00
    union {
        E _Buf[16];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A> basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const E* _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

#endif
