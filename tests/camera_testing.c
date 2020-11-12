#include "acutest.h"
#include "../cameras.h"

void test_Value_Node_Init(void)
{
	char value_str[] = "test_value";
	value_node* vn =  Value_Node_Init(value_str);

	TEST_ASSERT(vn != NULL);
	TEST_ASSERT(strcmp(vn->value,value_str) == 0);

	Delete_Value_Node(vn);
}

void test_Value_List_Init(void)
{
	value_list* vl = Value_List_Init();

	TEST_ASSERT(vl != NULL);

	Delete_Value_List(vl);
}

void test_Add_to_value_list(void)
{
	value_list* vl = Value_List_Init();
	char value_str[] = "test_value";
	value_node* vn =  Value_Node_Init(value_str);

	Add_to_value_list(vl, vn);

	// testing first add
	TEST_ASSERT(vl->first == vn);
	TEST_ASSERT(vl->last == vn);

	value_node* vn_ptr = vl->first;

	// testing multiple adds
	for(int i=0; i<5; i++)
	{
		value_node* vn =  Value_Node_Init(value_str);
		Add_to_value_list(vl, vn);
		// this last node is not added at the start of the list
		TEST_ASSERT(vl->first != vn);
		// its added at the end
		TEST_ASSERT(vl->last == vn);
		// its connected to the previous last node of the list
		TEST_ASSERT(vn_ptr->next == vn);
		vn_ptr = vn_ptr->next;
	}

	Delete_Value_List(vl);
}

void test_Spec_node_Init(void)
{
	char* key = "test_key";
	value_list* vl = Value_List_Init();
	Spec_node* sn = Spec_node_Init(key, vl);

	TEST_ASSERT(sn != NULL);
	TEST_ASSERT(strcmp(sn->key, key) == 0);
	TEST_ASSERT(sn->valuelist == vl);
	TEST_ASSERT(sn->next == NULL);

	Delete_Spec_node(sn);
}

void test_Spec_List_Init(void)
{
	Spec_List* sl = Spec_List_Init();
	TEST_ASSERT(sl != NULL);

	Delete_Spec_List(sl);
}

void test_Specs_Add_Node(void)
{
	Spec_List* sl = Spec_List_Init();

	char* key = "test_key";
	value_list* vl = Value_List_Init();
	Spec_node* sn = Spec_node_Init(key, vl);

	Specs_Add_Node(sl, sn);

	// testing first add
	TEST_ASSERT(sl->first == sn);
	TEST_ASSERT(sl->last == sn);

	Spec_node* sn_ptr = sl->first;

	// testing multiple adds
	for(int i=0; i<5; i++)
	{
		value_list* vl = Value_List_Init();
		Spec_node* sn = Spec_node_Init(key, vl);

		Specs_Add_Node(sl, sn);

		// this last node is not added at the start of the list
		TEST_ASSERT(sl->first != sn);
		// its added at the end
		TEST_ASSERT(sl->last == sn);
		// its connected to the previous last node of the list
		TEST_ASSERT(sn_ptr->next == sn);
		sn_ptr = sn_ptr->next;
	}

	Delete_Spec_List(sl);
}

void test_Camera_Init(void)
{
	char* name = "test name";
	Camera* c = Camera_Init(name);

	TEST_ASSERT(c != NULL);
	TEST_ASSERT(strcmp(c->id, name) == 0);
	TEST_ASSERT(c->spec_List != NULL);

	Delete_Camera(c);
}


TEST_LIST ={
	{"Value_Node_Init", test_Value_Node_Init},
	{"Value_List_Init", test_Value_List_Init},
	{"Add_to_value_list", test_Add_to_value_list},
	{"Spec_node_Init", test_Spec_node_Init},
	{"Spec_List_Init", test_Spec_List_Init},
	{"Specs_Add_Node", test_Specs_Add_Node},
	{"Camera_Init", test_Camera_Init},
	{NULL,NULL}
};
