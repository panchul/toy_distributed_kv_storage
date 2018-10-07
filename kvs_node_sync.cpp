#include <sstream>

#include "kvs_common.h"
#include "kvs_node.h"

void KVS_Node::SyncNodes() {

	time_t now = time(NULL);

	for (auto it : _known_nodes) {

		if (it->_last_heard_time + _refresh_rate <= now) { // to prevent from triggering avalanche of updates
			SendUpdateToNode(it);
		}
	}
}

int KVS_Node::SendUpdateToNode(KVS_Node_Config* pnode) {

	stringstream request;

	request << "bulkupdate_by_node ";

	request << this->_known_nodes.size() + 1 << "\n";
	for (auto it : this->_known_nodes) {
		request << it->_name << " " << it->_port_number << " " << it->_last_heard_time << "\n";
	}
	// Append the self at the end - we expect it at the end while processing this request.
	request << this->_config._name << " " << this->_config._port_number << " " << time(NULL) << "\n";

	request << this->_storage.size() << "\n";
	for (auto it : this->_storage) {
		request << it.first << " " << it.second._value << " " << it.second._last_updated << "\n";
	}

	stringstream response;

	// connect, send 'bulkupdate_by_node ...'
	int sockfd;
	struct sockaddr_in servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	servaddr.sin_port = htons(pnode->_port_number);

	if (inet_pton(AF_INET, pnode->_name.c_str(), &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", pnode->_name.c_str());

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	Write(sockfd, (void *) request.str().c_str(), request.str().size());

	Close(sockfd);

	return 0;
}
