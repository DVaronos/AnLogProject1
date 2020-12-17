#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "model.h"
// #include "list.h"

#define LEARNING_RATE 0.1
#define VERY_SMALL_NUMBER 0.1


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

void PrintWeightArray(Model* model)
{
	for(int i=0; i<model->array_size; i++)
		printf("%f\t", model->weight_array[i]);
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

float L(float p, int ismatch)
{
	float result;
	if(ismatch == 0)
		result = -log((double) p);
	else
		result = -log(1-(double)p);
}

Input* InitializeInput()
{
	Input* in = malloc(sizeof(Input));
	in->vector_array = NULL;
	in->matches_array = NULL;
	in->weight_size = 0;
	in->matches_array_size = 0;
}

char* getfield(char* line, char** first, char** second)
{
	char* token = strtok(line, ",");
	strcpy(*first, token);
	token = strtok(NULL, ",");
	strcpy(*second, token);
}


// afth h trainset list 8a exei prokupsei apo ola ta zeugh 
Input* MakeInputArray(char* filename1, char* filename2)
{
	FILE* fptr = fopen(filename1, "r");
	char* token=NULL;
	char line[300];
	char first[300];
	char second[300];
	int match=0,count=0,z=0,a=0;

	fgets(line, 300,fptr);
	token=strtok(line,",");
	if(strcmp(token,"left_spec_id") != 0)
	{
		printf("%s\n", token);
		printf("Error in file\n");
		return NULL;
	}
	while(fgets(line, 300,fptr) != NULL)
	{//Diavazei to csv file grami grami
		token = strtok(line, ",");
		strcpy(first, token);
		token = strtok(NULL, "\n");
		strcpy(second, token);
		printf("%s \t %s\n", first, second);
		count++;
		memset(line, 0, 300);		memset(first, 0, 300);		memset(second, 0, 300);	memset(token, 0, 300);
    }
    printf("%d\n", count);

	// Input* input = InitializeInput();

	// float* con_vec;	
	// // afta 8a einai ta tsn1_ptr->vector_array kai tsn1_ptr->vector_array...
	// float vector1[] = {0.01, 0.02, 0.03};
	// float vector2[] = {0.04, 0.05, 0.06};
	// int size = 3;
	// int count = 0;
	// int ismatch = 1;

	// TrainSetNode* tsn1_ptr, *tsn2_ptr;
	// tsn1_ptr = tsl->first;
	// while(tsn1_ptr != NULL)
	// {
	// 	tsn2_ptr = tsn1_ptr->next;
	// 	while(tsn2_ptr != NULL)
	// 	{
	// 		con_vec = Vector_Concat(vector1, vector2, size);

	// 		input->vector_array = realloc(input->vector_array, (count+1)*sizeof(con_vec));
	// 		input->vector_array[count] = con_vec;

	// 		input->matches_array = realloc(input->matches_array, (count+1)*sizeof(ismatch));
	// 		input->matches_array[count] = ismatch;
	// 		count++;

	// 		tsn2_ptr = tsn2_ptr->next;
	// 	}
	// 	tsn1_ptr = tsn1_ptr->next;
	// }
	// input->matches_array_size = count;
	// input->weight_size = 2*3;
	// return input;
}

void PrintInput(Input* input)
{
  for(int i=0; i<input->matches_array_size; i++)
  {
    for(int j=0; j<input->weight_size; j++)
    {
      printf("%f\t", input->vector_array[i][j]);
    }
    printf(" -----> %d\n", input->matches_array[i]);
    printf("\n");
  }
  printf("how many pairs: %d\n", input->matches_array_size);
}

int FreeInput(Input* input)
{
	free(input->matches_array);
	for(int i =0; i<input->weight_size; i++)
	{
		// free(input->vector_array[i]);
	}
	free(input->vector_array);
	free(input);
}



Model* Training(Model* model, Input* input)
{
	int correct_value;
	int result[] = {0}, i;
	float* vectror_array, old_weight;

	// gia ka8e leksh tou vovabulary upologizetai to varos ths kai
	// elegxetai sxetika me to learning rate
	// an einai ikanopoihtika mikrh h diafora pame sthn epomenh leksh
	// alliws ksanaupologizetai gia thn idia
	int j=0;
	while(j<input->weight_size)	// gia ka8e leksh sto vocabulary
	{
		correct_value = input->matches_array[j];

		old_weight = model->weight_array[j];
		for(i=0; i<input->matches_array_size; i++)
		{	
			*result = *result + (P(input->vector_array[i], model) - correct_value) * input->vector_array[i][j];
		}

		model->weight_array[j] = model->weight_array[j] - (LEARNING_RATE*(*result));
		if((model->weight_array[j] - old_weight) < VERY_SMALL_NUMBER )	//telos train gi auth th leksh
		{
			j++;
		}
		//alliws ksanagineai train gia thn idia leksh
	}
	return model;
}


float* Vector_Concat(float* vector1, float* vector2, int size)
{
	float* concat_vec = malloc(2*size*sizeof(float));
	memset(concat_vec, 0, 2*size*sizeof(float));
	int sum1 = 0, sum2 = 0;
	for(int i=0; i<size ; i++)
	{
		sum1 += vector1[i];
		sum2 += vector2[i];
	}
	if(sum1 > sum2)
	{
		memcpy(concat_vec, vector1, size*sizeof(float));
		memcpy(&concat_vec[size] , vector2, size*sizeof(float));
	}
	else
	{
		memcpy(concat_vec, vector2, size*sizeof(float));
		memcpy(&concat_vec[size] ,vector1, size*sizeof(float));
	}
	return concat_vec;
}
