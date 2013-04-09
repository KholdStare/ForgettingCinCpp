#ifndef UNIQUE_PTR_HPP_7SNLCJJQ
#define UNIQUE_PTR_HPP_7SNLCJJQ

// Minimal/dirty implementation of unique_ptr
// A lot of it is basically simplifying the boost implementation,
// and using only the bare minimum of what is needed for C++03
//
// Please see http://en.cppreference.com/w/cpp/memory/unique_ptr 
// for the "canonical" interface for this class in C++11.
// Most of the functionailty is preserved here


// this macro can be placed within a class definition to
// designate it as "movable but not copyable.
// The "magic" here are the implicit casts operators
// to the rvalue wrapper (defined below)
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

/**
 * rvalue struct for rvalue emulation.
 * Allows distinguishing copies vs moves in C++03
 */
template <class T>
class rv : public T
{
    rv();
    ~rv();
    rv(rv const&);
    void operator=(rv const&);
} MY_MOVE_ATTRIBUTE_MAY_ALIAS;

/** deletes a pointer */
template <typename T>
struct default_deleter {
    void operator()(T* ptr)
    {
        delete ptr;
    }
};
// TODO: specialize for array delete

/**
 * Unique ptr
 */
template <typename T, typename D = default_deleter<T> >
class unique_ptr {

    T* m_ptr;

    // prohibit copying, but allow "moves"
    MOVABLE_BUT_NOT_COPYABLE(unique_ptr)

    // Uses custom deleter to clean up the resource
    void clean_up() {
        D()(m_ptr);
    }

public:
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
            clean_up();
            m_ptr = p;
        }
    }

    /**
     * Explicitly move from this unique_ptr. Allows
     * distinguishing rvalues.
     *
     * In C++11, this is achieved through a single
     * std::move function for all types, and not through
     * a move member as done here. I implemented it this
     * way to keep things simple and local to the class
     * for the sake of demonstration.
     */
    rv<unique_ptr>& move()
    {
        return *static_cast<rv<unique_ptr>* >(this);
    }
};

}


#endif /* end of include guard: UNIQUE_PTR_HPP_7SNLCJJQ */
