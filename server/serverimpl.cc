// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"
#include "LogCabinWrapper.h"
#include "etcd.h"
#include "config.h"
#include <string>
#include <iostream>

#include <vector>
#include <fstream>

std::unique_ptr<bool>
api_v1_server::install(std::unique_ptr<ClusterDesc> cluster)
{
    std::unique_ptr<bool> res(new bool);

    // Add numbered hosts.
    Config::addHosts(cluster, "n", true);

    if (cluster->type == LogCabinType) {
        // Add logcabin hosts.
        Config::addHosts(cluster, "logcabin", false);

        LogCabinWrapper::initialize(cluster);
    } else if (cluster->type == EtcdType) {
        Etcd::initialize(cluster);
    }
    *res = true; //TODO: failure modes?
    return res;
}

std::unique_ptr<bool>
api_v1_server::run(std::unique_ptr<ClusterDesc> cluster)
{
    std::unique_ptr<bool> res(new bool);

    if (cluster->type == LogCabinType) {
        // Numbering starts at 1.
        // First node bootraps cluster, last node reconfigures.
        if (cluster->nodeId == 1) LogCabinWrapper::bootstrap(cluster->nodeId);
        LogCabinWrapper::startServer(cluster->nodeId);
        if (cluster->nodeId == cluster->private_ips.nodes.size()) {
            LogCabinWrapper::reconfigure(cluster);
        }
    } else if (cluster->type == EtcdType) {
        Etcd::startServer(cluster);
    }
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
  bool heal = false;
  for (auto num : group1_nodes) {
    group1.push_back("n" + std::to_string(num));
  }
  for (auto num : group2_nodes) {
    if (num==0) heal = true;
    group2.push_back("n" + std::to_string(num));
  }

  if (heal) {
    *res = Config::partitionNodes(group1, false);
  } else {
    try {
      bool in_group1;
      in_group1 = is_current_node_in_group1(group1);
      if(in_group1) {
        *res = Config::partitionNodes(group2, true);
      } else {
        *res = Config::partitionNodes(group1, true);
      }
    } catch (std::exception &e) {
        if (e.what() != NULL) { std::cerr << e.what() << std::endl; }
    }
  }
  *res = heal;
  return res;
}



int
find_num_nodes()
{
  std::ifstream f("/etc/hosts", std::fstream::in | std::fstream::app | std::fstream::out);
  if (f.fail()) return 0;
  std::string line;
  int count = 0;
  while (std::getline(f,line)) {
    size_t idx = line.find("logcabin");
    if (idx != std::string::npos) {
        count++;
    }
  }
  return count;
}



std::unique_ptr<bool>
api_v1_server::healPartition()
{
  std::unique_ptr<bool> res(new bool);
  
  int num_nodes = find_num_nodes();
  if (num_nodes == 0) {
    *res = false;
    return res;
  }

  std::vector<std::string> nodes;
  for (int i=1; i<=num_nodes; i++) {
    nodes.push_back("n" + std::to_string(i));
  }
  /*for (int i=1; i<=5; i++) {
    system(("sudo iptables -A INPUT -s n" + std::to_string(i) + " -j ACCEPT").c_str());
  }*/

  //*res = Config::healNodes(nodes);
  *res = false;

  return res;
}

