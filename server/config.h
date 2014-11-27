#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <vector>

class Config {
public:
  /*
   * Add each host in HOSTS to /etc/hosts.
   * If no name parameter is specified, the hosts will be numbered n1 through nN.
   * If NAME is specified, the nodes will all be mapped to NAME.
   */
  static bool addHosts(const std::vector<std::string>& hosts, const std::string& name, const bool isPrefix);

  /*
   * Takes in std::vector of nodes ("n1"..."n5") to partition.
   * The function assumes that the current node is _not_ in nodeList.
   */
  static bool partitionNodes(const std::vector<std::string>& nodes);

  /* 
   * Returns the host ip address of the current client node.
   */
  static std::string get_hostname();
};

#endif
