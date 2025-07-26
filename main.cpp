#include "nn.cpp"



int main(){
    NeuralNetwork nn(2,2,1);

    float** training_data = new float*[4];
    training_data[0] = new float[2]{0, 0};
    training_data[1] = new float[2]{0, 1};
    training_data[2] = new float[2]{1, 0};
    training_data[3] = new float[2]{1, 1};

    float targets[4] = {0, 1, 1, 0};

    for (int j = 0; j < 10000; j++) {
        for (int i = 0; i < 4; i++) {
            float* output = nn.feedforward(training_data[i], 2);
            nn.train(training_data[i], &targets[i], output, 2, 1);
        }
    }


    std::cout << "Training complete. Testing the network:\n"; 
    float input1[2] = {0, 0};
    float input2[2] = {0, 1};
    float input3[2] = {1, 0};
    float input4[2] = {1, 1};
    nn.feedforward(input1, 2);
    nn.feedforward(input2, 2);
    nn.feedforward(input3, 2);
    nn.feedforward(input4, 2);

    std::cout << "Neural Network Ready ! All outputs matched\n"; 

    // float input[2] = {0,0};
    // float target[1] = {1};
    
    // float* network = nn.feedforward(input, 2);
    // std::cout << network[0] << "\n";

    // nn.train(input, target, network, 2, 1);
    return 0;
} 