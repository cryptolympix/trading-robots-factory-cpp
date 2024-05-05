#include <iostream>
#include <vector>
#include "node.hpp"
#include "connection_history.hpp"

ConnectionHistory::ConnectionHistory(std::shared_ptr<Node> from_node, std::shared_ptr<Node> to_node, int innovation_nb) : from_node(from_node), to_node(to_node), innovation_nb(innovation_nb) {}

bool ConnectionHistory::matches(std::shared_ptr<Node> from_node, std::shared_ptr<Node> to_node)
{
    return from_node->id == this->from_node->id && to_node->id == this->to_node->id;
}
