// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"
#include "LogCabinWrapper.h"
#include "config.h"
#include <string>
#include <iostream>

#include <vector>
#include <fstream>

std::unique_ptr<bool>
api_v1_server::install(std::unique_ptr<ClusterDesc> arg)
{
    std::unique_ptr<bool> res(new bool);

    LogCabinWrapper logCabin;
    auto private_ips = arg->private_ips.nodes;

    std::vector<std::string> nodeList;
    for (auto addr : private_ips) {
        nodeList.push_back(addr);
    }

    // Add numbered hosts.
    Config::addHosts(nodeList, "n", true);
    // Add logcabin hosts.
    Config::addHosts(nodeList, "logcabin", false);

    logCabin.initialize(nodeList);
    *res = true; //TODO: failure modes?
    return res;
}

std::unique_ptr<bool>
api_v1_server::run(std::unique_ptr<ClusterDesc> arg)
{
    std::unique_ptr<bool> res(new bool);

    LogCabinWrapper logCabin;
    auto private_ips = arg->private_ips.nodes;
    std::vector<std::string> nodeList;
    for (auto addr : private_ips) {
        nodeList.push_back(addr);
    }

    // Numbering starts at 1.
    // First node bootraps cluster, last node reconfigures.
    if (arg->nodeId == 1) logCabin.bootstrap(arg->nodeId);
    logCabin.startServer(arg->nodeId);
    if (arg->nodeId == nodeList.size()) logCabin.reconfigure(nodeList);

    *res = true; //TODO: failure modes?
    return res;
}


std::unique_ptr<bool>
api_v1_server::makePartition(std::unique_ptr<Partition> arg)
{
  std::unique_ptr<bool> res(new bool);
  std::cout << "Reached server" << std::endl;
  auto group1_ips = arg->group1;
  auto group2_ips = arg->group2;
/*  
  std::vector<std::string> group1;
  std::vector<std::string> group2;

  for (auto addr : group1_ips) {
    group1.push_back(addr);
  }
  for (auto addr : group2_ips) {
    group2.push_back(addr);
  }


  try {
    std::cout << "partition happens" << std::endl;
  } catch (std::exception &e) {
      std::cout << "HERE" << std::endl;
      if (e.what() != NULL) { std::cerr << e.what() << std::endl; }
  }
*/
  *res = true;
  return res;
}



std::unique_ptr<bool>
api_v1_server::healPartition()
{
  std::unique_ptr<bool> res(new bool);
  
  // Fill in function body here
  
  return res;
}
