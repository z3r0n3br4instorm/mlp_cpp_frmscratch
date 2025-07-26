#include <iostream>
#include "math.cpp"
#include <cmath>

// H[i] = sig(W[i][j] * I[i] + B[i])

class NeuralNetwork {
private:
    int input_nodes, hidden_nodes, output_nodes;
    Matrix weights_ih;
    Matrix weights_ho;
    Matrix bias_h;
    Matrix bias_o;

public:
    NeuralNetwork(int numI, int numH, int numO) 
        : input_nodes(numI), hidden_nodes(numH), output_nodes(numO),
          weights_ih(hidden_nodes, input_nodes),
          weights_ho(output_nodes, hidden_nodes),
          bias_h(hidden_nodes, 1),
          bias_o(output_nodes, 1) {
        
        weights_ih.Randomize(-1,1);
        weights_ho.Randomize(-1,1);
    }

    float sigmoid(float x) {
        return 1.0f / (1.0f + std::exp(-x));
    }
    
    float* feedforward(float* arr, int inputs) {
        float** input = Matrix::fromArray(arr, inputs);
        std::cout << "Inputs:";
        Matrix::Print(input, 1, inputs);
        bias_o.Randomize(0, 1);
        bias_h.Randomize(0, 1);
        // std::cout << "\n";
        // weights_ih.Print();
        // std::cout << "\n";
        float** hidden = Matrix::MMultiply(weights_ih.GetData(), Matrix::Transpose(input, 1, inputs), hidden_nodes, inputs, input_nodes, 1);
        // Matrix::Print(hidden, hidden_nodes , 1);
        Matrix hidden_layer(hidden_nodes, 1);
        hidden_layer.EAdd(hidden);
        hidden_layer.EAdd(bias_h.GetData());
        // hidden_layer.Print();

        hidden_layer.map([this](float x) { return sigmoid(x); });
        // hidden_layer.Print();

        Matrix output_layer(output_nodes, 1);
        // float** output = Matrix::MMultiply(weights_ho.GetData(), Matrix::Transpose(hidden_layer.GetData(), hidden_layer.GetCols(), hidden_layer.GetRows()), weights_ho.GetRows(), weights_ho.GetCols(), hidden_layer.GetRows(), hidden_layer.GetCols());
        float** output = Matrix::MMultiply(weights_ho.GetData(), hidden_layer.GetData(), weights_ho.GetRows(), weights_ho.GetCols(), hidden_layer.GetRows(), hidden_layer.GetCols());
        // weights_ih.Print();
        // Matrix::Print(output, output_nodes, hidden_nodes);
        output_layer.EAdd(output);
        output_layer.EAdd(bias_o.GetData());
        // std::cout << "output :";
        output_layer.map([this](float x) { return sigmoid(x); });
        // output_layer.Print();
        return output_layer.toArray();

    }

    void train(float* inputs,float* answer) {
        //
    }
};