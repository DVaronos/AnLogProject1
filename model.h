#include "hash.h"
typedef struct Model
{
	float* weight_array;
	float b;
	int array_size;
}Model;

float* IninializeWeightArray(int vector_size);

Model* IninializeModel(int vector_size);

float F(float* vectror_array, Model* model);
float P(float* vectror_array, Model* model);

float ProbToBeAMatch(float p);
float ProbNotToBeAMatch(float p);

int IsMatch(float p);

void PrintWeightArray(Model* model);

void FreeWeightArray(float* weight_array);
void FreeModel(Model* model);

float L(float p, int ismatch);

typedef struct Input
{
	float** vector_array;
	int* matches_array;
	int weight_size;
	int matches_array_size;
}Input;

Input* InitializeInput();
void PrintInput(Input* input);
Input* MakeInputArray(char* filename1, char* filename2);

int FreeInput(Input* input);

float* Vector_Concat(float* vector1, float* vector2, int size);


Model* Training(Model* model, Input* input);