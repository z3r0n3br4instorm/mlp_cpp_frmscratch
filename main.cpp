#include "nn.cpp"

int main(){
    NeuralNetwork nn(2,2,2);

    float input[2] = {1,0};
    float target[2] = {1,0};
    
    float* network = nn.feedforward(input, 2);
    std::cout << network[0] << "\n";
    std::cout << network[1] << "\n";


    nn.train(input, target, network, 2, 2);
    return 0;
} 