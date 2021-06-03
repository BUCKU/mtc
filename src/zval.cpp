# include "../zmap.h"
# include <type_traits>

namespace mtc
{
  template <class A, class B> static  zval  GetMul( A a, B b )  {  return a * b;  }
  template <class A, class B> static  zval  GetDiv( A a, B b )  {  return a / b;  }
  template <class A, class B> static  zval  GetAdd( A a, B b )  {  return a + b;  }
  template <class A, class B> static  zval  GetSub( A a, B b )  {  return a - b;  }

  template <class A, class B> static  zval  GetAnd( A a, B b )  {  return a & b;  }
  template <class A, class B> static  zval  GetXor( A a, B b )  {  return a ^ b;  }
  template <class A, class B> static  zval  Get_Or( A a, B b )  {  return a | b;  }
  template <class A, class B> static  zval  GetPct( A a, B b )  {  return a % b;  }
  template <class A, class B> static  zval  GetShl( A a, B b )  {  return a << b;  }
  template <class A, class B> static  zval  GetShr( A a, B b )  {  return a >> b;  }

  /*
    операции над zval и целым или дробным значением - макрогенерация:
    template <class V> zval  (#)( zval, V )
  */
  # define  derive_operation_xvalue_value( funcname )                     \
  template <class V>  zval  funcname( const zval& z, V v )                \
    {                                                                     \
      switch ( z.get_type() )                                             \
      {                                                                   \
        case zval::z_char:    return funcname( *z.get_char(), v );        \
        case zval::z_byte:    return funcname( *z.get_byte(), v );        \
        case zval::z_int16:   return funcname( *z.get_int16(), v );       \
        case zval::z_int32:   return funcname( *z.get_int32(), v );       \
        case zval::z_int64:   return funcname( *z.get_int64(), v );       \
        case zval::z_word16:  return funcname( *z.get_word16(), v );      \
        case zval::z_word32:  return funcname( *z.get_word32(), v );      \
        case zval::z_word64:  return funcname( *z.get_word64(), v );      \
        case zval::z_float:   return funcname( *z.get_float(), v );       \
        case zval::z_double:  return funcname( *z.get_double(), v );      \
        default:        return zval();                                    \
      }                                                                   \
    }
    derive_operation_xvalue_value( GetMul )
    derive_operation_xvalue_value( GetDiv )
    derive_operation_xvalue_value( GetAdd )
    derive_operation_xvalue_value( GetSub )
  # undef  derive_operation_xvalue_value

  /*
    операции над zval и zval - макрогенерация:
    zval  (#)( const zval&, const zval& )
  */
  # define  derive_operation_xvalue_xvalue( funcname )                    \
  zval  funcname( const zval& z, const zval& x )                          \
    {                                                                     \
      switch ( x.get_type() )                                             \
      {                                                                   \
        case zval::z_char:    return funcname( z, *x.get_char() );        \
        case zval::z_byte:    return funcname( z, *x.get_byte() );        \
        case zval::z_int16:   return funcname( z, *x.get_int16() );       \
        case zval::z_int32:   return funcname( z, *x.get_int32() );       \
        case zval::z_int64:   return funcname( z, *x.get_int64() );       \
        case zval::z_word16:  return funcname( z, *x.get_word16() );      \
        case zval::z_word32:  return funcname( z, *x.get_word32() );      \
        case zval::z_word64:  return funcname( z, *x.get_word64() );      \
        case zval::z_float:   return funcname( z, *x.get_float() );       \
        case zval::z_double:  return funcname( z, *x.get_double() );      \
        default:        return zval();                                    \
      }                                                                   \
    }
    derive_operation_xvalue_xvalue( GetMul )
    derive_operation_xvalue_xvalue( GetDiv )
    derive_operation_xvalue_xvalue( GetSub )
  # undef derive_operation_xvalue_xvalue

  /*
    специализации StrCat с поддержкой суммирования однотипных строк
  */
  template <class A, class B>
  zval  StrCat( A, B )  {  return zval();  }
  zval  StrCat( const charstr& a, const charstr& b )  {  return std::move( zval( std::move( a + b ) ) );  }
  zval  StrCat( const widestr& a, const widestr& b )  {  return std::move( zval( std::move( a + b ) ) );  }
  zval  StrCat( char a, const charstr& b )  {  return std::move( zval( std::move( a + b ) ) );  }
  zval  StrCat( const charstr& a, char b )  {  return std::move( zval( std::move( a + b ) ) );  }

  template <class V>
  zval  StrCat( const zval& z, V v )
    {
      switch ( z.get_type() )
      {
        case zval::z_char:    return StrCat( *z.get_char(),   v );
        case zval::z_byte:    return StrCat( *z.get_byte(),   v );
        case zval::z_int16:   return StrCat( *z.get_int16(),  v );
        case zval::z_int32:   return StrCat( *z.get_int32(),  v );
        case zval::z_int64:   return StrCat( *z.get_int64(),  v );
        case zval::z_word16:  return StrCat( *z.get_word16(), v );
        case zval::z_word32:  return StrCat( *z.get_word32(), v );
        case zval::z_word64:  return StrCat( *z.get_word64(), v );
        case zval::z_float:   return StrCat( *z.get_float(),  v );
        case zval::z_double:  return StrCat( *z.get_double(), v );
        case zval::z_charstr: return StrCat( *z.get_charstr(), v );
        case zval::z_widestr: return StrCat( *z.get_widestr(), v );
        default:              return zval();
      }
    }

  auto  GetAdd( const zval& z, const zval& x ) -> zval
    {
      switch ( x.get_type() )
      {
        case zval::z_char:    return mtc::GetAdd( z, *x.get_char() );
        case zval::z_byte:    return mtc::GetAdd( z, *x.get_byte() );
        case zval::z_int16:   return mtc::GetAdd( z, *x.get_int16() );
        case zval::z_int32:   return mtc::GetAdd( z, *x.get_int32() );
        case zval::z_int64:   return mtc::GetAdd( z, *x.get_int64() );
        case zval::z_word16:  return mtc::GetAdd( z, *x.get_word16() );
        case zval::z_word32:  return mtc::GetAdd( z, *x.get_word32() );
        case zval::z_word64:  return mtc::GetAdd( z, *x.get_word64() );
        case zval::z_float:   return mtc::GetAdd( z, *x.get_float() );
        case zval::z_double:  return mtc::GetAdd( z, *x.get_double() );
        case zval::z_charstr: return mtc::StrCat( z, *x.get_charstr() );
        case zval::z_widestr: return mtc::StrCat( z, *x.get_widestr() );
        default:        return zval();
      }
    }

