
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

void FreeWeightArray(float* weight_array);
void FreeModel(Model* model);
