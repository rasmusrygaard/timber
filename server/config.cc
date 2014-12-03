#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "config.h"

/*
 * Add each host in HOSTS to /etc/hosts.
 * If no name parameter is specified, the hosts will be numbered n1 through nN.
 * If NAME is specified, the nodes will all be mapped to NAME.
 */
bool
Config::addHosts(const std::unique_ptr<ClusterDesc>& cluster,
                 const std::string& name,
                 const bool isPrefix) {
  std::ofstream f("/etc/hosts", std::fstream::in | std::fstream::app | std::fstream::out);
  if (f.fail()) return false;

  int count = 1;
  for (auto addr : cluster->private_ips.nodes) {
    f << addr << " " << name;
    if (isPrefix) f << count++;
    f << std::endl;
  }
  f.close();
  return true;
}


std::string
Config::get_hostname()
{
  //Get IP of current node
  std::string hostname;
  std::ifstream cn("/etc/hostname", std::fstream::in | std::fstream::app | std::fstream::out);
  if (cn.fail()) return "";

  std::getline(cn,hostname);
  size_t start_index = 0;
  start_index = hostname.find("ip-", start_index);
  if (start_index != std::string::npos) hostname.replace(start_index, 3, "");
  std::replace(hostname.begin(), hostname.end(), '-', '.'); // replace all '-' to '.'
  return hostname;
}


/*
 * Takes in std::vector of nodes ("n1"..."n5") to partition.
 * The function assumes that the current node is _not_ in nodeList.
 */
bool Config::partitionNodes(const std::vector<std::string>& nodes, const bool is_partition) {
  //Stop accepting traffic from these node names
  if (is_partition) {
    system("sudo iptables -F");
    for (auto ip : nodes) {
      system(("sudo iptables -A INPUT -s " + ip + " -j DROP").c_str());
    }
  } else {
      system("sudo iptables -F");
      system("sudo iptables -A INPUT -p tcp --dport 6160 -j ACCEPT");
      system("sudo iptables -A INPUT -p tcp --dport 61023 -j ACCEPT");
  }

  return true;
  //when to return false?
}


bool Config::healNodes(const std::vector<std::string>& nodes) {
  //accept traffic from all nodes in the cluster
  /*for (auto ip: nodes) {
    system(("sudo iptables -A INPUT -s " + ip + " -j ACCEPT").c_str());
  }*/
  system("sudo iptables -F");
  system("sudo iptables -A INPUT -p tcp --dport 6160 -j ACCEPT");
  system("sudo iptables -A INPUT -p tcp --dport 61023 -j ACCEPT");

  return false;
  //when to return false?

}
