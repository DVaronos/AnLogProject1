#include <stdio.h>
#include <string.h>

typedef struct Match_node
{
	char* match_id;
	struct Match_node* next;
};

typedef struct Matches
{
	struct Match_node* first;
	struct Match_node* last;
};

typedef struct Spec
{
	char* id;
	struct Matches* matches;
};

typedef struct Specs_List_Node
{
	Spec* spec;
	Spec* next;
};

typedef struct Specs_List
{
	Specs_List_Node* first_spec;
	Specs_List_Node* last_spec;
};


Specs_List_Node* Specs_List_Node_Init(Spec* sp);
Specs_List* Specs_List_Init(void);
void Specs_List_Add_Spec(Specs_List* sl, Spec* sp);
Match_node* Match_node_Init(char* m_id);
Matches* Matches_Init(void);
void Matches_Add_Node(Matches* matches, Match_node* mn);
Spec* Spec_Init(char* name);





