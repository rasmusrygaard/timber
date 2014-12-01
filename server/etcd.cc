#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "server/Etcd.h"

const std::string LOGCABIN_DIR = "~/logcabin/";
const std::string LOGCABIN_SCRIPTS_DIR = "~/timber/logcabin/";
const std::string LOGCABIN_STORAGE_DIR = "logcabinstorage";
const int ETCD_PEER_PORT = 61023;
const int ETCD_CLIENT_PORT = 61023;

void
Etcd::initialize(const std::vector<std::string>& hosts, const int id) {
    stringstream ss;
    ss << "./etcd \ "
       << " -name n" << id << " \ "
       << ""
}
