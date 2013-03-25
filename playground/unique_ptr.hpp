#ifndef UNIQUE_PTR_HPP_7SNLCJJQ
#define UNIQUE_PTR_HPP_7SNLCJJQ

// Minimal/dirty implementation of unique_ptr

#define MOVABLE_BUT_NOT_COPYABLE(TYPE)\
      private:\
  TYPE(TYPE &);\
  TYPE& operator=(TYPE &);\
  public:\
  operator ::my::rv<TYPE>&() \
  {  return *static_cast< ::my::rv<TYPE>* >(this);  }\
  operator const ::my::rv<TYPE>&() const \
  {  return *static_cast<const ::my::rv<TYPE>* >(this);  }\
  private:\


// workarounds for compilers- breaks some aliasing rules
#ifdef __GNUC__ 
  #define MY_MOVE_ATTRIBUTE_MAY_ALIAS __attribute__((__may_alias__)) 
#else 
  #define MY_MOVE_ATTRIBUTE_MAY_ALIAS
#endif

namespace my {

/** rvalue struct for rvalue emulation */
template <class T>
class rv : public T
{
    rv();
    ~rv();
    rv(rv const&);
    void operator=(rv const&);
} MY_MOVE_ATTRIBUTE_MAY_ALIAS;

template <typename T>
struct default_deleter {
    void operator()(T* ptr)
    {
        delete ptr;
    }
};

/**
 * Unique ptr
 */
template <typename T, typename D = default_deleter<T> >
class unique_ptr {

    T* m_ptr;

    // prohibit copying
    MOVABLE_BUT_NOT_COPYABLE(unique_ptr)

    void clean_up() {
        D()(m_ptr);
    }

public:
    class moved;

    explicit unique_ptr(T* p)
        : m_ptr(p)
    { }

    unique_ptr(rv<unique_ptr>& r )
        : m_ptr(r.release())
    { }

    ~unique_ptr() {
        clean_up();
    }

    T* get() const {
        return m_ptr;
    }

    T* operator -> () const {
        return this->get();
    }

    operator bool () {
        return m_ptr;
    }

    T* release() {
        T* result = m_ptr;
        m_ptr = NULL;
        return result;
    }

    void reset(T* p = NULL) {
        if (m_ptr != p) {
            delete m_ptr;
            m_ptr = p;
        }
    }

    rv<unique_ptr>& move()
    {
        return *static_cast<rv<unique_ptr>* >(this);
    }
};

}


#endif /* end of include guard: UNIQUE_PTR_HPP_7SNLCJJQ */
