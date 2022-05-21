#include <cmath>
#include <algorithm>

/**
 * 
 * From https://towardsdatascience.com/everything-you-need-to-know-about-activation-functions-in-deep-learning-models-84ba9f82c253
 *  
 */


float RELU(float x) {
    return std::max((float) 0, x);
}
float LeakyRELU(float x, float alpha=.01F) {
    return std::max(x, alpha * x);
}
float RELU6(float x, float max=6.0F) {
    return std::min(RELU(x), max);
}
float Sigmoid(float x) {
    return 1 / (1 + exp(-1 * x));
}
float TanH(float x) {
    return tanh(x);
}
float Swish(float x) {
    return x * Sigmoid(x);
}
float HardSwish(float x) {
    return x * RELU6(x+3) / 6;
}