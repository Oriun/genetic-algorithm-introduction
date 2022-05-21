
#include <iostream>
#include <ostream>
#include <vector>
#include <array>
#include <string>
#include <ctime>
#include <sstream>
#include <filesystem>
#include "selection.cc"
#include "train.cc"

std::array<std::string, 2> serialize(Individual agent)
{
    return {encodeGenome(agent.genome), std::to_string(agent.score)};
}
using TrainSet = std::vector<TrainData>;
using Population = std::vector<Individual>;

float evaluate(TrainData data, std::vector<float> outputs)
{
    float dx = data.input[3] + outputs[2] - outputs[3];
    float dy = data.input[2] + outputs[0] - outputs[1];
    return (4 - std::abs(data.max[1] - dx + data.max[0] - dy)) / 4;
};
void save(std::string launchTime, int generation, float bestScore, Population bests, Population population)
{
    // await writeFile(
    //         `data / ${launchTime} / results - gen${generation}.json`,
    //             JSON.stringify({
    //                 generation,
    //                 maxFitness : best[1],
    //                 bests : bests.map(ac),
    //                 population : population.map(ac),
    //             }));
}

int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));

    /**
     * To keep the same size use:
     *  population_size = selection_size² + 2 * selection_size
     **/
    int selection_size(25);
    int population_size(675);
    int max_generation(100);
    std::vector<int> topology = {4, 8, 16, 8, 4};
    std::string directory_name("data/");
    std::string launchTime = std::to_string(time(nullptr));
#ifndef __INTELLISENSE__
    try
    {
        std::filesystem::create_directories((directory_name).c_str());
    }
    catch (std::exception &error)
    {
        std::cerr << error.what();
    }
    std::filesystem::create_directories((directory_name + launchTime).c_str());
#endif
    Population population;
    for (int i(0); i < population_size; i++)
    {
        population.push_back(Individual(topology));
    }
    std::cout << "Brain size " << lengthFromTopology(topology) << '\n';

    
    Population bests = {};
    for (int generation = 0; generation < max_generation; generation++)
    {
        for (int j = 0; j < population.size(); j++)
        {
            for (int i = 0; i < train.size(); i++)
            {
                population[j].score += evaluate(train[i], Network(population[j].genome).compute(train[i].input));
            }
            population[j].score /= train.size();
        }

        std::sort(
            population.begin(),
            population.end(),
            [](Individual a, Individual b)
            {
                return a.score > b.score;
            });
        Individual best = Individual(population.front());
        if (bests.size() && best.score != bests.back().score)
        {
            std::cout << "\nBest " << best.score << "\n";
        }
        bests.push_back(best);
        if (generation % 100 == 0 || best.score > 0.9999F)
        {
            save(launchTime, generation, best.score, bests, population);
        }
        if (best.score > 0.9999)
        {
            std::cout << "\rGeneration n° " << generation;
            max_generation = generation;
            break;
        }
        if (generation + 1 < max_generation)
        {
            population = selection(population, selection_size, 2.0F);
            std::cout << "\rGeneration n° " << generation;
        }
        std::cout << std::flush;
    }

    std::cout << "\n**************\n";
    std::cout << "Generation : " << max_generation << "\n";
    std::cout << "Population : " << population.size() << "\n";
    std::cout << "Best : " << bests.back().score << "\n";
    std::cout << "Top 5 : " << population[5].score << "\n";

    std::cout << "TEST" << std::endl;
    std::cout << evaluate(
        TrainData({2, -0.5, 1, 1}, {1, -1}),
        Network(bests.back().genome).compute({2, -0.5, 1, 1})) << std::endl;
    std::cout << evaluate(
        TrainData({-2, -2, 0, 0}, {-1, -1}),
        Network(bests.back().genome).compute({-2, -2, 0, 0})) << std::endl;

    return 0;
}
