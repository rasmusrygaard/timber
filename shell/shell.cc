#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>

#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

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
    while (f >> id >> private_ip >> public_ip) {
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
    client->install(implementation, nodes, id);
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

void
Cmd_Etcd(int argc, const char* argv[])
{
    std::vector<Node> nodes = readConfig();
    std::vector<std::thread> threads(clients.size());

    for (int i = 0; i < clients.size(); ++i) {
        std::cout << "Setting up Etcd on client " << i + 1 << std::endl;
        threads[i] = std::thread(&Client_Setup, clients[i], Etcd, nodes, i + 1);
    }
    for (int i = 0; i < threads.size(); ++i) {
        std::cout << "Joining thread " << i << "." << std::endl;
        threads[i].join();
    }
    for (int i = 0; i < clients.size(); ++i) {
        std::cout << "Running etcd on server " << i << "." << std::endl;
        clients[i]->run(Etcd, nodes, i + 1);
    }
}

void
Client_Partition(Client* client,
                 const std::vector<int>& group1,
                 const std::vector<int>& group2) {
    client->makePartition(group1, group2);
}

void
Client_Slow_Network(Client* client) {
    client->slowNetwork();
}

void
Client_Heal_Network(Client* client) {
    client->healNetwork();
}

void
Partition(const std::vector<Client*> clients,
          const std::vector<int>& group1,
          const std::vector<int>& group2) {

    std::vector<std::thread> threads(clients.size());
    for (int i = 0; i < threads.size(); ++i) {
        threads[i] = std::thread(&Client_Partition, clients[i], group1, group2);
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}


void
Slow(const std::vector<Client*> clients) {
    std::vector<std::thread> threads(clients.size());
    for (int i = 0; i < threads.size(); ++i) {
        threads[i] = std::thread(&Client_Slow_Network, clients[i]);
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}

void
Heal(const std::vector<Client*> clients) {
    std::vector<std::thread> threads(clients.size());
    for (int i = 0; i < threads.size(); ++i) {
        threads[i] = std::thread(&Client_Heal_Network, clients[i]);
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}

void LogPartition(const std::vector<int>& g1, const std::vector<int>& g2) {
    cout << time(NULL) << " PARTITIONED left=";
    std::copy(g1.begin(), g1.end(), std::ostream_iterator<int>(std::cout,","));
    cout << " right=";
    std::copy(g2.begin(), g2.end(), std::ostream_iterator<int>(std::cout,","));
    cout << endl;
}

void LogHealCluster() {
    cout << time(NULL) << " HEALED " << endl;
}


/*
 * Partitions the Network in half. I.e. If there are nodes
 * n1... n5 in the cluster, nodes n1, n2 will be partitioned
 * from n3, n4, n5.
 */
void
Cmd_Partition(int argc, const char* argv[])
{
    //Gets all the nodes
    std::vector<Node> nodes = readConfig();
    int num_nodes = nodes.size();

    std::vector<int> group1;
    std::vector<int> group2;

    //splits the nodes in half
    int start = 1;
    for(int i=start; i <= nodes.size()/2; i++) {
        group1.push_back(i);
    }
    for(int i=nodes.size()/2 + 1; i<=nodes.size(); i++) {
        group2.push_back(i);
    }
    LogPartition(group1, group2);
    Partition(clients, group1, group2);
}


/*
 * Snubs specified nodes specified in command line
 * arguments.
 */
void
Cmd_SnubNodes(int argc, const char* argv[])
{
    std::vector<Node> nodes = readConfig();
    int num_nodes = nodes.size();

    std::vector<int> group1; //nodes specified by command line args
    std::vector<int> group2;

    int num_args = argc;
    //put nodes in argv in group 1
    int start = 1;
    for (int i=start; i<num_args; i++) {
        int elem = atoi(argv[i]);
        std::vector<int>::iterator it = std::find(group1.begin(), group1.end(), elem);
        if (it == group1.end() || group1.empty()) {
            group1.push_back(elem);
        }
    }

    // Put unspecified nodes in group 2
    for (int i=start; i<=num_nodes; i++) {
        std::vector<int>::iterator it = std::find(group1.begin(), group1.end(), i);
        if (it == group1.end()) {
            group2.push_back(i);
        }
    }

    LogPartition(group1, group2);
    Partition(clients, group1, group2);
}



void
Cmd_Heal_Cluster(int argc, const char* argv[])
{
    /*
    std::vector<Node> nodes = readConfig();
    int num_nodes = nodes.size();
    for (int i=0; i<clients.size(); i++) {
        clients[i]->heal_cluster(num_nodes);
    }
    */
    std::vector<Node> nodes = readConfig();
    int num_nodes = nodes.size();

    std::vector<int> group1;
    std::vector<int> group2;

    //splits the nodes in half
    int start = 1;
    for(int i=start; i <= nodes.size(); i++) {
        group1.push_back(i);
    }
    group2.push_back(0);
    LogHealCluster();
    Partition(clients, group1, group2);
}

void
Cmd_Slow(int argc, const char* argv[]) {
    cout << time(NULL) << " SLOW " << endl;
    Slow(clients);
}

void
Cmd_Heal(int argc, const char* argv[]) {
    cout << time(NULL) << " NORMAL " << endl;
    Heal(clients);
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
    } else if (cmd == "etcd") {
        Cmd_Etcd(argc, (const char**)argv);
    } else if (cmd == "partition") {
        Cmd_Partition(argc, (const char**)argv);
    } else if (cmd == "snubnodes") {
        Cmd_SnubNodes(argc, (const char**)argv);
    } else if (cmd == "heal_cluster") {
        Cmd_Heal_Cluster(argc, (const char**)argv);
    } else if (cmd == "slow") {
        Cmd_Slow(argc, (const char**)argv);
    } else if (cmd == "heal") {
        Cmd_Heal(argc, (const char**)argv);
    } else if (cmd == "sleep") {
        int time = std::stoi(argv[1]);
        sleep(time);
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
    ifstream f(file);

    std::string line;

    while (getline(f, line)) {
        cout << line << endl;
        DispatchCommand((char *)line.c_str());
    }
}

int
main(int argc, const char *argv[])
{

    // Setup connection
    try {
        std::vector<Node> nodes = readConfig();

        for (Node n : nodes) {
            clients.push_back(new Client());
            clients[clients.size() - 1]->open(n.private_ip);
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
            RunScript(argv[1]);
        }
    } catch(exception &e) {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
