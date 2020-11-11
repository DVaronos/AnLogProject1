#include <stdio.h>
#include <string.h>

//--------------------Domh valulist-----------------------------\\

typedef struct value_node
{
	char* value;
	struct value_node* next;
}value_node;


typedef struct value_list
{
	value_node* first;
	value_node* last;
}value_list;

value_node* Value_Node_Init(char* value_str);
void Value_node_Print(value_node* vn);
int Delete_Value_Node(value_node* vn);


void Add_to_value_list(value_list* vl, value_node* vn);

value_list* Value_List_Init(void);
void Value_List_Print(value_list* vl);
int Delete_Value_List(value_list* vl);


//--------------------Domh Spec_List-----------------------------\\

typedef struct Spec_node
{
	char* key;
	struct value_list* valuelist;
	struct Spec_node* next;
}Spec_node;

typedef struct Spec_List
{
	struct Spec_node* first;
	struct Spec_node* last;
}Spec_List;

Spec_node* Spec_node_Init(char* key, value_list* vl);
void Spec_Node_Print(Spec_node* sn);
void Delete_Spec_node(Spec_node* sn);
Spec_node* Read_Spec(FILE* fp, Spec_node* sn);

void Specs_Add_Node(Spec_List* sl, Spec_node* sn);

Spec_List* Spec_List_Init(void);
void Spec_List_Print(Spec_List* sl);
int Delete_Spec_List(Spec_List* sl);

//--------------------Domh camera-----------------------------\\


typedef struct Camera
{
	char* id;
	struct Spec_List* spec_List;
}Camera;


Camera* Camera_Init(char* name);
Camera* Read_Camera(FILE* fp, Camera* C);

void Camera_Print_Id(Camera* camera);
void Camera_Print_Specs(Camera* camera);
void Delete_Camera(Camera* camera);

void Read_key(FILE* fp, char** key_str);
void Read_field(FILE* fp, char** field_str);
void Read_value_list(FILE* fp, value_list* vl, int value_is_list);
