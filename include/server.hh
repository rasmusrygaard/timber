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

using IPAddr = xdr::xstring<15>;

struct IPList {
  xdr::xvector<IPAddr> nodes{};
};
namespace xdr {
template<> struct xdr_traits<::IPList>
  : xdr_struct_base<field_ptr<::IPList,
                              decltype(::IPList::nodes),
                              &::IPList::nodes>> {
  template<typename Archive> static void
  save(Archive &ar, const ::IPList &obj) {
    archive(ar, obj.nodes, "nodes");
  }
  template<typename Archive> static void
  load(Archive &ar, ::IPList &obj) {
    archive(ar, obj.nodes, "nodes");
  }
};
}

struct ClusterDesc {
  RaftType type{};
  IPList public_ips{};
  IPList private_ips{};
  std::int32_t nodeId{};
};
namespace xdr {
template<> struct xdr_traits<::ClusterDesc>
  : xdr_struct_base<field_ptr<::ClusterDesc,
                              decltype(::ClusterDesc::type),
                              &::ClusterDesc::type>,
                    field_ptr<::ClusterDesc,
                              decltype(::ClusterDesc::public_ips),
                              &::ClusterDesc::public_ips>,
                    field_ptr<::ClusterDesc,
                              decltype(::ClusterDesc::private_ips),
                              &::ClusterDesc::private_ips>,
                    field_ptr<::ClusterDesc,
                              decltype(::ClusterDesc::nodeId),
                              &::ClusterDesc::nodeId>> {
  template<typename Archive> static void
  save(Archive &ar, const ::ClusterDesc &obj) {
    archive(ar, obj.type, "type");
    archive(ar, obj.public_ips, "public_ips");
    archive(ar, obj.private_ips, "private_ips");
    archive(ar, obj.nodeId, "nodeId");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ClusterDesc &obj) {
    archive(ar, obj.type, "type");
    archive(ar, obj.public_ips, "public_ips");
    archive(ar, obj.private_ips, "private_ips");
    archive(ar, obj.nodeId, "nodeId");
  }
};
}

struct Partition {
  xdr::xvector<std::int32_t> group1{};
  xdr::xvector<std::int32_t> group2{};
};
namespace xdr {
template<> struct xdr_traits<::Partition>
  : xdr_struct_base<field_ptr<::Partition,
                              decltype(::Partition::group1),
                              &::Partition::group1>,
                    field_ptr<::Partition,
                              decltype(::Partition::group2),
                              &::Partition::group2>> {
  template<typename Archive> static void
  save(Archive &ar, const ::Partition &obj) {
    archive(ar, obj.group1, "group1");
    archive(ar, obj.group2, "group2");
  }
  template<typename Archive> static void
  load(Archive &ar, ::Partition &obj) {
    archive(ar, obj.group1, "group1");
    archive(ar, obj.group2, "group2");
  }
};
}

struct api_v1 {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "server_api";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "api_v1";

  struct install_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "install";
    using arg_type = ClusterDesc;
    using arg_wire_type = ClusterDesc;
    using res_type = bool;
    using res_wire_type = bool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.install(std::forward<A>(a)...)) {
      return c.install(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.install(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.install(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct run_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "run";
    using arg_type = ClusterDesc;
    using arg_wire_type = ClusterDesc;
    using res_type = bool;
    using res_wire_type = bool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.run(std::forward<A>(a)...)) {
      return c.run(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.run(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.run(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct makePartition_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 3;
    static constexpr const char *proc_name = "makePartition";
    using arg_type = Partition;
    using arg_wire_type = Partition;
    using res_type = bool;
    using res_wire_type = bool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.makePartition(std::forward<A>(a)...)) {
      return c.makePartition(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.makePartition(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.makePartition(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct healPartition_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 4;
    static constexpr const char *proc_name = "healPartition";
    using arg_type = void;
    using arg_wire_type = xdr::xdr_void;
    using res_type = bool;
    using res_wire_type = bool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.healPartition(std::forward<A>(a)...)) {
      return c.healPartition(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.healPartition(std::forward<A>(a)...)) {
      return c.healPartition(std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<install_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<run_t>(std::forward<A>(a)...);
      return true;
    case 3:
      t.template dispatch<makePartition_t>(std::forward<A>(a)...);
      return true;
    case 4:
      t.template dispatch<healPartition_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    install(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<install_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<install_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    run(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<run_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<run_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    makePartition(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<makePartition_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<makePartition_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    healPartition(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<healPartition_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<healPartition_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_INCLUDE_SERVER_HH_INCLUDED__
