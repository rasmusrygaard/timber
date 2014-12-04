#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <vector>

#include "include/server.hh"

class Config {
public:
  /*
   * Add each host in HOSTS to /etc/hosts.
   * If no name parameter is specified, the hosts will be numbered n1 through nN.
   * If NAME is specified, the nodes will all be mapped to NAME.
   */
    static bool addHosts(const std::unique_ptr<ClusterDesc>& cluster, const std::string& name, const bool isPrefix);

  /*
   * Takes in std::vector of nodes ("n1"..."n5") to partition.
   * The function assumes that the current node is _not_ in nodeList.
   */
    static bool partitionNodes(const std::vector<std::string>& nodes, const bool is_partition);

  /*
   * Returns the host ip address of the current client node.
   */
  static std::string get_hostname();

  static bool healNodes(const std::vector<std::string>& nodes);

  static void slow_network();

  static void heal_network();

};

#endif
