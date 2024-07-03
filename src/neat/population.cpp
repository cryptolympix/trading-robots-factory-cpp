#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <thread>
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "config.hpp"
#include "genome.hpp"
#include "connection_history.hpp"
#include "species.hpp"
#include "population.hpp"

neat::Population::Population(const Config &config, bool init_genomes) : config(config)
{
    this->generation = 0;
    this->best_fitness = 0;
    this->average_fitness = 0;
    this->best_genome = nullptr;
    this->innovation_history = {};
    this->species = {};
    this->genomes = {};

    if (init_genomes)
    {
        for (int i = 0; i < this->config.population_size; ++i)
        {
            Genome *genome = new Genome(config);
            genome->mutate(this->innovation_history);
            genome->generate_network();

            if (this->config.initial_connections == "full")
            {
                genome->fully_connect(innovation_history);
            }
            this->genomes.push_back(genome);
        }
    }
}

neat::Population::~Population()
{
    // Delete all the dynamically allocated species
    for (auto sp : this->species)
    {
        delete sp;
    }

    // Clear the vectors
    this->species.clear();
    this->genomes.clear();
}

void neat::Population::set_best_genome()
{
    if (!this->species.empty())
    {
        Genome *temp_best = this->species[0]->genomes[0];
        if (temp_best->fitness >= this->best_fitness)
        {
            this->best_genome = temp_best;
            this->best_fitness = temp_best->fitness;
        }
    }
}

void neat::Population::run(std::function<void(Genome *, int)> evaluate_genome, int nb_generations, std::function<void(Population *, int)> callback_generation)
{
    for (int i = 0; i < nb_generations; ++i)
    {
        // Use threads to evaluate genomes in parallel
        std::vector<std::thread> threads;
        for (size_t j = 0; j < this->genomes.size(); ++j)
            threads.emplace_back([&, j]()
                                 { evaluate_genome(this->genomes[j], i); });

        // Wait for all threads to finish
        for (auto &thread : threads)
        {
            thread.join();
        }

        this->speciate();
        this->sort_species();
        this->update_species();
        this->set_best_genome();
        this->set_average_fitness();
        this->kill_stagnant_species();
        this->kill_bad_species();
        this->reproduce_species();
        this->reset_on_extinction();

        // Callback function for each generation
        if (callback_generation)
        {
            callback_generation(this, i);
        }

        // Stop the algorithm if the fitness threshold is reached
        if (!this->config.no_fitness_termination && this->best_genome->fitness > this->config.fitness_threshold)
        {
            break;
        }

        // Stop the algorithm if the best fitness is infinity
        if (this->best_fitness == std::numeric_limits<double>::infinity())
        {
            break;
        }
    }
}

void neat::Population::speciate()
{
    // Reset the genomes in each species
    for (auto &s : this->species)
    {
        s->genomes.clear();
    }

    // Group the genomes by species
    for (auto &g : this->genomes)
    {
        bool species_found = false;

        for (auto &s : this->species)
        {
            if (s->same_species(g, this->config))
            {
                s->add_to_species(g);
                species_found = true;
                break;
            }
        }
        if (!species_found)
        {
            Species *new_species = new Species(g);
            this->species.push_back(new_species);
        }
    }

    // Remove the empty species
    auto it = std::remove_if(this->species.begin(), this->species.end(), [](const Species *s)
                             { return s->genomes.empty(); });
    this->species.erase(it, this->species.end());
}

void neat::Population::reproduce_species()
{
    // Calculate the sum of average fitness for all species
    float average_fitness_sum = get_average_fitness_sum();

    // Get the target population size from the configuration
    int population_size = this->config.population_size;

    // Create a vector to store the new generation of genomes
    std::vector<Genome *> children;

    for (auto &s : this->species)
    {
        // Clone the champion of each species and add it to the new generation
        children.push_back(s->champion->clone());

        // Calculate the number of children based on the species' contribution to the total average fitness
        int nb_of_children = (average_fitness_sum == 0) ? 0 : std::floor((s->average_fitness / average_fitness_sum) * population_size) - 1;

        // Generate babies and add them to the new generation
        for (int i = 0; i < nb_of_children; ++i)
        {
            children.push_back(s->give_me_baby(this->innovation_history));
        }
    }

    // Clone the best genome from the previous generation and add it to the new generation
    Genome *previous_best = genomes[0];
    if (children.size() < static_cast<size_t>(population_size))
    {
        children.push_back(previous_best->clone());
    }

    // If there are still not enough babies, get babies from the best species until reaching the target population size
    while (children.size() < static_cast<size_t>(population_size))
    {
        children.push_back(species[0]->give_me_baby(this->innovation_history));
    }

    // Update the population's genomes with the new generation
    this->genomes = children;

    // Increment the generation counter
    ++this->generation;

    // Generate neural networks for each genome in the new generation
    for (auto &g : this->genomes)
    {
        g->generate_network();
    }

    // Set the best genome in the population
    this->set_best_genome();
}

void neat::Population::sort_species()
{
    for (auto &s : this->species)
    {
        s->sort_genomes();
    }

    std::sort(this->species.begin(), this->species.end(), [](const Species *s1, const Species *s2)
              { return s1->best_fitness > s2->best_fitness; });
}

