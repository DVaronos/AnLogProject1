#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "model.h"
// #include "list.h"

#define LEARNING_RATE 0.6
#define VERY_SMALL_NUMBER 0.1
#define THRESHOLD 0.5


double* InitializeWeightArray(int vector_size)
{
	double* weight_array;
	weight_array = malloc(vector_size * sizeof(double));
	for(int i=0; i<vector_size; i++)
	{
		weight_array[i] = 0.0;
	}
	return weight_array;
}

Model* InitializeModel(int vector_size)
{
	Model* model = malloc(sizeof(Model));
	model->weight_array = InitializeWeightArray(vector_size);
	model->b = 0.0;
	model->array_size = vector_size;
	return model;
}

double F(double* vectror_array, Model* model)
{
	int vector_size = model->array_size;
	double result = model->b, wx;
	for(int i=0; i<vector_size; i++)
	{
		if(vectror_array[i] == 0)
			continue;
		wx = ((vectror_array[i]) * (model->weight_array[i]));
		result = result + wx;
	}
	return result;
}

double P(double* vectror_array, Model* model)
{
	double f = F(vectror_array, model);
	double p = (double) 1.0 / ((double)1.0 + exp(-1.0*f));
	return p;
}

double ProbToBeAMatch(double p)
{
	if(p > 1)
	{
		printf("Error in input data\n");
		return -1;
	}
	return p;
}

double ProbNotToBeAMatch(double p)
{
	if(p > 1)
	{
		printf("Error in input data\n");
		return -1;
	}
	return 1 - p;
}

int PredictMatch(double p)
{
	// int res = ProbToBeAMatch(p) - ProbNotToBeAMatch(p);
	// if(res > 1)
	// 	return 1;	//is match
	// else
	// 	return 0;	//not a match
	if(p > THRESHOLD)
		return 1;
	else
		return 0;
	// double i = 1.0-p;
	// double j = 0.0 + p;
	// if(i > j)
	// 	return 0;
	// else
	// 	return 1;
}

void PrintWeightArray(Model* model)
{
	for(int i=0; i<model->array_size; i++)
	{	
		printf("%f\n", model->weight_array[i]);
	}
}

void FreeWeightArray(double* weight_array)
{
	free(weight_array);
}

void FreeModel(Model* model)
{
	FreeWeightArray(model->weight_array);
	free(model);
}

