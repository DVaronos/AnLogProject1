#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "JSON_read.h"
#include "structs_n_functs.h"

int main(int argc, char const *argv[])
{

	Read_File_rec("camera_specs/2013_camera_specs");

	// Camera *camera = Camera_Init("site//230");

	// Read_from_JSON_file("230.json", camera);

	// Camera_Print_Specs(camera);

	// Delete_Camera(camera);
}