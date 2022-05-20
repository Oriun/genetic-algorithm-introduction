/**
 *
 *
 *  Layer Format
 *  ________________________________
 *  |            |            |
 *  | N-A1 basis | N-B1 basis | ...
 *  |____________|____________|_____
 *  |N-A2 weight |N-A2 weight | ...
 *  |N-B2 weight |N-B2 weight | ...
 *  |N-C2 weight |N-C2 weight | ...
 *  |N-D2 weight |N-D2 weight | ...
 *  | ...        | ...        | ...
 *  |____________|____________|_____
 */

import { Layer, LayerValueFunction, TranslateFunction } from "./types";


export function createLayer(
  n0Topology: number,
  n1Topology: number,
  defaultValue: LayerValueFunction = () => 0
): Layer {
  return Array.from({ length: n0Topology }, (_, i) =>
    Array.from({ length: n1Topology + 1 }, (_, j) =>
      defaultValue(i * (n1Topology + 1) + j)
    )
  );
}

export function lengthFromTopology(topology: number[]) {
  let length = 0;
  for(let i = 1; i < topology.length; i++) {
    length += (topology[i] + 1) * topology[i-1]
  }
  return length;
}

export function clear(A: Layer): void {
  if (!A.length) return;
  for (const element of A)
    for (let j = 0; j < element.length; j++) element[j] = 0;
}
export function apply(origin: Layer, destination: Layer): void {
  if (!origin.length || !destination.length) return;
  let k = Math.min(origin.length, destination.length);
  let l = Math.min(origin[0].length, destination[0].length);
  for (let i = 0; i < k; i++)
    for (let j = 0; j < l; j++) destination[i][j] = origin[i][j];
}

export function add(origin: Layer, destination: Layer): void {
  if (!origin.length || !destination.length) return;
  let k = Math.min(origin.length, destination.length);
  let l = Math.min(origin[0].length, destination[0].length);
  for (let i = 0; i < k; i++)
    for (let j = 0; j < l; j++) destination[i][j] += origin[i][j];
}

export function bi_vector(
  origin: Layer,
  destination: Layer,
  translate: TranslateFunction
): void {
  if (!origin.length || !destination.length) return;
  let k = Math.min(origin.length, destination.length);
  let l = Math.min(origin[0].length, destination[0].length);
  for (let i = 0; i < k; i++)
    for (let j = 0; j < l; j++)
      destination[i][j] = translate(origin[i][j], destination[i][j], i, j);
}
export function vector(A: Layer, vect: (a: number) => number): void {
  if (!A.length) return;
  for (const element of A)
    for (let j = 0; j < element.length; j++) element[j] = vect(element[j]);
}

export default createLayer;
