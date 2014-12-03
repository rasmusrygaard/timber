Getting Started
===============

To run the scripts here, you need all the dependencies for lab #1 listed below.
In addition, you should get boto for Python to talk to ec2.
The scripts assume that you have `AWS_KEY` and `AWS_SECRET` environment variables defined.
You can defined them using `export AWS_KEY=mykey` etc.


## Launching a Cluster

Once you have the dependencies installed, you can launch an EC2 cluster:

    python launch_ec2.py N

Here `N` is the number of instances to launch.
It should probably be `2f + 1` for some `f` > 1.
The script will create a file `timber.config` which stores the ids and ips for the instances.
Subsequent scripts (including the XDR shell) will depend on this file, so don't modify it unless you have a very good reason to do so.
The file like this (columns are tab-separated):

    instance-id  public-ip   private-ip

You will need the public ip to SSH into the instance and the private ip to set up communication between instances.

Now your cluster is up and running.
To start playing with it, first set up Timber and any dependencies:

    python boostrap_ec2.py

This will take a little while to run.
After the script finishes, you should be able to launch the shell (if this fails, try running boostrap again):

    shell/shell

This will pop you into a REPL where you can execute commands to run against the entire cluster.
The connections are established using `timber.config` as described above.
For example, to set up the cluster, run the `logcabin` command.
This will install logcabin and dependencies and configure the cluster.
This also takes a while, but you can keep track of the progress by SSH'ing into each instance and running `tail server.log` from the home directory.
The output from the XDR process gets written to that file, so you can easily see what is goin on on the other side.

Once the command finishes on all clusters, you should be able to start storing stuff in LogCabin.
There's a small demo program in `logcabin/Examples/HelloWorld` on the cluster machines.
If you run it, the script should finish immediately without any errors and you should see no output in your shell.
If you are `tail`ing the other servers, you might see some snapshots being recorded.

## Partitioning the Cluster

Once you have finished setting up the cluster (i.e. wait until the prompt has been able to write 'Joining thread 0' through 
'Joining thread (N - 1)'), you should be ready to experiment with partitioning the nodes. 

3 function calls (so far) do this : partition, snubnodes A B C..., and heal_cluster

    partition

This function call splits the cluster in half. So, if you have a cluster with N=6 machines, the system 
will be split in such a way that n1, n2, and n3 will be able to communicate with each other, but they
will not be able to communicate with n4, n5, and n6 (and vice versa).

    snubnodes A B

Here 'A' and 'B' are specific nodes identified by an integer. 
This function places specified nodes in a partition, and places unspecified nodes in a
different partition.  For example, if you wanted to have n1 and n5 reject traffic from n2, 
n3, and n4 but accept traffic from each other, you would call `snubnodes 1 5`, which 
partitions n1 and n5 from n2, n3 and n4.

    heal_cluster

This function will reset the iptables chains to their defaults, thereby accepting all traffic from 
all of the nodes in the cluster.

You can keep track of which computers can talk to each other by SSH'ing into each instance and running
`sudo iptables -L`. This lists all of the network configuration rules.




## Shutting Down The Cluster

When you're done, run the following script:

    python kill_ec2.py

This will nuke your EC2 instances and get rid of `timber.config`.

Old Lab #1 Documentation
========================

Lab #1
======

The purpose of Lab #1 is to acquaint you with the concept of Remote
Procedure Calls (RPCs).  You will be building a simple key-value store
similar to the Zookeeper paper you read for class, but with just a
single (unreplicated) server.  In the subsequent lab we will make this
reliable and you can use this as the foundation for your group
project.

Build Dependencies
-----------------

 * gmake
 * gcc or clang with C++11
 * bison
 * flex

Getting Started
---------------

Clone the source code with:

        $ git clone http://www.scs.stanford.edu/14au-cs244b/labs/lab1.git

XDRPP
-----

