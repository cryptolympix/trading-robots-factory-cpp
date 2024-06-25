#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <thread>
#include "config.hpp"
#include "genome.hpp"
#include "connection_history.hpp"
#include "species.hpp"
#include "population.hpp"

neat::Population::Population(const Config &config) : config(config)
{
    for (int i = 0; i < config.population_size; ++i)
    {
        Genome *genome = new Genome(config);
        genome->mutate(innovation_history);
        genome->generate_network();

        if (config.initial_connections == "full")
            genome->fully_connect(innovation_history);
        genomes.push_back(genome);
    }
}

neat::Population::~Population()
{
    // Delete all the dynamically allocated species
    for (auto sp : species)
        delete sp;

    // Delete all the dynamically allocated genomes
    for (auto g : genomes)
        delete g;

    // Delete the best genome
    delete best_genome;

    // Clear the vectors
    species.clear();
    genomes.clear();
}

void neat::Population::set_best_genome()
{
    Genome *temp_best = species[0]->genomes[0];
    if (temp_best->fitness >= best_fitness)
    {
        best_genome = temp_best;
        best_fitness = temp_best->fitness;
    }
}

void neat::Population::run(std::function<void(Genome *, int)> evaluate_genome, int nb_generations, std::function<void(Population *, int)> callback_generation)
{
    for (int i = 0; i < nb_generations; ++i)
    {
        // Use threads to evaluate genomes in parallel
        std::vector<std::thread> threads;
        for (size_t j = 0; j < genomes.size(); ++j)
            threads.emplace_back([&, j]()
                                 { evaluate_genome(genomes[j], i); });

        // Wait for all threads to finish
        for (auto &thread : threads)
            thread.join();

        speciate();
        sort_species();
        update_species();
        set_best_genome();
        set_average_fitness();
        kill_stagnant_species();
        kill_bad_species();
        reproduce_species();
        reset_on_extinction();

        // Execute the callback function after each generation and wait for it to finish
        if (callback_generation)
            callback_generation(this, i);

        // Stop the algorithm if the fitness threshold is reached
        if (!config.no_fitness_termination && best_genome->fitness > config.fitness_threshold)
            break;

        // Stop the algorithm if the best fitness is infinity
        if (best_fitness == std::numeric_limits<double>::infinity())
            break;
    }
}

void neat::Population::speciate()
{
    // Reset the genomes in each species
    for (auto &s : species)
        s->genomes.clear();

    // Group the genomes by species
    for (auto &g : genomes)
    {
        bool species_found = false;

        for (auto &s : species)
        {
            if (s->same_species(g, config))
            {
                s->add_to_species(g);
                species_found = true;
                break;
            }
        }
        if (!species_found)
        {
            Species *new_species = new Species(g);
            species.push_back(new_species);
        }
    }

    // Remove the empty species
    auto it = std::remove_if(species.begin(), species.end(), [](const Species *s)
                             { return s->genomes.empty(); });
    species.erase(it, species.end());
}

void neat::Population::reproduce_species()
{
    // Calculate the sum of average fitness for all species
    float average_fitness_sum = get_average_fitness_sum();

    // Get the target population size from the configuration
    int population_size = config.population_size;

    // Create a vector to store the new generation of genomes
    std::vector<Genome *> children;

    for (auto &s : species)
    {
        // Clone the champion of each species and add it to the new generation
        children.push_back(s->champion->clone());

        // Calculate the number of children based on the species' contribution to the total average fitness
        int nb_of_children = (average_fitness_sum == 0) ? 0 : std::floor((s->average_fitness / average_fitness_sum) * population_size) - 1;

        // Generate babies and add them to the new generation
        for (int i = 0; i < nb_of_children; ++i)
            children.push_back(s->give_me_baby(innovation_history));
    }

    // Clone the best genome from the previous generation and add it to the new generation
    Genome *previous_best = genomes[0];
    if (children.size() < static_cast<size_t>(population_size))
        children.push_back(previous_best->clone());

    // If there are still not enough babies, get babies from the best species until reaching the target population size
    while (children.size() < static_cast<size_t>(population_size))
        children.push_back(species[0]->give_me_baby(innovation_history));

    // Update the population's genomes with the new generation
    genomes = children;

    // Increment the generation counter
    ++generation;

    // Generate neural networks for each genome in the new generation
    for (auto &g : genomes)
        g->generate_network();

    // Set the best genome in the population
    set_best_genome();
}

void neat::Population::sort_species()
{
    for (auto &s : species)
        s->sort_genomes();

    std::sort(species.begin(), species.end(), [](const Species *s1, const Species *s2)
              { return s1->best_fitness > s2->best_fitness; });
}

void neat::Population::kill_stagnant_species()
{
    // Use remove_if along with a lambda function to filter out stagnant species
    auto it = std::remove_if(config.species_elitism < species.size() ? species.begin() + config.species_elitism : species.begin(), species.end(), [this](const Species *s)
                             { if (s->stagnation >= config.max_stagnation)
                                 {
                                     // Remove genomes of the stagnant species
                                     genomes.erase(std::remove_if(genomes.begin(), genomes.end(), [s](const Genome *g)
                                                                  { return std::find(s->genomes.begin(), s->genomes.end(), g) != s->genomes.end(); }),
                                                   genomes.end());
                                     return true; // Remove the stagnant species
                                 }
                                 return false; });

    // Erase the removed elements from the vector
    species.erase(it, species.end());
}

void neat::Population::kill_bad_species()
{
    float species_average_fitness = get_average_fitness_sum() / species.size();

    // Use remove_if along with a lambda function to filter out bad species
    auto it = std::remove_if(species.size() > 1 ? species.begin() + 1 : species.begin(), species.end(), [this, species_average_fitness](const Species *s)
                             { 
                                 if (s->average_fitness < species_average_fitness * config.bad_species_threshold)
                                 {
                                     // Remove genomes of the bad species from the genomes vector
                                     genomes.erase(std::remove_if(genomes.begin(), genomes.end(), [s](const Genome *g)
                                                                 { return std::find(s->genomes.begin(), s->genomes.end(), g) != s->genomes.end(); }), genomes.end());

                                     return true; // Remove the bad species
                                 }
                                 return false; });

    // Erase the removed elements from the vector
    species.erase(it, species.end());
}

void neat::Population::reset_on_extinction()
{
    if (species.empty())
    {
        genomes.clear();

        // Reinitialize with new genomes
        for (int i = 0; i < config.population_size; ++i)
            genomes.push_back(new Genome(config));
    }
}

double neat::Population::get_average_fitness_sum()
{
    float average_sum = 0;
    for (const auto &s : species)
        average_sum += s->average_fitness;
    return average_sum;
}

void neat::Population::set_average_fitness()
{
    average_fitness = get_average_fitness_sum() / species.size();
}

void neat::Population::update_species()
{
    for (auto &s : species)
    {
        s->kill_genomes(config);
        s->fitness_sharing();
        s->set_average_fitness();
    }
}

neat::Population *neat::Population::clone()
{
    Population *clone = new Population(config);
    clone->genomes = {};
    clone->species = {};

    // Clone the species
    for (auto &s : species)
        clone->species.push_back(s->clone());

    // Add the genomes of cloned species to the population
    for (auto &s : species)
        for (auto &g : s->genomes)
            clone->genomes.push_back(g);

    clone->generation = generation;
    clone->average_fitness = average_fitness;
    clone->best_fitness = best_fitness;
    clone->innovation_history = innovation_history;

    if (best_genome != NULL)
        clone->best_genome = best_genome->clone();

    return clone;
}
