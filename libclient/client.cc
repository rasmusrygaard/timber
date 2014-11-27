#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <set>
#include <string>

#include <xdrpp/srpc.h>
#include <xdrpp/rpcbind.hh>
#include <xdrpp/socket.h>

#include <include/rpcconfig.h>
#include <include/server.hh>
#include <include/client.h>

using namespace std;
using namespace xdr;

Client::Client()
{
    client = NULL;
}

Client::~Client()
{
    this->close();
}

void
Client::open(const std::string &host)
{
    if (isConnected()) {
        cout << "Already connected!" << endl;
        exit(1);
    }

    auto fd = tcp_connect(host.c_str(), UNIQUE_RPC_PORT);
    client = new srpc_client<api_v1>{fd.release()};
}

void
Client::close()
{
    if (isConnected()) {
        delete client;
        client = NULL;
    }
}

bool
Client::isConnected()
{
    return client != NULL;
}

ClusterDesc
Client::getClusterDesc(const RaftImplementation& implementation,
                       const std::vector<Node>& nodes,
                       const int id) {
    RaftType type;
    switch (implementation) {
    case LogCabin: type = LogCabinType; break;
    case GoRaft:   type = GoRaftType; break;
    }

    IPList public_ips, private_ips;
    for (auto node : nodes) {
        public_ips.nodes.push_back(node.public_ip);
        private_ips.nodes.push_back(node.private_ip);
    }

    ClusterDesc cluster;
    cluster.type = type;
    cluster.public_ips = public_ips;
    cluster.private_ips = private_ips;
    cluster.nodeId = id;

    return cluster;
}

Partition
Client::getPartition(const std::vector<Node>& g1, const std::vector<Node>& g2)
{
    Partition partition;
    for (auto node : g1) {
        partition.group1.push_back(node.private_ip);
    }
    for (auto node : g2) { 
        partition.group2.push_back(node.private_ip);
    }
    return partition;
}



bool
Client::install(const RaftImplementation& implementation, std::vector<Node> nodes, const int id)
{
    ClusterDesc cluster = getClusterDesc(implementation, nodes, id);

    auto r = client->install(cluster);
    if (*r == false) {
        //TODO: cleanup?
        return false;
    }

    return true;
}

bool
Client::run(const RaftImplementation& implementation, std::vector<Node> nodes, const int id)
{
    ClusterDesc cluster = getClusterDesc(implementation, nodes, id);

    auto r = client->run(cluster);
    if (*r == false)
        return false;

    return true;
}


bool
Client::makePartition(std::vector<Node> private_ips1, std::vector<Node> private_ips2)
{
    Partition partition = getPartition(private_ips1, private_ips2);
 
    std::cout << "================================" << std::endl;
    std::cout << "Group 1" << std::endl;
    for (int i=0; i<partition.group1.size(); i++) {
        std::cout << partition.group1[i] << std::endl;
    }
    std::cout << "Group 2" << std::endl;    
    for (int i=0; i<partition.group2.size(); i++) {
        std::cout << partition.group2[i] << std::endl;
    }

    auto r = client->makePartition(partition);
    if (*r == false)
        return false;

    return true;
}