You will be using the
[Open Network Computing (ONC) Remote Procedure Call (RPC)](http://en.wikipedia.org/wiki/Open_Network_Computing_Remote_Procedure_Call)
protocol to implement this lab.  RPC has been an Internet standard for
years and is widely used, especially in network storage protocols such
as NFS.  The latest RPC specification is available as
[RFC5531](https://tools.ietf.org/html/rfc5531), though you do not need
to read that specification for this lab.  The document you may want to
look at is the External Data Representation (XDR) standard, [RFC4506],
which describes the language in which you express data structures
transmitted between machines with RPC.

XDR is both a language for describing data structures, and a
specification for how to serialize those data structures.  The
language should feel familiar to C and C++ programmers.  We went over
the syntax in the first lecture, so in addition to [RFC4506] you may
want consult the
[lecture nodes](http://www.scs.stanford.edu/14au-cs244b/notes/intro.pdf#18).

Traditionally, C and C++ implementations of RPC have not been type
safe.  Fortunately, the instructor has built you a brand new RPC
implementation, called XDRPP, that takes advantage of C++11 features
to provide a type safe interface.  XDRPP consists of an XDR compiler,
called `xdrc`, and a runtime library `libxdrpp`.  Documentation is
available
[here](http://www.scs.stanford.edu/14au-cs244b/labs/xdrpp-doc/).
Source code will automatically be placed in a subdirectory of your lab
via the git submodule system.

If you have not used C++11, you may wish to familiarize yourself with
some of the features.  Stroustrup has made
[a tour of C++](https://isocpp.org/tour) available on line, and the
[wikipedia page on C++11](http://en.wikipedia.org/wiki/C%2B%2B11)
summarizes features that will be new to people who have only
programmed in C++03.

You may wish to look at the "Native Representations" section of the
[`xdrc` manual page](http://www.scs.stanford.edu/14au-cs244b/labs/xdrpp-doc/md_doc_xdrc_81.html).
`xdrc` introduces several new container types, including
`xdr::xstring`, `xdr::xarray`, and `xdr::xvector`.  However, these are
subtypes of the standard C++11 types
[`std::string`](http://en.cppreference.com/w/cpp/string/basic_string),
[`std::array`](http://en.cppreference.com/w/cpp/container/array), and
[`std::vector`](http://en.cppreference.com/w/cpp/container/vector), so
for the most part you can use these as if they were the standard
library types.  XDRPP introduces new subtypes both to keep track of
the maximum length of containers at the type level as well as to
provide a few class members that help unify treatment of marshaled
data types.  XDR optional data, which is specified in syntax like C
pointers, is represented by `xdr::pointer`, a subtype of
[`std::unique_ptr`](http://en.cppreference.com/w/cpp/memory/unique_ptr).

One useful feature of XDRPP is that is can trace and pretty-print RPC
calls if you run the following command to set environment variables:

    $ export XDR_TRACE_CLIENT= XDR_TRACE_SERVER=1

When something is going wrong, tracing calls is a good way to
determine whether the client or the server is to blame.

Overview
--------

Here we will show you how to get started with the system.  First inside the
lab1 directory you should try to run make.

        $ make

In order to define the RPCs that your server will support you will modify
`server.x` to define the parameters and return values.  Each RPC can take one
argument and one return value.  In order to pass multiple parameters you will
use structures.  The necessary XDR definition for the create method is shown
below.

        struct kvpair {
            string key<512>;
            string val<>;
        };

        program server_api {
          version api_v1 {
            bool create(kvpair) = 1;
          } = 1;
        } = 0x40048086;

We have included in the repository a sample `serverimpl.{cc|hh}` file that
implements the server side just for the create method.  Every time you modify
the RPC protocol definition you will need to use the XDRPP compiler to
regenerate these files.  You need to rename the old files and merge your
changes into the newly generated files.  To regenerate these files run:

        $ make scaffold

The create method shown below shows how we extract the arguments from the `arg`
parameter.  The necessary implementation, some sample logging for debugging,
and how the result is set into the local `res`.  We left out the path sanity
checking that you would want to do.

        std::unique_ptr<bool>
        api_v1_server::create(std::unique_ptr<kvpair> arg)
        {
          bool hasKey;
          std::string key = arg.get()->key;
          std::string val = arg.get()->val;
          std::unique_ptr<bool> res(new bool);

          // Fill in additional sanity checking (e.g. prevent malformed paths)

          hasKey = db.hasKey(arg.get()->key);
          if (hasKey) {
            (*res) = false;
            std::cout << "Created " << key << " Failed" << std::endl;
          } else {
            (*res) = true;
            db.set(key, val);
            std::cout << "Created " << key << " Succeeded" << std::endl;
          }

          return res;
        }

In the header for the server implementation we also created a simple
constructor to initialize and open the database.

The XDRPP library takes care of all the other serialization and network setup.
Our client library `libclient/client.cc` implements wrapper functions to
simplify the usage of the RPCs.  Here you should do basic sanity checking, call
the RPC method, parse the results and if needed throw an exception.  Below the
code for library's create method.

        bool
        Client::create(const std::string &path, const std::string &val)
        {
            kvpair args;

            args.key = path;
            args.val = val;

            auto r = client->create(args);

            return *r;
        }

Finally, you should be able to test the create method by running the server and
client shell in separate terminals.

        $ server/server

        $ shell/shell
        > create /test abc
        CREATED
        > create /test def
        KEY ALREADY EXISTS

N.B. Our create example is to get you started and does not sanity check the
path creation.  You should ensure that the server does not allow malformed
paths, i.e., paths must begin with a '/' and must contain only letters,
numbers, underscores and slashes to separate components.  Like the Zookeeper
paper, we should only create nodes if they have parents and delete nodes if
they have no children.

Defining the RPC Protocol
-------------------------

You will need to define an RPC protocol in XDR format and use `xdrc`
to compile the XDR types to C++ types and to generate the server and
client definitions.

Below is a part of the `include/server.x` protocol definition you will
need to modify.  We already filled in APIs for create, remove, set.
It is missing both get and list.  The get call returns a particular
key, and list returns a set of all keys under a given path.  You may find that
you need to change these APIs to express results as error codes.

        program server_api {
            version api_v1 {
                bool create(kvpair) = 1;
                bool remove(longstring) = 2;
                bool set(kvpair) = 3;
                // TODO: You will need to fill in more calls here
                // ...
            } = 1;
        } = 0x40048086;

Review `include/client.h` to see what parameters, return values and
error codes you will need to return to the client.  Here is a summary
of the C++ API you will need to implement.

`bool create(string key, string value)`
:   This method creates a key with the specified value.  On success the method
returns true.  If the key already exists it should return false.  For any other
errors or malformed keys (i.e. keys with spaces, not beginning with a '/') we
should throw an exception.

`bool remove(string key)`
:   This method removes a key and returns true on success.  Otherwise it will
return a failure.

`string get(string key)`
:   This method returns the value of the specified key.  If the key does not
exist it will throw an exception.

`void set(string key, string value)`
:   This method sets the value of the specified key.  If the key does not exist
it will throw an exception.

`set<string> list(string key)`
:   This method will return a set of all sub-keys.  The strings should contain
just the name of sub-key, and not the full path of the key.  If the parent key
does not exist it will throw an exception.

Once complete you complete the RPC definition, use the `xdrc` compiler
(located in `xdrpp/xdrc/xdrc`) to create the XDR definitions in
(`include/server.hh`) as follows:

        $ make include/server.hh

The XDR compiler will also generate the server stubs
(`server/serverimpl.{cc,h}`) that you will fill in next.  N.B. If you
later modify the protocol you will need to regenerate and merge these
files by hand.  To generate the server scaffolding, run:

        $ make scaffold

Since you will edit these files, you may want to add them to your git
repository with:

        $ git add server/serverimpl.{cc,hh}

You should choose a unique TCP port number to run your service.  To do this
modify the define UNIQUE_RPC_PORT in include/rpcconfig.h.  Choose a random
number above 6100.

RPC Client Library
------------------

The client library provides a useful wrapper around the RPC interface.
RPC only allows functions to pass in a single parameter and single
return value, and thus you will be using structures to pass multiple
parameters.  The include/client.h header file documents the expected
behavior of these functions and you can use shell/shell.cc as a
reference.

RPC Server
----------

With the `xdrc` compiler you should have generated serverimpl.cc
already and you will need to fill in the implementation.  Your
implementation will use the facilities provided to you by XDRPP to
parse the request, and serialize the results.  Your code should do all
the necessary handling to access the database, and return errors when
necessary.

As part of the implementation you will need to instantiate the
database.  We provided you with a simple persistent key-value store in
serverdb.h.  It provides you with hasKey, get, set, remove, and list,
operations that you will use to implement the full system.

ServerDB will not do any sanity checking for you.  For example it is
your responsibility to prevent the client from deleting a key that has
children.  Nor should a client be able to create a key with no parent.
The server should sanity check that key names only contain letters,
numbers, underscores, and slashes to divide the hierarchy.


RPC Shell
---------

We provided a simple shell for you to test your program.  In one
terminal run the server.

        $ server/server

In another you can run the shell by passing in the IP address of the
server.

        $ shell/shell 127.0.0.1
        > create /test abc
        CREATED
        > delete /test
        DELETED
        > list /
        > list /test
        KEY NOT FOUND
        >

You can run a test script using the shell as well

        $ shell/shell 127.0.0.1 tests/create.tst

Please do not modify the output of shell/shell.cc or the syntax of
existing commands as we will be using the script interface for
grading.  Note that the interface is a bit brittle and does not
support values with spaces.

Testing and Submitting
----------------------

Remember to run both the positive and negative test cases for each of the
functions provided.  Verify that the correct error code is returned for each
negative test case.

In the `tests` directory there are test scripts and sample outputs with both
positive and negative tests.  You can run them using the following command
line.

        $ shell/shell 127.0.0.1 tests/create.tst
        $ shell/shell 127.0.0.1 tests/remove.tst
        $ shell/shell 127.0.0.1 tests/getset.tst
        $ shell/shell 127.0.0.1 tests/list.tst

If you implemented everything correctly the output will be a binary match to
the output files in the same directory.

To submit the project please run:

        $ make clean
        $ cd ..
        $ tar zcvf lab1.tgz lab1

[RFC4506]: http://tools.ietf.org/html/rfc4506
