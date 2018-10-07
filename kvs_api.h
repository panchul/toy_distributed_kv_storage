#ifndef __KVS_API_H__
#define __KVS_API_H__

#include "kvs_common.h"

#define KVS_ERRORCODE_SUCCESS	0
#define KVS_ERRORCODE_GENERIC	1

extern int kvs_error; // 0 if no errors (KVS_ERRORCODE_SUCCESS)

string kvs_status(const string& node_ip, unsigned int node_port);
string kvs_get(const string& node_ip, unsigned int node_port, const string& key);
string kvs_set(const string& node_ip, unsigned int node_port, const string& key, const string& value);
string kvs_shutdown(const string& node_ip, unsigned int node_port);

string kvs_generic_request(const string& node_ip,
		unsigned int node_port, const string& request, bool expect_response = true);



#endif
