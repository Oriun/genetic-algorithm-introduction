export type Genome = [number[], number[]];
export type Fitness = number;
export type Stress = number;
export type SelectionItem = [Genome, Fitness];
export type Offspring = [Genome, 0];
export type TrainData = {
  input: number[];
  max: [number, number];
}[];
export type Layer = number[][];
export type LayerValueFunction = (index: number) => number;
export type TranslateFunction = (
  origin: number,
  destination: number,
  nodeIndex: number,
  weightIndex: number
) => number;