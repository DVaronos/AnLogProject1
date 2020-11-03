#ifndef STRUCTS_N_FUNCTS_H
#define STRUCTS_N_FUNCTS_H

#include <stdio.h>
#include <string.h>

typedef struct Spec_node
{
	char* key;
	char* value;
	struct Spec_node* next;
}Spec_node;

typedef struct Spec_List
{
	struct Spec_node* first;
	struct Spec_node* last;
}Spec_List;

typedef struct Camera
{
	char* id;
	struct Spec_List* spec_List;
}Camera;



Camera* Camera_Init(char* name);
void Camera_Print_Id(Camera* camera);
void Camera_Print_Specs(Camera* camera);

Spec_node* Spec_node_Init(char* key, char* value);
void Spec_Node_Print(Spec_node* sn);


Spec_List* Spec_List_Init(void);
void Specs_Add_Node(Spec_List* sl, Spec_node* sn);
void Spec_List_Print(Spec_List* sl);


void Delete_Spec_node(Spec_node* sn);
int Delete_Spec_List(Spec_List* sl);
void Delete_Camera(Camera* camera);


#endif

// typedef struct Cameras_List_Node
// // {
// // 	Camera* Camera;
// // 	struct Cameras_List_Node* next;
// // }Cameras_List_Node;

// // typedef struct Cameras_List
// // {
// // 	Cameras_List_Node* first_Camera;
// // 	Cameras_List_Node* last_Camera;
// // }Cameras_List;


// Cameras_List_Node* Cameras_List_Node_Init(Camera* sp);
// Cameras_List* Cameras_List_Init(void);
// void Cameras_List_Add_Camera(Cameras_List* sl, Camera* sp);


