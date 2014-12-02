#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "server/etcd.h"

const std::string ETCD_SCRIPTS_DIR = "~/timber/etcd/";
const std::string ETCD_DIR = "~/etcd/";
const int ETCD_PEER_PORT = 61023;
const int ETCD_CLIENT_PORT = 4001;

void
Etcd::initialize(const std::unique_ptr<ClusterDesc>& cluster) {
    std::cout << "Running init.sh for Etcd" << std::endl;
    system(("bash " + ETCD_SCRIPTS_DIR + "init.sh").c_str());
    std::cout << "Done running init.sh for Etcd" << std::endl;
}

void
Etcd::startServer(const std::unique_ptr<ClusterDesc>& cluster) {
    std::cout << "Starting etcd server on node " << cluster->nodeId << std::endl;
    const int id = cluster->nodeId;
    std::string private_ip = cluster->private_ips.nodes[id - 1];
    std::string public_ip = cluster->private_ips.nodes[id - 1];
    std::stringstream ss;
    ss << ETCD_DIR << "etcd \\ "
       << " -name n" << id << " \\ "
       << "-initial-advertise-peer-urls http://" << private_ip << ":" << ETCD_PEER_PORT << " \\ "
       << "-listen-peer-urls http://" << private_ip << ":" << ETCD_PEER_PORT << " \\ "
       << "-advertise-client-urls http://" << public_ip << ":" << ETCD_CLIENT_PORT << " \\ "
       << "-listen-client-urls http://0.0.0.0:" << ETCD_CLIENT_PORT << " \\ "
       << "-initial-cluster " << joinPrivateIPs(cluster) << " \\ "
       << "-initial-cluster-state new &";
    system(ss.str().c_str());
}

std::string
Etcd::joinPrivateIPs(const std::unique_ptr<ClusterDesc>& cluster) {
    std::stringstream ss;
    for (int i = 1; i <= cluster->private_ips.nodes.size(); ++i) {
        if (i > 1) ss << ";";
        ss << "n" << i << "=http://" << cluster->private_ips.nodes[i - 1] << ETCD_PEER_PORT;
    }
    return ss.str();
}
