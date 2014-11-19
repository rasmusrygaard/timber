#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "server/LogCabinWrapper.h"

  const std::string LOGCABIN_DIR = "~/logcabin/";
  const std::string LOGCABIN_SCRIPTS_DIR = "~/timber/logcabin/";
  const int LOGCABIN_PORT = 61023;

void
LogCabinWrapper::initialize(const std::vector<std::string>& hosts) {
    writeConfig(hosts);
    system((LOGCABIN_SCRIPTS_DIR + "init.sh").c_str());
}

void
LogCabinWrapper::writeConfig(const std::vector<std::string>& hosts) {
  std::ofstream fs;
  fs.open ("logcabin.conf", std::fstream::trunc | std::fstream::out);
  fs << "storageModule = filesystem" << std::endl;
  fs << "storagePath = smoketeststorage" << std::endl;
  fs << "servers = " << joinHosts(hosts) << std::endl;
  fs.close();
}

void
LogCabinWrapper::bootstrap(int id) {
  system((LOGCABIN_DIR + "build/LogCabin --bootstrap --id " + std::to_string(id)).c_str());
}

void
LogCabinWrapper::startServer(int id) {
  system((LOGCABIN_DIR + "build/LogCabin --id " + std::to_string(id)).c_str());
}

void
LogCabinWrapper::reconfigure(const std::vector<std::string>& hosts) {
  system((LOGCABIN_DIR + "build/Examples/Reconfigure" + joinHosts(hosts)).c_str());
}

std::string
LogCabinWrapper::joinHosts(const std::vector<std::string> hosts) {
  std::stringstream ss;
  bool first = true;
  for (auto host : hosts) {
    if (!first) {
      ss << ";";
    } else {
      first = false;
    }
    ss << host << ":" << LOGCABIN_PORT;
  }
  return ss.str();
}
