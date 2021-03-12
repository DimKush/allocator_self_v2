
#ifndef ALLOC_MEM_EXPORT_H
#define ALLOC_MEM_EXPORT_H

#ifdef ALLOC_MEM_STATIC_DEFINE
#  define ALLOC_MEM_EXPORT
#  define ALLOC_MEM_NO_EXPORT
#else
#  ifndef ALLOC_MEM_EXPORT
#    ifdef alloc_mem_EXPORTS
        /* We are building this library */
#      define ALLOC_MEM_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define ALLOC_MEM_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef ALLOC_MEM_NO_EXPORT
#    define ALLOC_MEM_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef ALLOC_MEM_DEPRECATED
#  define ALLOC_MEM_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef ALLOC_MEM_DEPRECATED_EXPORT
#  define ALLOC_MEM_DEPRECATED_EXPORT ALLOC_MEM_EXPORT ALLOC_MEM_DEPRECATED
#endif

#ifndef ALLOC_MEM_DEPRECATED_NO_EXPORT
#  define ALLOC_MEM_DEPRECATED_NO_EXPORT ALLOC_MEM_NO_EXPORT ALLOC_MEM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef ALLOC_MEM_NO_DEPRECATED
#    define ALLOC_MEM_NO_DEPRECATED
#  endif
#endif

#endif /* ALLOC_MEM_EXPORT_H */
