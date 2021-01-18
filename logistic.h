#include "hash.h"


typedef struct inp{
	HVector** Cons;
	int* matches;
	int size;
}Input;

Input* InputInit();
Input* InputMake(char* ,char* ,Hash* );

void FreeInput(Input* );


typedef struct Model
{
	double* weight_array;
	double b;
	int array_size;
}Model;


HVector* GetCameraVector(char* , Hash* );
HVector* VectorConcat(HVector* ,HVector* ,int );


Model Training(Input*,Hash* );
double Fx(Model ,HVector* );
double Predict(Model ,HVector* );
double Norm(Model );

void TestAllData(Hash* ,Model );
void Testing(char* ,Model ,Hash* );
