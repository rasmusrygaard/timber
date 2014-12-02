// -*- C++ -*-
// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_SERVERIMPL_HH_INCLUDED__
#define __XDR_SERVER_SERVERIMPL_HH_INCLUDED__ 1

#include "include/server.hh"

class api_v1_server {
public:
  using rpc_interface_type = api_v1;

  std::unique_ptr<bool> install(std::unique_ptr<ClusterDesc> arg);
  std::unique_ptr<bool> run(std::unique_ptr<ClusterDesc> arg);
  std::unique_ptr<bool> makePartition(std::unique_ptr<Partition> arg);
  std::unique_ptr<bool> splitCluster(std::unique_ptr<Partition> arg);
  std::unique_ptr<bool> healPartition();
};

#endif // !__XDR_SERVER_SERVERIMPL_HH_INCLUDED__