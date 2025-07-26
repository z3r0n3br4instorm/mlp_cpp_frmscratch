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
    Matrix output_layer;
    Matrix hidden_layer;
    float learning_rate = 0.1;


public:
    NeuralNetwork(int numI, int numH, int numO) 
        : input_nodes(numI), hidden_nodes(numH), output_nodes(numO),
          weights_ih(hidden_nodes, input_nodes),
          weights_ho(output_nodes, hidden_nodes),
          bias_h(hidden_nodes, 1),
          bias_o(output_nodes, 1),
          output_layer(output_nodes, 1),
          hidden_layer(hidden_nodes, 1) {

        weights_ih.Randomize(-1, 1);
        weights_ho.Randomize(-1, 1);
        bias_h.Randomize(-1, 1);
        bias_o.Randomize(-1, 1);
    }

    float sigmoid(float x) {
        return 1.0f / (1.0f + std::exp(-x));
    }
    
    float derivative_sigmoid(float y) {
        return y * (1.0f - y);
    }

    float* feedforward(float* arr, int inputs) {

        for(int i = 0; i < hidden_nodes; i++) {
            for(int j = 0; j < 1; j++) {
                hidden_layer.GetData()[i][j] = 0.0f;
            }
        }
        for(int i = 0; i < output_nodes; i++) {
            for(int j = 0; j < 1; j++) {
                output_layer.GetData()[i][j] = 0.0f;
            }
        }

        float** input = Matrix::fromArray(arr, inputs);
        std::cout << "Inputs:";
        Matrix::Print(input, 1, inputs);

        float** hidden = Matrix::MMultiply(weights_ih.GetData(), Matrix::Transpose(input, 1, inputs), hidden_nodes, input_nodes, input_nodes, 1);
        // Matrix::Print(hidden, hidden_nodes , 1);
        hidden_layer.EAdd(hidden);
        hidden_layer.EAdd(bias_h.GetData());
        // hidden_layer.Print();

        hidden_layer.map([this](float x) { return sigmoid(x); });
        // hidden_layer.Print();
        // float** output = Matrix::MMultiply(weights_ho.GetData(), Matrix::Transpose(hidden_layer.GetData(), hidden_layer.GetCols(), hidden_layer.GetRows()), weights_ho.GetRows(), weights_ho.GetCols(), hidden_layer.GetRows(), hidden_layer.GetCols());
        float** output = Matrix::MMultiply(weights_ho.GetData(), hidden_layer.GetData(), weights_ho.GetRows(), weights_ho.GetCols(), hidden_layer.GetRows(), hidden_layer.GetCols());
        // weights_ih.Print();
        // Matrix::Print(output, output_nodes, hidden_nodes);
        output_layer.EAdd(output);
        output_layer.EAdd(bias_o.GetData());
        // std::cout << "output :";
        output_layer.map([this](float x) { return sigmoid(x); });
        std::cout << "Output: \n";
        Matrix::Print(output_layer.GetData(), output_layer.GetRows(), output_layer.GetCols());        
        // output_layer.Print();
        return output_layer.toArray();

    }

    void train(float* inputs, float* targets, float* outputs, int input_size, int output_size) {
        // Convert arrays to column vectors
        float** output = Matrix::fromArray(outputs, output_size);
        float** target = Matrix::fromArray(targets, output_size);
        // std::cout << "Output: \n";
        // Matrix::Print(output, 1, output_size);
        // Matrix::Print(target, 1, output_size);
        float** output_err = Matrix::Subtract(target, output, 1, output_size);
        float** output_error_trns = Matrix::Transpose(output_err, 1, output_size);
        // std::cout << "Output Error: \n";
        // Matrix::Print(output_err, 1, output_size);
        // Matrix::Print(output_error_trns, output_size, 1);
        // output_layer.map([this](float x) { return derivative_sigmoid(x); });

        // float** gradient = Matrix::map(output_layer.GetData(), output_layer.GetRows(), output_layer.GetCols(), [this](float x) { return derivative_sigmoid(x); });
        
        
        Matrix gradient_matrix(output_size, 1);
        gradient_matrix.EAdd(output_layer.GetData());
        gradient_matrix.map([this](float y) { return derivative_sigmoid(y); });
        gradient_matrix.MMultiply(output_error_trns);
        gradient_matrix.ScalarMultiply(learning_rate);
        // std::cout << "Gradient Matrix: \n";
        // gradient_matrix.Print();

        float** hidden_t = Matrix::Transpose(hidden_layer.GetData(), hidden_layer.GetRows(), hidden_layer.GetCols());
        float** weight_ho_deltas = Matrix::MMultiply(gradient_matrix.GetData(), hidden_t, output_size, 1, 1, hidden_nodes);
        bias_o.EAdd(gradient_matrix.GetData());
        float** weight_ho_deltas_trns = Matrix::Transpose(weight_ho_deltas, output_size, hidden_nodes);
        
        // gradient_matrix.Print(); 
        // weights_ho.EAdd(weight_ho_deltas_trns);

        float** hidden_errors = Matrix::MMultiply(Matrix::Transpose(weights_ho.GetData(), weights_ho.GetRows(), weights_ho.GetCols()), output_error_trns, weights_ho.GetCols(), weights_ho.GetRows(), output_size, 1);

        Matrix hidden_gradient(hidden_nodes, 1);
        hidden_gradient.EAdd(hidden_layer.GetData());
        hidden_gradient.map([this](float y) { return derivative_sigmoid(y); });
        hidden_gradient.MMultiply(hidden_errors);
        hidden_gradient.ScalarMultiply(learning_rate);
        
        float** input_t = Matrix::Transpose(Matrix::fromArray(inputs, input_size), 1, input_size);
        float** weight_ih_deltas = Matrix::MMultiply(hidden_gradient.GetData(), input_t, hidden_nodes, 1, 1, input_size);
        bias_h.EAdd(hidden_gradient.GetData());
        float** weight_ih_deltas_trns = Matrix::Transpose(weight_ih_deltas, hidden_nodes, input_size);

        weights_ih.EAdd(weight_ih_deltas);
        // std::cout << "Hidden Gradient: \n";
        // hidden_gradient.Print();
        weights_ho.EAdd(weight_ho_deltas);

        // std::cout << "Weights IH: \n";
        // Matrix::Print(weights_ih.GetData(), weights_ih.GetRows(), weights_ih.GetCols());
        // std::cout << "Weights HO: \n";
        // Matrix::Print(weights_ho.GetData(), weights_ho.GetRows(), weights_ho.GetCols());

        // std::cout << "Output: \n";
        // Matrix::Print(output, 1, output_size);
        // std::cout << "Target: \n";
        // Matrix::Print(target, 1, output_size);
        // std::cout << "Hidden Error:  \n";
        // Matrix::Print(hidden_errors, 1, weights_ho.GetCols());



    }
};