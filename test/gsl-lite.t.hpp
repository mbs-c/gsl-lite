// Copyright 2015 by Martin Moene
//
// gsl-lite is based on GSL: Guidelines Support Library.
// For more information see https://github.com/gsl-lite/gsl-lite
//
// This code is licensed under the MIT License (MIT).
//

#ifndef GSL_TEST_GSL_LITE_HPP_INCLUDED
#define GSL_TEST_GSL_LITE_HPP_INCLUDED

// Compiler warning suppression for ancient versions of Visual Studio:

#if defined( _MSC_VER )
# if _MSC_VER < 1900
#  pragma warning( disable : 4127 ) // conditional expression is constant
# endif
# if _MSC_VER < 1800
#  pragma warning( disable : 4345 ) // behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
# endif
#endif
#if defined( _MSC_VER ) && _MSC_VER < 1800
# pragma warning( disable : 4345 ) // behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
#endif

// Select GSL Lite for VC6 if compiling with VC6:

#if defined(_MSC_VER) && _MSC_VER < 1300
# include "gsl/gsl-lite-vc6.hpp"
#else
# include "gsl/gsl-lite.hpp"
#endif

// Limit C++ Core Guidelines checking to GSL Lite:

#ifdef gsl_TESTING_CPPCORECHECK_
# include <CppCoreCheck/warnings.h>
# pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)
#endif

// Compiler warning suppression for usage of lest:

#ifdef __clang__
# pragma clang diagnostic ignored "-Wunknown-pragmas" // don't warn if any of the following warnings are introduced later than the current compiler version
# pragma clang diagnostic ignored "-Wunknown-warning-option" // see above, but for newer compilers
# pragma clang diagnostic ignored "-Wstring-conversion"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wunused-template"
# pragma clang diagnostic ignored "-Wunused-function"
# pragma clang diagnostic ignored "-Wunused-member-function"
# pragma clang diagnostic warning "-Wunknown-warning-option" // we want to see warnings about unknown warning options
# pragma clang diagnostic warning "-Wunknown-pragmas" // we want to see warnings about unknown pragmas
#elif defined( __GNUC__ )
# pragma GCC   diagnostic ignored "-Wunused-parameter"
# pragma GCC   diagnostic ignored "-Wunused-function"
#elif defined( _MSC_VER )
# if gsl_BETWEEN(gsl_COMPILER_MSVC_VERSION, 1, 140)
#  pragma warning( disable : 4702 ) // unreachable code
#  pragma warning( disable : 4512 ) // assignment operator could not be generated 
# endif // gsl_BETWEEN(gsl_COMPILER_MSVC_VERSION, 1, 140)
# if gsl_BETWEEN(gsl_COMPILER_MSVC_VERSION, 1, 142) || !gsl_CPP17_OR_GREATER
#  pragma warning( disable : 4100 ) // unreferenced formal parameter
# endif // !__has_cpp_attribute(maybe_unused) || !gsl_CPP17_OR_GREATER
#endif

// gsl-lite only depends on <ios>, but we're instantiating templates using streams, so we need <ostream>.
#include <ostream>

// Some test cases use stringstreams and other iostream functionality.
#include <sstream>
#include <iostream>

// Additional general requirements.
#include <cstring>


namespace lest {

// These functions cannot be found via ADL, so we have to define them before including lest.

#if gsl_HAVE( ARRAY )
template< typename T, std::size_t N >
inline std::ostream & operator<<( std::ostream & os, std::array<T,N> const & )
{
    return os << std::hex << "[std::array[" << N << "]";
}
#endif

#if gsl_HAVE( WCHAR )
// We do this with a loop and explicit casts to avoid warnings about implicit narrowing casts (which we don't care about because we don't have to handle non-ASCII strings in the tests).
inline std::string narrowString( std::wstring const & str )
{
    std::string result(str.size(), '\0');
    for (std::size_t i = 0, n = str.size(); i != n; ++i)
    {
        result[i] = static_cast<char>(str[i]);
    }
    return result;
}

inline std::ostream & operator<<( std::ostream & os, std::wstring const & text )
{
    return os << narrowString( text );
}
#endif // gsl_HAVE( WCHAR )

} // namespace lest

#include "lest_cpp03.hpp"

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace gsl {

inline const void * nullptr_void() { return gsl_nullptr; }

// use operator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

inline std::ostream & operator<<( std::ostream & os, byte b )
{
    return os << std::hex << "0x" << to_integer<int>(b);
}

template< typename T >
inline std::ostream & operator<<( std::ostream & os, span<T> s )
{
    return os << "[", std::copy( s.begin(), s.end(), std::ostream_iterator<T>(os, ",") ), os << "]";
}

} // namespace gsl

inline void suppress_warning_unused_template_ensure_sentinel()
{
    (void) gsl::ensure_z( "zero-terminated" );
}

#endif // GSL_TEST_GSL_LITE_HPP_INCLUDED

// end of file
