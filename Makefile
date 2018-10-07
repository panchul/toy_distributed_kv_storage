
#---
#
## Task: Implement distributed key-value store.
#
#Description: Implement a set of two apps - node and client. Node application should act as a storage server which
#is able to store and retrieve string key-value pairs. In case of multiple nodes launched all nodes should connect
#to each other and share all keys and values they have including newly added keys on any of the nodes. Node should
#be able to bootstrap itself with a single IP:port combination of any node already running. Client should be able
#to connect to any node with IP:port and key specified and retrieve value for the corresponding key.
#
#Language choice: Go, C++.
#---

CXX = g++
CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++1z


OBJS1 =	node.o kvs_node.o kvs_networking.o kvs_node_sync.o
OBJS2 =	client.o kvs_api.o kvs_networking.o
INCLUDES = kvs_common.h common_wrappers.h kvs_node.h kvs_api.h

LIBS = -lresolv -lpthread

TARGET1 = node
TARGET2 = client

all: $(TARGET1) $(TARGET2)

$(TARGET1):	$(OBJS1)
	$(CXX) -o $(TARGET1) $(OBJS1) $(LIBS)

$(TARGET2):	$(OBJS2) 
	$(CXX) -o $(TARGET2) $(OBJS2) $(LIBS)

%.o: %.c $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $<
    
clean:
	rm -f *.o $(TARGET1) $(TARGET2)

test: $(TARGET1) $(TARGET2)
	./test_client_store_retrieve.sh
	 