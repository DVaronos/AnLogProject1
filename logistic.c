#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "logistic.h"


HVector* GetCameraVector(char* camera_id, Hash* H){
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
	while(L!=NULL){
		if(strcmp(L->camera,camera_id) == 0)	return L->vector;
		L=L->Next;
	}
	return NULL;
}

HVector* VectorConcat(HVector* F,HVector* S,int dif){
	int size=(F->count)+(S->count),c=0;
	size+=(size*0.3);
	HVector* C=CreateHVector(size);

	int fr,se;
	fr=F->sum;
	se=S->sum;
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
			//printf("paw sta %s kai %s\n",first,second );
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



Model Training(Model model,Input* input,Hash* H,JobSheduler* Sheduler){

  int   i,j,start=0,limit,msize=10,curr=0;
	double w,b;
	Job* job;
	TrainStruct** array=malloc(sizeof(TrainStruct*)*msize);
	while(start<input->size){ //Gia ola ta zevgaria tou input
		if(curr<msize){//Ean iparxei xwros ston array
			array[curr]=malloc(sizeof(TrainStruct)*Sheduler->num); //ftiakse mia lista apo TrainStruct
		}else{//Resize kata 10 thesis ton pinaka array
			msize+=10;
			array=realloc(array,msize*sizeof(TrainStruct*));
			array[curr]=malloc(sizeof(TrainStruct)*Sheduler->num);
		}
		for(i=0 ; i<Sheduler->num ; i++ ){	//Gia kathe thread ftiakse mia domh TrainStruct kai arxikopieise thn
			array[curr][i].w=malloc(model.array_size*sizeof(double));
			for(j=0 ; j<model.array_size ; j++){
				array[curr][i].w[j]=model.weight_array[j];
			}
			array[curr][i].b=model.b;
			array[curr][i].input=input;
			array[curr][i].start=start;
			start+=1000;
			if(start>=input->size) start=input->size;
			array[curr][i].end=start;
			start++;
			job=JobCreate(JobTraining,(void*)(array[curr]+i));	//Dimiourgise ena neo job
			JSAddWork(Sheduler,job);	//Dwse to job ston Sheduler
			if(start==((input->size)+1)) break;	//An elegthoun ola ta zevgaria vges ap thn for
		}
		JSWaitalltasks(Sheduler);	//Perimene na teleiosoun ta threads
		limit=i;
		if(start==((input->size)+1)) limit++;
		for(i=0 ; i<model.array_size ; i++){	//arxikopieise to w kai to b
			w=0;
			for(j=0 ; j<limit ; j++){
				w+=array[curr][j].w[i];
			}
			w=w/limit;
			model.weight_array[i]=w;
		}
		b=0;
		for(i=0 ; i<limit ; i++){
			b+=array[curr][i].b;
		}
		b=b/limit;
		model.b=b;
		curr++;
	}
	JSWaitalltasks(Sheduler);
	for(int r=0 ; r<msize ; r++){	//Apodesmefse to array
		if(r<curr){
			for(int t=0 ; t<Sheduler->num ; t++){
				if((r==(curr-1)) && (t==limit)) break;
				free(array[r][t].w);
			}
			free(array[r]);
		}

	}
	free(array);
	return model;
}


void JobTraining(void *args){

	TrainStruct* arg=(TrainStruct*)args;
	int i,j,c;
	double k,p,result;
	HVector* Con;
	for( i=arg->start ; i<arg->end ; i++){
			Con=arg->input->Cons[i];
    	p=Predict(&(arg->w),&(arg->b),Con);	//Ipologizoume thn provlepsh tou modelou
    	k=p-(arg->input->matches[i]);	//Ipologizoume thn apoklhsh ths provlepshs
			c=0;
    	for( j=0 ;  j<Con->size ; j++){	//Gia kathe varos
				if(Con[j].key!=-1){
					c++;
					result=k*Con[j].value;	//Ipologizoume to ginomeno ths apoklishs epi thn timh tou concat se afthn thn diastash
					arg->w[Con[j].key]=arg->w[Con[j].key]-(0.6*result);
					arg->b = (double)((arg->b) +k)/(double)2;	//enimeronoume to b
				}
				if(c==Con->count) break;
    	}
  }
}


void Testing(char* filename,Model model,Hash* H){
  FILE* fptr = fopen(filename, "r");
  int tcount,match;
  char line[300];
  char* token=NULL;
  char* first;
  char* second;
  double p;
  double correct=0,sum=0;
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
   p=Predict(&(model.weight_array),&(model.b),Con);
	 double cor;
   cor=1-p;
   sum++;
   if(cor>0.35){
     if(!match) correct++;
   }else{
     if(match)correct++;
   }
   free(first);
   free(second);
	 free(Con);
 }
 fclose(fptr);
 printf("Success rate is:%f\n",correct/sum);
}


