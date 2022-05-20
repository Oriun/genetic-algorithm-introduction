import { mutation } from "./genome";
import { SelectionItem, Stress, Offspring, Genome } from "./types";

function chunks<T>(table: T[], size: number): T[][] {
  return Array.from({ length: Math.ceil(table.length / size) }).map((_, i) =>
    table.slice(size * i, size * (i + 1))
  );
}

// export function k_points_crossover(
//   A: SelectionItem,
//   B: SelectionItem,
//   stress_A: Stress,
//   stress_B: Stress,
//   k: number = 0
// ): [Offspring, Offspring] {
//   const nbOfPoints = k > 0 ? k : Math.floor(Math.random() * 10)
//   ********To implement*********
// }

export function uniform_crossover(
    A: SelectionItem,
    B: SelectionItem,
    stress_A: Stress,
    stress_B: Stress,
    chunk_size: number = 1
  ): [Offspring, Offspring] {
    const gene_a = chunks(mutation(A[0], stress_A)[1], chunk_size);
    const gene_b = chunks(mutation(B[0], stress_B)[1], chunk_size);
    const crossed = Array.from({ length: gene_a.length }, (_, i) =>
      Math.random() > 0.5 ? [gene_b[i], gene_a[i]] : [gene_a[i], gene_b[i]]
    ).flat(Infinity);
    const new_A = [A[0][0], crossed[0]] as Genome;
    const new_B = [B[0][0], crossed[1]] as Genome;
    return [
      [new_A, 0],
      [new_B, 0],
    ];
  }

export function uniform_independant_crossover(
  A: SelectionItem,
  B: SelectionItem,
  stress_A: Stress,
  stress_B: Stress
): [Offspring, Offspring] {
  const gene_a = chunks(mutation(A[0], stress_A)[1], 20);
  const gene_b = chunks(mutation(B[0], stress_B)[1], 20);
  const new_A = [
    A[0][0],
    Array.from({ length: gene_a.length }, (_, i) =>
      Math.random() > 0.5 ? gene_b[i] : gene_a[i]
    ).flat(Infinity),
  ] as Genome;
  const new_B = [
    B[0][0],
    Array.from({ length: gene_a.length }, (_, i) =>
      Math.random() > 0.5 ? gene_a[i] : gene_b[i]
    ).flat(Infinity),
  ] as Genome;
  return [
    [new_A, 0],
    [new_B, 0],
  ];
}
export function uniform_independant_bias_crossover(
  A: SelectionItem,
  B: SelectionItem,
  stress_A: Stress,
  stress_B: Stress,
  chunk_size: number = 1
): [Offspring, Offspring] {
  const half = stress_A / (stress_A + stress_B);
  const gene_a = chunks(mutation(A[0], stress_A)[1], chunk_size);
  const gene_b = chunks(mutation(B[0], stress_B)[1], chunk_size);
  const new_A = [
    A[0][0],
    Array.from({ length: gene_a.length }, (_, i) =>
      Math.random() < half ? gene_a[i] : gene_b[i]
    ).flat(Infinity),
  ] as Genome;
  const new_B = [
    B[0][0],
    Array.from({ length: gene_a.length }, (_, i) =>
      Math.random() > half ? gene_a[i] : gene_b[i]
    ).flat(Infinity),
  ] as Genome;
  return [
    [new_A, 0],
    [new_B, 0],
  ];
}
