#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "server/LogCabinWrapper.h"

  const std::string LOGCABIN_DIR = "~/logcabin/";
  const std::string LOGCABIN_SCRIPTS_DIR = "~/timber/logcabin/scripts/";
  const int LOGCABIN_PORT = 61023;

void
LogCabinWrapper::initialize(const std::vector<std::string>& hosts) {
  system((LOGCABIN_SCRIPTS_DIR + "/init.sh").c_str());
  writeConfig(hosts);
}

void
LogCabinWrapper::writeConfig(const std::vector<std::string>& hosts) {
  std::fstream fs;
  fs.open (LOGCABIN_DIR + "logcabin.conf", std::fstream::in | std::fstream::trunc);
  fs << "storageModule = filesystem";
  fs << "storagePath = smoketeststorage";
  fs << "servers = " << joinHosts(hosts);
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
