/**
 * 
 * From https://towardsdatascience.com/everything-you-need-to-know-about-activation-functions-in-deep-learning-models-84ba9f82c253
 *  
 */


export function RELU(x: number): number {
    return Math.max(0,x)
}
export function LeakyRELU(x: number, alpha=.01): number {
    return Math.max(x, alpha * x)
}
export function RELU6(x: number, max=6): number {
    return Math.min(RELU(x), max)
}
export function Sigmoid(x: number): number {
    return 1 / (1 + Math.exp(-1 * x))
}
export function TanH(x: number): number {
    return Math.tanh(x)
}
export function Swish(x: number): number {
    return x * Sigmoid(x)
}
export function HardSwish(x: number): number {
    return x * RELU6(x+3) / 6
}