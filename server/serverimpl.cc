// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"
#include "LogCabinWrapper.h"
#include <string>
#include <vector>
#include <fstream>

std::unique_ptr<bool>
api_v1_server::setup(std::unique_ptr<ClusterDesc> arg)
{
  std::unique_ptr<bool> res(new bool);

  LogCabinWrapper logCabin;
  auto private_ips = arg->private_ips.nodes;
  std::vector<std::string> nodeList;
  std::ofstream f("/etc/hosts");
  for (auto addr : private_ips) {
	  f << addr + " logcabin" << std::endl;
	  nodeList.push_back(addr);
  }
  logCabin.initialize(nodeList);
  if (arg->nodeId == 0) {
	  logCabin.bootstrap(arg->nodeId);
  } else {
  	  logCabin.startServer(arg->nodeId);
  }

  *res = true; //TODO: failure modes?
  return res;
}

std::unique_ptr<bool>
api_v1_server::makePartition(std::unique_ptr<Partition> arg)
{
  std::unique_ptr<bool> res(new bool);

  // Fill in function body here

  return res;
}

std::unique_ptr<bool>
api_v1_server::healPartition()
{
  std::unique_ptr<bool> res(new bool);

  // Fill in function body here

  return res;
}
