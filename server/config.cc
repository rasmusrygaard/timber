#include <fstream>
#include <vector>

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

/*
 * Takes in std::vector of nodes ("n1"..."n5") to partition.
 * The function assumes that the current node is _not_ in nodeList.
 */
bool Config::partitionNodes(const std::vector<std::string>& nodes) {
  return false;
}
