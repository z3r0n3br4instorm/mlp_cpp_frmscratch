#include <iostream>
#include <cstdlib>
#include <ctime>
#include <functional>

class Matrix {
    private:
        int rows;
        int cols;
        float** matrix;

    public:
        int GetRows() const { return rows; }
        int GetCols() const { return cols; }
    
    
    Matrix(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        matrix = new float*[rows];
        for (int i = 0; i < rows; i++) {
            matrix[i] = new float[cols];
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = 0.0f;
            }
        }
    }

    Matrix(const Matrix& other) {
        rows = other.rows;
        cols = other.cols;
        matrix = new float*[rows];
        for (int i = 0; i < rows; i++) {
            matrix[i] = new float[cols];
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            // Clean up existing memory
            for (int i = 0; i < rows; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;
            
            // Copy new data
            rows = other.rows;
            cols = other.cols;
            matrix = new float*[rows];
            for (int i = 0; i < rows; i++) {
                matrix[i] = new float[cols];
                for (int j = 0; j < cols; j++) {
                    matrix[i][j] = other.matrix[i][j];
                }
            }
        }
        return *this;
    }

    float** GetData(){
        return matrix;
    }

    ~Matrix() {
        for (int i = 0; i < rows; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    // normal ops


    static float** fromArray(float* arr, int length) {
        float** matrix = new float*[1];
        matrix[0] = new float[length];
        for (int i = 0; i < length; i++) {
            matrix[0][i] = arr[i];
        }
        return matrix;
    }

    float* toArray() {
        float* arr = new float[rows * cols];
        int index = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                arr[index++] = matrix[i][j];
            }
        }
        return arr;
    }

    void ScalarMultiply(float scalar){
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                this->matrix[i][j] *= scalar;
            }
        }
    }

    void Randomize(float min, float max) {
        srand(static_cast<unsigned>(time(nullptr)));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                this->matrix[i][j] = min + r * (max - min);
            }
        }
    }


    void Add(float number){
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++) {
                this->matrix[i][j] += number;
            }
        }
    }


    // element wise ops

    void EAdd(float** vector){
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++) {
                matrix[i][j] += vector[i][j];
            }
        }
    }

    static float** EAdd(float** matrix, float** vector, int rows, int cols){
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++) {
                matrix[i][j] += vector[i][j];
            }
        }

        return matrix;
    }

    // void EMultiply(float** vector){
    //     for (int i = 0; i < rows; i++){
    //         for (int j = 0; j < cols; j++) {
    //             matrix[i][j] *= vector[i][j];
    //         }
    //     }
    // }

    // float** DotProduct(float** matrix_n){
    //     float** new_matrix = new float*[rows];

    //     for (int i = 0; i < rows; i++) {
    //         new_matrix[i] = new float[rows];
    //         for (int j = 0; j < rows; j++) {
    //             new_matrix[i][j] = 0.0f;
    //         }
    //     }

    //     for (int i = 0; i < rows; i++){
    //         for (int j = 0; j < rows; j++) {
    //             for (int k = 0; k < cols; k++) { 
    //                 new_matrix[i][j] += matrix[i][k] * matrix_n[k][j];
    //             }
    //         }
    //     }
        
    //     return new_matrix;
    // }

    static float** MMultiply(float** matrix_m, float** matrix_n, int rows_m, int rows_n, int cols_m, int cols_n){
        if (cols_m != rows_n) {
            return nullptr; 
        }
        
        float** new_matrix = new float*[rows_m];
        for (int i = 0; i < rows_m; i++) {
            new_matrix[i] = new float[cols_n];
            for (int j = 0; j < cols_n; j++) {
                new_matrix[i][j] = 0.0f;
            }
        }

        for (int i = 0; i < rows_m; i++){
            for (int j = 0; j < cols_n; j++) {
                for (int k = 0; k < cols_m; k++) { 
                    new_matrix[i][j] += matrix_m[i][k] * matrix_n[k][j];
                }
            }
        }

        return new_matrix;
    }

    

    float** Transpose() {

        float** transposed_matrix = new float*[cols];

        for (int i = 0; i < cols; i++) {
            transposed_matrix[i] = new float[cols];
            for (int j = 0; j < rows; j++) {
                transposed_matrix[i][j] = 0.0f;
            }
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                transposed_matrix[j][i] = matrix[i][j];
            }
        }

        return transposed_matrix;
    }

    static float** Transpose(float** matrix, int rows, int cols) {
        float** transposed_matrix = new float*[cols];

        for (int i = 0; i < cols; i++) {
            transposed_matrix[i] = new float[rows];
            for (int j = 0; j < rows; j++) {
                transposed_matrix[i][j] = 0.0f;
            }
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                transposed_matrix[j][i] = matrix[i][j];
            }
        }

        return transposed_matrix;
    }

    void map(std::function<float(float)> func) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = func(matrix[i][j]);
            }
        }
    }

    static void Print(float** matrix, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void Print() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

};