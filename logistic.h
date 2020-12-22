#include "hash.h"

typedef struct Model
{
	double* weight_array;
	double b;
	int array_size;
}Model;


double* GetCameraVector(char* camera_id, Hash* H);
double* Vector_Concat(double* vector1, double* vector2, int size);

Model Training(char* ,char* ,Hash* );
double Fx(Model ,double* ,int );
double Predict(Model ,double* ,int );

void Testing(char* ,Model ,Hash* );