void neat::Population::kill_stagnant_species()
{
    // Use remove_if along with a lambda function to filter out stagnant species
    auto it = std::remove_if(this->config.species_elitism < species.size() ? this->species.begin() + this->config.species_elitism : this->species.begin(), this->species.end(), [this](const Species *s)
                             { if (s->stagnation >= this->config.max_stagnation)
                                 {
                                     // Remove genomes of the stagnant species
                                     this->genomes.erase(std::remove_if(this->genomes.begin(), this->genomes.end(), [s](const Genome *g)
                                                                  { return std::find(s->genomes.begin(), s->genomes.end(), g) != s->genomes.end(); }),
                                                   genomes.end());
                                     return true; // Remove the stagnant species
                                 }
                                 return false; });

    // Erase the removed elements from the vector
    this->species.erase(it, this->species.end());
}

void neat::Population::kill_bad_species()
{
    float species_average_fitness = this->get_average_fitness_sum() / this->species.size();

    // Use remove_if along with a lambda function to filter out bad species
    auto it = std::remove_if(this->species.size() > 1 ? this->species.begin() + 1 : this->species.begin(), this->species.end(), [this, species_average_fitness](const Species *s)
                             { 
                                 if (s->average_fitness < species_average_fitness * this->config.bad_species_threshold)
                                 {
                                     // Remove genomes of the bad species from the genomes vector
                                     this->genomes.erase(std::remove_if(this->genomes.begin(), this->genomes.end(), [s](const Genome *g)
                                                                 { return std::find(s->genomes.begin(), s->genomes.end(), g) != s->genomes.end(); }), this->genomes.end());

                                     return true; // Remove the bad species
                                 }
                                 return false; });

    // Erase the removed elements from the vector
    this->species.erase(it, this->species.end());
}

void neat::Population::reset_on_extinction()
{
    if (this->species.empty())
    {
        this->genomes.clear();

        // Reinitialize with new genomes
        for (int i = 0; i < this->config.population_size; ++i)
        {
            this->genomes.push_back(new Genome(this->config));
        }
    }
}

double neat::Population::get_average_fitness_sum()
{
    float average_sum = 0;
    for (const auto &s : this->species)
    {
        average_sum += s->average_fitness;
    }
    return average_sum;
}

void neat::Population::set_average_fitness()
{
    this->average_fitness = this->get_average_fitness_sum() / this->species.size();
}

void neat::Population::update_species()
{
    for (auto &s : this->species)
    {
        s->kill_genomes(this->config);
        s->fitness_sharing();
        s->set_average_fitness();
    }
}

neat::Population *neat::Population::clone()
{
    Population *clone = new Population(this->config);
    clone->genomes = {};
    clone->species = {};

    // Clone the species
    for (auto &s : this->species)
    {
        clone->species.push_back(s->clone());
    }

    // Add the genomes of cloned species to the population
    for (auto &s : this->species)
    {
        for (auto &g : s->genomes)
            clone->genomes.push_back(g);
    }

    clone->generation = this->generation;
    clone->average_fitness = this->average_fitness;
    clone->best_fitness = this->best_fitness;
    clone->innovation_history = this->innovation_history;

    if (this->best_genome != nullptr)
    {
        clone->best_genome = this->best_genome->clone();
    }

    return clone;
}

void neat::Population::save(const std::string &filename)
{
    try
    {
        // Check if the directory exists, create it if not
        std::filesystem::path dir = std::filesystem::path(filename).parent_path();
        if (!std::filesystem::exists(dir))
        {
            std::filesystem::create_directories(dir);
        }

        // Check if the filename is a directory
        if (std::filesystem::is_directory(filename))
        {
            throw std::runtime_error("Invalid filename: " + filename);
        }

        // If the file has no extension, append ".json"
        std::filesystem::path path = filename;
        if (path.extension().empty())
        {
            path += ".json";
        }

        nlohmann::json json;
        json["generation"] = this->generation;
        json["average_fitness"] = this->average_fitness;
        json["best_fitness"] = this->best_fitness;

        // Save the best genome
        if (this->best_genome != nullptr)
        {
            json["best_genome"] = this->best_genome->to_json();
        }

        // Save the species
        json["species"] = nlohmann::json::array();
        for (auto &s : this->species)
        {
            json["species"].push_back(s->to_json());
        }

        // Save the genomes
        json["genomes"] = nlohmann::json::array();
        for (auto &g : this->genomes)
        {
            json["genomes"].push_back(g->to_json());
        }

        std::ofstream file(filename);
        file << json.dump(4);
        file.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to save population: " << e.what() << std::endl;
    }
}

neat::Population *neat::Population::load(const std::string &filename, const Config &config)
{
    try
    {
        // Read JSON from file
        std::ifstream file(filename);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }

        nlohmann::json population_json;
        file >> population_json;

        // Create a new Population object
        Population *loadedPopulation = new Population(config, false);

        // Deserialize simple data members
        loadedPopulation->best_fitness = population_json["best_fitness"];
        loadedPopulation->average_fitness = population_json["average_fitness"];
        loadedPopulation->generation = population_json["generation"];

        if (population_json.contains("best_genome"))
        {
            // Deserialize the champion genome
            nlohmann::json best_genome_json = population_json["best_genome"];

            // Create a new Genome object from the JSON data
            Genome *champion = Genome::from_json(best_genome_json);

            // Set the best genome
            loadedPopulation->best_genome = champion;
        }

        // Deserialize the species
        nlohmann::json species_json = population_json["species"];
        for (const auto &s : species_json)
        {
            Species *species = Species::from_json(s);
            loadedPopulation->species.push_back(species);
        }

        // Deserialize the genomes
        nlohmann::json genomes_json = population_json["genomes"];
        for (const auto &g : genomes_json)
        {
            Genome *genome = Genome::from_json(g);
            loadedPopulation->genomes.push_back(genome);
        }

        return loadedPopulation;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to load population: " << e.what() << std::endl;
        return nullptr;
    }
}
