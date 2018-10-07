#!/usr/bin/env bash

echo "Starting node1 ..."
./node 127.0.0.1 1234 &

echo "Starting node2 bootstrapping to node1 ..."
./node 127.0.0.1 1235 127.0.0.1 1234 &

echo "Starting node3 bootstrapping to node2 ..."
./node 127.0.0.1 1236 127.0.0.1 1235 &

sleep 1

echo "Storing \"mykey->\"myvalue\" on node1"
./client 127.0.0.1 1234  set mykey myvalue

echo "status at nodes"
./client 127.0.0.1 1234 status
sleep 1
./client 127.0.0.1 1235 status
sleep 1
./client 127.0.0.1 1236 status

sleep 1

echo "Retrieving \"mykey\" at node2. Should be \"myvalue\""
./client 127.0.0.1 1235 get mykey

sleep 1

echo "shutting down node1"
./client 127.0.0.1 1234 shutdown

echo "shutting down node2"
./client 127.0.0.1 1235 shutdown

echo "shutting down node3"
./client 127.0.0.1 1236 shutdown


