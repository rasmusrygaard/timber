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

bool
Client::setup(const RaftImplementation& implementation, std::vector<Node> nodes, const int id)
{
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
    auto r = client->setup(cluster);
    if (*r == false) {
        //TODO: cleanup?
        return false;
    }

    return true;
}
