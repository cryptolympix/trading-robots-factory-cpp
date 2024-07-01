#include <iostream>
#include "math_utils.hpp"
#include "config.hpp"
#include "node.hpp"
#include "connection_gene.hpp"

neat::ConnectionGene::ConnectionGene(std::shared_ptr<Node> from, std::shared_ptr<Node> to, double w, int innovation, bool enabled) : from_node(from), to_node(to), weight(w), innovation_nb(innovation), enabled(enabled) {}

neat::ConnectionGene::~ConnectionGene()
{
    this->from_node.reset();
    this->to_node.reset();
}

void neat::ConnectionGene::mutate(const neat::Config &config)
{
    // Change the weight of the connection or disable/enable it
    if (randrange() < config.weight_replace_rate)
    {
        this->weight = uniform(config.weight_min_value, config.weight_max_value);
    }
    else if (randrange() < config.weight_mutate_rate)
    {
        // Otherwise, slightly change it
        this->weight += normal(config.weight_init_mean, config.weight_init_stdev) / 50;
        // Keep weight between bounds
        this->weight = std::min(std::max(weight, config.weight_min_value), config.weight_max_value);
    }

    if (randrange() < config.enabled_mutate_rate)
    {
        this->enabled = !this->enabled;
    }
}

bool neat::ConnectionGene::is_equal(std::shared_ptr<ConnectionGene> other)
{
    return this->from_node->id == other->from_node->id && this->to_node->id == other->to_node->id && this->weight == other->weight && this->innovation_nb == other->innovation_nb && this->enabled == other->enabled;
}

std::shared_ptr<neat::ConnectionGene> neat::ConnectionGene::clone(std::shared_ptr<Node> from, std::shared_ptr<Node> to)
{
    return std::make_shared<ConnectionGene>(from, to, this->weight, this->innovation_nb, this->enabled);
}
