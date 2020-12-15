// Copyright (c) 2018-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/config/

#ifndef TAO_CONFIG_INTERNAL_DEBUG_TRAITS_HPP
#define TAO_CONFIG_INTERNAL_DEBUG_TRAITS_HPP

#include <tao/json/contrib/variant_traits.hpp>

#include "array.hpp"
#include "concat.hpp"
#include "entry.hpp"
#include "json.hpp"
#include "json_traits.hpp"
#include "object.hpp"
#include "pegtl.hpp"

namespace tao::config::internal
{
   template< typename T >
   struct debug_traits
      : json::traits< T >
   {};

   template<>
   struct debug_traits< void >
      : json::traits< void >
   {};

   template<>
   struct debug_traits< pegtl::position >
   {
      TAO_JSON_DEFAULT_KEY( "position" );

      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const pegtl::position& p )
      {
         c.string( p.source + ':' + std::to_string( p.line ) + ':' + std::to_string( p.column ) );
      }
   };

   template<>
   struct debug_traits< part_star_t >
   {
      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const part_star_t /*unused*/ )
      {
         c.string( "star" );
      }
   };

   template<>
   struct debug_traits< part_minus_t >
   {
      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const part_minus_t /*unused*/ )
      {
         c.string( "minus" );
      }
   };

   template<>
   struct debug_traits< key1_kind >
   {
      TAO_JSON_DEFAULT_KEY( "key1_kind" );

      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const key1_kind k )
      {
         switch( k ) {
            case key1_kind::star:
               c.string( "star" );
               return;
            case key1_kind::minus:
               c.string( "minus" );
               return;
            case key1_kind::name:
               c.string( "name" );
               return;
            case key1_kind::index:
               c.string( "index" );
               return;
         }
         assert( false );  // UNREACHABLE
      }
   };

   template<>
   struct debug_traits< key1_part >
   {
      TAO_JSON_DEFAULT_KEY( "key1_part" );

      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const key1_part& p )
      {
         c.begin_object( 2 );
         c.key( "key1_kind" );
         json::events::produce< Traits >( c, p.kind() );
         c.member();
         c.key( "key1_data" );
         json::events::produce< Traits >( c, p.data );
         c.member();
         c.end_object( 2 );
      }
   };

   template<>
   struct debug_traits< key1 >
      : json::traits< std::vector< key1_part > >
   {
      TAO_JSON_DEFAULT_KEY( "key1" );
   };

   template<>
   struct debug_traits< ref2_kind >
   {
      TAO_JSON_DEFAULT_KEY( "ref2_kind" );

      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const ref2_kind k )
      {
         switch( k ) {
            case ref2_kind::minus:
               c.string( "minus" );
               return;
            case ref2_kind::name:
               c.string( "name" );
               return;
            case ref2_kind::index:
               c.string( "index" );
               return;
            case ref2_kind::vector:
               c.string( "reference" );
               return;
         }
         assert( false );  // UNREACHABLE
      }
   };

   template<>
   struct debug_traits< ref2_part >
   {
      TAO_JSON_DEFAULT_KEY( "ref2_part" );

      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const ref2_part& p )
      {
         c.begin_object( 2 );
         c.key( "ref2_kind" );
         json::events::produce< Traits >( c, p.kind() );
         c.member();
         c.key( "ref2_data" );
         json::events::produce< Traits >( c, p.data );
         c.member();
         c.end_object( 2 );
      }
   };

   template<>
   struct debug_traits< ref2 >
      : json::traits< std::vector< ref2_part > >
   {
      TAO_JSON_DEFAULT_KEY( "ref2" );
   };

   template<>
   struct debug_traits< entry_kind >
   {
      TAO_JSON_DEFAULT_KEY( "entry_kind" );

      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const entry_kind k )
      {
         switch( k ) {
            case entry_kind::value:
               c.string( "value" );
               return;
            case entry_kind::reference:
               c.string( "reference" );
               return;
            case entry_kind::array:
               c.string( "array" );
               return;
            case entry_kind::object:
               c.string( "object" );
               return;
         }
         assert( false );  // UNREACHABLE
      }
   };

   template<>
   struct debug_traits< json::basic_value< json_traits > >
   {
      template< template< typename... > class, typename Consumer >
      static void produce( Consumer& c, const json::basic_value< json_traits >& v )
      {
         json::events::from_value( c, v );
      }
   };

   template<>
   struct debug_traits< entry >
   {
      template< template< typename... > class Traits, typename Consumer >
      static void produce( Consumer& c, const entry& v )
      {
         c.begin_object();
         // c.key( "type" );
         // json::events::produce< Traits >( c, v.type() );
         // c.member();
         switch( v.kind() ) {
            case entry_kind::value:
               c.key( "value" );
               json::events::produce< Traits >( c, v.get_value() );
               break;
            case entry_kind::reference:
               c.key( "reference" );
               json::events::produce< Traits >( c, v.get_reference() );
               break;
            case entry_kind::array:
               c.key( "array" );
               json::events::produce< Traits >( c, v.get_array() );
               break;
            case entry_kind::object:
               c.key( "object" );
               json::events::produce< Traits >( c, v.get_object() );
               break;
            default:
               assert( false );  // UNREACHABLE
         }
         c.member();
         c.end_object();
      }
   };

   template<>
   struct debug_traits< concat >
      : json::binding::object< // TAO_JSON_BIND_REQUIRED( "position", &concat::p ),
                               TAO_JSON_BIND_REQUIRED( "concat_list", &concat::concat ) >
   {};

   template<>
   struct debug_traits< array >
      : json::binding::object< // TAO_JSON_BIND_REQUIRED( "position", &array::position ),
                               TAO_JSON_BIND_REQUIRED( "array_data", &array::array ) >
   {};

   template<>
   struct debug_traits< object >
      : json::binding::object< // TAO_JSON_BIND_REQUIRED( "position", &object::position ),
                               TAO_JSON_BIND_REQUIRED( "object_data", &object::object ) >
   {};

   template< typename T >
   struct debug_traits< const T* >
   {
      template< template< typename... > class Traits >
      static void assign( json::basic_value< Traits >& v, const T* p )
      {
         v.set_opaque_ptr( p );
      }
   };

   template< typename T >
   struct debug_traits< T* >
      : debug_traits< const T* >
   {};

}  // namespace tao::config::internal

#endif
