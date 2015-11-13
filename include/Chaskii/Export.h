/* 
 *  Chaskii - Copyright (C) 2008  Digital Production & Design
 */

#ifndef CHASKII_EXPORT_DEF
#define CHASKII_EXPORT_DEF 1

#if defined(_MSC_VER)
//    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
//    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4275 )
//    #pragma warning( disable : 4290 )
//    #pragma warning( disable : 4786 )
//    #pragma warning( disable : 4541 )
//    #pragma warning( disable : 4800 )
    #pragma warning( disable : 4996 )
#endif



#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( II_LIBRARY_STATIC )
    #    define II_EXPORT
    #  elif defined( II_LIBRARY )
    #    define II_EXPORT   __declspec(dllexport)
    #  else
    #    define II_EXPORT   __declspec(dllimport)
    #  endif 
#else
    #  define II_EXPORT
#endif  

// set up define for whether member templates are supported by VisualStudio compilers, i.e for compilers after VS6.0
// also set a define to know when VS6.0 is being used.
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
#else
#  define __VS6_IMPLEMENTATION
# endif
#endif

/* Define NULL pointer value */

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#endif
