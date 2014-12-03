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
#include <string>
#include <ctime>

#include "Client/Client.h"

namespace {

using LogCabin::Client::Cluster;
using LogCabin::Client::Tree;
using LogCabin::Client::Result;

/**
 * Parses argv for the main function.
 */
class OptionParser {
  public:
    OptionParser(int& argc, char**& argv)
        : argc(argc)
        , argv(argv)
        , cluster("logcabin:61023")
        , duration(0)
        , length(1000)
        , verbose(false)
    {
        while (true) {
            static struct option longOptions[] = {
               {"cluster",  required_argument, NULL, 'c'},
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
                    cluster = optarg;
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
        std::cout << "  -c, --cluster <address> "
                  << "The network address of the LogCabin cluster "
                  << "(default: logcabin:61023)" << std::endl;
        std::cout << "  -d, --duration <milliseconds> "
                  << "The duration for which to write (in ms), superceeds length"
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
    std::string cluster;
    int length;
    int duration;
    bool verbose;
};

} // anonymous namespace

void
verifyContents(std::string contents, int length, bool verbose) {
    std::stringstream ss;
    for (int i = 0; i < length; i++) {
        ss << i << ' ';
    }
    if (verbose) {
        std::cout << "Actual contents:" << std::endl << contents << std::endl;
        std::cout << "Expected contents:" << std::endl << ss.str() << std::endl;
    }
    assert(contents == ss.str());
}

void
writeInt(int i, Tree tree, bool verbose) {
    std::string contents;
    Result res = tree.read("/intlist", contents);
    while (res.status != LogCabin::Client::Status::OK) {
        if (verbose) {
            std::cout << "Read Error " << res.status << ": " << res.error << std::endl; 
        }
        res = tree.read("/intlist", contents);
    }
    // if (verbose)
    //     std::cout << "Contents so far: " << contents << std::endl;
    std::stringstream ss;
    ss << i << ' ';
    res = tree.write("/intlist", contents + ss.str());
    while (res.status != LogCabin::Client::Status::OK) {
        if (verbose) {
            std::cout << "Write Error " << res.status << ": " << res.error << std::endl; 
        }
        res = tree.write("/intlist", contents + ss.str());
    }
}

int
main(int argc, char** argv)
{
    OptionParser options(argc, argv);
    Cluster cluster(options.cluster);
    Tree tree = cluster.getTree();
    tree.writeEx("/intlist", "");
    if (options.duration == 0) {
        for (int i = 0; i < options.length; i++) {
            writeInt(i, tree, options.verbose);
        }
    } else {
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
    std::string contents = tree.readEx("/intlist");
    tree.removeFileEx("/intlist");
    verifyContents(contents, options.length, options.verbose);
    return 0;
}