double Fx(double** w,double* b,HVector* con){
  double wx,sum=*(b);
	double* W=*w;
	int c=0;
  for(int i=0; i<con->size ; i++){	//Ipologismos tou f(X)
		if(con[i].key!=-1){
			c++;
			wx=W[con[i].key]*(con[i].value);
			sum+=wx;
		}
		if(c==con->count) break;
	}
  return sum;
}

double Predict(double** w,double* b,HVector* con){
  double f=Fx(w,b,con);	//ipologizoume to f(x)
  double p = 1 / (1 + exp(-(f)));	//ipologizoume to p(f(x))
	return p;
}

double Newpred(double* w,double b,HVector* F,HVector* S,int dif){//Vriskei to predict xwris na ginei to concat

	int fr,se,c;
	double wx,sum=b;

	fr=F->sum;
	se=S->sum;
	if(fr>se){//Eelgxo pio ap ta dio vectors tha mpei proto
		c=0;
		for(int i=0 ; i<F->size ; i++){	//Isago tis leksis tou protou vector
			if(F[i].key!=-1){
				c++;
				wx=w[F[i].key]*(F[i].value);
				sum+=wx;
			}
			if(c==F->count) break;
		}
		c=0;
		for(int i=0 ; i<S->size ; i++){	//Isago tis leksis tou defterou vectour me afksimeno to key kata dif
			if(S[i].key!=-1){
				c++;
				wx=w[(S[i].key)+dif]*(S[i].value);
				sum+=wx;
			}
			if(c==S->count) break;
		}
	}else{
		c=0;
		for(int i=0 ; i<S->size ; i++){
			if(S[i].key!=-1){
				c++;
				wx=w[(S[i].key)]*(S[i].value);
				sum+=wx;
			}
			if(c==S->count) break;
		}
		c=0;
		for(int i=0 ; i<F->size ; i++){
			if(F[i].key!=-1){
				c++;
				wx=w[(F[i].key)+dif]*(F[i].value);
				sum+=wx;
			}
			if(c==F->count) break;
		}
	}
	sum=1 / (1 + exp(-(sum)));	//ipologizoume to p(f(x))
	return sum;
}

void TestAndAdd(Hash* H,Model model,JobSheduler* Sheduler,Input** input,double threshold){
	Job* job;
	int i,sum,who=0,start=1,msize=10,curr=0;
	TestStruct** current=malloc(sizeof(TestStruct*)*msize);
	NList** nodes;
	MakeArrays(H,&sum,&nodes); //Ftiaxnw ena array me NList komvous gia kathe kamera pou den exei klika
	while(who<(sum-1)){ //Gia oles aftes is kameres
		if(curr<msize){	//Ean iparxei xoros sto array
	    current[curr]=malloc(sizeof(TestStruct)*Sheduler->num);
	  }else{//Allios resize to array kata 10
	    msize+=10;
	    current=realloc(current,msize*sizeof(TestStruct*));
	    current[curr]=malloc(sizeof(TestStruct)*Sheduler->num);
	  }
		for(i=0 ; i<Sheduler->num ; i++){	//Gia kathe thread ftiakse ena TestStruct kai arxikopieise to
			current[curr][i].who=who;
			current[curr][i].start=start;
			start+=1000;
			if(start>=(sum-1)) start=sum-1;
			current[curr][i].end=start;
			current[curr][i].b=model.b;
			current[curr][i].w=model.weight_array;
			current[curr][i].threshold=threshold;
			current[curr][i].input=*input;
			current[curr][i].hash=H;
			current[curr][i].Sheduler=Sheduler;
			current[curr][i].nodes=nodes;
			current[curr][i].sum=sum-1;
			job=JobCreate(TestData,(void*)(current[curr]+i)); //ftiakse ena job
			JSAddWork(Sheduler,job); //Dose to job sto Sheduler
			start++;
			if(start==sum){
				who++;
				if(who==sum-1) break;
				start=who+1;
			}
		}
		JSWaitalltasks(Sheduler); //perimene na teliosoun ta threads
		curr++;
	}

	JSWaitalltasks(Sheduler);
	for(int k=0 ; k<curr ; k++){
		free(current[k]);
	}
	free(current);
	free(nodes);
}

