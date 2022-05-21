#include <vector>
#include <functional>
#include <ctime>
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

// import { Layer, LayerValueFunction, TranslateFunction } from "./types";
using Layer = std::vector<std::vector<float>>;

Layer createLayer(
    float n0Topology,
    float n1Topology,
    std::function<float(int)> defaultValue = [](int x)
    { return 0; })
{
    Layer layer = {};
    for (int i = 0; i < n0Topology; i++)
    {
        layer.push_back({});
        for (int j = 0; j <= n1Topology; j++)
        {
            layer[i].push_back(defaultValue(i * (n1Topology + 1) + j));
        }
    }
    return layer;
}

long long lengthFromTopology(std::vector<int> topology)
{
    long long length(0);
    if (topology.size() == 0)
        return 0;
    if (topology.size() == 1)
        return topology[0];
    for (int i = 1; i < topology.size(); i++)
    {
        length += (topology[i] + 1) * topology[i - 1];
    }
    return length;
}

void clear(Layer &A)
{
    if (!A.size())
        return;
    for (std::vector<float> &element : A)
        for (float &node : element)
            node = 0;
}

void apply(Layer &origin, Layer &destination)
{
    if (!origin.size() || !destination.size())
        return;
    int k = std::min(origin.size(), destination.size());
    int l = std::min(origin[0].size(), destination[0].size());
    for (int i = 0; i < k; i++)
        for (int j = 0; j < l; j++)
            destination[i][j] = origin[i][j];
}

void add(Layer &origin, Layer &destination)
{
    if (!origin.size() || !destination.size())
        return;
    int k = std::min(origin.size(), destination.size());
    int l = std::min(origin[0].size(), destination[0].size());
    for (int i = 0; i < k; i++)
        for (int j = 0; j < l; j++)
            destination[i][j] = origin[i][j];
}

void bi_vector(
    Layer &origin, Layer &destination,
    std::function<float(float, float, float, float)> translate)
{
    if (!origin.size() || !destination.size())
        return;
    int k = std::min(origin.size(), destination.size());
    int l = std::min(origin[0].size(), destination[0].size());
    for (int i = 0; i < k; i++)
        for (int j = 0; j < l; j++)
            destination[i][j] = translate(origin[i][j], destination[i][j], i, j);
}

void translation(Layer &A, std::function<int(int)> vect)
{
    if (!A.size())
    {
        return;
    }
    for (std::vector<float> &element : A)
        for (float &node : element)
            node = vect(node);
}