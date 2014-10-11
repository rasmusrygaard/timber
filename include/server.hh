// -*- C++ -*-
// Automatically generated from include/server.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_INCLUDE_SERVER_HH_INCLUDED__
#define __XDR_INCLUDE_SERVER_HH_INCLUDED__ 1

#include <xdrpp/types.h>

using longstring = xdr::xstring<>;

struct kvpair {
  xdr::xstring<512> key{};
  xdr::xstring<> val{};
};
namespace xdr {
template<> struct xdr_traits<::kvpair>
  : xdr_struct_base<field_ptr<::kvpair,
                              decltype(::kvpair::key),
                              &::kvpair::key>,
                    field_ptr<::kvpair,
                              decltype(::kvpair::val),
                              &::kvpair::val>> {
  template<typename Archive> static void
  save(Archive &ar, const ::kvpair &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.val, "val");
  }
  template<typename Archive> static void
  load(Archive &ar, ::kvpair &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.val, "val");
  }
};
}

enum ServerError : std::uint32_t {
  KEY_NOT_FOUND,
  NO_PARENT,
  HAS_CHILDREN,
  MALFORMED_KEY,
};
namespace xdr {
template<> struct xdr_traits<::ServerError>
  : xdr_integral_base<::ServerError, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::ServerError val) {
    switch (val) {
    case ::KEY_NOT_FOUND:
      return "KEY_NOT_FOUND";
    case ::NO_PARENT:
      return "NO_PARENT";
    case ::HAS_CHILDREN:
      return "HAS_CHILDREN";
    case ::MALFORMED_KEY:
      return "MALFORMED_KEY";
    default:
      return nullptr;
    }
  }
};
}

