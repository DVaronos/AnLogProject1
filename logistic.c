#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "logistic.h"

double* GetCameraVector(char* camera_id, Hash* H)
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

double* Vector_Concat(double* vector1, double* vector2, int size)
{
	double* concat_vec = malloc(2*size*sizeof(double));
	memset(concat_vec, 0, 2*size*sizeof(double));
	int sum1 = 0, sum2 = 0;
	for(int i=0; i<size ; i++)
	{
		sum1 += vector1[i];
		sum2 += vector2[i];
	}
	if(sum1 > sum2)
	{
		memcpy(concat_vec, vector1, size*sizeof(double));
		memcpy(&concat_vec[size] , vector2, size*sizeof(double));
	}
	else
	{
		memcpy(concat_vec, vector2, size*sizeof(double));
		memcpy(&concat_vec[size] ,vector1, size*sizeof(double));
	}
	return concat_vec;
}

Model Training(char* filename1 ,char* filename2,Hash* H){

  FILE* fptr = fopen(filename1, "r");
  int   c=0,ismatch=1,size = H->Head->Next->vec_size,z=0,a=0;
  char line[300];
  char* first;
  char* second;
  char* token=NULL;
  double* F;
  double* S;
  double* Con;
	Model model;
  model.weight_array=malloc(sizeof(double)*2*size);
  memset(model.weight_array, 0, 2*size*sizeof(double));
	model.b=0;
  double k,result,p;
  for(int i=0 ; i<2; i++){
    while (fgets(line,sizeof(line),fptr)){//Diavazei to csv file grami grami
			if(!i){
				a++;
			}else{
				if(z==a){
					break;
				}else{
					z++;
				}
			}
      token=strtok(line,",");
      if(!strcmp(token,"left_spec_id"))  continue;
      first=malloc(sizeof(char)*(strlen(token)+1));
      strcpy(first,token);
      token = strtok(NULL, " \n");
      second=malloc(sizeof(char)*(strlen(token)+1));
      strcpy(second,token);


      c++;

      F=GetCameraVector(first,H);
      S=GetCameraVector(second,H);
      Con=Vector_Concat(F,S, size);
      p=Predict(model,Con,2*size);
      k=p-ismatch;
      for(int i=0 ;  i<size*2 ; i++){
        result=k*Con[i];
        model.weight_array[i]=model.weight_array[i]-result;
				model.b = (double)(model.b +k)/(double)2;
      }

      free(Con);
      free(first);
      free(second);
    }
    fclose(fptr);
    if(!i){
      fptr = fopen(filename2, "r");
  		ismatch = 0;
    }
  }
  return model;
}

void Testing(char* filename,Model model,Hash* H){
  FILE* fptr = fopen(filename, "r");
  int tcount,match,size = H->Head->Next->vec_size;
  char line[300];
  char* token=NULL;
  char* first;
  char* second;
  double p;
  double correct=0,sum=0;;
  double* F;
  double* S;
  double* Con;
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
   Con=Vector_Concat(F,S, size);
   p=Predict(model,Con,2*size);
    //printf("Gia %s kai %s exw einai %d kai exw %f\n",first,second,match,p );
	double cor;
   cor=1-p;
   sum++;
   if(cor>0.5){
     if(!match){
			 correct++;
			// printf("SOSTO:Gia %s kai %s exw einai %d kai exw %f\n",first,second,match,p );
		 }else{
			 //printf("LATHOS:Gia %s kai %s exw einai %d kai exw %f\n",first,second,match,p );
		 }

   }else if(cor<0.5){
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

double Fx(Model model,double* con,int size){
  double sum=model.b,wx;

  for(int i=0; i<size ; i++){
		wx = model.weight_array[i]*con[i];
		//printf("%d.exw x:%f kai w:%f ara wx:%f\n",i,vectror_array[i],(model->weight_array[i]),wx);
		sum+= wx;
	}
  return sum;
}

double Predict(Model model,double* con,int size){
  double f=Fx(model,con,size);
  double p = 1 / (1 + exp(-f));
	return p;
}
