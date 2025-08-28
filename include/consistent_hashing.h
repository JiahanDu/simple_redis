#ifndef CONSISTENT_HASHING
#define CONSISTENT_HASHING
#include <stddef.h>

typedef struct ConsistentHash ConsistentHash;

//Initialize a new consistent hash ring
ConsistentHash* ch_initialize();

//Free the consistent hash ring
void ch_free(ConsistentHash* ch);

//Add a node to the hash ring with given number of virtual nodes
void ch_add_node(ConsistentHash* ch, const char* node, int virtual_nodes);

//get the node responsible for a given key
const char* ch_get_node(ConsistentHash* ch, const char* key);

//remove a node
void ch_remove_node(ConsistentHash* ch, const char* node);

#endif 