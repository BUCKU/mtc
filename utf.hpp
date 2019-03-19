# pragma once
# if !defined( __mtc_utf_hpp__ )
# define __mtc_utf_hpp__
# include <cstdint>

# if !defined( __widechar_defined__ )
# define  __widechar_defined__
#   if defined(WCHAR_MAX) && (WCHAR_MAX >> 16) == 0
    typedef wchar_t         widechar;
#   else
    typedef unsigned short  widechar;
#   endif  // size
# endif  // __widechar_defined__

namespace mtc {
namespace utf {

  /*
   *  cbchar( utfstr )
   *  ���������� ���������� ����, ���������� ������ ������ ������.
   */
  size_t  cbchar( const char* str, size_t  len = (size_t)-1 );

  /*
   *  cbchar( uint32_t uc )
   *  ���������� ���������� ��������, �������� ���������� ���� ������.
   */
  size_t  cbchar( uint32_t n );

  /*
   *  cbchar( const widechar* wcs, size_t len )
   *  ���������� ���������� ����, ���������� ������
   */
  size_t  cbchar( const widechar* wcs, size_t len = (size_t)-1 );

  /*
      encode( pszout, cchout, pwssrc, cchsrc )
      �������� wcs-������ � utf8
   */  
  size_t  encode( char* out, size_t, const widechar* src, size_t = (size_t)-1 );

  /*
   *  decode( pwsout, cchout, pszsrc, cchsrc )
   *  ���������� ������ �� utf8 � utf16
   */
  size_t  decode( widechar* out, size_t, const char* src, size_t = (size_t)-1 );

}}

# endif   // !__mtc_utf_hpp__
