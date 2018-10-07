#include <cassert>

#include "kvs_common.h"
#include "kvs_node.h"

using namespace std;

void Usage() {
	cout << "Usage:\n\n"
	"     node <ip> <listening port> [ <bootstrap server ip> <bootstrap server port>]\n\n";
}

int main(int argc, char *argv[]) {

	KVS_Node node;

	if (argc == 3) {
		node._config._name = string(argv[1]);
		node._config._port_number = atoi(argv[2]);
		cout << "Starting unattached node \"" << node._config._name
				<< "\" listening to port " << node._config._port_number
				<< " \n";

	} else if (argc == 5) {
		node._config._name = string(argv[1]);
		node._config._port_number = atoi(argv[2]);

		node._known_nodes.push_back(new KVS_Node_Config);
		assert(node._known_nodes.back());

		node._known_nodes.back()->_name = string(argv[3]);
		node._known_nodes.back()->_port_number = atoi(argv[4]);

		cout << "Starting node \"" << node._config._name
				<< "\" listening to port " << node._config._port_number
				<< " and attaching to the cluster via node "
				<< node._known_nodes.back()->_name << ":"
				<< node._known_nodes.back()->_port_number << "\n";
	} else {
		Usage();
		return 1;
	}

	if (node.EstablishListening()) {
		cout << "ERROR: Can't establish listening! node \""
				<< node._config._name << "\"\n";
		return 1;
	}

	int connfd;

	while (!node.is_shutdown) {

		node.SyncNodes();

		connfd = Accept(node.listenfd, (SA *) NULL, NULL);

		if (connfd != -1) {

			node.ProcessRequest(connfd);
		}
	}

	cout << "Exiting node \"" << node._config._name << ":"
			<< node._config._port_number << "\"\n";

	Close(node.listenfd);

	return 0;
}

