import data from "./train.json";
import { Genome, TrainData } from "./types";
import selection from "./selection";
import { testPopulation } from "./test";
import { lengthFromTopology } from "./layer";
import { writeFile, mkdir } from "fs/promises";
import { encodeGenome, randomGenome } from "./genome";

async function main(argc: number, argv: string[]) {
  /**
   * To keep the same size use:
   *  population_size = selection_size² + 2 * selection_size
   **/
  const selection_size = 25;
  const population_size = 675;
  const max_generation = 10_000;
  const nbOfTests = 20;
  const ac = (a: [Genome, number]) => [encodeGenome(a[0]), a[1]];
  const launchTime = Date.now();
  await mkdir(`data/${launchTime}`);
  let population: [Genome, number][] = Array.from(
    { length: population_size },
    () => [randomGenome([4, 8, 16, 8, 4]), 0]
  );
  console.log("Brain size", lengthFromTopology(population[0][0][0]));

  const trainData = data.slice(0, nbOfTests) as TrainData;
  const bests: [Genome, number][] = [];
  for (let generation = 0; generation < max_generation; generation++) {
    const res = trainData.map(({ input, max }) =>
      testPopulation(population, input, max)
    );
    res.forEach((a) => {
      a.forEach((v, i) => {
        population[i][1] += v;
      });
    });
    population = population.map((a) => [a[0], a[1] / trainData.length]);
    population.sort((a, b) => b[1] - a[1]);
    const best = population[0];

    if (best[1] !== bests.at(-1)?.[1]) {
      process.stdout.write("\nBest " + best[1].toFixed(10) + "\n");
      bests.push([...best]);
    }
    if (!(generation % 100))
      await writeFile(
        `data/${launchTime}/results-gen${generation}.json`,
        JSON.stringify({
          generation,
          maxFitness: best[1],
          bests: bests.map(ac),
          population: population.map(ac),
        })
      );
    if (best[1] > 0.9999) {
      process.stdout.write("\rGeneration n°" + generation);
      break;
    }
    if (generation + 1 < max_generation)
      population = selection(population, selection_size * 2);
    process.stdout.write("\rGeneration n°" + generation);
  }

  process.stdout.write(`
  **************
  Generation : ${max_generation}
  Population : ${population.length}
  Best : ${population[0][1]}
  Worst : ${population.at(-1)![1]}
  Average : ${population.reduce((a, b) => a + b[1], 0) / population.length}
  Top 5 : ${population[5][1]}
  Top -5 : ${population.at(-5)![1]}
  **************\n`);

  await writeFile(
    `data/${launchTime}/results-gen${max_generation}.json`,
    JSON.stringify({
      generation: max_generation,
      maxFitness: bests.at(-1)![1],
      bests: bests.map(ac),
      population: population.map(ac),
    })
  );

  console.log("TEST");
  console.log(testPopulation([bests.at(-1)!], [2, -0.5, 1, 1], [1, -1])[0]);
  console.log(testPopulation([bests.at(-1)!], [-2, -2, 0, 0], [-1, -1])[0]);
}

main(process.argv.length - 2, process.argv.slice(2));
