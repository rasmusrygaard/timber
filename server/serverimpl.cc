// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"

std::unique_ptr<MaybeBool>
api_v1_server::create(std::unique_ptr<kvpair> arg)
{
  bool hasKey;
  std::string key = arg->key;
  std::string val = arg->val;
  std::unique_ptr<MaybeBool> res(new MaybeBool);

  // Fill in additional sanity checking (e.g. prevent malformed paths)
  ServerError err;
  if (!validateKey(key)) {

  }
  hasKey = db.hasKey(arg->key);
  if (hasKey) {
      (*res) = false;
      std::cout << "Created " << key << " Failed" << std::endl;
  } else {
      (*res) = true;
      db.set(key, val);
      std::cout << "Created " << key << " Succeded" << std::endl;
  }

  return res;  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<MaybeBool>
api_v1_server::remove(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<MaybeBool> res(new MaybeBool);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<MaybeBool>
api_v1_server::set(std::unique_ptr<kvpair> arg)
{
  std::unique_ptr<MaybeBool> res(new MaybeBool);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<MaybeString>
api_v1_server::get(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<MaybeString> res(new MaybeString);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<MavbeSetString>
api_v1_server::list(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<MavbeSetString> res(new MavbeSetString);
  
  // Fill in function body here
  
  return res;
}
