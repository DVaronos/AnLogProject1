#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cameras.h"

//--------------------Sinartiseis gia thn value_list-----------------------------\\


value_node* Value_Node_Init(char* value_str)
{
	value_node* vn = malloc(sizeof(value_node));
	vn->value = strdup(value_str);
	vn->next = NULL;
	return vn;
}


void Value_node_Print(value_node* vn)
{
	printf("%s\n", vn->value);
}


int Delete_Value_Node(value_node* vn)
{
	if(vn == NULL)
	{
		return 0;
	}
	else if(vn->next == NULL)
	{
		free(vn->value);
		free(vn);;
		return 0;
	}
	free(vn->value);
	free(vn);
	return 1;
}


void Add_to_value_list(value_list* vl, value_node* vn)
{
	if(vl->first == NULL)
	{
		vl->first = vn;
	}
	else
	{
		vl->last->next = vn;
	}
	vl->last = vn;
}


value_list* Value_List_Init(void)
{
 value_list* vl = malloc(sizeof(value_list));
 vl->first = NULL;
 vl->last = NULL;
}


void Value_List_Print(value_list* vl)
{
	if(vl != NULL)
	{
		value_node* vn_ptr = vl->first;
		int count=0;
		while(vn_ptr != NULL)
		{	count++;
			printf("\t");
			Value_node_Print(vn_ptr);
			vn_ptr = vn_ptr->next;
		}
	}
	printf("=========================\n\n");
}

int Delete_Value_List(value_list* vl)
{

	if(vl == NULL)
	{
		printf("EMPTY LIST\n");
		free(vl);
		return 1;
	}

	value_node* vn = vl->first;

	if(vn == NULL)
	{
		printf("EMPTY LIST\n");
		free(vl);
		return 1;
	}

	value_node* vn_next = vl->first->next;

	while(1)
	{
		Delete_Value_Node(vn);
		if(vn_next == NULL)
		{
			free(vl);
			return 1;
		}
		vn = vn_next;
		vn_next = vn_next->next;
	}
}


//--------------------Sinartiseis gia thn Spec_List-----------------------------\\



Spec_node* Spec_node_Init(char* key, value_list* vl)
{
	Spec_node* sn = malloc(sizeof(Spec_node));
	sn->key = strdup(key);
	sn->valuelist = vl;
	sn->next = NULL;
	return sn;
}


void Spec_Node_Print(Spec_node* sn)
{
	if(sn != NULL)
	{
		printf("%s\n-------------------------\n",sn->key);
		Value_List_Print(sn->valuelist);
	}
}


void Delete_Spec_node(Spec_node* sn)
{
	free(sn->key);
	Delete_Value_List(sn->valuelist);
	free(sn);
}


Spec_node* Read_Spec(FILE* fp, Spec_node* sn)
{
	char* key_str;
	int value_is_list = 0;
	Read_key(fp,  &key_str);

	int c = fgetc(fp);
	while(c != '"')
	{
		if(c == '[')
			value_is_list = 1;
		c = fgetc(fp);
	}
	value_list* vl = Value_List_Init();
	Read_value_list(fp, vl, value_is_list);

	sn = Spec_node_Init(key_str, vl);
	free(key_str);
	c = fgetc(fp);
	return sn;
}


void Specs_Add_Node(Spec_List* sl, Spec_node* sn)
{
	if(sl->first == NULL)
	{
		sl->first = sn;
	}
	else
	{
		sl->last->next = sn;
	}
	sl->last = sn;
}


Spec_List* Spec_List_Init(void)
{
	Spec_List* sl = malloc(sizeof(Spec_List));
	sl->first = NULL;
	sl->last = NULL;
	return sl;
}


void Spec_List_Print(Spec_List* sl)
{
	if(sl != NULL)
	{
		Spec_node* sn_ptr = sl->first;
		int count=0;
		while(sn_ptr != NULL)
		{	count++;
			Spec_Node_Print(sn_ptr);
			sn_ptr = sn_ptr->next;
			printf("\n");
		}
	}
}


int Delete_Spec_List(Spec_List* sl)
{

	if(sl == NULL)
	{
		printf("EMPTY LIST\n");
		free(sl);
		return 1;
	}

	Spec_node* sn = sl->first;

	if(sn == NULL)
	{
		printf("EMPTY LIST\n");
		free(sl);
		return 1;
	}

	Spec_node* sn_next = sl->first->next;

	while(1)
	{
		Delete_Spec_node(sn);
		if(sn_next == NULL)
		{
			free(sl);
			return 1;
		}
		sn = sn_next;
		sn_next = sn_next->next;
	}
}


//--------------------Sinartiseis gia thn Camera-----------------------------\\


Camera* Camera_Init(char* name)
{
	Camera* camera = malloc(sizeof(Camera));
	camera->id =malloc(sizeof(char)*(strlen(name)+1));
	strcpy(camera->id,name);
	camera->spec_List = Spec_List_Init();
	return camera;
}

void Read_Camera(FILE* fp, Camera* C)
{
	int value_is_list=0;
	int c;
	Spec_List* sl = C->spec_List;
	while((c = fgetc(fp)) != EOF)
	{
		if( c == '"')
		{
			Spec_node* sn;
			sn = Read_Spec(fp, sn);
			Specs_Add_Node(sl, sn);
		}
		value_is_list = 0;
	}
}


void Camera_Print_Id(Camera* camera)
{
	printf("Id: %s\n", camera->id);
}

void Camera_Print_Specs(Camera* camera)
{
	Camera_Print_Id(camera);
	printf("\nCAMERA SPECS\n\n");
	Spec_List_Print(camera->spec_List);
	printf("\n");
}


void Delete_Camera(Camera* camera)
{
	free(camera->id);
	Delete_Spec_List(camera->spec_List);
	free(camera);
}


void Read_key(FILE* fp, char** key_str)
{
	Read_field(fp, key_str);
}


void Read_field(FILE* fp, char** field_str)
{
	char str[15000];
	memset(str, 0 , 100);
	int char_count =0;
	int c = fgetc(fp);
	while( c != '"')
	{
		if( c == 92)
		{
			c = fgetc(fp);
			if(c != '"')
			{
				str[char_count] = (char)92;
				char_count++;
			}
		}
		str[char_count] = (char)c;
		c = fgetc(fp);
		char_count++;
	}
	str[char_count]='\0';
	*field_str = strdup(str);
}


void Read_value_list(FILE* fp, value_list* vl, int value_is_list)
{
	int c = -1;
	char* value_str = NULL;

			Read_field(fp, &value_str);
		value_node* vn = Value_Node_Init(value_str);
		Add_to_value_list(vl, vn);
	free(value_str);

		c = fgetc(fp);

	if( value_is_list == 1)
	{
		while(c != ']')
		{
			if(c == '"')
			{
				Read_field(fp, &value_str);
				value_node* vn = Value_Node_Init(value_str);
				Add_to_value_list(vl, vn);
				c = fgetc(fp);
				free(value_str);
				continue;
			}
			c = fgetc(fp);
		}
	}
}
