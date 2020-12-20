#include "hash.h"
typedef struct Model
{
	double* weight_array;
	double b;
	int array_size;
}Model;

double* InitializeWeightArray(int vector_size);

Model* InitializeModel(int vector_size);

double F(double* vectror_array, Model* model);
double P(double* vectror_array, Model* model);

double ProbToBeAMatch(double p);
double ProbNotToBeAMatch(double p);

int PredictMatch(double p);

void PrintWeightArray(Model* model);

void FreeWeightArray(double* weight_array);
void FreeModel(Model* model);

double L(double p, int ismatch);

typedef struct Input
{
	double** vector_array;
	int* matches_array;
	int weight_size;
	int matches_array_size;
}Input;

Input* InitializeInput();
void PrintInput(Input* input);
Input* MakeInputArray(FILE*, FILE*, Hash* H);

int FreeInput(Input* input);

double* Vector_Concat(double* vector1, double* vector2, int size);


Model* Training(Model* model, Input* input);

Input* MakeTestInputArray(FILE* fptr, Hash* H, int test_per);

void Testing(Input* input, Model* model);

double* GetCameraVector(char* camera_id, Hash* H);
