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
    case Etcd:     type = EtcdType; break;
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
Client::getPartition(const std::vector<int>& g1,
                    const std::vector<int>& g2)
{
    Partition partition;
    for (auto node : g1) {
        partition.group1.push_back(node);
    }
    for (auto node : g2) {
        partition.group2.push_back(node);
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
//Client::makePartition(std::vector<Node> private_ips1, std::vector<Node> private_ips2)
Client::makePartition(std::vector<int> gr1, std::vector<int> gr2)
{
    Partition arg = getPartition(gr1, gr2);

    auto r = client->makePartition(arg);
    std::cout << (*r == true) << std::endl;
    if (*r == false)
        return false;

    return true;
}

bool
Client::heal_cluster()
{
    /*
    auto r = client->healPartition();
    std::cout << ("here") << std::endl;

    if (*r == false)
        return false;
    */
    return true;
}
