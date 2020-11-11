#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "json_read.h"


int Read_from_JSON_file(char* filename, Camera* S)
{
	FILE *fp;
	fp = fopen(filename,"r");
	if(fp == NULL)
		return 0;

	Read_Camera(fp, S);

	fclose(fp);
	return 1;
}



int Read_File_rec(char* filename)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(filename);

    char *site_name = strtok(filename, "_");

    if (!dir)
        return 0;

    printf("%s\n", filename);

    while(site_name != NULL)
    {
    	printf("%s\n", site_name);
    	site_name = strtok(NULL, "_");
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
          
            strcpy(path, filename);
            strcat(path, "/");
            strcat(path, dp->d_name);

            Read_File_rec(path);
        }
    }

    free(site_name);

    closedir(dir);
}
