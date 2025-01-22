/**
 * @brief DAS Node client to send add atom requests
 */
#pragma once
#include "DistributedAlgorithmNode.h"
#include "das_node/star_node.h"
#include "utils/queue.h"

using namespace atom_space_node;

namespace das {
    class ServerNode : public StarNode{
        public:
            /**
             * @brief Client constructor
             * @param node_id ID of this node in the network.
             * @param server_id ID of a server.
             */
            ServerNode(const string &node_id, const string &server_id);
            /**
             * Destructor
             */
            ~ServerNode();
            void create_link(string &request);
        private:
            const string CREATE_ATOM = "create_atom";
            Queue<string> requests_queue;


    };
}
