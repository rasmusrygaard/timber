#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>

#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <xdrpp/srpc.h>

#include <include/server.hh>
#include <include/client.h>

using namespace std;

std::vector<Client*> clients;

#define DEBUG_MAX_ARGS      5
#define DEBUG_MAX_LINE      256

/* Read the cluster configuration from timber.config.
   The file is assumed to be of the form "ID\tPUBLIC_IP\tPRIVATE_IP". */
std::vector<Node>
readConfig() {
    std::ifstream f("timber.config");
    std::vector<Node> result;
    std::string id, public_ip, private_ip;
    while (f >> id >> public_ip >> private_ip) {
        Node n = { id, public_ip, private_ip };
        result.push_back(n);
    }
    return result;
}

void
Cmd_Help(int argc, const char *argv[])
{
    cout << "partition  Partition a set of nodes" << endl;
    cout << "logcabin   Set up logcabin" << endl;
    cout << "create     Create a node" << endl;
    cout << "echo       Echo arguments" << endl;
    cout << "exit       Exit shell" << endl;
    cout << "get        Get a node" << endl;
    cout << "help       Display the list of commands" << endl;
    cout << "remove     Remove a node" << endl;
    cout << "set        Set a node" << endl;
}

void
Cmd_Echo(int argc, const char *argv[])
{
    int i;

    for (i = 1; i < argc; i++)
    {
        cout << argv[i] << " ";
    }
    cout << endl;
}

void
Client_Setup(Client* client,
             const RaftImplementation& implementation,
             const std::vector<Node>& nodes,
             const int id) {
    client->install(LogCabin, nodes, id);
}

void
Cmd_Logcabin(int argc, const char* argv[])
{
    std::vector<Node> nodes = readConfig();
    std::vector<std::thread> threads(clients.size());

    for (int i = 0; i < clients.size(); ++i) {
        std::cout << "Setting up LogCabin on client " << i + 1 << std::endl;
        threads[i] = std::thread(&Client_Setup, clients[i], LogCabin, nodes, i + 1);
    }

    for (int i = 0; i < threads.size(); ++i) {
        std::cout << "Joining thread " << i << "." << std::endl;
        threads[i].join();
    }

    for (int i = 0; i < clients.size(); ++i) {
        clients[i]->run(LogCabin, nodes, i + 1);
    }
}


/* Partitions the Network */
void
Cmd_Partition(int argc, const char* argv[])
{
    //Gets all the nodes
    std::vector<Node> nodes = readConfig();

    std::vector<Node> group1;
    std::vector<Node> group2;

    //splits the nodes in half
    for(int i=0; i < nodes.size()/2; i++) {
        group1.push_back(nodes[i]);
    }
    for(int i=nodes.size()/2; i<=nodes.size(); i++) {
        group2.push_back(nodes[i]);    
    }

    for (int i=0; i<clients.size(); i++) {
       clients[i]->makePartition(group1, group2);
    }
}


/* Snubs specified nodes */
void 
Cmd_SnubNodes(int argc, const char* argv[])
{
    //Fill in function body
}


void
DispatchCommand(char *buf)
{
    int argc;
    char *argv[DEBUG_MAX_ARGS];
    char *nextArg;

    // parse input
    argv[0] = buf;
    for (argc = 1; argc < DEBUG_MAX_ARGS; argc++) {
        nextArg = strchr(argv[argc - 1], ' ');
        if (nextArg == NULL)
        {
            break;
        }

        *nextArg = '\0';
        argv[argc] = nextArg + 1;
    }

    // execute command
    string cmd = argv[0];
    if (cmd == "help") {
        Cmd_Help(argc, (const char **)argv);
    } else if (cmd == "echo") {
        Cmd_Echo(argc, (const char **)argv);
    } else if (cmd == "exit") {
        exit(0);
    } else if (cmd == "logcabin") {
        Cmd_Logcabin(argc, (const char**)argv);
    } else if (cmd == "partition") {
        Cmd_Partition(argc, (const char**)argv);
    } else if (cmd == "snub_nodes") {
        Cmd_SnubNodes(argc, (const char**)argv);
    } else if (cmd == "#") {
        // Ignore comments
    } else if (cmd != "") {
        printf("Unknown command '%s'\n", argv[0]);
    }
}

void
Prompt()
{
    char buf[DEBUG_MAX_LINE];
    cout << "Client Shell" << endl;

    while (cin.good()) {
        cout << "> ";

        // read input
        cin.getline((char *)&buf, DEBUG_MAX_LINE);

        DispatchCommand(buf);
    }
}

void
RunScript(const char *file)
{
    char buf[DEBUG_MAX_LINE];
    fstream fin {file};

    while (!fin.eof()) {
        // read input
        fin.getline((char *)&buf, DEBUG_MAX_LINE);

        DispatchCommand(buf);
    }
}

int
main(int argc, const char *argv[])
{

    // Setup connection
    try {
        std::vector<Node> nodes = readConfig();

        for (Node n : nodes) {
            cout << "Connecting to " << n.public_ip << endl;
            clients.push_back(new Client());
            clients[clients.size() - 1]->open(n.public_ip);
        }
    } catch (exception &e) {
        cout << "Connection failed!" << endl;
        cout << "Exception: " << e.what() << endl;
        return 1;
    }

    // Either execute script or prompt
    try {
        if (argc == 1) {
            cout << "Prompting" << endl;
            Prompt();
        } else {
            RunScript(argv[2]);
        }
    } catch(exception &e) {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