void TestData(void* args){
	TestStruct* argument=(TestStruct*)args;
	double p;
	int j,ismatch;
	NList* N=argument->nodes[argument->who];
	NList* Nother;
	HVector* F=N->vector;
	HVector* S;
	HVector* Con;
	for(j=(argument->start) ; j < (argument->end) ; j++){
		Nother=argument->nodes[j];
		S=Nother->vector;	//Apothikevo to vector
		p=Newpred((argument->w),(argument->b),F,S,1000);	//Vrisko to predict
		if((p<(argument->threshold)) || (p>(1-(argument->threshold)))){ //Ean einai isxhrh h pithanothta
			if(p<(argument->threshold)){
				ismatch=0;
			}else{
				ismatch=1;
			}
		 		if(SearchDiffList(N->clique,Nother->clique) == 0){	//an den uparxei  susxethsh metaksi twn camerwn
					pthread_mutex_lock(&(argument->Sheduler->lock)); //Klidwnei to mutex
					if( N->clique==NULL){	//Ean h klika den iparxei arxikopieite
						N->clique=CreateCList();
						InsertCList(N->clique,N->camera,N);
					}
					if(Nother->clique==NULL){//Ean h klika den iparxei arxikopieite
						Nother->clique=CreateCList();
						InsertCList(Nother->clique,Nother->camera,Nother);
					}
					if(ismatch){ //Ean match ==1 ta proionta teriazoun
						Nother->clique=AppendCList(N->clique,Nother->clique);
				  }else{ //Ean match==0 ta proionta den teriazoun
				    Diff(N->clique,Nother->clique);
				  }

					//Vriskoume to concatenation kai to isagoume sto input
					Con=VectorConcat(F,S,1000);

					argument->input->Cons=realloc(argument->input->Cons, (argument->input->size+1)*sizeof(HVector*));
	 				argument->input->Cons[argument->input->size] = Con;

					argument->input->matches=realloc(argument->input->matches, (argument->input->size+1)*sizeof(int));
	 				argument->input->matches[argument->input->size] = ismatch;
					argument->input->size++;
					pthread_mutex_unlock(&(argument->Sheduler->lock)); //Klidoma tou mutex

				}
		}
	}
}


void MakeArrays(Hash* H,int* s,NList*** nodes){ //Dimiourgo ena pinaka me NList nodes twn kamerwn pou den einai mesa se kapia klika
	NList* N;
	int sum=0;
	for(int i=0 ; i<H->size ; i++){
		N=(NList*)H[i].Head;
    while(N->Next!=NULL){
      N=N->Next;
			if(N->clique==NULL){
      	sum++;
			}
    }
	}

	NList** nl=malloc(sum*sizeof(NList*));
	sum=0;
	for(int i=0 ; i<H->size ; i++){
		N=(NList*)H[i].Head;
    while(N->Next!=NULL){
      N=N->Next;
			if(N->clique==NULL){
				nl[sum]=N;
      	sum++;
			}
    }
	}
	*s=sum;
	*nodes=nl;
}

Model RepetitiveTaining(Input* input,Hash* H,JobSheduler* Sheduler){
	int size = H->Head->Next->vec_size,ep=1;
	Model model;
	double threshold=0.1,stepvalue=0.1;
	model.array_size=2*size;
  model.weight_array=malloc(sizeof(double)*(model.array_size));
  memset(model.weight_array, 0,(model.array_size)*sizeof(double));
	model.b=0;
	while (threshold <=0.3){
		model = Training(model,input,H,Sheduler);
		printf("Just finished the training for %d time\n",ep);
		TestAndAdd(H,model,Sheduler,&input,threshold);
		threshold+=stepvalue;
		ep++;
	}
	return model;
}
