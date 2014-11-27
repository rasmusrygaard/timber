#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <set>
#include <string>
#include <vector>
#include "server.hh"

struct Node {
    std::string id;
    std::string public_ip;
    std::string private_ip;
};

enum ClientError {
    /*
     * Set, Get or Remove failed because they key was not found.
     */
    KEY_NOT_FOUND,
    /*
     * Create operation failed because the key has no parent.
     */
    NO_PARENT,
    /*
     * The key has children so it cannot be deleted.
     */
    HAS_CHILDREN,
    /*
     * The key path violates our formatting rules
     */
    MALFORMED_KEY,
};

enum RaftImplementation { LogCabin, GoRaft };

/*
 * For server and/or library errors that must delivered to the client
 */
class ClientException : public std::exception {
public:
    ClientException(enum ClientError err) { errcode = err; }
    virtual ~ClientException() { }
    ClientError code() const { return errcode; }
    const char *what() {
        switch (errcode) {
            case KEY_NOT_FOUND:
                return "KEY NOT FOUND";
            case NO_PARENT:
                return "NO PARENT";
            case HAS_CHILDREN:
                return "HAS CHILDREN";
            case MALFORMED_KEY:
                return "MALFORMED KEY";
        }
        return "UNKNOWN ERROR CODE";
    }
private:
    enum ClientError errcode;
};

/*
 * For all the remote calls in the Client class you should use the ClientError
 * class to throw exceptions that the client's can understand.
 */
class Client {
public:
    Client();
    ~Client();
    /*
     * Connect to a server
     */
    void open(const std::string &host);
    /*
     * Disconnect from a server
     */
    void close();
    /*
     * Test if still connected to a server
     *
     * Return Value: true if connected
     */
    bool isConnected();
    /*
     * Creates a key with specified path and value
     *
     * Return Value: true if key was created, false if key already exists
     */
    bool install(const RaftImplementation& implementation, std::vector<Node> nodes, const int id);

    bool run(const RaftImplementation& implementation, std::vector<Node> nodes, const int id);

    bool makePartition(std::vector<Node> pr_ips1, std::vector<Node> pr_ips2);

private:
    xdr::srpc_client<api_v1> *client;
    ClusterDesc getClusterDesc(const RaftImplementation& implementation,
                               const std::vector<Node>& nodes,
                               const int id);
    Part getPartition(const std::vector<Node>& g1, 
                            const std::vector<Node>& g2);

};
#endif /* __CLIENT_H__ */
