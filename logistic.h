#include "hash.h"

typedef struct Model
{
	double* weight_array;
	double b;
	int array_size;
}Model;


HVector* GetCameraVector(char* , Hash* );
HVector* VectorConcat(HVector* ,HVector* ,int );

Model Training(char* ,char* ,Hash* );
double Fx(Model ,HVector* );
double Predict(Model ,HVector* );
double Norm(Model );

void TestAllData(Hash* ,Model );
void Testing(char* ,Model ,Hash* );
