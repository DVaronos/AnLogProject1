#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "logistic.h"

HVector* GetCameraVector(char* camera_id, Hash* H)
{
	NList *L;
	int i=0,j=0,in;
	char name[20];
	while(camera_id[i]!='/')
	{//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
		i++;
	}
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(camera_id))
	{
		name[j]=camera_id[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	int index=hash(in,H->size);
	L=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
	L = L->Next;
	while(L!=NULL)
	{
		if(strcmp(L->camera,camera_id) == 0)	return L->vector;
		L=L->Next;
	}
}

HVector* VectorConcat(HVector* F,HVector* S,int dif){
	int size=(F->count)+(S->count),c=0;
	size=size+size*0.3;
	HVector* C=CreateHVector(size);

	int fr,se;
	fr=HVSumValues(F);
	se=HVSumValues(S);
	if(fr>se){//Eelgxo pio ap ta dio vectors tha mpei proto
		c=0;
		for(int i=0 ; i<F->size ; i++){	//Isago tis leksis tou protou vector
			if(F[i].key!=-1){
				c++;
				C=InsertHVector(C,F[i].key,F[i].value);
			}
			if(c==F->count) break;
		}
		c=0;
		for(int i=0 ; i<S->size ; i++){	//Isago tis leksis tou defterou vectour me afksimeno to key kata dif
			if(S[i].key!=-1){
				c++;
				C=InsertHVector(C,(S[i].key)+dif,S[i].value);
			}
			if(c==S->count) break;
		}

	}else{
		c=0;
		for(int i=0 ; i<S->size ; i++){
			if(S[i].key!=-1){
				c++;
				C=InsertHVector(C,S[i].key,S[i].value);
			}
			if(c==S->count) break;
		}
		c=0;
		for(int i=0 ; i<F->size ; i++){
			if(F[i].key!=-1){
				c++;
				C=InsertHVector(C,(F[i].key)+dif,F[i].value);
			}
			if(c==F->count) break;
		}
	}
	return C;
}

Input* InputInit(){
	Input* in = malloc(sizeof(Input));
	in->Cons = NULL;
	in->matches = NULL;
	in->size = 0;
	return in;
}

void FreeInput(Input* input)
{
	free(input->matches);
	for(int i =0; i<input->size; i++)
	{
		FreeHVector(input->Cons[i]);
	}
	free(input->Cons);
	free(input);
}

Input* InputMake(char* filename1,char* filename2,Hash* H){

	FILE* fptr;
	int   a,z,ismatch,size = H->Head->Next->vec_size,count=0;
	char line[300];
	char* first;
	char* second;
	char* token=NULL;
	HVector* F;
	HVector* S;
	HVector* Con;
	Input* input=InputInit();

	fptr = fopen(filename1, "r");	//Anigma tou protou arxiou(me tis thetikes sisxetiseis)
	ismatch=1;	//oi sisxetiseis einai oles thetikes
	a=0;
	z=0;
	for(int i=0 ; i<2; i++){ //Gia kathe ena ap ta dio arxia

		while (fgets(line,sizeof(line),fptr)){//Diavazei to csv file grami grami
			if(!i){	//Ean eimaste sto proto arxio
					a++;	//Metrame tis thetikes sisxetisis
			}else{	//Ean eimaste sto deftero arxio
					if(z==2*a){	//Ean oi arnitikes sisxetisis einai oi diplasies stamatame to trainig gia afthn thn epanalipsh
							break;
					}else{	//Alios afksanoume tis arnitikes sisxetisis
							z++;
					}
			}
			token=strtok(line,",");
			if(!strcmp(token,"left_spec_id"))  continue;	//Ean einai h proth gramh twn csv kanoume skip thn epanalipsh
			first=strdup(token);
			token = strtok(NULL, " \n");
			second=strdup(token);
			F=GetCameraVector(first,H);	//Pernoume to vector ths proths kameras
			S=GetCameraVector(second,H);	//Pernoume to vector ths defterhs kameras
			Con=VectorConcat(F,S,size);	//Ipologizoume to concatenation

			input->Cons=realloc(input->Cons, (count+1)*sizeof(HVector*));
			input->Cons[count] = Con;

			input->matches= realloc(input->matches, (count+1)*sizeof(int));
			input->matches[count] = ismatch;
			count++;


			free(first);
			free(second);
		}
		fclose(fptr);//klinoume to arxio
		if(!i){	//Ean imastan sto proto arxio anoigoume to deftero(me tis arnitikes sisxerisis)
			fptr = fopen(filename2, "r");
			ismatch = 0;
		}
	}
	input->size=count;
	return input;
}



Model Training(Input* input,Hash* H){

	FILE* fptr;
	int size = H->Head->Next->vec_size,tc=0,c=0;
	double k,p,result;
	Model model;
	model.array_size=2*size;
	model.weight_array=malloc(sizeof(double)*(model.array_size));
	memset(model.weight_array, 0,(model.array_size)*sizeof(double));
	model.b=0;
	HVector* Con;
	while(tc<3){	//H ekpedefsth twv varewn tha ginei 5 fores
		tc++;
		for(int i=0 ; i<input->size ; i++){
				Con=input->Cons[i];
      	p=Predict(model,Con);	//Ipologizoume thn provlepsh tou modelou
			//	printf("gia to %do vrika p:%f\n",i,p );
      	k=p-input->matches[i];	//Ipologizoume thn apoklhsh ths provlepshs
				c=0;
      	for(int j=0 ;  j<Con->size ; j++){	//Gia kathe varos
					if(Con[j].key!=-1){
						c++;
						result=k*Con[j].value;	//Ipologizoume to ginomeno ths apoklishs epi thn timh tou concat se afthn thn diastash
						model.weight_array[Con[j].key]=model.weight_array[Con[j].key]-0.6*result;
						model.b = (double)(model.b +k)/(double)2;	//enimeronoume to b
					}
					if(c==Con->count) break;
      	}
    }
	}
  return model;
}

void Testing(char* filename,Model model,Hash* H){
  FILE* fptr = fopen(filename, "r");
  int tcount,match;
  char line[300];
  char* token=NULL;
  char* first;
  char* second;
  double p;
  double correct=0,sum=0;;
  HVector* F;
  HVector* S;
  HVector* Con;
  while (fgets(line,sizeof(line),fptr)){//Diavazei to csv file grami grami
    token=strtok(line,",");
    if(!strcmp(token,"left_spec_id"))  continue;
    tcount=0;
    while(token!=NULL){//apothikefsi tou kathe stixoiou ths gramhs se metavlites
     tcount++;
     switch(tcount){
       case 1://ean tcount==1 tote to token isoute me to proto proion
         first=malloc(sizeof(char)*(strlen(token)+1));
         strcpy(first,token);
         break;
       case 2://ean tcount==2 tote to token isoute me to deftero proion
         second=malloc(sizeof(char)*(strlen(token)+1));
         strcpy(second,token);
         break;
       case 3://Ean tcount==3 tote to token einai 0 h 1 analoga to an teriazoun ta proionta
         match=atoi(token);
         break;
     }
     token=strtok(NULL,",");
   }
   F=GetCameraVector(first,H);
   S=GetCameraVector(second,H);
   Con=VectorConcat(F,S,model.array_size/2);
   p=Predict(model,Con);
	 double cor;
   cor=1-p;
   sum++;
   if(cor>0.5){
		 //printf("Gia %s kai %s exw einai %d kai exw %f kaiii %f\n",first,second,match,p,1-p );
     if(!match){
			 correct++;
		//	printf("SOSTO:Gia %s kai %s exw einai %d kai exw %f\n",first,second,match,p );
		 }else{
			// printf("LATHOS:Gia %s kai %s exw einai %d kai exw %f\n",first,second,match,p );
		 }

   }else{
     if(match){
			  correct++;
				//printf("SOSTO:Gia %s kai %s exw einai %d kai exw %f\n",first,second,match,p );
		}else{
			//printf("LATHOS:Gia %s kai %s exw einai %d kai exw %f\n",first,second,match,p );
		}
   }
   free(first);
   free(second);
	 free(Con);
 }
 fclose(fptr);
 printf("Success rate is:%f\n",correct/sum);
}


double Fx(Model model,HVector* con){
  double sum=model.b,wx;
	int c=0;
  for(int i=0; i<con->size ; i++){	//Ipologismos tou f(X)
		if(con[i].key!=-1){
			c++;
			wx=model.weight_array[con[i].key]*con[i].value;
			sum+=wx;
		}
		if(c==con->count) break;
	}
  return sum;
}

double Predict(Model model,HVector* con){
  double f=Fx(model,con);	//ipologizoume to f(x)
  double p = 1 / (1 + exp(-(f)));	//ipologizoume to p(f(x))
	return p;
}


void TestAllData(Hash* H,Model model){
	NList* E;
	NList* M;
	HVector* F;
  HVector* S;
  HVector* Con;
	double p;
	NList* T1;
	NList* T2;
	int ca=0;
	for(int i=0 ; i<H->size ; i++){	//Gia kathe bucket-list tou Hash
		E=(NList*)H[i].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
		T1=E;
		while(T1->Next!=NULL){	//Arxika elegxw ta dedonena ths Listas metaksi tous
			T1=T1->Next;	//Gia kathe camera ths listas
			if(T1->clique==NULL){	//Ean h kamera den anoikei se kapia klika
				F=GetCameraVector(T1->camera,H);	//Apothikevo to vector ths cameras
				T2=T1;
				while(T2->Next!=NULL){	//Gia olou tous epomenous komvous ths listas
					T2=T2->Next;
					if(T2->clique==NULL){	//Ean h camera den anoikei se kapia klika kai den einai h idia me thn prohgoumeni
						ca++;
						S=GetCameraVector(T2->camera,H);	//Apothikevo to vector ths
						Con=VectorConcat(F,S,model.array_size/2);	//Vrisko to concatenation
						p=Predict(model,Con);	//Vrisko thn provlepsh
						if(p<0.002 || p>0.998) printf("%d,%s me %s exoun %f\n",ca,T1->camera,T2->camera,p);
						FreeHVector(Con);
					}
				}
			}
		}
		for(int j=i+1 ; j<H->size ; j++){//Gia kathe epomenh bucket list
			E=(NList*)H[i].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable

			while(E->Next!=NULL){ //Gia kathe komvo ths listas
					E=E->Next;
					if(E->clique==NULL){	//Ean h camera den anoikei se kapia klika
						M=(NList*)H[j].Head;//pernw thn lista pou vriskete sto j bucket tou HashTable
						F=GetCameraVector(E->camera,H);	//Vrisko to vector ths cameras
						while(M->Next!=NULL){	//Gia kathe komvo ths defterhs listas
							M=M->Next;
							if(M->clique==NULL){ //Ean h camera den anoikei se kapia klika kai den einai h idia me thn prohgoumeni
								ca++;
						    S=GetCameraVector(M->camera,H); //Apothikevo to vector ths
						    Con=VectorConcat(F,S,model.array_size/2); //Vrisko to concatenation
								p=Predict(model,Con); //Vrisko thn provlepsh
								if(p<0.002 || p>0.998) printf("%d,%s me %s exoun %f\n",ca,E->camera,M->camera,p);
								FreeHVector(Con);
							}
						}
					}
			}
		}
	}
	printf("Htan sinolo %d\n",ca);
}

double Norm(Model model){	//Ipologismos ths normas tou vector  tou modleou
	int sum=0;
	for(int i=0 ; i<model.array_size ; i++){
		sum+=model.weight_array[i]*model.weight_array[i];
	}
	return sqrt(sum);
}

// =====================================

Input TestAndAdd(Input* initial_input,Model* model, Hash* H, char* filename, float threshold)
{
	FILE* fptr = fopen(filename, "r");
	int tcount, ismatch;
	char line[300], *first, *second, *token=NULL;
	double p, correct=0, sum=0;
	HVector *F, *S;
	HVector *Con;

	while (fgets(line,sizeof(line),fptr)){//Diavazei to csv file grami grami
	  token=strtok(line,",");
	  if(!strcmp(token,"left_spec_id"))  continue;
	  tcount=0;
	  while(token!=NULL){//apothikefsi tou kathe stixoiou ths gramhs se metavlites
	   tcount++;
	   switch(tcount){
		 case 1://ean tcount==1 tote to token isoute me to proto proion
		   first=malloc(sizeof(char)*(strlen(token)+1));
		   strcpy(first,token);
		   break;
		 case 2://ean tcount==2 tote to token isoute me to deftero proion
		   second=malloc(sizeof(char)*(strlen(token)+1));
		   strcpy(second,token);
		   break;
		   // den xreiazetai h 3h sthlh gt emeis 8a kanoume mono predict kai oxi elegxo me pragmatikh timh
		 // case 3://Ean tcount==3 tote to token einai 0 h 1 analoga to an teriazoun ta proionta
		 //   match=atoi(token);
		 //   break;
	   }
	   token=strtok(NULL,",");
	 }
	 F=GetCameraVector(first,H);
	 S=GetCameraVector(second,H);
	 Con=VectorConcat(F,S,model->array_size/2);
	 p=Predict(*model,Con);

	 if(p<threshold)	//add as not match
	 {
		 ismatch = 0;
		 if(CheckIfOpposite(H, first, second) == 0)	//an den uparxei hdh arnhtikh susxethsh
		 	HashConect(H, first, second, ismatch);
	 }
	 else if(p> 1- threshold)	//add as match
	 {
		 ismatch = 1;
		 if(CheckIfOpposite(H, first, second) == 0)
		 	HashConect(H, first, second, ismatch);
	 }

	 // pros8hkh tou neou vector sto palio input
	 initial_input->Cons=realloc(initial_input->Cons, (initial_input->size +1)*sizeof(HVector*));
	 initial_input->Cons[initial_input->size] = Con;

	 initial_input->matches= realloc(initial_input->matches, (initial_input->size +1)*sizeof(int));
	 initial_input->matches[initial_input->size] = ismatch;
	 initial_input->size++;

	 free(first);
	 free(second);
	   free(Con);
   }
   return *initial_input;
}

Model RepetitiveTaining(Input* initial_input, Hash* H, Input* test_set)
{
	int size = H->Head->Next->vec_size;
	float threshold = 0.1;
	float step_value = 0.2;
	Input* training_set = initial_input;
	Model* model = malloc(sizeof(Model));
	// model initialization
	model->array_size=2*size;
	model->weight_array=malloc(sizeof(double)*(model->array_size));
	memset(model->weight_array, 0,(model->array_size)*sizeof(double));
	model->b=0;

	while (threshold <= 0.5)
	{
		*model = Training(training_set, H);
		*training_set = TestAndAdd(training_set, model, H, "Testing.csv", threshold);
		threshold += step_value;
	}
	return *model;
}