  /*
    макрогенерация битовых операций над целочисленными значениями
  */
  # define  derive_math( funcname )                                       \
  template <class V>  zval  funcname( const zval& z, V v )                \
    {                                                                     \
      switch ( z.get_type() )                                             \
      {                                                                   \
        case zval::z_char:    return funcname( *z.get_char(), v );        \
        case zval::z_byte:    return funcname( *z.get_byte(), v );        \
        case zval::z_int16:   return funcname( *z.get_int16(), v );       \
        case zval::z_int32:   return funcname( *z.get_int32(), v );       \
        case zval::z_int64:   return funcname( *z.get_int64(), v );       \
        case zval::z_word16:  return funcname( *z.get_word16(), v );      \
        case zval::z_word32:  return funcname( *z.get_word32(), v );      \
        case zval::z_word64:  return funcname( *z.get_word64(), v );      \
        default:        return zval();                                    \
      }                                                                   \
    }                                                                     \
  zval  funcname( const zval& z, const zval& x )                          \
    {                                                                     \
      switch ( x.get_type() )                                             \
      {                                                                   \
        case zval::z_char:    return funcname( z, *x.get_char() );        \
        case zval::z_byte:    return funcname( z, *x.get_byte() );        \
        case zval::z_int16:   return funcname( z, *x.get_int16() );       \
        case zval::z_int32:   return funcname( z, *x.get_int32() );       \
        case zval::z_int64:   return funcname( z, *x.get_int64() );       \
        case zval::z_word16:  return funcname( z, *x.get_word16() );      \
        case zval::z_word32:  return funcname( z, *x.get_word32() );      \
        case zval::z_word64:  return funcname( z, *x.get_word64() );      \
        default:        return zval();                                    \
      }                                                                   \
    }
    derive_math( GetPct )
    derive_math( GetShl )
    derive_math( GetShr )
    derive_math( GetAnd )
    derive_math( GetXor )
    derive_math( Get_Or )
  # undef derive_math

  // compare procedures

  /*
    базовый функционал сравнения:
      <   0x01
      >   0x02
      ==  0x04
          0x00 - операция не поддерживается
  */
  template <class C> struct is_zmap {  static  constexpr bool  value = false;  };
  template <> struct is_zmap<zmap> {  static  constexpr bool  value = true;  };

  template <class C> struct is_zval {  static  constexpr bool  value = false;  };
  template <> struct is_zval<zval> {  static  constexpr bool  value = true;  };

  template <class C> struct is_uuid {  static  constexpr bool  value = false;  };
  template <> struct is_uuid<uuid> {  static  constexpr bool  value = true;  };

  class compare
  {
    template <class T>  using is_signed = std::is_signed<T>;
    template <class T>  using is_unsigned = std::is_unsigned<T>;
    template <class T>  using is_floating = std::is_floating_point<T>;
    template <class T>  using is_number = std::integral_constant<bool,
      is_signed<T>::value || is_unsigned<T>::value || is_floating<T>::value>;

    template <class C>
    struct is_string {  static  constexpr bool  value = false;  };
    template <class C, class T, class A>  struct is_string<std::basic_string<C, T, A>> {  static  constexpr bool  value = true;  };

    template <class C>
    struct is_vector {  static  constexpr bool  value = false;  };
    template <class V, class A> struct is_vector<std::vector<V, A>> {  static  constexpr bool  value = true;  };

    struct gt_value { template <class A, class B> static unsigned test( const A&, const B& )  {  return zval::compare_gt;  } };
    struct lt_value { template <class A, class B> static unsigned test( const A&, const B& )  {  return zval::compare_lt;  } };

    class number_value
    {
      struct easy_diff
      {
        template <class A, class B>
        static  unsigned  test( A a, B b )
          {  return  (a > b) ? zval::compare_gt : (a < b) ? zval::compare_lt : zval::compare_eq;  }
      };

      class signed_value
      {
        struct to_unsigned
        {
          template <class A, class B>
          static  unsigned  test( A a, B b )
          {
            if ( a >= 0 )
            {
              return (((typename std::make_unsigned<A>::type)a) > b) ? zval::compare_gt :
                     (((typename std::make_unsigned<A>::type)a) < b) ? zval::compare_lt : zval::compare_eq;
            }
            return zval::compare_lt;
          }
        };

      public:
        template <class A, class B>
        static  unsigned  test( A a, const B& b )
        {
          using comparator_type =
            typename std::conditional<is_signed<B>::value || is_floating<B>::value, easy_diff, to_unsigned>::type;

          return comparator_type::test( a, b );
        };
      };

      struct unsigned_value
      {
        struct to_signed
        {
          template <class A, class B>
          static  unsigned  test( A a, B b )
          {
            auto  tr = signed_value::test( b, a );

            return tr == zval::compare_gt ? zval::compare_lt :
                   tr == zval::compare_lt ? zval::compare_gt : zval::compare_eq;
          }
        };

      public:
        template <class A, class B>
        static  unsigned  test( A a, const B& b )
        {
          using comparator_type =
            typename std::conditional<is_signed<B>::value, to_signed, easy_diff>::type;

          return comparator_type::test( a, b );
        };
      };

    public:
      template <class A, class B>
      static  unsigned  test( A a, const B& b )
      {
        using any_is_float = std::integral_constant<bool,
          is_floating<A>::value || is_floating<B>::value>;
        using comparator_type =
          typename std::conditional<is_zmap  <B>::value, lt_value,
          typename std::conditional<is_vector<B>::value, lt_value,
          typename std::conditional<is_string<B>::value, lt_value,
          typename std::conditional<is_uuid  <B>::value, lt_value,
          typename std::conditional<any_is_float::value, easy_diff,
          typename std::conditional<is_unsigned<A>::value, unsigned_value,
            signed_value>::type>::type>::type>::type>::type>::type;

        return comparator_type::test( a, b );
      }
    };

  // Строка больше численных значений и uuid, но меньше всех остальных - векторов и zmap'ов
    class string_value
    {
      struct to_string
      {
        template <class S1, class S2>
        static  unsigned  test( const S1& s1, const S2& s2 )
        {
          auto  rc = w_strcmp( s1.c_str(), s2.c_str() );

          return rc > 0 ? zval::compare_gt :
                 rc < 0 ? zval::compare_lt : zval::compare_eq;
        }
      };

    public:
      template <class S, class B>
      static  unsigned  test( const S& s, const B& b )
      {
        using comparator_type =
          typename std::conditional<is_number<B>::value, gt_value,
          typename std::conditional<is_uuid  <B>::value, gt_value,
          typename std::conditional<is_string<B>::value, to_string, lt_value>::type>::type>::type;

        return comparator_type::test( s, b );
      }
    };

