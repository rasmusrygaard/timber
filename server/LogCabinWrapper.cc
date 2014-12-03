#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "server/LogCabinWrapper.h"
#include "util.h"

const std::string LOGCABIN_DIR = "~/logcabin/";
const std::string LOGCABIN_SCRIPTS_DIR = "~/timber/logcabin/";
const std::string LOGCABIN_STORAGE_DIR = "logcabinstorage";
const int LOGCABIN_PORT = 61023;

void
LogCabinWrapper::initialize(const std::unique_ptr<ClusterDesc>& cluster) {
    writeConfig(cluster);
    std::cout << "Running init.sh for LogCabin" << std::endl;
    Util::run_cmd("bash " + LOGCABIN_SCRIPTS_DIR + "init.sh");
    std::cout << "Done running init.sh for LogCabin" << std::endl;
}

void
LogCabinWrapper::bootstrap(int id) {
    std::cout << "Bootstrapping with id " << id << std::endl;
    Util::run_cmd(LOGCABIN_DIR + "build/LogCabin --bootstrap --id " + std::to_string(id) + " > bootstrap.log");
    std::cout << "Done boostrapping with id " << id << std::endl;
}

void
LogCabinWrapper::startServer(int id) {
    std::cout << "Starting server with id " << id << std::endl;
    Util::run_cmd_async(LOGCABIN_DIR + "build/LogCabin --id " + std::to_string(id) + " > logcabin_server.log");
    std::cout << "Done starting server with id " << id << std::endl;
}

void
LogCabinWrapper::reconfigure(const std::unique_ptr<ClusterDesc>& cluster) {
    std::cout << "Reconfiguring cluster with cluster " << joinPrivateIPs(cluster, " ") << std::endl;
    Util::run_cmd(LOGCABIN_DIR + "build/Examples/Reconfigure " + joinPrivateIPs(cluster, " ") + " > logcabin_reconfigur.log");
    std::cout << "Done reconfiguring cluster" << std::endl;
}

void
LogCabinWrapper::writeConfig(const std::unique_ptr<ClusterDesc>& cluster) {
    std::cout << "Writing logcabin.conf with cluster " << joinPrivateIPs(cluster, ";") << std::endl;
    std::ofstream fs;
    fs.open ("logcabin.conf", std::fstream::trunc | std::fstream::out);
    fs << "storageModule = filesystem" << std::endl;
    fs << "storagePath = " << LOGCABIN_STORAGE_DIR << std::endl;
    fs << "servers = " << joinPrivateIPs(cluster, ";") << std::endl;
    fs.close();
    std::cout << "Done writing logcabin.conf" << std::endl;
}

std::string
LogCabinWrapper::joinPrivateIPs(const std::unique_ptr<ClusterDesc>& cluster, const std::string& delim) {
    std::vector<std::string> hosts;
    for (auto private_ip : cluster->private_ips.nodes) {
        hosts.push_back(private_ip);
    }
    std::stringstream ss;
    bool first = true;
    for (auto host : hosts) {
        if (!first) {
            ss << delim;
        } else {
            first = false;
        }
        ss << host << ":" << LOGCABIN_PORT;
    }
    return ss.str();
}
