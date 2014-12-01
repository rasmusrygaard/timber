#ifndef __ETCD__
#define __ETCD__

#include <vector>

#include "include/server.hh"

class Etcd {
public:
    static void initialize(const std::unique_ptr<ClusterDesc>& cluster);
    static void startServer(const std::unique_ptr<ClusterDesc>& cluster);
private:
    static std::string joinPrivateIPs(const std::unique_ptr<ClusterDesc>& cluster);
};

#endif