    // Вектор больше численных значений, uuid и строк, но меньше, чем zmap
    class vector_value
    {
      struct to_vector
      {
        template <class V1, class V2>
        static  unsigned  test( const V1& a, const V2& b )
        {
          auto  ia = a.begin();
          auto  ib = b.begin();
          auto  rc = (unsigned)0;

          while ( ia != a.end() && ib != b.end() && (rc = compare::test( *ia, *ib )) == zval::compare_eq )
            {  ++ia;  ++ib;  }

          if ( rc == zval::compare_eq )
          {
            rc = a.size() > b.size() ? zval::compare_gt :
                 a.size() < b.size() ? zval::compare_lt : zval::compare_eq;
          }
          return rc;
        }
      };

    public:
      template <class V, class B>
      static  unsigned  test( const V& v, const B& b )
      {
        using comparator_type =
          typename std::conditional<is_zmap  <B>::value, lt_value,
          typename std::conditional<is_vector<B>::value, to_vector, gt_value>::type>::type;

        return comparator_type::test( v, b );
      }
    };

    class zmap_value
    {
      struct to_zmap
      {
        static  unsigned  test( const mtc::zmap& z1, const mtc::zmap& z2 )
        {
          auto  i1 = z1.begin();
          auto  i2 = z2.begin();
          int   rc;

          for ( ; i1 != z1.end() && i1 != z2.end(); ++i1, ++i2 )
          {
            if ( (rc = i1->first.compare( i2->first )) == 0 )
            {
              auto  tr = compare::test( i1->second, i2->second );

              if ( tr != zval::compare_eq )
                return tr;
            } else return rc > 0 ? zval::compare_gt : zval::compare_lt;
          }

          if ( (rc = (i1 != z1.end()) - (i2 != z2.end())) != 0 )
            return rc > 0 ? zval::compare_gt : zval::compare_lt;

          return zval::compare_eq;
        }
      };

    public:
      template <class B>
      static  unsigned  test( const zmap& z, const B& b )
      {
        using comparator_type =
          typename std::conditional<is_zmap<B>::value, to_zmap, gt_value>::type;
        return comparator_type::test( z, b );
      }
    };

    struct zval_value
    {
      template <class B>
      static  unsigned  test( const zval& a, const B& b )
      {
        switch ( a.get_type() )
        {
          case zval::z_char:    return compare::test( *a.get_char(), b );
          case zval::z_byte:    return compare::test( *a.get_byte(), b );
          case zval::z_int16:   return compare::test( *a.get_int16(), b );
          case zval::z_int32:   return compare::test( *a.get_int32(), b );
          case zval::z_int64:   return compare::test( *a.get_int64(), b );
          case zval::z_float:   return compare::test( *a.get_float(), b );
          case zval::z_word16:  return compare::test( *a.get_word16(), b );
          case zval::z_word32:  return compare::test( *a.get_word32(), b );
          case zval::z_word64:  return compare::test( *a.get_word64(), b );
          case zval::z_double:  return compare::test( *a.get_double(), b );

          case zval::z_uuid:    return compare::test( *a.get_uuid(), b );

          case zval::z_charstr: return compare::test( *a.get_charstr(), b );
          case zval::z_widestr: return compare::test( *a.get_widestr(), b );

          case zval::z_zmap:    return compare::test( *a.get_zmap(), b );

          case zval::z_array_char:    return compare::test( *a.get_char(), b );
          case zval::z_array_byte:    return compare::test( *a.get_array_byte(), b );
          case zval::z_array_int16:   return compare::test( *a.get_array_int16(), b );
          case zval::z_array_word16:  return compare::test( *a.get_array_int32(), b );
          case zval::z_array_int32:   return compare::test( *a.get_array_int64(), b );
          case zval::z_array_word32:  return compare::test( *a.get_array_float(), b );
          case zval::z_array_int64:   return compare::test( *a.get_array_word16(), b );
          case zval::z_array_word64:  return compare::test( *a.get_array_word32(), b );
          case zval::z_array_float:   return compare::test( *a.get_array_word64(), b );
          case zval::z_array_double:  return compare::test( *a.get_array_double(), b );

          case zval::z_array_charstr: return compare::test( *a.get_array_charstr(), b );
          case zval::z_array_widestr: return compare::test( *a.get_array_widestr(), b );
          case zval::z_array_zval:    return compare::test( *a.get_array_zval(), b );
          case zval::z_array_zmap:    return compare::test( *a.get_array_zmap(), b );
          case zval::z_array_uuid:    return compare::test( *a.get_uuid(), b );
          case zval::z_untyped:       return 0x00;
          default:                    break;
        }
        throw std::logic_error( strprintf( "compare operation for zval type '%s' not supported",
          zval::type_name( (zval::z_type)a.get_type() ) ) );
      }
    };

    class uuid_value
    {
      struct to_uuid
      {
        static  unsigned test( const uuid& u1, const uuid& u2 )
        {
          auto  tr = u1.compare( u2 );

          return tr > 0 ? zval::compare_gt :
                 tr < 0 ? zval::compare_lt : zval::compare_eq;
        }
      };

    public:
      template <class B>
      static  unsigned  test( const uuid& u, const B& b )
      {
        using comparator_type =
          typename std::conditional<is_number<B>::value, gt_value,
          typename std::conditional<is_uuid  <B>::value, to_uuid, lt_value>::type>::type;

        return comparator_type::test( u, b );
      }
    };

