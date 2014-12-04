#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "server/etcd.h"
#include "util.h"
#include "config.h"

const std::string ETCD_SCRIPTS_DIR = "~/timber/etcd/";
const std::string ETCD_DIR = "~/etcd/";
const int ETCD_PEER_PORT = 61023;
const int ETCD_CLIENT_PORT = 4001;

void
Etcd::initialize(const std::unique_ptr<ClusterDesc>& cluster) {
    std::cout << "Running init.sh for Etcd" << std::endl;
    Util::run_cmd("bash " + ETCD_SCRIPTS_DIR + "init.sh");
    std::cout << "Done running init.sh for Etcd" << std::endl;
}

void
Etcd::startServer(const std::unique_ptr<ClusterDesc>& cluster) {
    std::cout << "Starting etcd server on node " << cluster->nodeId << std::endl;
    const int id = cluster->nodeId;
    std::string private_ip = cluster->private_ips.nodes[id - 1];
    std::string public_ip = cluster->private_ips.nodes[id - 1];
    std::stringstream ss;
    ss << ETCD_DIR << "etcd "
       << "-name n" << id << " "
       << "-peer-addr " << private_ip << ":" << ETCD_PEER_PORT << " "
       << "-peer-bind-addr " << private_ip << ":" << ETCD_PEER_PORT << " "
       << "-addr " << public_ip << ":" << ETCD_CLIENT_PORT << " "
       << "-bind-addr 0.0.0.0:" << ETCD_CLIENT_PORT << " ";
    if (cluster->nodeId == 1) {
        ss << "-peers " << joinPrivateIPs(cluster) << " ";
    }
    ss << "-http-write-timeout 1 " << " "
       << "-http-read-timeout 1 " << " "
       << "&> etcd_server.log";
    Util::run_cmd_async(ss.str());
    std::cout << "Done with etcd server on node " << cluster->nodeId << std::endl;
}

std::string
Etcd::joinPrivateIPs(const std::unique_ptr<ClusterDesc>& cluster) {
    std::stringstream ss;
    int count = 0;
    for (auto private_ip : cluster->private_ips.nodes) {
        if (private_ip == Config::get_hostname()) continue;
        if (count > 0) ss << ",";
        ss << private_ip << ":" << ETCD_PEER_PORT;
        count++;
    }
    return ss.str();
}
