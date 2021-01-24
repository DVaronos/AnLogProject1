#include "hash.h"
#include "JobSheduler.h"

typedef struct inp{
	HVector** Cons;
	int* matches;
	int size;
}Input;

Input* InputInit();
Input* InputMake(char* ,char* ,Hash* );

void FreeInput(Input* );

typedef struct ts{
	int start;
	int end;
	double b;
	double* w;
	Input* input;
}TrainStruct;

typedef struct tes{
	int start;
	int end;
	int who;
	int sum;
	double b;
	double threshold;
	double* w;
	Hash* hash;
	Input* input;
	NList** nodes;
	JobSheduler* Sheduler;
}TestStruct;

typedef struct Model
{
	double* weight_array;
	double b;
	int array_size;
}Model;


HVector* GetCameraVector(char* , Hash* );
HVector* VectorConcat(HVector* ,HVector* ,int );



Model Training(Model ,Input* ,Hash* ,JobSheduler* );
Model RepetitiveTaining(Input* ,Hash* ,JobSheduler* );

double Fx(double** ,double*  ,HVector* );
double Predict(double** ,double*  ,HVector* );
double Newpred(double* ,double ,HVector* ,HVector* ,int );

void TestAndAdd(Hash* ,Model ,JobSheduler* ,Input** ,double );
void TestData(void *);
void Testing(char* ,Model ,Hash* );
void JobTraining(void* );
void MakeArrays(Hash* ,int* ,NList*** );
