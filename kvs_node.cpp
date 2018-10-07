#include <sstream>

#include "kvs_common.h"
#include "kvs_node.h"

#define HELP_REPL "KVStorage v.0.0.1 \n\nServer api commands:\n" \
    			"\tshutdown\n" \
    			"\tstatus\n" \
    			"\tset <key> <value> <timestamp>\n" \
    			"\tsetbulk <number> <key> <value> <timestamp> [<key> <value> <timestamp> ...]\n" \
				"\tbulkupdate_by_node <number> <nodename> <nodeport> <key> <value> <timestamp> [<key> <value> <timestamp> ...]\n" \
    			"\tget \n" \
    			"\tdelete <key> <timestamp> (for debugging only, do not use)\n"

int KVS_Node::EstablishListening() {

	struct sockaddr_in servaddr;

	this->listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(this->_config._port_number);

	Bind(this->listenfd, (SA *) &servaddr, sizeof(servaddr));
	Listen(this->listenfd, LISTENQ);

	return 0;
}

int KVS_Node::ProcessRequest(int connfd) {

	char buffer[MAX_REQUEST_BUFSIZE + 1];
	long ret;
	ret = Read(connfd, buffer, MAX_REQUEST_BUFSIZE);
	if (ret == 0 || ret == -1) {
		cout << "ERROR: failed to read request on fconnfd " << connfd << "\n";
		return 1;
	}

	if (ret > 0 && ret < MAX_REQUEST_BUFSIZE) {	// return code is ok
		buffer[ret] = 0;
	} else {
		buffer[0] = 0;
	}

	//cout << "got request \"" << buffer << "\"\n";

	stringstream ss(buffer);
	std::string command;

	ss >> command;
	stringstream response;

	if (command == "help") {

		this->is_shutdown = true;
		response << "OK " HELP_REPL;

	} else if (command == "shutdown") {

		this->is_shutdown = true;
		//response << "OK \n";

	} else if (command == "set" || command == "setbulk"
			|| command == "bulkupdate_by_node") {

		std::string key, value;
		time_t timestamp;
		int total_number = 1;

		if (command == "setbulk")
			ss >> total_number;

		if (command == "bulkupdate_by_node") {
			int number_of_hosts;
			ss >> number_of_hosts;

			while (number_of_hosts--) {
				string nodename;
				int nodeport;
				time_t timestamp;
				ss >> nodename >> nodeport >> timestamp;

				bool already_exists = false;
				for (auto it : this->_known_nodes) {
					if (it->_name == nodename && it->_port_number == nodeport) {
						already_exists = true;

						if (number_of_hosts == 0) { // the sending node appends its name last
							it->_last_heard_time = time(NULL);
						} else {
							it->_last_heard_time = max(timestamp,
									it->_last_heard_time);
						}
					}
				}
				if ((this->_config._name != nodename
						|| this->_config._port_number != nodeport)
						&& !already_exists) {
					this->_known_nodes.push_back(
							new KVS_Node_Config(nodename, nodeport,
									time(NULL)));
					//cout << "added to known_hosts:  node " << nodename << ":"
					//		<< nodeport << "\n";
				}
			}

			ss >> total_number;
		}

		while (total_number-- > 0) {
			ss >> key >> value >> timestamp;

			if (this->_storage.find(key) == this->_storage.end()) {
				this->_storage[key] = TimestampedValue(value, timestamp);
				response << "OK created\n";
			} else {
				if (this->_storage[key]._last_updated <= timestamp) {
					this->_storage[key] = TimestampedValue(value, timestamp);
					response << "OK updating existing\n";
				} else {
					response << "OK ignoring stale\n";
				}
			}
		}

	} else if (command == "delete") {

		std::string key;
		time_t timestamp;

		ss >> key >> timestamp;

		if (this->_storage.find(key) == this->_storage.end()) {
			response << "OK already deleted\n";
		} else {
			if (this->_storage[key]._last_updated <= timestamp) {
				this->_storage.erase(key);
				response << "OK deleted\n";
			} else {
				response << "OK ignoring stale delete\n";
			}
		}

	} else if (command == "get") {

		string key;
		ss >> key;
		// response << "OK " << this->_storage[key] << "\n";
		response << this->_storage[key]._value << "\n";

	} else if (command == "status") {

		response << "OK \n" << " Total known nodes: "
				<< this->_known_nodes.size() + 1 << "\n";

		for (auto it : this->_known_nodes) {
			response << "\"" << it->_name << "\":" << it->_port_number
					<< " last heard time " << it->_last_heard_time << "\n";
		}
		response << "\"" << this->_config._name << "\":"
				<< this->_config._port_number << " last heard time "
				<< this->_config._last_heard_time << "  (self)\n";

		response << " Total storage: " << this->_storage.size() << "\n";

		for (auto it : this->_storage) {
			response << it.first << " " << it.second._value << " "
					<< it.second._last_updated << "\n";
		}

	} else {
		cout << "got unknown command \"" << command << "\"\n";
		response << "ERROR " << "Unknown command \"" << command << "\"\n"
		HELP_REPL;
	}

	if (!this->is_shutdown)
		Write(connfd, (void*) (response.str().c_str()), response.str().size());

	Close(connfd);

	return 0;
}

