// -*- C++ -*-
// Automatically generated from include/server.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_INCLUDE_SERVER_HH_INCLUDED__
#define __XDR_INCLUDE_SERVER_HH_INCLUDED__ 1

#include <xdrpp/types.h>

using longstring = xdr::xstring<>;

enum RaftType : std::uint32_t {
  LogCabinType,
  GoRaftType,
};
namespace xdr {
template<> struct xdr_traits<::RaftType>
  : xdr_integral_base<::RaftType, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::RaftType val) {
    switch (val) {
    case ::LogCabinType:
      return "LogCabinType";
    case ::GoRaftType:
      return "GoRaftType";
    default:
      return nullptr;
    }
  }
};
}

struct api_v1 {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "server_api";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "api_v1";

  struct setup_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "setup";
    using arg_type = RaftType;
    using arg_wire_type = RaftType;
    using res_type = bool;
    using res_wire_type = bool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.setup(std::forward<A>(a)...)) {
      return c.setup(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.setup(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.setup(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<setup_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    setup(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<setup_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<setup_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_INCLUDE_SERVER_HH_INCLUDED__
