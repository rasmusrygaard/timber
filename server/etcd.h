#ifndef __ETCD__
#define __ETCD__

#include <vector>

class Etcd {
public:
  static void initialize(const std::vector<std::string>& hosts, const int id);
};

#endif
