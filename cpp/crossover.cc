#include <array>
#include <vector>
#include "network.cc"

struct Individual
{
    Genome genome;
    float score;
    float distance;
    Individual() : genome(Genome()), score(0), distance(0){};
    Individual(Genome g) : genome(g), score(0), distance(0){};
    Individual(Genome g, float s, float d) : genome(g), score(s), distance(d){};
    Individual(std::vector<int> t) : genome(randomGenome(t)), score(0), distance(0){};
    Individual(const Individual& i): genome(i.genome), score(i.score), distance(i.distance){};
};

template <typename T>
std::vector<std::vector<T>> chunks(const std::vector<T> table, long long size)
{
    std::vector<T> tmp = table;
    std::vector<std::vector<T>> chunks = {{}};
    while(tmp.size()) {
        if(chunks.back().size() < size){
            chunks.back().push_back(tmp[0]);
        }else {
            chunks.push_back({ tmp[0] });
        }
        tmp.erase(tmp.begin());
    }
    return chunks;
}

std::vector<Individual> uniform_independant_bias_crossover(
    Individual A,
    Individual B,
    float stress_A,
    float stress_B,
    long long chunk_size = 1)
{
    float half = stress_A / (stress_A + stress_B);
    std::vector<std::vector<float>> gene_a = chunks(mutation(A.genome, stress_A).genes, chunk_size);
    std::vector<std::vector<float>> gene_b = chunks(mutation(B.genome, stress_B).genes, chunk_size);
    std::vector<float> new_gene_a;
    std::vector<float> new_gene_b;
    for (long long i = 0; i < gene_a.size(); i++)
    {
        if (((float)rand() / RAND_MAX) < half)
        {
            std::for_each(
                gene_a[i].begin(),
                gene_a[i].end(),
                [&](float g)
                { new_gene_a.push_back(g); });
        }
        else
            std::for_each(
                gene_b[i].begin(),
                gene_b[i].end(),
                [&](float g)
                { new_gene_a.push_back(g); });
        if (((float)rand() / RAND_MAX) < half)
        {
            std::for_each(
                gene_b[i].begin(),
                gene_b[i].end(),
                [&](float g)
                { new_gene_b.push_back(g); });
        }
        else
            std::for_each(
                gene_a[i].begin(),
                gene_a[i].end(),
                [&](float g)
                { new_gene_b.push_back(g); });
    }
    return {
        Individual(Genome(A.genome.topology, new_gene_a)),
        Individual(Genome(B.genome.topology, new_gene_b))};
}
