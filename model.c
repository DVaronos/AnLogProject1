#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "model.h"


float* IninializeWeightArray(int vector_size)
{
	float* weight_array;
	weight_array = malloc(vector_size * sizeof(float));
	for(int i=0; i<vector_size; i++)
	{
		weight_array[i] = 1.0;
	}
	return weight_array;
}

Model* IninializeModel(int vector_size)
{
	Model* model = malloc(sizeof(Model));
	model->weight_array = IninializeWeightArray(vector_size);
	model->b = 1.0;
	model->array_size = vector_size;
	return model;
}

float F(float* vectror_array, Model* model)
{
	int vector_size = model->array_size;
	float result = model->b, wx;
	for(int i=0; i<vector_size; i++)
	{
		wx = ((vectror_array[i]) * (model->weight_array[i]));
		result = result + wx;
	}
	return result;
}

float P(float* vectror_array, Model* model)
{
	double e = exp(1);
	float f = F(vectror_array, model);
	float p = 1 / (1 + e - f);
	return p;
}

float ProbToBeAMatch(float p)
{
	if(p > 1)
	{
		printf("Error in input data\n");
		return -1;
	}
	return p;
}

float ProbNotToBeAMatch(float p)
{
	if(p > 1)
	{
		printf("Error in input data\n");
		return -1;
	}
	return 1 - p;
}

int IsMatch(float p)
{
	int res = ProbToBeAMatch - ProbNotToBeAMatch;
	if(res > 1)
		return 1;	//is match
	else
		return 0;	//not a match
}

void FreeWeightArray(float* weight_array)
{
	free(weight_array);
}

void FreeModel(Model* model)
{
	FreeWeightArray(model->weight_array);
	free(model);
}