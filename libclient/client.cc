
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <set>
#include <string>

#include <xdrpp/srpc.h>
#include <xdrpp/rpcbind.hh>
#include <xdrpp/socket.h>

#include <include/rpcconfig.h>
#include <include/server.hh>
#include <include/client.h>

using namespace std;
using namespace xdr;

Client::Client()
{
    client = NULL;
}

Client::~Client()
{
    this->close();
}

void
Client::open(const std::string &host)
{
    if (isConnected()) {
        cout << "Already connected!" << endl;
        exit(1);
    }

    auto fd = tcp_connect(host.c_str(), UNIQUE_RPC_PORT);
    client = new srpc_client<api_v1>{fd.release()};
}

void
Client::close()
{
    if (isConnected()) {
        delete client;
        client = NULL;
    }
}

bool
Client::isConnected()
{
    return client != NULL;
}

ClientError translateErrorCode(const ErrorCode& serverError) {
    switch (serverError) {
    case KEY_NOT_FOUND_ERROR:
        return ClientError::KEY_NOT_FOUND;
    case NO_PARENT_ERROR:
        return ClientError::NO_PARENT;
    case HAS_CHILDREN_ERROR:
        return ClientError::HAS_CHILDREN;
    case MALFORMED_KEY_ERROR:
        return ClientError::MALFORMED_KEY;
    default:
        return ClientError::MALFORMED_KEY;
    }
}

bool
Client::create(const std::string &path, const std::string &val)
{
    kvpair args;

    args.key = path;
    args.val = val;

    auto r = client->create(args);
    if (r->discriminant() == 0) {
        return r->value();
    } else {
        throw ClientException(translateErrorCode(r->errorCode()));
    }
}

bool
Client::remove(const std::string &path)
{
    longstring key = path;
    auto r = client->remove(key);
    if (r->discriminant() == 0) {
        return r->value();
    } else {
        throw ClientException(translateErrorCode(r->errorCode()));        
    }
}

std::string
Client::get(const std::string &path)
{
    longstring key = path;
    auto r = client->get(key);
    if (r->discriminant() == 0) {
        return r->value();
    } else {
        throw ClientException(translateErrorCode(r->errorCode()));        
    }
}

void
Client::set(const std::string &path, const std::string &val)
{
    kvpair args;

    args.key = path;
    args.val = val;

    auto r = client->set(args);
    if (r->discriminant() == 1) {
        throw ClientException(translateErrorCode(r->errorCode()));        
    }
}

std::set<string>
Client::list(const string &path)
{
    std::set<string> res;
    longstring key = path;
    auto r = client->list(key);
    if (r->discriminant() == 0) {
        for (std::string s : r->value()) { res.insert(s); }
    } else {
        throw ClientException(translateErrorCode(r->errorCode()));
    }

    return res;
}

