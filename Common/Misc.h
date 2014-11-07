#ifndef _MISC_H
#define _MISC_H

template <typename T>
class FastBuf
{
    static const int max_size_ = 20 * 1024;
public:
    FastBuf(int size)
    {
        if (size > max_size_) {
            buf = new T[size];
        } else {
            buf = buf_;
        }
    }
    ~FastBuf()
    {
        if (buf != buf_) {
            delete buf;
        }
    }
    T  buf_[max_size_];
    T *buf;
};

#define GETTER_SETTER(type, name) \
    type name##_; \
    void Set##name(const type& name) { name##_ = name; } \
    type Get##name() { return name##_; }

#endif // _MISC_H
