#include <fstream>
#include <vector>
#include <string>

#include "config.h"

/*
 * Add each host in HOSTS to /etc/hosts.
 * If no name parameter is specified, the hosts will be numbered n1 through nN.
 * If NAME is specified, the nodes will all be mapped to NAME.
 */
bool
Config::addHosts(const std::vector<std::string>& hosts, const std::string& name, const bool isPrefix) {
  std::ofstream f("/etc/hosts", std::fstream::in | std::fstream::app | std::fstream::out);
  if (f.fail()) return false;

  int count = 1;
  for (auto addr : hosts) {
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
bool Config::partitionNodes(const std::vector<std::string>& nodes) {
  //Stop accepting traffic from these node names
  for (auto ip : nodes) {
    system(("sudo iptables -A INPUT -s " + ip + " -j DROP").c_str());
  }
  
  return true;
  //when to return false?
}


bool Config::healNodes(const std::vector<std::string>& nodes) {
  for (auto ip: nodes) {
    system(("sudo iptables -A INPUT -s " + ip + " -j ACCEPT").c_str());
  }
  return true;
  //when to return false?

}