double L(double p, int ismatch)
{
	double result;
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

double* GetCameraVector(char* camera_id, Hash* H)
{
	NList *L;
	int i=0,j=0,in;
	char name[20];
	while(camera_id[i]!='/')
	{//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
		i++;
	}
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(camera_id))
	{
		name[j]=camera_id[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	int index=hash(in,H->size);
	L=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
	L = L->Next;
	while(L!=NULL)
	{
		if(strcmp(L->camera,camera_id) == 0)	return L->vector;
		L=L->Next;
	}
}

// afth h trainset list 8a exei prokupsei apo ola ta zeugh 
Input* MakeInputArray(FILE* fptr, FILE* fptr2, Hash* H)
{
	int size = H->Head->Next->vec_size;
	int ismatch = 1;
	Input* input = InitializeInput();

	// FILE* fptr = fopen(filename1, "r");
	char* token=NULL;
	char line[300];
	char first[300];
	char second[300];
	int match=0,count=0;

	for(int i=0; i<2; i++)
	{
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
			token = strtok(NULL, " \n");
			strcpy(second, token);
			// printf("%s \t %s\n", first, second);
	
			double* con_vec;	
			// printf("%s\n", first);
			// afta 8a einai ta tsn1_ptr->vector_array kai tsn1_ptr->vector_array...
			double* vector1 = GetCameraVector(first, H);
			double* vector2 = GetCameraVector(second, H);

			con_vec = Vector_Concat(vector1, vector2, size);
	
			input->vector_array = realloc(input->vector_array, (count+1)*sizeof(con_vec));
			input->vector_array[count] = con_vec;
	
			input->matches_array = realloc(input->matches_array, (count+1)*sizeof(int));
			input->matches_array[count] = ismatch;
			count++;
			memset(line, 0, 300);
			memset(first, 0, 300);
			memset(second, 0, 300);
	
		}
		if(i == 1)
			break;
		fclose(fptr);
		fptr = fptr2;
		ismatch = 0;
	}
	fclose(fptr);
    printf("%d\n", count);

    input->matches_array_size = count;
    input->weight_size = 2*size;
    printf("pairs are %d\n", count);
	return input;
}

Input* MakeTestInputArray(FILE* fptr,Hash* H, int test_per)
{
	int size = H->Head->Next->vec_size;
	int ismatch;
	Input* input = InitializeInput();

	// FILE* fptr = fopen(filename1, "r");
	char* token=NULL;
	char line[300];
	char first[300];
	char second[300];
	int match=0,count=0;

	fgets(line, 300,fptr);
	token=strtok(line,",");

	while(fgets(line, 300,fptr) != NULL)
	{//Diavazei to csv file grami grami
		token = strtok(line, ",");
		strcpy(first, token);
		token = strtok(NULL, " ,");
		strcpy(second, token);
		token = strtok(NULL, " ,\n");
		ismatch = atoi(token);
		// if(ismatch == 0)
		// 	continue;
		// printf("%s \t %s \t---> %d\n", first, second, ismatch);

		double* con_vec;	
		// printf("%s\n", first);
		// afta 8a einai ta tsn1_ptr->vector_array kai tsn1_ptr->vector_array...
		double* vector1 = GetCameraVector(first, H);
		double* vector2 = GetCameraVector(second, H);

		con_vec = Vector_Concat(vector1, vector2, size);

		input->vector_array = realloc(input->vector_array, (count+1)*sizeof(con_vec));
		input->vector_array[count] = con_vec;

		input->matches_array = realloc(input->matches_array, (count+1)*sizeof(int));
		input->matches_array[count] = ismatch;
		count++;
		memset(line, 0, 300);
		memset(first, 0, 300);
		memset(second, 0, 300);
		if(count == test_per)
			break;
	}

	// fclose(fptr);
    printf("%d\n", count);

    input->matches_array_size = count;
    input->weight_size = 2*size;
    printf("pairs are %d\n", count);
	return input;
}

void PrintInput(Input* input)
{
	int nonzero, nztotal=0;
  for(int i=0; i<input->matches_array_size; i++)
  {
  	nonzero = 0;
    for(int j=0; j<input->weight_size; j++)
    {
    	if(input->vector_array[i][j] != 0.0)
     	{	
     		printf("%f\t", input->vector_array[i][j]);
     		nonzero++;
 		}
  		if(j == 5)
  		{printf("|||||\t");	j = input->weight_size - 5;}
    }
     	if(nonzero != 0)
 		{	
 			printf("non zero is %d\n", nonzero);
 			nztotal++;
 		}
    printf(" -----> %d\n", input->matches_array[i]);
    printf(" \tnonzero elements are : %d / %d\n", nonzero,input->weight_size);

    printf("\n");
  }
  printf("nonzero total are: %d \n", nztotal);
  printf("how many pairs: %d\n", input->matches_array_size);
  printf("Weights number is: %d\n", input->weight_size);
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

double* Vector_Concat(double* vector1, double* vector2, int size)
{
	double* concat_vec = malloc(2*size*sizeof(double));
	memset(concat_vec, 0, 2*size*sizeof(double));
	int sum1 = 0, sum2 = 0;
	for(int i=0; i<size ; i++)
	{
		sum1 += vector1[i];
		sum2 += vector2[i];
	}
	if(sum1 > sum2)
	{
		memcpy(concat_vec, vector1, size*sizeof(double));
		memcpy(&concat_vec[size] , vector2, size*sizeof(double));
	}
	else
	{
		memcpy(concat_vec, vector2, size*sizeof(double));
		memcpy(&concat_vec[size] ,vector1, size*sizeof(double));
	}
	return concat_vec;
}

float CalculateP(Input* in, Model* model)
{
	float p = 0;
	for(int i=0; i<in->matches_array_size; i++)
	{
		p += P(in->vector_array[i], model);
	}
	return p;
}


// Model* Training(Model* model, Input* input)
// {
// 	int correct_value;
// 	float result;
// 	double* vectror_array, old_weight;

// 	model = InitializeModel(input->weight_size);

// 	int j=0;
// 	double p , parray[input->matches_array_size];
// 	int max_train = 1;
// 	int ttimes = 0;

// 	for(int i=0; i<input->matches_array_size; i++)
// 	{
// 		parray[i] = P(input->vector_array[i], model);
// 	}

// 	// ============================================================
// 	int i = 0;
// 	// while(i<input->weight_size)
// 	for(int i=0; i<input->weight_size; i++)
// 	{
// 		// double p = parray[];
// 		result = 0.0;
// 		// for(int j=0; j<10000; j++)
// 		for(int j=0; j<input->matches_array_size; j++)
// 		{
// 			double p = parray[j];
// 			correct_value = input->matches_array[j];
// 			result += (p-correct_value)*input->vector_array[j][i];
// 		}
// 		// model->weight_array[j] -= result;
// 		old_weight = model->weight_array[i];
// 		model->weight_array[i] = model->weight_array[i] - (LEARNING_RATE*(result));
// 		// if((model->weight_array[i] - old_weight) < VERY_SMALL_NUMBER )	//telos train gi auth th leksh
// 		// {
// 		// 	i++;
// 		// 	ttimes = 0;
// 		// }
// 		// else
// 		// 	ttimes++;
// 		// if(max_train == ttimes)
// 		// {
// 		// 	i++;
// 		// 	ttimes = 0;
// 		// }
// 	}

// 	return model;
// }

// Model* Training(Model* model, Input* input)
// {
// 	int correct_value;
// 	float result;
// 	double* vectror_array, old_weight;

// 	model = InitializeModel(input->weight_size);

// 	int j=0;
// 	double p , parray[input->matches_array_size];
// 	int max_train = 1;
// 	int ttimes = 0;

// 	for(int i=0; i<input->matches_array_size; i++)
// 	{
// 		parray[i] = P(input->vector_array[i], model);
// 	}

// 	for(int i=0; i<input->matches_array_size; i++)
// 	// for(int i=0; i<10000; i++)
// 	{
// 		result = 0.0;
// 		correct_value = input->matches_array[i];
// 		for(int j=0; j<input->weight_size; j++)
// 		{
// 			result = (parray[i] - correct_value)* input->vector_array[i][j];
// 			old_weight = model->weight_array[j];
// 			model->weight_array[j] = model->weight_array[j] - (LEARNING_RATE*(result));
// 		}
// 		model->b = (model->b + (parray[i] - correct_value))/2;
// 		parray[i] = P(input->vector_array[i], model);
// 	}

// 	return model;
// }

Model* Training(Model* model, Input* input)
{
	int correct_value[input->matches_array_size];
	float result;
	double* vectror_array, old_weight;

	model = InitializeModel(input->weight_size);

	int j=0;
	double p , parray[input->matches_array_size];
	int max_train = 1;
	int ttimes = 0;

	for(int i=0; i<input->matches_array_size; i++)
	{
		parray[i] = P(input->vector_array[i], model);
		correct_value[i] = input->matches_array[i]; 
	}

	for(int i=0; i<input->weight_size; i++)
	{
		result = 0.0;
		// for(int j=0; j<10000; j++)
		for(int j=0; j<input->matches_array_size; j++)
		{
			result = (parray[j] - correct_value[j]) * input->vector_array[j][i];
			old_weight = model->weight_array[i];
			model->weight_array[i] = model->weight_array[i] - (LEARNING_RATE*(result));
			model->b = (float)(model->b + (parray[i] - correct_value[j]))/(float)2;		
		}
	}

	return model;
}


void Testing(Input* input, Model* model)
{
	int correct_value;
	float result , i;
	double* vectror_array, old_weight;

	int j=0, correct=0, wrong=0;
	double p;
	int prediction;

	for(int i=0; i<input->matches_array_size; i++)
	{
		correct_value = input->matches_array[i];
		// p = P(input->vector_array[i], model);
		// if(i <3)
		{			
			p = P(input->vector_array[i], model);
			// printf("p is %f\n", p);
		}
		prediction = PredictMatch(p);

		if(prediction == correct_value)
			correct++;
		else
			wrong++;
	}
	printf("Success rate is: %f\n", ((float)correct / (float)input->matches_array_size));
}