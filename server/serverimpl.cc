// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include <string>
#include <iostream>
#include <algorithm>

#include "server/serverimpl.hh"

const int KEY_VALID      = 1 << 0;
const int PARENT_EXISTS  = 1 << 1;
const int KEY_EXISTS     = 1 << 2;
const int NO_CHILDREN    = 1 << 3;

bool api_v1_server::keyIsValid(const std::string& key) {
    size_t length = key.size();

    if (key.empty() || key[0] != '/') return false;
    if (length == 1 && key[0] == '/') return false;

    for (size_t i = 1; i < length; ++i) {
        if (std::isalnum(key[i]) || key[i] == '_') {
            // Allow alphanumeric characters and underscores
            continue;
        } else if (key[i] == '/' && key[i - 1] != '/' && i != length - 1) {
            // Allow '/' unless the previous char is '/' or '/' is the last character.
            continue;
        } else {
            return false;
        }
    }
    return true;
}

bool
api_v1_server::parentExists(const std::string& key) {
    size_t lastSlash = key.find_last_of('/');
    if (lastSlash == 0) return true;

    std::string parentPath = key.substr(0, lastSlash);
    return db.hasKey(parentPath);
}

bool api_v1_server::validateKey(const std::string& key, const int flags, ErrorCode& err) {
    bool success = true;
    if ((flags & KEY_VALID) && !keyIsValid(key)) {
        std::cout << "Key: '" << key << "' is invalid." << std::endl;
        err = ErrorCode::MALFORMED_KEY_ERROR;
        success = false;
    }
    if (success && (flags & PARENT_EXISTS) && !parentExists(key)) {
        std::cout << "Key: '" << key << "' does not have a parent." << std::endl;
        err = ErrorCode::NO_PARENT_ERROR;
        success = false;
    }
    if (success && (flags & KEY_EXISTS) && !db.hasKey(key)) {
        std::cout << "Key: '" << key << "' does not exist." << std::endl;
        err = ErrorCode::KEY_NOT_FOUND_ERROR;
        success = false;
    }
    if (success && (flags & NO_CHILDREN) && !db.list(key).empty()) {
        std::cout << "Key: '" << key << "' has children." << std::endl;
        err = ErrorCode::HAS_CHILDREN_ERROR;
        success = false;
    }
    return success;
}

std::unique_ptr<MaybeBool>
api_v1_server::create(std::unique_ptr<kvpair> arg)
{
    bool hasKey;
    std::string key = arg->key;
    std::string val = arg->val;
    std::unique_ptr<MaybeBool> res(new MaybeBool);

    int toCheck = KEY_VALID | PARENT_EXISTS;
    ErrorCode err;
    if (!validateKey(key, toCheck, err)) {
        res->discriminant(1);
        res->errorCode() = err;
        return res;
    }
    
    res->discriminant(0);
    hasKey = db.hasKey(arg->key);
    if (hasKey) {
        res->value() = false;
        std::cout << "Create '" << key << "' failed." << std::endl;
    } else {
        res->value() = true;
        db.set(key, val);
        std::cout << "Create '" << key << " succeeded." << std::endl;
    }

    return res;  
}

std::unique_ptr<MaybeBool>
api_v1_server::remove(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<MaybeBool> res(new MaybeBool);
  std::string key = (*arg);

  int toCheck = KEY_VALID | KEY_EXISTS | NO_CHILDREN;
  ErrorCode err;
  if (!validateKey(key, toCheck, err)) {
      res->discriminant(1);
      res->errorCode() = err;
      return res;
  }

  db.remove(key);
  res->discriminant(0);
  res->value() = true;
  
  return res;
}

std::unique_ptr<MaybeBool>
api_v1_server::set(std::unique_ptr<kvpair> arg)
{
  std::unique_ptr<MaybeBool> res(new MaybeBool);
  std::string key = arg->key;
  std::string val = arg->val;

  int toCheck = KEY_VALID | KEY_EXISTS;
  ErrorCode err;
  if (!validateKey(key, toCheck, err)) {
      res->discriminant(1);
      res->errorCode() = err;
      return res;
  }

  res->discriminant(0);
  res->value() = db.set(key, val);
  
  return res;
}

std::unique_ptr<MaybeString>
api_v1_server::get(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<MaybeString> res(new MaybeString);
  std::string key = (*arg);

  int toCheck = KEY_VALID | KEY_EXISTS;
  ErrorCode err;
  if (!validateKey(key, toCheck, err)) {
      res->discriminant(1);
      res->errorCode() = err;
      return res;
  }

  res->discriminant(0);
  res->value() = db.get(key);
  
  return res;
}

std::unique_ptr<MaybeSetString>
api_v1_server::list(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<MaybeSetString> res(new MaybeSetString);
  std::string key = (*arg);

  int toCheck = KEY_VALID | KEY_EXISTS;
  ErrorCode err;
  if (!validateKey(key, toCheck, err)) {
      res->discriminant(1);
      res->errorCode() = err;
      return res;
  }

  res->discriminant(0);
  auto children = db.list(key);
  std::copy(children.begin(), children.end(), res->value().begin());
  
  return res;
}
