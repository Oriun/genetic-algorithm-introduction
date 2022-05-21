#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "layer.cc"

struct Genome
{
    std::vector<int> topology;
    std::vector<float> genes;
    Genome(std::vector<int> t, std::vector<float> g) : topology(t), genes(g){};
    Genome() : topology({}), genes({}){};
};

std::string encodeGenome(Genome genome)
{
    std::stringstream stringified;
    std::for_each(
        genome.topology.begin(),
        genome.topology.end(),
        [&](const int &n)
        { stringified << n << ';'; });
    stringified << '|';
    std::for_each(
        genome.genes.begin(),
        genome.genes.end(),
        [&](const float &n)
        { stringified << n << ';'; });
    std::string res = stringified.str();
    res.pop_back();
    return res;
}

Genome parseGenome(const std::string &genomeString)
{
    std::vector<int> topology;
    std::vector<float> genes;
    auto half = genomeString.find("|");
    std::string a = genomeString.substr(0, half);
    std::string b = genomeString.substr(half);

    size_t pos = 0;
    while ((pos = a.find(";")) != std::string::npos)
    {
        topology.push_back(std::stoi(a.substr(0, pos)));
        a.erase(0, pos + 1);
    }
    topology.push_back(std::stoi(a));
    pos = 0;
    while ((pos = b.find(";")) != std::string::npos)
    {
        genes.push_back(std::stof(b.substr(0, pos)));
        b.erase(0, pos + 1);
    }
    genes.push_back(std::stoi(b));

    return Genome(topology, genes);
}

Genome randomGenome(std::vector<int> topology)
{
    std::vector<float> genes;
    long long length = lengthFromTopology(topology);
    for(long long i = 0; i < length; i++) {
        genes.push_back(2 * (float) rand()/RAND_MAX - 1);
    }
    return Genome(topology, genes);
}

Genome mutation(Genome genome, float mutationFactor)
{
    std::vector<float> genes = {};
    for(const float g: genome.genes) {
        float r = rand() / RAND_MAX;
        bool does_mutate = r < std::max((float) .1, mutationFactor);
        if(!does_mutate) {
            genes.push_back(g);
        }else {
            genes.push_back(2 * (float) rand()/RAND_MAX - 1);
        }
    }
    return Genome(genome.topology, genes);
}
