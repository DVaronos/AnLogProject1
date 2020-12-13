#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "model.h"

int main(int argc, char const *argv[])
{
	float vec_array[] = {0.0587, 0.0, 0.0, 0.16, 0.16, 0.0};
	int vec_size = 6;
	Model* model = IninializeModel(vec_size);

	float p = P(vec_array, model);
	printf("p(x) is : %f\n", p);

	printf("ismatch: %d\n", IsMatch(p));

	FreeModel(model);
	return 0;
}