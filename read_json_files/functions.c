#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs_n_functs.h"


Spec_node* Spec_node_Init(char* key, char* value)
{
	Spec_node* sn = malloc(sizeof(Spec_node));
	sn->key = strdup(key);
	sn->value = strdup(value);
	sn->next = NULL;
	return sn;
}

Spec_List* Spec_List_Init(void)
{
	Spec_List* sl = malloc(sizeof(Spec_List));
	sl->first = NULL;
	sl->last = NULL;
	return sl;
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

Camera* Camera_Init(char* name)
{
	Camera* camera = malloc(sizeof(Camera));
	camera->id = name;
	camera->spec_List = Spec_List_Init();
	// Spec_node* sn = Spec_node_Init(name);
	// Specs_Add_Node(sl, sn);
	return camera;
}

void Camera_Print_Id(Camera* camera)
{
	printf("Id: %s\n", camera->id);
}


void Delete_Spec_node(Spec_node* sn)
{
	if(sn->next == NULL)
		printf("NO NEXT FOR THIS SPEC NODE\n");
	free(sn->key);
	free(sn->value);
	free(sn);
}

int Delete_Spec_List(Spec_List* sl)
{

	if(sl == NULL)
	{	
		printf("EMPTY LIST\n");
		return 1;
	}

	Spec_node* sn = sl->first;

	if(sn == NULL)
	{
		printf("EMPTY LIST\n");
		return 1;
	}

	Spec_node* sn_next = sl->first->next;

	while(1)
	{
		// free(sn);
		Delete_Spec_node(sn);
		if(sn_next == NULL)
			return 1;
		sn = sn_next;
		sn_next = sn_next->next;
	}
}


void Delete_Camera(Camera* camera)
{
	Delete_Spec_List(camera->spec_List);
	free(camera);
}

void Camera_Print_Specs(Camera* camera)
{
	printf("\nCAMERA SPECS\n\n");
	Spec_List_Print(camera->spec_List);
	printf("\n");
}

void Spec_Node_Print(Spec_node* sn)
{
	if(sn != NULL)
		printf("%s : %s\n",sn->key, sn->value );
}

void Spec_List_Print(Spec_List* sl)
{
	Spec_node* sn_ptr = sl->first;
	while(sn_ptr != NULL)
	{
		Spec_Node_Print(sn_ptr);
		sn_ptr = sn_ptr->next;
	}
}

// --------------------------------------------------------------------------------------

// Cameras_List_Node* Cameras_List_Node_Init(Camera* sp)
// {
// 	Cameras_List_Node* sp_node = malloc(sizeof(Cameras_List_Node));
// 	sp_node->Camera = sp;
// 	sp_node->next = NULL;
// }

// Cameras_List* Cameras_List_Init(void)
// {
// 	Cameras_List* sl = malloc(sizeof(Cameras_List));
// 	sl->first_Camera = NULL;
// 	sl->last_Camera = NULL;
// }

// void Cameras_List_Add_Camera(Cameras_List* sl, Camera* sp)
// {
// 	Cameras_List_Node* Camera_node = Cameras_List_Node_Init(sp);
// 	if(sl->first_Camera == NULL)
// 	{
// 		sl->first_Camera = Camera_node;
// 	}
// 	else
// 	{
// 		sl->last_Camera->next = Camera_node;
// 	}
// 	sl->last_Camera = Camera_node;
// }