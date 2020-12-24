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
	//printf("pame\n");
	int size=(F->count)+(S->count),c=0;
	size=size+size*0.3;
	//printf("exw size %d\n",size);
	HVector* C=CreateHVector(size);

	int fr,se;
	fr=HVSumValues(F);
	//printf("exw fr %f\n",fr);
	se=HVSumValues(S);
//	printf("exw se %f\n",se);
	if(fr>se){
		c=0;
		for(int i=0 ; i<F->size ; i++){
			if(F[i].key!=-1){
				c++;
				C=InsertHVector(C,F[i].key,F[i].value);
			}
			if(c==F->count) break;
		}
		c=0;
		for(int i=0 ; i<S->size ; i++){
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
//	printf("ok h hvector\n" );
	return C;
}


Model Training(char* filename1 ,char* filename2,Hash* H){

  FILE* fptr;
  int   ismatch,z,a,size = H->Head->Next->vec_size,c=0;
	int tc=0;
	double k,result,p;
  char line[300];
  char* first;
  char* second;
  char* token=NULL;
	HVector* F;
  HVector* S;
  HVector* Con;
	Model model;
	model.array_size=2*size;
  model.weight_array=malloc(sizeof(double)*(model.array_size));
  memset(model.weight_array, 0,(model.array_size)*sizeof(double));
	model.b=0;
	while(tc<3){	//H ekpedefsth twv varewn tha ginei 5 fores
		tc++;
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
      	Con=VectorConcat(F,S,model.array_size/2);	//Ipologizoume to concatenation
      	p=Predict(model,Con);	//Ipologizoume thn provlepsh tou modelou

      	k=p-ismatch;	//Ipologizoume thn apoklhsh ths provlepshs
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
      	FreeHVector(Con);
      	free(first);
      	free(second);
    	}
    	fclose(fptr);//klinoume to arxio
    	if(!i){	//Ean imastan sto proto arxio anoigoume to deftero(me tis arnitikes sisxerisis)
      	fptr = fopen(filename2, "r");
  			ismatch = 0;
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


void OlaGiaOla(Hash* H,Model model){
	NList* E;
	NList* M;
	HVector* F;
  HVector* S;
  HVector* Con;
	double p;
	NList* T1;
	NList* T2;
	int ca=0;
	for(int i=0 ; i<H->size ; i++){
		E=(NList*)H[i].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
		T1=E;
		while(T1->Next!=NULL){
			T1=T1->Next;
			if(T1->clique==NULL){
				F=GetCameraVector(T1->camera,H);
				T2=T1;
				while(T2->Next!=NULL){
					T2=T2->Next;
					if(T2->clique==NULL && strcmp(T1->camera,T2->camera)){
						ca++;
						S=GetCameraVector(T2->camera,H);
						Con=VectorConcat(F,S,model.array_size/2);
						p=Predict(model,Con);
						if(p<0.002 || p>0.998) printf("%d,%s me %s exoun %f\n",ca,T1->camera,T2->camera,p);
						FreeHVector(Con);
					}
				}
			}
		}
		for(int j=i+1 ; j<H->size ; j++){
			E=(NList*)H[i].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable

			while(E->Next!=NULL){
					E=E->Next;
					if(E->clique==NULL){
						M=(NList*)H[j].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
						F=GetCameraVector(E->camera,H);
						while(M->Next!=NULL){
							M=M->Next;
							if(M->clique==NULL && strcmp(E->camera,M->camera) ){
								ca++;
						    S=GetCameraVector(M->camera,H);
						    Con=VectorConcat(F,S,model.array_size/2);
								p=Predict(model,Con);
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

double Norm(Model model){
	int sum=0;
	for(int i=0 ; i<model.array_size ; i++){
		sum+=model.weight_array[i]*model.weight_array[i];
	}
	return sqrt(sum);
}
