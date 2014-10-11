// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include <string>
#include <iostream>

#include "server/serverimpl.hh"

bool validateKey(const std::string& key, ErrorCode& err) {
    if (key.find('/') == std::string::npos) {
        err = ErrorCode::MALFORMED_KEY_ERROR;
        return false;
    }
    return true;
}

std::unique_ptr<MaybeBool>
api_v1_server::create(std::unique_ptr<kvpair> arg)
{
    bool hasKey;
    std::string key = arg->key;
    std::string val = arg->val;
    std::unique_ptr<MaybeBool> res(new MaybeBool);

    // Fill in additional sanity checking (e.g. prevent malformed paths)
    ErrorCode err;
    if (!validateKey(key, err)) {
        res->discriminant(1);
        res->errorCode() = err;
        return res;
    } 

    res->discriminant(0);
    hasKey = db.hasKey(arg->key);
    if (hasKey) {
        res->value() = false;
        std::cout << "Created " << key << " Failed" << std::endl;
    } else {
        res->value() = true;
        db.set(key, val);
        std::cout << "Created " << key << " Succeded" << std::endl;
    }

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

std::unique_ptr<MaybeSetString>
api_v1_server::list(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<MaybeSetString> res(new MaybeSetString);
  
  // Fill in function body here
  
  return res;
}
