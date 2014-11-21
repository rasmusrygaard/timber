#ifndef __LOG_CABIN_WRAPPER__
#define __LOG_CABIN_WRAPPER__

#include <string>
#include <vector>

class LogCabinWrapper {
public:
  static void initialize(const std::vector<std::string>& hosts);
  static void bootstrap(int id);
  static void startServer(int id);
  static void reconfigure(const std::vector<std::string>& hosts);
private:
  static void writeConfig(const std::vector<std::string>& hosts);
  static std::string joinHosts(const std::vector<std::string> hosts, const std::string& delim);

};

#endif
