#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "server/LogCabinWrapper.h"

const std::string LOGCABIN_DIR = "~/logcabin/";
const std::string LOGCABIN_SCRIPTS_DIR = "~/timber/logcabin/";
const std::string LOGCABIN_STORAGE_DIR = "logcabinstorage";
const int LOGCABIN_PORT = 61023;

void
LogCabinWrapper::initialize(const std::vector<std::string>& hosts) {
    writeConfig(hosts);
    std::cout << "Running init.sh for LogCabin" << std::endl;
    system(("bash " + LOGCABIN_SCRIPTS_DIR + "init.sh").c_str());
    std::cout << "Done running init.sh for LogCabin" << std::endl;
}

void
LogCabinWrapper::writeConfig(const std::vector<std::string>& hosts) {
  std::cout << "Writing logcabin.conf with hosts " << joinHosts(hosts, ";") << std::endl;
  std::ofstream fs;
  fs.open ("logcabin.conf", std::fstream::trunc | std::fstream::out);
  fs << "storageModule = filesystem" << std::endl;
  fs << "storagePath = " << LOGCABIN_STORAGE_DIR << std::endl;
  fs << "servers = " << joinHosts(hosts, ";") << std::endl;
  fs.close();
  std::cout << "Done writing logcabin.conf" << std::endl;
}

void
LogCabinWrapper::bootstrap(int id) {
  std::cout << "Bootstrapping with id " << id << std::endl;
  system((LOGCABIN_DIR + "build/LogCabin --bootstrap --id " + std::to_string(id) + " > bootstrap.log").c_str());
  std::cout << "Done boostrapping with id " << id << std::endl;
}

void
LogCabinWrapper::startServer(int id) {
  std::cout << "Starting server with id " << id << std::endl;
  system((LOGCABIN_DIR + "build/LogCabin --id " + std::to_string(id) + " > logcabin_server.log &").c_str());
  std::cout << "Done starting server with id " << id << std::endl;
}

void
LogCabinWrapper::reconfigure(const std::vector<std::string>& hosts) {
  std::cout << "Reconfiguring cluster with hosts " << joinHosts(hosts, " ") << std::endl;
  system((LOGCABIN_DIR + "build/Examples/Reconfigure " + joinHosts(hosts, " ") + " > logcabin_reconfigur.log &").c_str());
  std::cout << "Done reconfiguring cluster" << std::endl;
}

std::string
LogCabinWrapper::joinHosts(const std::vector<std::string> hosts, const std::string& delim) {
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
