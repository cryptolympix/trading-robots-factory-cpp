#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "genome.hpp"
#include "species.hpp"

neat::Species::Species()
{
    this->champion = nullptr;
    this->best_fitness = 0;
    this->average_fitness = 0;
    this->stagnation = 0;
}

neat::Species::Species(Genome *genome)
{
    if (genome == nullptr)
    {
        throw std::invalid_argument("Creating a species with a null genome.");
    }

    this->champion = genome;
    this->best_fitness = genome->fitness;
    this->average_fitness = 0;
    this->stagnation = 0;
    this->genomes.push_back(genome);
}

neat::Species::~Species()
{
    for (auto &genome : this->genomes)
    {
        delete genome;
    }
}

void neat::Species::add_to_species(Genome *genome)
{
    this->genomes.push_back(genome);
}

bool neat::Species::same_species(Genome *genome, const Config &config)
{
    float compatibility_threshold = config.compatibility_threshold;
    float compatibility_disjoint_coefficient = config.compatibility_disjoint_coefficient;
    float compatibility_weight_coefficient = config.compatibility_weight_coefficient;

    int excess_and_disjoint = get_excess_disjoint_genes(genome, this->champion);
    float avg_weight_diff = average_weight_diff(genome, this->champion);

    int large_genome_normalizer = this->genomes.size() - 20;
    if (large_genome_normalizer < 1)
    {
        large_genome_normalizer = 1;
    }

    // Compatibility formula
    float compatibility = (compatibility_disjoint_coefficient * excess_and_disjoint) / large_genome_normalizer +
                          compatibility_weight_coefficient * avg_weight_diff;

    return compatibility_threshold > compatibility;
}

int neat::Species::get_excess_disjoint_genes(Genome *genome1, Genome *genome2)
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

float neat::Species::average_weight_diff(Genome *genome1, Genome *genome2)
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
        {
            if (g1->innovation_nb == g2->innovation_nb)
            {
                matching++;
                total_diff += std::abs(g1->weight - g2->weight);
                break;
            }
        }
    }

    if (matching == 0)
    {
        // Divide by 0 error
        return 100;
    }

    return total_diff / matching;
}

void neat::Species::sort_genomes()
{
    std::sort(this->genomes.begin(), this->genomes.end(), [](const Genome *g1, const Genome *g2)
              { return g1->fitness > g2->fitness; });

    if (this->genomes[0]->fitness > this->best_fitness)
    {
        this->stagnation = 0;
        this->best_fitness = genomes[0]->fitness;
        this->champion = genomes[0]->clone();
    }
    else
    {
        this->stagnation++;
    }
}

void neat::Species::set_average_fitness()
{
    float sum = 0;
    for (auto &g : this->genomes)
    {
        sum += g->fitness;
    }
    average_fitness = sum / this->genomes.size();
}

neat::Genome *neat::Species::give_me_baby(std::vector<std::shared_ptr<ConnectionHistory>> innovation_history)
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

neat::Genome *neat::Species::select_genome()
{
    float fitness_sum = 0;
    for (size_t i = 0; i < this->genomes.size(); ++i)
    {
        fitness_sum += this->genomes[i]->fitness;
    }

    float running_sum = 0;
    for (size_t i = 0; i < this->genomes.size(); ++i)
    {
        running_sum += this->genomes[i]->fitness;
        if (running_sum > rand() / static_cast<float>(RAND_MAX) * fitness_sum)
        {
            return this->genomes[i];
        }
    }

    return this->genomes[0];
}

void neat::Species::kill_genomes(const Config &config)
{
    int survivals_nb = std::floor(this->genomes.size() * config.survival_threshold);

    // Ensure minimum species size
    if (survivals_nb < config.min_species_size)
    {
        survivals_nb = config.min_species_size;
    }

    // Ensure no memory issues
    if (survivals_nb < this->genomes.size())
    {
        this->genomes.erase(this->genomes.begin() + survivals_nb, this->genomes.end());
    }
}

void neat::Species::fitness_sharing()
{
    for (auto &g : this->genomes)
    {
        g->fitness /= this->genomes.size();
    }
}

bool neat::Species::is_equal(Species *other)
{
    // Compare the genomes
    for (auto &genome1 : this->genomes)
    {
        bool found = false;
        for (auto &genome2 : other->genomes)
        {
            if (genome1->is_equal(genome2))
            {
                found = true;
                break;
            }
            if (!found)
            {
                return false;
            }
        }
    }

    return true;
}

neat::Species *neat::Species::clone()
{
    Species *clone = new Species(this->champion->clone());
    clone->average_fitness = this->average_fitness;
    clone->best_fitness = this->best_fitness;
    clone->stagnation = this->stagnation;
    for (auto &g : this->genomes)
    {
        clone->add_to_species(g);
    }
    return clone;
}

nlohmann::json neat::Species::to_json() const
{
    nlohmann::json species_json;
    species_json["best_fitness"] = this->best_fitness;
    species_json["average_fitness"] = this->average_fitness;
    species_json["stagnation"] = this->stagnation;
    species_json["champion"] = this->champion->to_json();

    nlohmann::json genomes_json;
    for (const auto &genome : this->genomes)
    {
        nlohmann::json genome_json = genome->to_json();
        genomes_json.push_back(genome_json);
    }
    species_json["genomes"] = genomes_json;

    return species_json;
}

neat::Species *neat::Species::from_json(const nlohmann::json &json)
{
    Species *species = new Species();
    species->best_fitness = json["best_fitness"];
    species->average_fitness = json["average_fitness"];
    species->stagnation = json["stagnation"];
    species->champion = Genome::from_json(json["champion"]);

    for (const auto &genome_json : json["genomes"])
    {
        Genome *genome = Genome::from_json(genome_json);
        species->genomes.push_back(genome);
    }

    return species;
}
void neat::Species::save(const std::string &file_path)
{
    try
    {
        // Check if the directory exists, create it if not
        std::filesystem::path dir = std::filesystem::path(file_path).parent_path();
        if (!std::filesystem::exists(dir))
        {
            std::filesystem::create_directories(dir);
        }

        // If the file has no extension, append ".json"
        std::filesystem::path path = file_path;
        if (path.extension().empty())
        {
            path += ".json";
        }

        // Create a JSON object to hold the species data
        nlohmann::json species_json = this->to_json();

        // Write JSON to file
        std::ofstream file(file_path);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file for writing: " + file_path);
        }
        file << species_json.dump();
        file.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to save species: " << e.what() << std::endl;
    }
}

neat::Species *neat::Species::load(const std::string &file_path)
{
    try
    {
        // Read JSON from file
        std::ifstream file(file_path);
        nlohmann::json species_json;
        file >> species_json;

        // Create a new Species object
        Species *loadedSpecies = neat::Species::from_json(species_json);
        return loadedSpecies;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to load species: " << e.what() << std::endl;
        return nullptr;
    }
}
