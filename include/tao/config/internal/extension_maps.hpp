// Copyright (c) 2018-2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/config/

#ifndef TAO_CONFIG_INTERNAL_EXTENSION_MAPS_HPP
#define TAO_CONFIG_INTERNAL_EXTENSION_MAPS_HPP

#include <utility>

#include "extension_types.hpp"

namespace tao::config::internal
{
   struct extension_maps
   {
      extension_maps() = delete;

      extension_maps( value_extension_map&& in_inner, member_extension_map&& in_member, member_extension_map&& in_value )
         : inner( std::move( in_inner ) ),
           member( std::move( in_member ) ),
           value( std::move( in_value ) )
      {}

      extension_maps( extension_maps&& ) = delete;
      extension_maps( const extension_maps& ) = delete;

      ~extension_maps() = default;

      void operator=( extension_maps&& ) = delete;
      void operator=( const extension_maps& ) = delete;

      const value_extension_map inner;
      const member_extension_map member;
      const member_extension_map value;
   };

}  // namespace tao::config::internal

#endif