  public:
    template <class T1, class T2>
    static  unsigned  test( const T1& _1, const T2& _2 )
    {
      using comparator_type =
        typename std::conditional<is_number<T1>::value, number_value,
        typename std::conditional<is_string<T1>::value, string_value,
        typename std::conditional<is_vector<T1>::value, vector_value,
        typename std::conditional<is_uuid  <T1>::value, uuid_value,
        typename std::conditional<is_zval  <T1>::value, zval_value, zmap_value>::type>::type>::type>::type>::type;

      return comparator_type::test( _1, _2 );
    }
    template <class T1>
    static  unsigned  test( const T1& _1, const zval& _2 )
    {
      switch ( _2.get_type() )
      {
        case zval::z_char:    return compare::test( _1, *_2.get_char() );
        case zval::z_byte:    return compare::test( _1, *_2.get_byte() );
        case zval::z_int16:   return compare::test( _1, *_2.get_int16() );
        case zval::z_int32:   return compare::test( _1, *_2.get_int32() );
        case zval::z_int64:   return compare::test( _1, *_2.get_int64() );
        case zval::z_float:   return compare::test( _1, *_2.get_float() );
        case zval::z_word16:  return compare::test( _1, *_2.get_word16() );
        case zval::z_word32:  return compare::test( _1, *_2.get_word32() );
        case zval::z_word64:  return compare::test( _1, *_2.get_word64() );
        case zval::z_double:  return compare::test( _1, *_2.get_double() );

        case zval::z_uuid:    return compare::test( _1, *_2.get_uuid() );

        case zval::z_charstr: return compare::test( _1, *_2.get_charstr() );
        case zval::z_widestr: return compare::test( _1, *_2.get_widestr() );

        case zval::z_zmap:    return compare::test( _1, *_2.get_zmap() );

        case zval::z_array_char:    return compare::test( _1, *_2.get_char() );
        case zval::z_array_byte:    return compare::test( _1, *_2.get_array_byte() );
        case zval::z_array_int16:   return compare::test( _1, *_2.get_array_int16() );
        case zval::z_array_word16:  return compare::test( _1, *_2.get_array_int32() );
        case zval::z_array_int32:   return compare::test( _1, *_2.get_array_int64() );
        case zval::z_array_word32:  return compare::test( _1, *_2.get_array_float() );
        case zval::z_array_int64:   return compare::test( _1, *_2.get_array_word16() );
        case zval::z_array_word64:  return compare::test( _1, *_2.get_array_word32() );
        case zval::z_array_float:   return compare::test( _1, *_2.get_array_word64() );
        case zval::z_array_double:  return compare::test( _1, *_2.get_array_double() );

        case zval::z_array_charstr: return compare::test( _1, *_2.get_array_charstr() );
        case zval::z_array_widestr: return compare::test( _1, *_2.get_array_widestr() );
        case zval::z_array_zval:    return compare::test( _1, *_2.get_array_zval() );
        case zval::z_array_zmap:    return compare::test( _1, *_2.get_array_zmap() );
        case zval::z_array_uuid:    return compare::test( _1, *_2.get_array_uuid() );
        case zval::z_untyped:       return 0x00;
        default:  break;
      }
      throw std::logic_error( strprintf( "compare operation for zval type '%u' not supported",
        _2.get_type() ) );
    }

  };

  // zval implementation

  auto  zval::inner() const -> const zval::inner_t& {  return *reinterpret_cast<const inner_t*>( storage );  }
  auto  zval::inner()       ->       zval::inner_t& {  return *reinterpret_cast<      inner_t*>( storage );  }

  zval::zval(): vx_type( z_untyped )
    {}

  zval::zval( zval&& zv ): vx_type( z_untyped )
    {  fetch( std::move( zv ) );  }

  zval::zval( const zval& zv ): vx_type( z_untyped )
    {  fetch( zv );  }

  zval& zval::operator = ( zval&& zv )
    {  return fetch( std::move( zv ) );  }

  zval& zval::operator = ( const zval& zv )
    {  return fetch( zv );  }

  zval::~zval()
    {  clear();  }

  zval::zval( bool b ): vx_type( z_untyped )
    {  set_byte( b ? 1 : 0 );  }

  zval& zval::operator= ( bool b )
    {  set_byte( b ? 1 : 0 );  return *this;  }

