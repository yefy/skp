#ifndef SKPGLOBAL_H
#define SKPGLOBAL_H


template <typename T> static inline T *skpGetPtrHelper(T *ptr) { return ptr; }


#define SKP_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(skpGetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(skpGetPtrHelper(d_ptr)); } \
    friend class Class##Private;

#define SKP_DECLARE_PUBLIC(Class) \
    inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
    friend class Class;

#define SKP_D(Class) Class##Private * const skpD = d_func()
#define SKP_Q(Class) Class * const skpQ = q_func()


typedef void (*skp_callback_function)(int fd, short event, void *arg);


#endif // SKPGLOBAL_H
