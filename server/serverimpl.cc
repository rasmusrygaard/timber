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



bool
is_current_node_in_group1(std::vector<std::string> group1)
{
  std::string host_ip;
  host_ip = Config::get_hostname();
  if (host_ip=="") return false;
  std::ifstream f("/etc/hosts", std::fstream::in | std::fstream::app | std::fstream::out);
  if (f.fail()) return false;
  std::string line;
  bool found = false;
  while (std::getline(f,line)) {
    size_t idx = line.find(host_ip);
    if (idx != std::string::npos) {
        for (auto n : group1) {
          if(line.find(n) != std::string::npos) return true;
        }
    }
  }
  return found;
}

std::unique_ptr<bool>
api_v1_server::makePartition(std::unique_ptr<Partition> arg)
{
  std::unique_ptr<bool> res(new bool);
 
  auto group1_nodes = arg->group1;
  auto group2_nodes = arg->group2;
 
  std::vector<std::string> group1;
  std::vector<std::string> group2;

  for (auto num : group1_nodes) {
    group1.push_back("n" + std::to_string(num));
  }
  for (auto num : group2_nodes) {
    group2.push_back("n" + std::to_string(num));
  }
  try {
    bool in_group1;
    in_group1 = is_current_node_in_group1(group1);
    if(in_group1) {
      *res = Config::partitionNodes(group2);
    } else {
      *res = Config::partitionNodes(group1);
    }
  } catch (std::exception &e) {
      if (e.what() != NULL) { std::cerr << e.what() << std::endl; }
  }



  // Fill in function body here
  *res = true;
  return res;
}

std::unique_ptr<bool>
api_v1_server::splitCluster(std::unique_ptr<Partition> arg)
{
  std::unique_ptr<bool> res(new bool);
/*
  auto group1_nodes = arg->group1;
  auto group2_nodes = arg->group2;
 
  std::vector<std::string> group1;
  std::vector<std::string> group2;

  for (auto num : group1_nodes) {
    group1.push_back("n" + std::to_string(num));
  }
  for (auto num : group2_nodes) {
    group2.push_back("n" + std::to_string(num));
  }

  try {
    bool in_group1;
    in_group1 = is_current_node_in_group1(group1);
    if(in_group1) {
      *res = Config::partitionNodes(group2);
    } else {
      *res = Config::partitionNodes(group1);
    }
  } catch (std::exception &e) {
      if (e.what() != NULL) { std::cerr << e.what() << std::endl; }
  }
*/
  *res = true;
  return res;
}



std::unique_ptr<bool>
api_v1_server::healPartition(std::unique_ptr<int> arg)
{
  std::unique_ptr<bool> res(new bool);
  
  std::vector<std::string> nodes;
  for (int i=1; i<=*arg; i++) {
    nodes.push_back("n" + std::to_string(i));
  }

  *res = Config::healNodes(nodes);
  
  return res;
}
