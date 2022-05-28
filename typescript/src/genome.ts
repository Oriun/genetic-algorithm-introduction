import { lengthFromTopology } from "./layer";
import { Genome } from "./types";

/**
 *
 *
 * Genome Format:
 *      encoded :
 *          |topology| layers weights and basis
 *          4;8;8;4;2|0.1;0.8;-0.6;0.4;-0.3;...
 *      parsed :
 *             topology   layers weights and basis
 *          [[4,8,8,4,2],[0.1,0.8,-0.6,0.4,-0.3,...]]
 *
 */


export function parseGenome(genomeString: string): Genome {
  return genomeString
    .split("|")
    .map((a) => a.split(";").map((b) => parseFloat(b))) as Genome;
}
export function encodeGenome(genome: Genome): string {
  return genome.map((a) => a.join(";")).join("|");
}

export function randomGenome(topology: number[] = [4, 8, 8, 8, 4]): Genome {
  const length = lengthFromTopology(topology);
  return [
    topology,
    Array.from({ length }, () =>
      parseFloat((Math.random() * 2 - 1).toFixed(2))
    ),
  ] as Genome;
}

export function mutation(genome: Genome, mutationFactor: number): Genome {
  return [
    genome[0],
    genome[1].map((a) => {
      const rand = Math.random();
      const doesMutate = rand < Math.max(.1, mutationFactor);
      if (!doesMutate) return a;
      return Math.random() * 2 - 1;
    }),
  ];
}
export function close_mutation(genome: Genome, mutationFactor: number): Genome {
  return [
    genome[0],
    genome[1].map((a) => {
      const rand = Math.random();
      const doesMutate = rand < Math.max(.1, mutationFactor);
      if (!doesMutate) return a;
      return Math.max(-1, Math.min(1, a + Math.random() - 1));
    }),
  ];
}
