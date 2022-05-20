import Network from "./network";
import { Genome } from "./types";

export function testIndividual(
  agent: Network,
  inputs: number[],
  evaluate: (outputs: number[]) => number
): number {
  return evaluate(agent.compute(inputs));
}

export function testPopulation(
  genomes: [Genome, ...any][],
  input: number[],
  max: [number, number]
): number[] {
  function evaluate(outputs: number[]): number {
    let d = {
      x: input[3] + outputs[2] - outputs[3],
      y: input[2] + outputs[0] - outputs[1],
    };
    return (4 - Math.abs(max[1] - d.x + max[0] - d.y)) / 4;
  }
  const results: number[] = [];
  for (const element of genomes) {
    results.push(testIndividual(new Network(element[0]), input, evaluate));
  }
  return results;
}
