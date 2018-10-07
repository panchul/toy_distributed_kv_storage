
# Key-Value Cluster

---

Task: Implement distributed key-value store.

Description: Implement a set of two apps - node and client. Node application should act as a storage server which
is able to store and retrieve string key-value pairs. In case of multiple nodes launched all nodes should connect
to each other and share all keys and values they have including newly added keys on any of the nodes. Node should
be able to bootstrap itself with a single IP:port combination of any node already running. Client should be able
to connect to any node with IP:port and key specified and retrieve value for the corresponding key.

Language choice: Go, C++.

---

Implementation is done in C++, using standard network calls. Tested on Mac and Linux(Ubuntu).
Each node sends its information(about itself, known hosts it connected to, and data) to all other nodes.

Each key-value pair is supplemented with the timestamp of the last update. So, when the cluster
updates data at each node, the eventual consistency is achieved.


## node (server)

For simplicity, it is a regular non-deamon application that can be put to background.

```
Usage:
   node <ip> <listening port> [ <bootstrap server ip> <bootstrap server port>]
```            

A single-node cluster will work.

To start new nodes as part of the cluster, they need to be given the address and port of an existing node.
            
## client

Application ```client``` is implemted with the following commands:

```
Usage:
  To get node status:  $ client <node ip> <node port> status
  To shutdown a node:  $ client <node ip> <node port> shutdown
  To retrieve:         $ client <node ip> <node port> get <key>
  To store:            $ client <node ip> <node port> set <key> <value>
```

It is using specially-implemented client API with these public functions:
 
```
string kvs_set(const string& node_ip, unsigned int node_port, const string& key, const string& value);
string kvs_get(const string& node_ip, unsigned int node_port, const string& key);
string kvs_status(const string& node_ip, unsigned int node_port);
string kvs_shutdown(const string& node_ip, unsigned int node_port);
```

There is also a more generic ```kvm_generic_request()``` that they all call.
In case of error, they all can set global variable kvs_error, but for simplicity just exit.


## Test run on Mac or Linux:

Compiles the code, and runs the scenario:

- starts 3 nodes in the background, on the localhost, ports 1234, 1235, 1236.
- creates key-value "mykey"->"myvalue" by contacting the node at port 1234
- retrieves the key-value by contacting another node, at port 1235
- shuts down all nodes


```

$ make clean
rm -f *.o node client

$ ls -1
Makefile
Readme.md
client.cpp
common_wrappers.h
kvs_api.cpp
kvs_api.h
kvs_common.h
kvs_networking.cpp
kvs_node.cpp
kvs_node.h
kvs_node_sync.cpp
node.cpp
test_client_store_retrieve.sh


$ make
g++ -O2 -g -Wall -fmessage-length=0 -std=c++1z   -c -o node.o node.cpp
g++ -O2 -g -Wall -fmessage-length=0 -std=c++1z   -c -o kvs_node.o kvs_node.cpp
g++ -O2 -g -Wall -fmessage-length=0 -std=c++1z   -c -o kvs_networking.o kvs_networking.cpp
g++ -O2 -g -Wall -fmessage-length=0 -std=c++1z   -c -o kvs_node_sync.o kvs_node_sync.cpp
g++ -o node node.o kvs_node.o kvs_networking.o kvs_node_sync.o -lresolv -lpthread
g++ -O2 -g -Wall -fmessage-length=0 -std=c++1z   -c -o client.o client.cpp
g++ -O2 -g -Wall -fmessage-length=0 -std=c++1z   -c -o kvs_api.o kvs_api.cpp
g++ -o client client.o kvs_api.o kvs_networking.o -lresolv -lpthread


$ make test
./test_client_store_retrieve.sh
Starting node1 ...
Starting node2 bootstrapping to node1 ...
Starting node3 bootstrapping to node2 ...
Starting unattached node "127.0.0.1" listening to port 1234 
Starting node "127.0.0.1" listening to port 1235 and attaching to the cluster via node 127.0.0.1:1234
Starting node "127.0.0.1" listening to port 1236 and attaching to the cluster via node 127.0.0.1:1235
Storing "mykey->"myvalue" on node1
OK created
status at nodes
OK 
 Total known nodes: 3
"127.0.0.1":1235 last heard time 1538941200
"127.0.0.1":1236 last heard time 1538941200
"127.0.0.1":1234 last heard time 0  (self)
 Total storage: 1
mykey myvalue 1538941201
OK 
 Total known nodes: 3
"127.0.0.1":1234 last heard time 1538941201
"127.0.0.1":1236 last heard time 1538941201
"127.0.0.1":1235 last heard time 0  (self)
 Total storage: 1
mykey myvalue 1538941201
OK 
 Total known nodes: 3
"127.0.0.1":1235 last heard time 1538941202
"127.0.0.1":1234 last heard time 1538941202
"127.0.0.1":1236 last heard time 0  (self)
 Total storage: 1
mykey myvalue 1538941201
Retrieving "mykey" at node2. Should be "myvalue"
myvalue
shutting down node1
Exiting node "127.0.0.1:1234"
shutting down node2
Exiting node "127.0.0.1:1235"
shutting down node3
Exiting node "127.0.0.1:1236"

```

Success.

