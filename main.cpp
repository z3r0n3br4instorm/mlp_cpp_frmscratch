#include "nn.cpp"

int main(){
    NeuralNetwork nn(2,2,1);

    float input[2] = {0,0};
    
    float* network = nn.feedforward(input, 2);
    std::cout << network[0];
    return 0;
}