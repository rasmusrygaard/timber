// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include <string>
#include <iostream>
#include <vector>

#include "server/serverimpl.hh"
#include "server/LogCabinWrapper.h"

std::unique_ptr<bool>
api_v1_server::setup(std::unique_ptr<RaftType> arg)
{
  std::unique_ptr<bool> res(new bool);
  std::vector<std::string> hosts;
  LogCabinWrapper::initialize(hosts);

  // Fill in additional sanity checking (e.g. prevent malformed paths)

  return res;
}
