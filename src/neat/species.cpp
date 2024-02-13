#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "genome.hpp"
#include "species.hpp"

Species::Species(Genome *genome)
{
    champion = genome;
    best_fitness = genome->fitness;
    average_fitness = 0;
    stagnation = 0;
    if (genome != NULL)
    {
        genomes.push_back(genome);
    }
}

Species::~Species()
{
    for (auto &genome : genomes)
    {
        if (genome != NULL)
        {
            delete genome;
        }
    }
}

void Species::add_to_species(Genome *genome)
{
    genomes.push_back(genome);
}

bool Species::same_species(Genome *genome, const NeatConfig &config)
{
    float compatibility_threshold = config.compatibility_threshold;
    float compatibility_disjoint_coefficient = config.compatibility_disjoint_coefficient;
    float compatibility_weight_coefficient = config.compatibility_weight_coefficient;

    int excess_and_disjoint = get_excess_disjoint_genes(genome, champion);
    float avg_weight_diff = average_weight_diff(genome, champion);

    int large_genome_normalizer = genomes.size() - 20;
    if (large_genome_normalizer < 1)
    {
        large_genome_normalizer = 1;
    }

    // Compatibility formula
    float compatibility = (compatibility_disjoint_coefficient * excess_and_disjoint) / large_genome_normalizer +
                          compatibility_weight_coefficient * avg_weight_diff;

    return compatibility_threshold > compatibility;
}

int Species::get_excess_disjoint_genes(Genome *genome1, Genome *genome2)
{
    int matching = 0;
    for (auto &g1 : genome1->genes)
    {
        for (auto &g2 : genome2->genes)
        {
            if (g1->innovation_nb == g2->innovation_nb)
            {
                matching++;
                break;
            }
        }
    }

    return genome1->genes.size() + genome2->genes.size() - 2 * matching;
}

float Species::average_weight_diff(Genome *genome1, Genome *genome2)
{
    if (genome1->genes.empty() || genome2->genes.empty())
    {
        return 0;
    }

    int matching = 0;
    float total_diff = 0;
    for (auto &g1 : genome1->genes)
    {
        for (auto &g2 : genome2->genes)
            if (g1->innovation_nb == g2->innovation_nb)
            {
                matching++;
                total_diff += std::abs(g1->weight - g2->weight);
                break;
            }
    }

    if (matching == 0)
    {
        // Divide by 0 error
        return 100;
    }

    return total_diff / matching;
}

void Species::sort_genomes()
{
    std::sort(genomes.begin(), genomes.end(), [](const Genome *g1, const Genome *g2)
              { return g1->fitness > g2->fitness; });

    if (genomes[0]->fitness > best_fitness)
    {
        stagnation = 0;
        best_fitness = genomes[0]->fitness;
        champion = genomes[0]->clone();
    }
    else
    {
        stagnation++;
    }
}

void Species::set_average_fitness()
{
    float sum = 0;
    for (auto &g : genomes)
    {
        sum += g->fitness;
    }

    average_fitness = sum / genomes.size();
}

Genome *Species::give_me_baby(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history)
{
    Genome *baby;
    if (rand() / static_cast<float>(RAND_MAX) < 0.25)
    {
        baby = select_genome()->clone();
    }
    else
    {
        // 75% of the time do crossover
        Genome *parent1 = select_genome();
        Genome *parent2 = select_genome();

        // The crossover function expects the highest fitness parent to be the object and the lowest as the argument
        if (parent1->fitness < parent2->fitness)
        {
            baby = parent2->crossover(parent1);
        }
        else
        {
            baby = parent1->crossover(parent2);
        }
    }

    baby->mutate(innovation_history);
    return baby;
}

Genome *Species::select_genome()
{
    float fitness_sum = 0;
    for (size_t i = 0; i < genomes.size(); ++i)
    {
        fitness_sum += genomes[i]->fitness;
    }

    float running_sum = 0;
    for (size_t i = 0; i < genomes.size(); ++i)
    {
        running_sum += genomes[i]->fitness;
        if (running_sum > rand() / static_cast<float>(RAND_MAX) * fitness_sum)
        {
            return genomes[i];
        }
    }

    return genomes[0];
}

void Species::kill_genomes(const NeatConfig &config)
{
    int survivals_nb = std::floor(genomes.size() * config.survival_threshold);

    // Ensure minimum species size
    if (survivals_nb < config.min_species_size)
    {
        survivals_nb = config.min_species_size;
    }

    // Ensure no memory issues
    if (survivals_nb < genomes.size())
    {
        genomes.erase(genomes.begin() + survivals_nb, genomes.end());
    }
}

void Species::fitness_sharing()
{
    for (auto &g : genomes)
    {
        g->fitness /= genomes.size();
    }
}

bool Species::is_equal(Species *other)
{
    // Compare the genomes
    for (auto &genome1 : genomes)
    {
        bool found = false;
        for (auto &genome2 : other->genomes)
        {
            if (genome1->is_equal(genome2))
            {
                found = true;
                break;
            }
            if (found == false)
            {
                return false;
            }
        }
    }

    return true;
}

Species *Species::clone()
{
    Species *clone = new Species();
    clone->champion = champion->clone();
    clone->average_fitness = average_fitness;
    clone->best_fitness = best_fitness;
    clone->stagnation = stagnation;
    for (auto &g : genomes)
    {
        clone->add_to_species(g);
    }
    return clone;
}
