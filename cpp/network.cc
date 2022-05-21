#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "genome.cc"
#include "maths.cc"

class Network {
  private:
    std::vector<Layer> layers = {};
    Genome genome;
    Layer tmpLayer = {};
    long long maxTopologySize = 0;
  
    void mount() {
      /**
       * Create Network layers according to genome
       */
      for(int i = 0; i < genome.topology.size() - 1; i++){
          int it = 0;
          for(int k = 0; k < i; k++){
            it += genome.topology[k] * (genome.topology[k + 1] + 1);
          }
          layers.push_back(createLayer(
            genome.topology[i],
            genome.topology[i + 1] ?: 0,
            [&](float j){ return genome.genes[it + j] ?: 0; }
          ));
        };

      /**
       * Create temporary layer for computation
       * large enough to hold layers values in both dimensions
       */
      maxTopologySize = *std::max_element(genome.topology.begin(), genome.topology.end());
      tmpLayer = createLayer(
        maxTopologySize,
        maxTopologySize + 1
      );
    }

  public:
    Network(std::string _genome) {
      genome = parseGenome(_genome);
      mount();
    }
    Network(Genome _genome) {
      genome = _genome;
      mount();
    }

    std::vector<float> compute(std::vector<float> inputs) {
      int g = maxTopologySize + 1;

      Layer inputsT = {};
      for(const float input: inputs){
        inputsT.push_back({ TanH(input) });
      }
      /**
       * 1. Copy inputs to tmpLayers
       */
      apply(
        inputsT,
        tmpLayer
      );

      /**
       * Loop for each layer
       */
      for (Layer layer : layers) {
        /**
         * 2. Pass sums to the 2nd layer
         */
        bi_vector(
          layer,
          tmpLayer,
          [](float origin, float destination, float _, float weightIndex) {
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
        for (int i = 0; i < layer.size(); i++) {
          tmpLayer[i][0] = RELU6(tmpLayer[i][0]);
          for (int j = 1; j < g; j++) {
            tmpLayer[j - 1][g] += tmpLayer[i][0] * tmpLayer[i][j];
          }
        }
        /**
         * 4. Put Summarize weights at the beginning
         */
        for (int i = 0; i < layer.size(); i++) {
          tmpLayer[i][0] = tmpLayer[i][g];
          tmpLayer[i][g] = 0;
        }
      }

      /**
       * 5. Pass Summarize weights through Sigmoid function and return
       */
      std::vector<float> outputs = {};
      
      for(int k = 0; k < genome.topology.back(); k++){
        outputs.push_back(Sigmoid(tmpLayer[k][0]));
      }

      clear(tmpLayer);
      return outputs;
    }
};
