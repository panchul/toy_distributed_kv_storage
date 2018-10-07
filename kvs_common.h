
#ifndef __KVS_COMMON_H__
#define __KVS_COMMON_H__

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <time.h>

#include "common_wrappers.h"

#define MAX_REQUEST_BUFSIZE 8096

using namespace std;

class KVS_Node_Config {
public:
	string _name;	// it is our ip, e.g. "127.0.0.1"
	int _port_number;
	time_t _last_heard_time; // the timestamp of when we received last communication from it

    KVS_Node_Config(): _name("unknown"),_port_number(0), _last_heard_time(0) {};
    KVS_Node_Config(const string& name, int port, time_t last_heard_time):
    	_name(name),_port_number(port), _last_heard_time(last_heard_time) {};
    ~KVS_Node_Config() {};
};

typedef class KVS_Node_Config KVS_Node_Config;

struct TimestampedValue {
	string _value;	// the workload, the subject of the storage
	time_t _last_updated; // the timestamp within the cluster

	TimestampedValue():_value(""),_last_updated(0) {};
	TimestampedValue(const string& value, time_t last_updated):_value(value),_last_updated(last_updated) {};
	~TimestampedValue() {};
};




#endif
