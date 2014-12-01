#ifndef __LOG_CABIN_WRAPPER__
#define __LOG_CABIN_WRAPPER__

#include <string>
#include <vector>
#include "include/server.hh"

class LogCabinWrapper {
public:
    static void initialize(const std::unique_ptr<ClusterDesc>& cluster);
    static void bootstrap(int id);
    static void startServer(int id);
    static void reconfigure(const std::unique_ptr<ClusterDesc>& cluster);
private:
    static void writeConfig(const std::unique_ptr<ClusterDesc>& cluster);
    static std::string joinPrivateIPs(const std::unique_ptr<ClusterDesc>& cluster, const std::string& delim);

};

#endif
