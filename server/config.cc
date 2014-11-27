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

/*
std::string
get_hostname(std::ifstream cn)
{
  //Get IP of current node
  std::string hostname;
  std::getline(cn,hostname);
  size_t start_index = 0;
  start_index = hostname.find("ip-", start_index);
  if (start_index != std::string::npos) hostname.replace(start_index, 3, "");
  std::replace(hostname.begin(), hostname.end(), '-', '.'); // replace all '-' to '.'
  return hostname;

}

void 
change_files(const std::vector<std::string>& nodes)
{
    // Create a temporary file of etc/hosts that will later replace /etc/hosts 
  std::ifstream f("/etc/hosts", std::fstream::in | std::fstream::app | std::fstream::out);
  std::ofstream table("/etc/iptable", std::fstream::in | std::fstream::app | std::fstream::out);
  //if (f.fail() || table.fail()) return false;

  std::string line;
  while (std::getline(f,line)) {
    //see if any of the nodes listed 
    bool found = false;
    for (auto name : nodes) {
      if (line.find(name) != std::string::npos) {
          found = true;
          break;
      }
    }

    if (!found) table << line;
  }

  f.close();
  table.close();
}
*/



/*
 * Takes in std::vector of nodes ("n1"..."n5") to partition.
 * The function assumes that the current node is _not_ in nodeList.
 */
bool Config::partitionNodes(const std::vector<std::string>& nodes) {
  std::string hostname;
  std::ifstream cn("/etc/hostname", std::fstream::in | std::fstream::app | std::fstream::out);
  if (cn.fail()) return false;

  //hostname = get_hostname(cn)

  //change_files(nodes)



  
  return false;
}
