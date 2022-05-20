import { parseGenome } from "./genome";
import createLayer, { apply, bi_vector, clear } from "./layer";
import { RELU6, Sigmoid, TanH } from './maths';
import { Layer, Genome } from "./types";

export class Network {
  layers: Layer[] = [];
  genome: Genome = [[], []];
  topology: number[];
  tmpLayer: Layer = [];
  maxTopologySize = 0;
  score = 0;
  round = 0;
  constructor(genome: string | Genome) {
    /**
     * Parse Genome
     */
    if (typeof genome == "string") {
      const parsed = parseGenome(genome);
      this.genome = parsed;
      this.topology = parsed[0];
    } else {
      this.genome = genome;
      this.topology = genome[0];
    }

    /**
     * Create Network layers according to genome
     */
    this.layers = this.topology.map((a, i, g) => {
      let it = this.topology.slice(0, i).reduce((acc, _, k) => {
        return acc + this.topology[k] * ((this.topology[k + 1] ?? 0) + 1);
      }, 0);
      return createLayer(
        a,
        g[i + 1] ?? 0,
        (j: number) => this.genome[1][it + j] ?? 0
      );
    });

    /**
     * Create temporary layer for computation
     * large enough to hold layers values in both dimensions
     */
    this.maxTopologySize = this.topology.reduce((a, b) => Math.max(a, b));
    this.tmpLayer = createLayer(
      this.maxTopologySize,
      this.maxTopologySize + 1,
      () => 0
    );
  }
  compute(inputs: number[]): number[] {
    const g = this.maxTopologySize + 1;
    /**
     * 1. Copy inputs to tmpLayers
     */
    apply(
      inputs.map((a) => [TanH(a)]),
      this.tmpLayer
    );

    /**
     * Loop for each layer
     */
    for (const layer of this.layers) {
      /**
       * 2. Pass sums to the 2nd layer
       */
      bi_vector(
        layer,
        this.tmpLayer,
        function (origin, destination, _, weightIndex) {
          /**
           * 2.1 Add Weight Sum to basis
           */
          if (!weightIndex) return origin + destination;
          /**
           * 2.2. Keep 2nd layer's weights
           */
          return origin;
        }
      );
      /**
       * 3. Run activation function and sum weights to at the end of each line
       */
      for (let i = 0; i < this.topology[0]; i++) {
        this.tmpLayer[i][0] = RELU6(this.tmpLayer[i][0]);
        for (let j = 1; j < g; j++) {
          this.tmpLayer[j - 1][g] += this.tmpLayer[i][0] * this.tmpLayer[i][j];
        }
      }
      /**
       * 4. Put Summarize weights at the beginning
       */
      for (let i = 0; i < this.topology[0]; i++) {
        this.tmpLayer[i][0] = this.tmpLayer[i][g];
        this.tmpLayer[i][g] = 0;
      }
    }

    /**
     * 5. Pass Summarize weights through Sigmoid function and return
     */
    const outputs: number[] = this.tmpLayer
      .slice(0, this.topology.at(-1))
      .map((a) => Sigmoid(a[0]));

    clear(this.tmpLayer);

    return outputs;
  }
}

export default Network;
