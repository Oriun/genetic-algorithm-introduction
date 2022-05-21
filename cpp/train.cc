#include <vector>
#include <array>

struct TrainData
{
    std::vector<float> input;
    std::array<float, 2> max;
    TrainData(std::vector<float> i, std::array<float, 2> m) : input(i), max(m){};
};

using TrainSet = std::vector<TrainData>;
TrainSet train = {
    TrainData({1, 1, 0, 0}, {1, 1}),
    TrainData({1, 0, 0, 0}, {1, 0}),
    TrainData({0, 1, 0, 0}, {0, 1}),
    TrainData({-1, -1, 0, 0}, {-1, -1}),
    TrainData({0, 0, 0, 0}, {0, 0}),
    TrainData({0, 0.5, 0, 0}, {0, 0.5}),
    TrainData({0.5, 0.5, 0, 0}, {0.5, 0.5}),
    TrainData({0.5, 0, 0, 0}, {0.5, 0}),
    TrainData({0, 0, 0, 0}, {0, 0}),
    TrainData({0, -0.5, 0, 0}, {0, 0.5}),
    TrainData({-0.5, -0.5, 0, 0}, {-0.5, -0.5}),
    TrainData({-0.5, 0, 0, 0}, {-0.5, 0}),
    TrainData({2, 0, 0, 0}, {1, 0}),
    TrainData({2, 1, 0, 0}, {1, 1}),
    TrainData({2, 2, 0, 0}, {1, 1}),
    TrainData({-2, 2, 0, 0}, {-1, 1}),
    TrainData({-2, -2, 0, 0}, {-1, -1})
};