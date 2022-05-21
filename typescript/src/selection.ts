import { uniform_crossover, uniform_independant_bias_crossover } from "./crossover";
import { Fitness, Genome, SelectionItem } from "./types";

export function getDistance(
  A: Genome,
  fitness_A: Fitness,
  B: Genome,
  fitness_B: Fitness
): number {
  const layers_A = A[1];
  const layers_B = B[1];
  console.assert(layers_A instanceof Array, 'layers_a not an array', layers_A)
  return layers_A.reduce((acc, cur, index) => {
    return acc + 2 - Math.abs(cur - layers_B[index]);
  }, 0) / layers_A.length + Math.abs(fitness_A - fitness_B) ;
}
export function sortPopulation(
  population: SelectionItem[],
  best: SelectionItem
): SelectionItem[] {
  const withDistance = population.map((a) => [
    ...a,
    getDistance(...a, ...best),
  ]) as [Genome, Fitness, number][];
  withDistance.sort((a, b) => a[2] - b[2]);
  return withDistance.map((a) => a.slice(0, 2)) as SelectionItem[];
}

export function selection(
  population: SelectionItem[],
  selection_size: number,
  stressAugmenter: number = 1
) {
  let pop = population;
  const best = pop[0];
  const worst = pop.at(-1)!
  pop = sortPopulation(pop.slice(1,-1), best);
  pop = [best, ...pop.slice(0, selection_size - 2), worst];
  if(best[1] !== pop[0][1]) throw new Error('sorting removed best')
  for (let i = 0; i < selection_size; i++) {
    for (let j = i; j < selection_size; j++) {
      pop.push(
        ...uniform_independant_bias_crossover(
          pop[i],
          pop[j],
          (1 - pop[i][1]) * stressAugmenter,
          (1 - pop[j][1]) * stressAugmenter
        )
      );
    }
    pop[i][1] = 0;
  }
  return pop;
}

export default selection;