  # define derive_init( _type_ )                    \
  zval::zval( _type_##_t t ): vx_type( z_untyped )  \
    {  set_##_type_( t );  }                        \
  zval& zval::operator = ( _type_##_t t )           \
    {   set_##_type_( t );  return *this;  }
    derive_init( char )
    derive_init( byte )
    derive_init( int16 )
    derive_init( int32 )
    derive_init( int64 )
    derive_init( word16 )
    derive_init( word32 )
    derive_init( word64 )
    derive_init( float )
    derive_init( double )
  # undef derive_init

  zval::zval( const uuid_t& uuid ): vx_type( z_untyped )
    {  set_uuid( uuid );  }
  zval& zval::operator= ( const uuid_t& uuid )
    {  return set_uuid( uuid ), *this;  }

  zval::zval( const char* psz, size_t len ): vx_type( z_untyped )
    {  set_charstr( charstr( psz, len != (size_t)-1 ? len : w_strlen( psz ) ) );  }
  zval& zval::operator = ( const char* psz )
    {  return set_charstr( charstr( psz ) ), *this;  }

  zval::zval( const widechar* psz, size_t len ): vx_type( z_untyped )
    {  set_widestr( widestr( psz, len != (size_t)-1 ? len : w_strlen( psz ) ) );  }
  zval& zval::operator = ( const widechar* psz )
    {  return set_widestr( widestr( psz ) ), *this;  }

  # define derive_init( _type_ )                            \
  zval::zval( const _type_##_t& t ): vx_type( z_untyped )   \
    {  set_##_type_( t );  }                                \
  zval::zval( _type_##_t&& t ): vx_type( z_untyped )        \
    {  set_##_type_( std::move( t ) );  }                   \
  zval& zval::operator = ( const _type_##_t& t )            \
    {  set_##_type_( t );  return *this;  }                 \
  zval& zval::operator = ( _type_##_t&& t )                 \
    {  set_##_type_( std::move( t ) );  return *this;  }
    derive_init( zmap )
    derive_init( charstr )
    derive_init( widestr )

    derive_init( array_char )
    derive_init( array_byte )
    derive_init( array_int16 )
    derive_init( array_int32 )
    derive_init( array_int64 )
    derive_init( array_word16 )
    derive_init( array_word32 )
    derive_init( array_word64 )
    derive_init( array_float )
    derive_init( array_double )
    derive_init( array_charstr )
    derive_init( array_widestr )
    derive_init( array_zval )
    derive_init( array_uuid )
  # undef derive_init

  zval::zval( array_zmap_t&& t ): vx_type( z_untyped )
    {  set_array_zmap( std::move( t ) );  }
  zval& zval::operator = ( array_zmap_t&& t )
    {  set_array_zmap( std::move( t ) );  return *this;  }

  # define  derive_access_type( _type_ )                              \
    const _type_##_t*  zval::get_##_type_() const                     \
      {                                                               \
        return vx_type == z_##_type_ ? &inner().v_##_type_ : nullptr; \
      }                                                               \
    _type_##_t*  zval::get_##_type_()                                 \
      {                                                               \
        return vx_type == z_##_type_ ? &inner().v_##_type_ : nullptr; \
      }                                                               \
    _type_##_t*  zval::set_##_type_( const _type_##_t& t )            \
      {                                                               \
        clear().vx_type = z_##_type_;                                 \
        return new( &inner().v_##_type_ ) _type_##_t( t );            \
      }
    derive_access_type( char )
    derive_access_type( byte )
    derive_access_type( int16 )
    derive_access_type( int32 )
    derive_access_type( int64 )
    derive_access_type( word16 )
    derive_access_type( word32 )
    derive_access_type( word64 )
    derive_access_type( float )
    derive_access_type( double )
    derive_access_type( uuid )
  # undef derive_access_type

  # define derive_access_type( _type_ )                                 \
    const _type_##_t*  zval::get_##_type_() const                       \
      {                                                                 \
        return vx_type == z_##_type_ ? &inner().v_##_type_ : nullptr;   \
      }                                                                 \
    _type_##_t*  zval::get_##_type_()                                   \
      {                                                                 \
        return vx_type == z_##_type_ ? &inner().v_##_type_ : nullptr;   \
      }                                                                 \
    _type_##_t*  zval::set_##_type_( _type_##_t&& t )                   \
      {                                                                 \
        clear().vx_type = z_##_type_;                                   \
        return new( &inner().v_##_type_ ) _type_##_t( std::move( t ) ); \
      }                                                                 \
    _type_##_t*  zval::set_##_type_( const _type_##_t& t )              \
      {                                                                 \
        clear().vx_type = z_##_type_;                                   \
        return new( &inner().v_##_type_ ) _type_##_t( t );              \
      }

    derive_access_type( zmap )
    derive_access_type( charstr )
    derive_access_type( widestr )
  # undef derive_access_type

  auto  zval::set_zmap()  -> zmap_t*
    {
      clear().vx_type = z_zmap;
      return new( &inner().v_zmap ) zmap_t();
    }

  # define derive_access_array( _type_ )                                        \
  array_##_type_* zval::set_array_##_type_( array_##_type_&& t )                \
    {                                                                           \
      clear().vx_type = z_array_##_type_;                                       \
      return new( &inner().v_array_##_type_ ) array_##_type_( std::move( t ) ); \
    }                                                                           \
  array_##_type_* zval::set_array_##_type_( const array_##_type_& t )           \
    {                                                                           \
      clear().vx_type = z_array_##_type_;                                       \
      return new( &inner().v_array_##_type_ ) array_##_type_( t );              \
    }                                                                           \
  array_##_type_* zval::get_array_##_type_()                                    \
    {                                                                           \
      return vx_type == z_array_##_type_ ? &inner().v_array_##_type_ : nullptr; \
    }                                                                           \
  const array_##_type_* zval::get_array_##_type_() const                        \
    {                                                                           \
      return vx_type == z_array_##_type_ ? &inner().v_array_##_type_ : nullptr; \
    }
    derive_access_array( char )
    derive_access_array( byte )
    derive_access_array( int16 )
    derive_access_array( int32 )
    derive_access_array( int64 )
    derive_access_array( word16 )
    derive_access_array( word32 )
    derive_access_array( word64 )
    derive_access_array( float )
    derive_access_array( double )
    derive_access_array( charstr )
    derive_access_array( widestr )
    derive_access_array( zval )
    derive_access_array( uuid )
  # undef derive_access_array

  const array_zmap* zval::get_array_zmap() const
    {
      return vx_type == z_array_zmap ? &inner().v_array_zmap : nullptr;
    }
  array_zmap* zval::get_array_zmap()
    {
      return vx_type == z_array_zmap ? &inner().v_array_zmap : nullptr;
    }
  array_zmap* zval::set_array_zmap()
    {
      clear().vx_type = z_array_zmap;
      return new( &inner().v_array_zmap ) array_zmap();
    }
  array_zmap* zval::set_array_zmap( array_zmap_t&& t )
    {
      clear().vx_type = z_array_zmap;
      return new( &inner().v_array_zmap ) array_zmap( std::move( t ) );
    }

  bool  zval::empty() const
    {
      return vx_type == z_untyped;
    }

  auto  zval::clear() -> zval&
    {
      switch ( vx_type )
      {
      # define  destruct( _type_ )  case z_##_type_:  inner().v_##_type_.~_type_##_t();  break;
        destruct( char )
        destruct( byte )
        destruct( int16 )
        destruct( int32 )
        destruct( int64 )
        destruct( word16 )
        destruct( word32 )
        destruct( word64 )
        destruct( float )
        destruct( double )

        destruct( charstr )
        destruct( widestr )
        destruct( zmap )
        destruct( uuid )

        destruct( array_char )
        destruct( array_byte )
        destruct( array_int16 )
        destruct( array_int32 )
        destruct( array_int64 )
        destruct( array_word16 )
        destruct( array_word32 )
        destruct( array_word64 )
        destruct( array_float )
        destruct( array_double )
        destruct( array_charstr )
        destruct( array_widestr )
        destruct( array_zval )
        destruct( array_zmap )
        destruct( array_uuid )
      # undef destruct
      }
      vx_type = z_untyped;
      return *this;
    }

  auto  zval::get_type() const -> unsigned
    {  return vx_type;  }

  auto  zval::is_numeric() const -> bool
    {  return get_type() >= z_char && get_type() <= z_double;  }

  zval  zval::operator *  ( const zval& r ) const {  return GetMul( *this, r );  }
  zval  zval::operator /  ( const zval& r ) const {  return GetDiv( *this, r );  }
  zval  zval::operator %  ( const zval& r ) const {  return GetPct( *this, r );  }
  zval  zval::operator +  ( const zval& r ) const {  return GetAdd( *this, r );  }
  zval  zval::operator -  ( const zval& r ) const {  return GetSub( *this, r );  }
  zval  zval::operator << ( const zval& r ) const {  return GetShl( *this, r );  }
  zval  zval::operator >> ( const zval& r ) const {  return GetShr( *this, r );  }
  zval  zval::operator &  ( const zval& r ) const {  return GetAnd( *this, r );  }
  zval  zval::operator ^  ( const zval& r ) const {  return GetXor( *this, r );  }
  zval  zval::operator |  ( const zval& r ) const {  return Get_Or( *this, r );  }

  zval  zval::operator ~ ()  const
    {
      switch ( get_type() )
      {
        case z_char:    return ~*get_char();
        case z_byte:    return ~*get_byte();
        case z_int16:   return ~*get_int16();
        case z_int32:   return ~*get_int32();
        case z_int64:   return ~*get_int64();
        case z_word16:  return ~*get_word16();
        case z_word32:  return ~*get_word32();
        case z_word64:  return ~*get_word64();
        default:        return zval(); 
      }
    }

  size_t  zval::GetBufLen() const
  {
    switch ( vx_type )
    {
  # define derive_size_plain( _type_ )  case z_##_type_: return 1 + sizeof(_type_##_t);
  # define derive_size_smart( _type_ )  case z_##_type_: return 1 + ::GetBufLen( inner().v_##_type_ );
      derive_size_plain( char )
      derive_size_plain( byte )
      derive_size_plain( int16 )
      derive_size_plain( word16 )

      derive_size_smart( int32 )
      derive_size_smart( word32 )
      derive_size_smart( int64 )
      derive_size_smart( word64 )
      derive_size_smart( float )
      derive_size_smart( double )
      derive_size_smart( charstr )
      derive_size_smart( widestr )
      derive_size_smart( zmap )
      derive_size_smart( uuid )
      derive_size_smart( array_char )
      derive_size_smart( array_byte )
      derive_size_smart( array_int16 )
      derive_size_smart( array_word16 )
      derive_size_smart( array_int32 )
      derive_size_smart( array_word32 )
      derive_size_smart( array_int64 )
      derive_size_smart( array_word64 )
      derive_size_smart( array_float )
      derive_size_smart( array_double )
      derive_size_smart( array_charstr )
      derive_size_smart( array_widestr )
      derive_size_smart( array_zval )
      derive_size_smart( array_zmap )
      derive_size_smart( array_uuid )
  # undef derive_size_smart
  # undef derive_size_plain
      case z_untyped: return ::GetBufLen( vx_type );
      default:  return 0;
    }
  }

  auto  zval::CompTo( const zval& x ) const -> unsigned
    {
      return compare::test( *this, x );
    }

  bool  zval::operator == ( const zval& z ) const
    {
      if ( get_type() != z.get_type() )
        return false;
      switch ( get_type() )
      {
      # define derive_compare( _type_ ) case z_##_type_:  return *get_##_type_() == *z.get_##_type_();
        derive_compare( char )
        derive_compare( byte )
        derive_compare( int16 )
        derive_compare( word16 )
        derive_compare( int32 )
        derive_compare( word32 )
        derive_compare( int64 )
        derive_compare( word64 )
        derive_compare( float )
        derive_compare( double )

        derive_compare( charstr )
        derive_compare( widestr )

        derive_compare( zmap )
        derive_compare( uuid )

        derive_compare( array_char )
        derive_compare( array_byte )
        derive_compare( array_int16 )
        derive_compare( array_word16 )
        derive_compare( array_int32 )
        derive_compare( array_word32 )
        derive_compare( array_int64 )
        derive_compare( array_word64 )
        derive_compare( array_float )
        derive_compare( array_double )

        derive_compare( array_charstr )
        derive_compare( array_widestr )

        derive_compare( array_zmap )
        derive_compare( array_uuid )

      # undef derive_compare
        default:  return get_type() == z_untyped;
      }
    }

  auto  zval::type_name( z_type type ) -> const char* 
    {
      switch ( type )
      {
        case z_char:          return "char";
        case z_byte:          return "byte";
        case z_int16:         return "int16";
        case z_int32:         return "int32";
        case z_int64:         return "int64";
        case z_word16:        return "word16";
        case z_word32:        return "word32";
        case z_word64:        return "word64";
        case z_float:         return "float";
        case z_double:        return "double";

        case z_charstr:       return "charstr";
        case z_widestr:       return "widestr";

        case z_zmap:          return "zmap";
        case z_uuid:          return "uuid";

        case z_array_char:    return "array_char";
        case z_array_byte:    return "array_byte";
        case z_array_int16:   return "array_int16";
        case z_array_int32:   return "array_int32";
        case z_array_int64:   return "array_int64";
        case z_array_word16:  return "array_word16";
        case z_array_word32:  return "array_word32";
        case z_array_word64:  return "array_word64";
        case z_array_float:   return "array_float";
        case z_array_double:  return "array_double";

        case z_array_charstr: return "array_charstr";
        case z_array_widestr: return "array_widestr";
        case z_array_zmap:    return "array_zmap";
        case z_array_zval:    return "array_zval";
        case z_array_uuid:    return "array_uuid";

        case z_untyped:       return "untyped";
        default:              throw std::invalid_argument( "undefined zval type" );
      }
    }

  std::string zval::to_string() const
    {
      switch ( get_type() )
      {
        case z_char:          return std::move( to_string( *get_char() ) );
        case z_byte:          return std::move( to_string( *get_byte() ) );
        case z_int16:         return std::move( to_string( *get_int16() ) );
        case z_int32:         return std::move( to_string( *get_int32() ) );
        case z_int64:         return std::move( to_string( *get_int64() ) );
        case z_word16:        return std::move( to_string( *get_word16() ) );
        case z_word32:        return std::move( to_string( *get_word32() ) );
        case z_word64:        return std::move( to_string( *get_word64() ) );
        case z_float:         return std::move( to_string( *get_float() ) );
        case z_double:        return std::move( to_string( *get_double() ) );

        case z_charstr:       return std::move( to_string( *get_charstr() ) );
        case z_widestr:       return std::move( to_string( *get_widestr() ) );

        case z_zmap:          return std::move( to_string( *get_zmap() ) );
        case z_uuid:          return std::move( mtc::to_string( *get_uuid() ) );

        case z_array_char:    return std::move( to_string( *get_array_char() ) );
        case z_array_byte:    return std::move( to_string( *get_array_byte() ) );
        case z_array_int16:   return std::move( to_string( *get_array_int16() ) );
        case z_array_int32:   return std::move( to_string( *get_array_int32() ) );
        case z_array_int64:   return std::move( to_string( *get_array_int64() ) );
        case z_array_word16:  return std::move( to_string( *get_array_word16() ) );
        case z_array_word32:  return std::move( to_string( *get_array_word32() ) );
        case z_array_word64:  return std::move( to_string( *get_array_word64() ) );
        case z_array_float:   return std::move( to_string( *get_array_float() ) );
        case z_array_double:  return std::move( to_string( *get_array_double() ) );

        case z_array_charstr: return std::move( to_string( *get_array_charstr() ) );
        case z_array_widestr: return std::move( to_string( *get_array_widestr() ) );
        case z_array_zmap:    return std::move( to_string( *get_array_zmap() ) );
        case z_array_zval:    return std::move( to_string( *get_array_zval() ) );
        case z_array_uuid:    return std::move( to_string( *get_array_uuid() ) );

        case z_untyped:       return "<untyped>";
        default:
          throw std::invalid_argument( "undefined xvalue<> type" );
      }
    }

  auto  zval::fetch( zval&& zv ) -> zval&
    {
      if ( this != &zv )
        switch ( clear().vx_type = zv.vx_type )
        {
        # define  move( _type_ )  case z_##_type_:  \
          new( &inner().v_##_type_ ) _type_##_t( std::move( zv.inner().v_##_type_ ) );  break;
          move( char )
          move( byte )
          move( int16 )
          move( int32 )
          move( int64 )
          move( word16 )
          move( word32 )
          move( word64 )
          move( float )
          move( double )

          move( charstr )
          move( widestr )
          move( zmap )
          move( uuid )

          move( array_char )
          move( array_byte )
          move( array_int16 )
          move( array_int32 )
          move( array_int64 )
          move( array_word16 )
          move( array_word32 )
          move( array_word64 )
          move( array_float )
          move( array_double )
          move( array_charstr )
          move( array_widestr )
          move( array_zval )
          move( array_zmap )
          move( array_uuid )
        # undef move
        }
      return *this;
    }

  auto  zval::fetch( const zval& zv ) -> zval&
    {
      if ( this != &zv )
        switch ( clear().vx_type = zv.vx_type )
        {
        # define  copy( _type_ )  case z_##_type_:    \
        new( &inner().v_##_type_ ) _type_##_t( zv.inner().v_##_type_ );  break;
          copy( char )
          copy( byte )
          copy( int16 )
          copy( int32 )
          copy( int64 )
          copy( word16 )
          copy( word32 )
          copy( word64 )
          copy( float )
          copy( double )

          copy( charstr )
          copy( widestr )
          copy( zmap )
          copy( uuid )

          copy( array_char )
          copy( array_byte )
          copy( array_int16 )
          copy( array_int32 )
          copy( array_int64 )
          copy( array_word16 )
          copy( array_word32 )
          copy( array_word64 )
          copy( array_float )
          copy( array_double )
          copy( array_charstr )
          copy( array_widestr )
          copy( array_zval )
          copy( array_zmap )
          copy( array_uuid )
        # undef copy
        }
      return *this;
    }

  // zval::dump implementation

  template <unsigned z_type, class T>
  auto  zval::dump::get() const -> value_t<T>
    {
      return source != nullptr && (byte)*source == z_type ? value_t<T>::as_src( 1 + source ) : value_t<T>();
    }

  auto  zval::dump::get_type() const -> unsigned  {  unsigned t = z_untyped;  ::FetchFrom( source, t ); return t;  }

  auto  zval::dump::get_char() const -> value_t<char> {  return get<z_char, char>();  }
  auto  zval::dump::get_byte() const -> value_t<byte> {  return get<z_byte, byte>();  }
  auto  zval::dump::get_int16() const -> value_t<int16_t> {  return get<z_int16, int16_t>();  }
  auto  zval::dump::get_int32() const -> value_t<int32_t> {  return get<z_int32, int32_t>();  }
  auto  zval::dump::get_int64() const -> value_t<int64_t> {  return get<z_int64, int64_t>();  }
  auto  zval::dump::get_word16() const -> value_t<word16_t> {  return get<z_word16, word16_t>();  }
  auto  zval::dump::get_word32() const -> value_t<word32_t> {  return get<z_word32, word32_t>();  }
  auto  zval::dump::get_word64() const -> value_t<word64_t> {  return get<z_word64, word64_t>();  }
  auto  zval::dump::get_float() const -> value_t<float> {  return get<z_float, float>();  }
  auto  zval::dump::get_double() const -> value_t<double> {  return get<z_double, double>();  }
  auto  zval::dump::get_charstr() const -> value_t<charstr> {  return get<z_charstr, charstr>();  }
  auto  zval::dump::get_widestr() const -> value_t<widestr> {  return get<z_widestr, widestr>();  }
  auto  zval::dump::get_uuid() const -> value_t<uuid> {  return get<z_uuid, uuid>();  }
  auto  zval::dump::get_zmap() const -> value_t<zmap::dump> {  return get<z_zmap, zmap::dump>();  }

  auto  zval::dump::get_array_char() const -> value_t<array_t<char>>  {  return get<z_array_char, array_t<char>>();  }
  auto  zval::dump::get_array_byte() const -> value_t<array_t<byte>> {  return get<z_array_byte, array_t<byte>>();  }
  auto  zval::dump::get_array_int16() const -> value_t<array_t<int16_t>> {  return get<z_array_int16, array_t<int16_t>>();  }
  auto  zval::dump::get_array_int32() const -> value_t<array_t<int32_t>> {  return get<z_array_int32, array_t<int32_t>>();  }
  auto  zval::dump::get_array_int64() const -> value_t<array_t<int64_t>> {  return get<z_array_int64, array_t<int64_t>>();  }
  auto  zval::dump::get_array_word16() const -> value_t<array_t<word16_t>> {  return get<z_array_word16, array_t<word16_t>>();  }
  auto  zval::dump::get_array_word32() const -> value_t<array_t<word32_t>> {  return get<z_array_word32, array_t<word32_t>>();  }
  auto  zval::dump::get_array_word64() const -> value_t<array_t<word64_t>> {  return get<z_array_word64, array_t<word64_t>>();  }
  auto  zval::dump::get_array_float() const -> value_t<array_t<float>> {  return get<z_array_float, array_t<float>>();  }
  auto  zval::dump::get_array_double() const -> value_t<array_t<double>> {  return get<z_array_double, array_t<double>>();  }
  auto  zval::dump::get_array_charstr() const -> value_t<array_t<charstr>> {  return get<z_array_charstr, array_t<charstr>>();  }
  auto  zval::dump::get_array_widestr() const -> value_t<array_t<widestr>> {  return get<z_array_widestr, array_t<widestr>>();  }
  auto  zval::dump::get_array_uuid() const -> value_t<array_t<uuid>> {  return get<z_array_uuid, array_t<uuid>>();  }
  auto  zval::dump::get_array_zval() const -> value_t<array_t<zval::dump>> {  return get<z_array_zval, array_t<zval::dump>>();  }
  auto  zval::dump::get_array_zmap() const -> value_t<array_t<zmap::dump>> {  return get<z_array_zmap, array_t<zmap::dump>>();  }

  bool  zval::dump::operator==( const zval& v ) const
    {
      auto  mytype = get_type();

      if ( mytype != v.get_type() )
        return false;

      switch ( mytype )
      {
        case z_char:      return *get_char() == *v.get_char();
        case z_byte:      return *get_byte() == *v.get_byte();
        case z_int16:     return *get_int16() == *v.get_int16();
        case z_int32:     return *get_int32() == *v.get_int32();
        case z_int64:     return *get_int64() == *v.get_int64();
        case z_word16:    return *get_word16() == *v.get_word16();
        case z_word32:    return *get_word32() == *v.get_word32();
        case z_word64:    return *get_word64() == *v.get_word64();
        case z_float:     return *get_float() == *v.get_float();
        case z_double:    return *get_double() == *v.get_double();
        case z_charstr:   return *get_charstr() == *v.get_charstr();
        case z_widestr:   return *get_widestr() == *v.get_widestr();
        case z_uuid:      return *get_uuid() == *v.get_uuid();
        case z_zmap:      return *get_zmap() == *v.get_zmap();

        case z_array_char:      return *get_array_char() == *v.get_array_char();
        case z_array_byte:      return *get_array_byte() == *v.get_array_byte();
        case z_array_int16:     return *get_array_int16() == *v.get_array_int16();
        case z_array_int32:     return *get_array_int32() == *v.get_array_int32();
        case z_array_int64:     return *get_array_int64() == *v.get_array_int64();
        case z_array_word16:    return *get_array_word16() == *v.get_array_word16();
        case z_array_word32:    return *get_array_word32() == *v.get_array_word32();
        case z_array_word64:    return *get_array_word64() == *v.get_array_word64();
        case z_array_float:     return *get_array_float() == *v.get_array_float();
        case z_array_double:    return *get_array_double() == *v.get_array_double();
        case z_array_charstr:   return *get_array_charstr() == *v.get_array_charstr();
        case z_array_widestr:   return *get_array_widestr() == *v.get_array_widestr();
        case z_array_uuid:      return *get_array_uuid() == *v.get_array_uuid();
        case z_array_zval:      return *get_array_zval() == *v.get_array_zval();
        case z_array_zmap:      return *get_array_zmap() == *v.get_array_zmap();

        default:  return false;
      }
    }

  // zval::view implementation

  template <class T>  auto  zval::view::make_value( const T* t ) -> value_t<T>
    {  return t != nullptr ? value_t<T>::as_ptr( t ) : value_t<T>();  }
/*
  auto  zval::view::make_view( const value_t<zmap::dump>& s ) -> value_t<view>
    {  return s->source != nullptr ? value_t<view>( dump( s->source ) ) : value_t<view>();  }
  auto  zval::view::make_view( const mtc::zmap* zmap ) -> value_t<view>
    {  return zmap != nullptr ? value_t<view>( zval( *zmap ) ) : value_t<view>();  }*/
//  template <class T>  auto  zval::view::make_view( const value_t<T>& t ) -> value_t<view>
//    {  return t != nullptr ? value_t<view>( *t ) : value_t<view>();  }
//  template <class T>  auto  zmap::view::make_view( const T* t ) -> value_t<view>
//    {  return t != nullptr ? value_t<view>( *t ) : value_t<view>();  }
//  auto  zval::view::make_view( const value_t<dump>& t ) -> value_t<view>
//    {  return t != nullptr ? value_t<view>( *t ) : value_t<view>();  }

  auto  zval::view::get_char() const -> value_t<char>
    {  return asDump.source != nullptr ? asDump.get_char() : make_value( asZval.get_char() );  }
  auto  zval::view::get_byte() const -> value_t<byte>
    {  return asDump.source != nullptr ? asDump.get_byte() : make_value( asZval.get_byte() );  }
  auto  zval::view::get_int16() const -> value_t<int16_t>
    {  return asDump.source != nullptr ? asDump.get_int16() : make_value( asZval.get_int16() );  }
  auto  zval::view::get_int32() const -> value_t<int32_t>
    {  return asDump.source != nullptr ? asDump.get_int32() : make_value( asZval.get_int32() );  }
  auto  zval::view::get_int64() const -> value_t<int64_t>
    {  return asDump.source != nullptr ? asDump.get_int64() : make_value( asZval.get_int64() );  }
  auto  zval::view::get_word16() const -> value_t<word16_t>
    {  return asDump.source != nullptr ? asDump.get_word16() : make_value( asZval.get_word16() );  }
  auto  zval::view::get_word32() const -> value_t<word32_t>
    {  return asDump.source != nullptr ? asDump.get_word32() : make_value( asZval.get_word32() );  }
  auto  zval::view::get_word64() const -> value_t<word64_t>
    {  return asDump.source != nullptr ? asDump.get_word64() : make_value( asZval.get_word64() );  }
  auto  zval::view::get_float() const -> value_t<float>
    {  return asDump.source != nullptr ? asDump.get_float() : make_value( asZval.get_float() );  }
  auto  zval::view::get_double() const -> value_t<double>
    {  return asDump.source != nullptr ? asDump.get_double() : make_value( asZval.get_double() );  }
  auto  zval::view::get_charstr() const -> value_t<charstr>
    {  return asDump.source != nullptr ? asDump.get_charstr() : make_value( asZval.get_charstr() );  }
  auto  zval::view::get_widestr() const -> value_t<widestr>
    {  return asDump.source != nullptr ? asDump.get_widestr() : make_value( asZval.get_widestr() );  }
  auto  zval::view::get_uuid() const -> value_t<uuid>
    {  return asDump.source != nullptr ? asDump.get_uuid() : make_value( asZval.get_uuid() );  }
  auto  zval::view::get_zmap() const -> value_t<view>
    {  return asDump.source != nullptr ? make_view( asDump.get_zmap() ) : make_view( asZval.get_zmap() );  }
  /*
  auto  zval::view::get_char( const key& k, char c ) const -> char  {  return dump::get( get_char(), c );  }
  auto  zval::view::get_byte( const key& k, byte b ) const -> byte  {  return dump::get( get_byte(), b );  }
  auto  zval::view::get_int16( const key& k, int16_t i ) const -> int16_t  {  return dump::get( get_int16(), i );  }
  auto  zval::view::get_int32( const key& k, int32_t i ) const -> int32_t  {  return dump::get( get_int32(), i );  }
  auto  zval::view::get_int64( const key& k, int64_t i ) const -> int64_t  {  return dump::get( get_int64(), i );  }
  auto  zval::view::get_word16( const key& k, word16_t w ) const -> word16_t  {  return dump::get( get_word16(), w );  }
  auto  zval::view::get_word32( const key& k, word32_t w ) const -> word32_t  {  return dump::get( get_word32(), w );  }
  auto  zval::view::get_word64( const key& k, word64_t w ) const -> word64_t  {  return dump::get( get_word64(), w );  }
  auto  zval::view::get_float( const key& k, float f ) const -> float  {  return dump::get( get_float(), f );  }
  auto  zval::view::get_double( const key& k, double d ) const -> double  {  return dump::get( get_double(), d );  }
  auto  zval::view::get_charstr( const key& k, const charstr& s ) const -> charstr  {  return dump::get( get_charstr(), s );  }
  auto  zval::view::get_widestr( const key& k, const widestr& w ) const -> widestr  {  return dump::get( get_widestr(), w );  }
  auto  zval::view::get_uuid( const key& k, const uuid& w ) const -> uuid  {  return dump::get( get_uuid(), w );  }
  auto  zval::view::get_zmap( const key& k, const zmap& w ) const -> view
    {
      auto  pd = get_zmap( k );

      return pd != nullptr ? view( *pd ) : view( w );
    }*/

}