struct MaybeBool {
private:
  std::uint32_t discriminant_;
  union {
    bool result_;
    ServerError errorCode_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == 0 ? 1
      : which == 1 ? 2
      : 0;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case 0:
      _f(&MaybeBool::result_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&MaybeBool::errorCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in MaybeBool");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~MaybeBool();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  MaybeBool(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  MaybeBool(const MaybeBool &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  MaybeBool(MaybeBool &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~MaybeBool() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  MaybeBool &operator=(const MaybeBool &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~MaybeBool();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  MaybeBool &operator=(MaybeBool &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~MaybeBool();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  MaybeBool &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  bool &result() {
    if (_xdr_field_number(discriminant_) == 1)
      return result_;
    throw xdr::xdr_wrong_union("MaybeBool: result accessed when not selected");
  }
  const bool &result() const {
    if (_xdr_field_number(discriminant_) == 1)
      return result_;
    throw xdr::xdr_wrong_union("MaybeBool: result accessed when not selected");
  }
  ServerError &errorCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errorCode_;
    throw xdr::xdr_wrong_union("MaybeBool: errorCode accessed when not selected");
  }
  const ServerError &errorCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errorCode_;
    throw xdr::xdr_wrong_union("MaybeBool: errorCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::MaybeBool> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::MaybeBool;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "result"
      : which == 1 ? "errorCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::MaybeBool &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in MaybeBool");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::MaybeBool &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in MaybeBool");
  }
  template<typename Archive> static void
  load(Archive &ar, ::MaybeBool &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct MabyeString {
private:
  std::uint32_t discriminant_;
  union {
    longstring result_;
    ServerError errorCode_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == 0 ? 1
      : which == 1 ? 2
      : 0;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case 0:
      _f(&MabyeString::result_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&MabyeString::errorCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in MabyeString");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~MabyeString();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  MabyeString(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  MabyeString(const MabyeString &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  MabyeString(MabyeString &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~MabyeString() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  MabyeString &operator=(const MabyeString &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~MabyeString();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  MabyeString &operator=(MabyeString &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~MabyeString();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  MabyeString &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  longstring &result() {
    if (_xdr_field_number(discriminant_) == 1)
      return result_;
    throw xdr::xdr_wrong_union("MabyeString: result accessed when not selected");
  }
  const longstring &result() const {
    if (_xdr_field_number(discriminant_) == 1)
      return result_;
    throw xdr::xdr_wrong_union("MabyeString: result accessed when not selected");
  }
  ServerError &errorCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errorCode_;
    throw xdr::xdr_wrong_union("MabyeString: errorCode accessed when not selected");
  }
  const ServerError &errorCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errorCode_;
    throw xdr::xdr_wrong_union("MabyeString: errorCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::MabyeString> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::MabyeString;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "result"
      : which == 1 ? "errorCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::MabyeString &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in MabyeString");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::MabyeString &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in MabyeString");
  }
  template<typename Archive> static void
  load(Archive &ar, ::MabyeString &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct MaybeSetString {
private:
  std::uint32_t discriminant_;
  union {
    xdr::xvector<longstring> result_;
    ServerError errorCode_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == 0 ? 1
      : which == 1 ? 2
      : 0;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case 0:
      _f(&MaybeSetString::result_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&MaybeSetString::errorCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in MaybeSetString");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~MaybeSetString();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  MaybeSetString(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  MaybeSetString(const MaybeSetString &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  MaybeSetString(MaybeSetString &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~MaybeSetString() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  MaybeSetString &operator=(const MaybeSetString &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~MaybeSetString();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  MaybeSetString &operator=(MaybeSetString &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~MaybeSetString();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  MaybeSetString &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  xdr::xvector<longstring> &result() {
    if (_xdr_field_number(discriminant_) == 1)
      return result_;
    throw xdr::xdr_wrong_union("MaybeSetString: result accessed when not selected");
  }
  const xdr::xvector<longstring> &result() const {
    if (_xdr_field_number(discriminant_) == 1)
      return result_;
    throw xdr::xdr_wrong_union("MaybeSetString: result accessed when not selected");
  }
  ServerError &errorCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errorCode_;
    throw xdr::xdr_wrong_union("MaybeSetString: errorCode accessed when not selected");
  }
  const ServerError &errorCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errorCode_;
    throw xdr::xdr_wrong_union("MaybeSetString: errorCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::MaybeSetString> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::MaybeSetString;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "result"
      : which == 1 ? "errorCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::MaybeSetString &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in MaybeSetString");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::MaybeSetString &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in MaybeSetString");
  }
  template<typename Archive> static void
  load(Archive &ar, ::MaybeSetString &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct api_v1 {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "server_api";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "api_v1";

  struct create_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "create";
    using arg_type = kvpair;
    using arg_wire_type = kvpair;
    using res_type = MaybeBool;
    using res_wire_type = MaybeBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.create(std::forward<A>(a)...)) {
      return c.create(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.create(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.create(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct remove_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "remove";
    using arg_type = longstring;
    using arg_wire_type = longstring;
    using res_type = MaybeBool;
    using res_wire_type = MaybeBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.remove(std::forward<A>(a)...)) {
      return c.remove(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.remove(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.remove(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct set_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 3;
    static constexpr const char *proc_name = "set";
    using arg_type = kvpair;
    using arg_wire_type = kvpair;
    using res_type = MaybeBool;
    using res_wire_type = MaybeBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.set(std::forward<A>(a)...)) {
      return c.set(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.set(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.set(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct get_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 4;
    static constexpr const char *proc_name = "get";
    using arg_type = longstring;
    using arg_wire_type = longstring;
    using res_type = MaybeString;
    using res_wire_type = MaybeString;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.get(std::forward<A>(a)...)) {
      return c.get(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.get(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.get(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct list_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 5;
    static constexpr const char *proc_name = "list";
    using arg_type = longstring;
    using arg_wire_type = longstring;
    using res_type = MavbeSetString;
    using res_wire_type = MavbeSetString;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.list(std::forward<A>(a)...)) {
      return c.list(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.list(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.list(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<create_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<remove_t>(std::forward<A>(a)...);
      return true;
    case 3:
      t.template dispatch<set_t>(std::forward<A>(a)...);
      return true;
    case 4:
      t.template dispatch<get_t>(std::forward<A>(a)...);
      return true;
    case 5:
      t.template dispatch<list_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    create(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<create_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<create_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    remove(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<remove_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<remove_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    set(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<set_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<set_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    get(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<get_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<get_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    list(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<list_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<list_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_INCLUDE_SERVER_HH_INCLUDED__
