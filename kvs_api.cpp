#include <string>
#include <sstream>

#include "kvs_api.h"

using namespace std;

int kvs_error; // 0 if no errors (KVS_ERRORCODE_SUCCESS)

string kvs_status(const string& node_ip, unsigned int node_port) {

	stringstream request;

	request << "status \n";
	string response = kvs_generic_request(node_ip, node_port, request.str());

	return response;
}

string kvs_get(const string& node_ip, unsigned int node_port,
		const string& key) {

	stringstream request;

	request << "get " << key << "\n";
	string response = kvs_generic_request(node_ip, node_port, request.str());

	return response;
}

string kvs_set(const string& node_ip, unsigned int node_port, const string& key,
		const string& value) {

	stringstream request;

	request << "set " << key << " " << value << " " << time(NULL) << "\n";
	string response = kvs_generic_request(node_ip, node_port, request.str());

	return response;
}

string kvs_shutdown(const string& node_ip, unsigned int node_port) {

	stringstream request;

	request << "shutdown \n";
	string response = kvs_generic_request(node_ip, node_port, request.str(),
			false);

	return response;
}

string kvs_generic_request(const string& node_ip, unsigned int node_port,
		const string& request, bool expect_response) {

	KVS_Node_Config config;
	stringstream response;

	config._name = node_ip;
	config._port_number = node_port;

	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	servaddr.sin_port = htons(config._port_number);

	if (inet_pton(AF_INET, config._name.c_str(), &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", config._name.c_str());

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	Write(sockfd, (void*) request.c_str(), request.size());

	if (expect_response) {

		while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
			recvline[n] = 0;	// null terminate

			response << recvline;
		}
		if (n < 0)
			err_sys("read error");
	}

	Close(sockfd); // for simplicity, we do not persist the connection

	return response.str();
}

