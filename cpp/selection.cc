#include <vector>
#include <algorithm>
#include "crossover.cc"

using Population = std::vector<Individual>;

float getDistance(
    Individual A,
    Individual B)
{
    std::vector<float> layers_A = A.genome.genes;
    std::vector<float> layers_B = B.genome.genes;
    long long diversity = 0;
    for (long long i; i < layers_A.size(); i++)
    {
        diversity += 2 - std::abs((layers_A[i], layers_B[i]));
    }
    diversity /= layers_A.size();
    return diversity + std::abs(A.score - B.score);
}

Population sortPopulation(
    const Population population,
    Individual best)
{
    Population withDistance = population;
    for (Individual &agent : withDistance)
    {
        agent.distance = getDistance(agent, best);
    }
    sort(
        withDistance.begin(),
        withDistance.end(),
        [](Individual a, Individual b)
        { return a.distance < b.distance; });
    return withDistance;
}

Population selection(
    const Population population,
    int selection_size,
    float stressAugmenter = 1)
{
    Population pop = population;
    Individual best = Individual(pop.front());
    Individual worst = Individual(pop.back());
    pop.erase(pop.begin());
    pop.pop_back();
    pop = sortPopulation(pop, best);
    pop.resize(selection_size - 2);
    pop.push_back(best);
    pop.push_back(worst);
    for (int i = 0; i < selection_size; i++)
    {
        for (int j = i; j < selection_size; j++)
        {
            Population offspring = uniform_independant_bias_crossover(
                    pop[i],
                    pop[j],
                    (1 - pop[i].score) * stressAugmenter,
                    (1 - pop[j].score) * stressAugmenter);
            std::for_each(
                offspring.begin(),
                offspring.end(),
                [&](Individual o){ pop.push_back(o); }
            );
        }
        pop[i].score = 0;
    }
    return pop;
}
