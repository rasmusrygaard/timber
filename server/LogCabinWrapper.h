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
    static void reconfigure(const std::vector<std::string>& hosts);
private:
    static void writeConfig(const std::unique_ptr<ClusterDesc>& cluster);
    static std::string joinHosts(const std::vector<std::string> hosts, const std::string& delim);

};

#endif
