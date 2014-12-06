/* Copyright (c) 2012 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <cassert>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <set>
#include <vector>


#include "LogCabin/Client.h"

namespace {

using LogCabin::Client::Cluster;
using LogCabin::Client::Tree;
using LogCabin::Client::Result;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

/**
 * Parses argv for the main function.
 */
class OptionParser {
  public:
    OptionParser(int& argc, char**& argv)
        : argc(argc)
        , argv(argv)
        , clusterFile("timber.config")
        , duration(0)
        , length(1000)
        , verbose(false)
    {
        while (true) {
            static struct option longOptions[] = {
               {"clusterFile",  required_argument, NULL, 'c'},
               {"help",  no_argument, NULL, 'h'},
               {"length", required_argument, NULL, 'l'},
               {"duration", required_argument, NULL, 'd'},
               {"verbose",  no_argument, NULL, 'v'},
               {0, 0, 0, 0}
            };
            int c = getopt_long(argc, argv, "c:d:hl:v", longOptions, NULL);

            // Detect the end of the options.
            if (c == -1)
                break;

            switch (c) {
                case 'c':
                    clusterFile = optarg;
                    break;
                case 'd':
                {
                    std::stringstream ss(optarg);
                    ss >> duration;
                    break;
                }
                case 'l':
                {
                    std::stringstream ss(optarg);
                    ss >> length;
                    break;
                }
                case 'v':
                    verbose = true;
                    break;
                case 'h':
                    usage();
                    exit(0);
                case '?':
                default:
                    // getopt_long already printed an error message.
                    usage();
                    exit(1);
            }
        }
    }

    void usage() {
        std::cout << "Usage: " << argv[0] << " [options] <servers>"
                  << std::endl;
        std::cout << "Options: " << std::endl;
        std::cout << "  -c, --clusterFile <filename> "
                  << "The file describing the logcabin cluster "
                  << "(default: timber.config)" << std::endl;
        std::cout << "  -d, --duration <seconds> "
                  << "The duration for which to write, superceeds length"
                  << "(default 0, meaning unbounded)" << std::endl;
        std::cout << "  -l, --length <length> "
                  << "The number of ints to write to the test file "
                  << "(default 1000)" << std::endl;
        std::cout << "  -h, --help              "
                  << "Print this usage information" << std::endl;
        std::cout << "  -v, --verbose           "
                  << "Print out extra information while running" << std::endl;
    }

    int& argc;
    char**& argv;
    std::string clusterFile;
    int length;
    int duration;
    bool verbose;
};

} // anonymous namespace

enum class Op {
    Read,
    Write,
    Rm
};

class TreeWrapper {
public:
    TreeWrapper(std::vector<string> ips, Tree t)
        : tree(t)
        , addrs(ips)
        , currPos(0)
    { }

    void
    retryOp(Op op, const string& name, string& contents) {
        while (true) {
            Result res;
            string opStr;
            if (op == Op::Read) {
                opStr = "Read";
                res = tree.read(name, contents);
            } else if (op == Op::Write) {
                opStr = "Write";
                res = tree.write(name, contents);
            } else if (op == Op::Rm) {
                opStr = "Remove";
                res = tree.removeFile(name);
            }

            if (res.status != LogCabin::Client::Status::OK) {
                std::cout << opStr << " Error " << res.status << ": " << res.error << std::endl;
                rotateTree();
            } else {
                return;
            }
        }
    }

    void
    retryOp(Op op, const string& name) {
        string str = "";
        retryOp(op, name, str);
    }


    static std::vector<string>
    parseCluster(string filename) {
        std::vector<string> addrs;
        ifstream f(filename);
        string ignore;
        while (!f.eof()) {
            f >> ignore; //node name
            f >> ignore; //public ip
            string ip;
            f >> ip; //private ip (the one we want)
            addrs.push_back(ip + ":61023"); //default logcabin port
        }
        return addrs;
    }

    static TreeWrapper
    newTreeWrapper(const string& filename) {
        auto addrs = parseCluster(filename);
        Cluster clust(addrs[0]);
        return TreeWrapper(addrs, clust.getTree());
    }
private:
    Tree tree;
    std::vector<string> addrs;
    int currPos;

    void
    rotateTree() {
        currPos++;
        Cluster clust(addrs[currPos]);
        //delete tree;
        tree = clust.getTree();
    }
};

void
verifyContents(std::string contents, int length) {
    std::vector<int> expected;
    for (int i = 0; i < length; i++) {
        expected.push_back(i);
    }

    int dupes = 0;
    std::set<int> actual;
    stringstream ss(contents);
    while (ss) {
        int i = -1;
        ss >> i;
        if (i < 0) break;
        if (actual.find(i) != actual.end()) {
            dupes++;
        } else {
            actual.insert(i);
        }
    }
    int drops = length - actual.size();

    float dropPct = 100 * (float) drops / length;
    float dupePct = 100 * (float) dupes / length;

    cout << "|A|: " << length << endl;
    cout << "|R|: " << actual.size() << endl;
    cout << "|S|: " << dupes << endl;
    cout << "|D|: " << drops << endl;
    cout << "|D|/|A|: " << dropPct << endl;
    cout << "|S|/|A|: " << dupePct << endl;
}

void
writeInt(int i, TreeWrapper tree, bool verbose) {
    string contents;
    tree.retryOp(Op::Read, "/intlist", contents);
    if (verbose)
        std::cout << "Contents so far: " << contents << std::endl;
    stringstream ss;
    ss << i << ' ';
    contents += ss.str();
    tree.retryOp(Op::Write, "/intlist", contents);
}

int
main(int argc, char** argv)
{
    OptionParser options(argc, argv);
    TreeWrapper tree = TreeWrapper::newTreeWrapper(options.clusterFile);
    tree.retryOp(Op::Write, "/intlist");
    if (options.duration == 0) {
        cout << "Writing " << options.length << " ints" << endl;
        for (int i = 0; i < options.length; i++) {
            writeInt(i, tree, options.verbose);
        }
    } else {
        cout << "Writing ints for " << options.duration << " seconds" << endl;
        time_t startTime;
        time(&startTime);
        time_t currTime = startTime;
        int i = 0;
        while (currTime <= startTime + options.duration) {
            writeInt(i, tree, options.verbose);
            i++;
            time(&currTime);
        }
        options.length = i;
    }
    string contents;
    tree.retryOp(Op::Read, "/intlist", contents);
    tree.retryOp(Op::Rm, "/intlist");
    verifyContents(contents, options.length);
    return 0;
}
