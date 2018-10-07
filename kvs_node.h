
#ifndef __KVS_NODE_H__
#define __KVS_NODE_H__

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <time.h>

#include "kvs_common.h"

using namespace std;

class KVS_Node {
public:
	KVS_Node_Config _config; // the information that defines this node.

	map<string, TimestampedValue> _storage; // the data this node stores.

	vector<KVS_Node_Config*> _known_nodes; // the information about other nodes we aware of.

	time_t	_refresh_rate; // to prevent unnecessary avalanche of updates, we can keep the
							// difference of the data, or introduce a window in time
							// when we presume the data is consistent. It is a tradeoff.
	int	listenfd;
	bool is_shutdown;	// the flag to stop the server.

    KVS_Node(): _refresh_rate(1), listenfd(0), is_shutdown(false) {};
    ~KVS_Node() {
    	while(!_known_nodes.empty()) {
    		delete _known_nodes.back();
    		_known_nodes.pop_back();
    	}
    };

    int EstablishListening(); // opening the listening socket.

    int ProcessRequest(int connfd); // receive request, parse, and act on it.

    void SyncNodes(); // to update the data and visible cluster nodes.
    int SendUpdateToNode(KVS_Node_Config* pnode); // update one single node with the information we have.
};

typedef class KVS_Node KVS_Node;

#endif
