#include <iostream>
#include <sstream>
#include "kvs_api.h"

using namespace std;

void Usage() {
	cout << "Usage:\n\n"
		"To get node status:  client <node ip> <node port> status\n"
		"To shutdown a node:  client <node ip> <node port> shutdown\n"
		"To retrieve:         client <node ip> <node port> get <key>\n"
		"To store:            client <node ip> <node port> set <key> <value>\n\n";
}

int main(int argc, char *argv[]) {

	if (argc >= 4) {

		string node_ip = string(argv[1]);
		unsigned int node_port = atoi(argv[2]);
		string command = string(argv[3]);

		if (command == "status") {
			cout << kvs_status(node_ip, node_port);
			return 0;

		} else if (command == "shutdown") {
			cout << kvs_shutdown(node_ip, node_port);
			return 0;

		} else if (command == "get" && argc == 5) {
			string key = string(argv[4]);

			cout << kvs_get(node_ip, node_port, key);
			return 0;

		} else if (command == "set" && argc == 6) {
			string key = string(argv[4]);
			string value = string(argv[5]);

			cout << kvs_set(node_ip, node_port, key, value);
			return 0;
		}
	}

	Usage();
	return 1;
}
