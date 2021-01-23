#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <dirent.h>
#include <sys/times.h>
#include "logistic.h"
#include "JobSheduler.h"

int main( int argc, char *argv[] ){

    double t1, t2,time,ticspersec,t3;
    struct tms tb1, tb2;

    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);

   DIR* directory;
   DIR* indirectory;
   struct dirent* new_directory;
   struct dirent* curfile;
   FILE *dataw;
   FILE *scsv;
   FILE *dcsv;
   FILE *testing;
   FILE* validation;
   char* wfile;
   char* dd;
   char* token=NULL;
   char line[300];
   int inputexist=0,dexist=0,files=0,jcount=0;
   char c;
   int oe=0;

   for(int i=1 ; i<argc ; i++){//Elegxos gia to an iparxei arxio eisodou
     if(strcmp(argv[i],"-w")==0){//Efoson iparxi inputfile apothikefsi tou onomatos tou
       wfile=malloc(sizeof(char)*(strlen(argv[i+1])+1));
       strcpy(wfile,argv[i+1]);
       inputexist=1;
     }else if(strcmp(argv[i],"-d")==0){//Efoson iparxi inputfile apothikefsi tou onomatos tou
       dd=malloc(sizeof(char)*(strlen(argv[i+1])+1));
       strcpy(dd,argv[i+1]);
       dexist=1;
     } else if(strcmp(argv[i],"-o")==0){//Elegxos gia to an iparxei epilogi na dokimastoun gia to modelo ola ta dedomena tou datasetX metaxi tous
       oe=1;
     }
   }

   if(inputexist){//Elegxos gia an iparxi input file
     if((dataw=fopen(wfile,"r"))==NULL){//ean den iparxei to arxeio pou dothike san input file vgenei error kai termatizei h efarmogh
       printf("ERROR:There no such wfile\n");
       free(dataw);
       return 0;
     }
   }else{//Ean den iparxei kan kapoio inputfile vgeneierror kai termatizei h efarmogh
     printf("ERROR:There is no inputfile\n");
     return 0;
   }

   if(dexist){//Elegxos gia an iparxi input file
     if((directory=opendir(dd))==NULL){//ean den iparxei to arxeio pou dothike san input file vgenei error kai termatizei h efarmogh
       printf("ERROR:There no such directory\n");
       free(dd);
       free(wfile);
       fclose(dataw);
       return 0;
     }
   }else{//Ean den iparxei kan kapoio inputfile vgeneierror kai termatizei h efarmogh
     printf("ERROR:There is no input directory\n");
     free(wfile);
     fclose(dataw);
     return 0;
   }

   char* newfile;
   char* newdir;
   char* newid;
   char word[15];
   Hash* H=HashCreate(1000);  //Domh hash gia apotikefsh tou dataset x
   LHash* LEK=LHashCreate(1000);  //Domh LHash gia to leksilogio
   LHash* Common=LHashCreate(200);  //Domh LHash gia tis koines lekeis
   char* temp=strdup("start");
   int charcount=0;
   FILE *com;
   WHash* S;

   if( (com=fopen("common.txt","r"))==NULL){//Anigma tou common.tx pou periexei tis koines leksis
     perror("Fopen");
     return 0;
   }

   while((c = fgetc(com))!= EOF){//Apothikevo tis common leksis se ena Lhash
 		if(c ==',' || c== '\n'){

       word[charcount]='\0';

       free(temp);
       temp=malloc(sizeof(char)*(strlen(word)+1));
       strcpy(temp,word);
       Common=LHashInsert(Common,temp);

 			charcount=0;
     }else{
 			word[charcount]=c;
       charcount++;
 		}
 	}

  fclose(com);

   while( new_directory=readdir(directory) ){ //Diavazw to periexomeno tou fakelou pou dothike(diladi tous ipofakelous)

        if(strcmp(new_directory->d_name,".") && strcmp(new_directory->d_name,"..")){ //Ean den einai o eaftos rou h o proigoumenos

            newdir=malloc(sizeof(char)*( strlen(dd)+strlen(new_directory->d_name) +2)); //To newdir einai to path tou neou ipofakelou
            strcpy(newdir,dd);
            strcat(newdir,"/");
            strcat(newdir,new_directory->d_name);

            if( (indirectory=opendir(newdir))==NULL){
                perror("ERORR\n");
                return 0;
            }else{
                while( curfile=readdir(indirectory) ){ //Divazo to periexomenotou ypofakelou(Diladi ta json files pou einai messa se afton ton fakelo)

                    if(strcmp(curfile->d_name,".") && strcmp(curfile->d_name,"..")){  //Ean den einai o eaftos rou h o proigoumenos

                      newid=malloc(sizeof(char)*( strlen(curfile->d_name)+strlen(new_directory->d_name) +3)); //To newid tha exei to id ths cameras pou antistixei se afto to json file
                      strcpy(newid,new_directory->d_name);
                      strcat(newid,"//");
                      strcat(newid,curfile->d_name);
                      newid[strlen(newid)-5]='\0'; //Den theloume thn katalhksh ".json"

                      newfile=malloc(sizeof(char)*( strlen(curfile->d_name)+strlen(new_directory->d_name)+strlen(dd) +3)); //To newfile tha exei to path tou sigekrimenou json arxeiou
                      strcpy(newfile,dd);
                      strcat(newfile,"/");
                      strcat(newfile,new_directory->d_name);
                      strcat(newfile,"/");
                      strcat(newfile,curfile->d_name);

                      H=HashInsert(H,newid); //Apothikevo to id ths cameras sthn domh
                      LEK=Readjson(newfile,LEK,Common,&S); //Diavazo to json file kai apothikevo ta dedomena sto leksilogio kai se ena WHash
                      //jcount++;
                      H=HashReplaceSpear(H,newid,S);  //Eisago to whash sthn atnisthxh thesh sthn domh
                      free(newfile);
                      free(newid);

                    }
                }
                closedir(indirectory); // klinw ton ipofakelo
            }
            free(newdir);
        }
    }
   printf("The storage of datasetX  just finished\n");

   LHashTfIdf(LEK,H->count); //Ipologizo to IDF twn leksewn tou leksilogiou
   LEK=NMostLHash(LEK,1000);  //Pairnw tis 1000 pio simantikes leksi
   H=HashVectorts(H,LEK); //Dimiourgw gia kathe kamera ena vector simfona me to leksilogio
   FreeLHash(Common); //Apodesmevw to leksilogio kai to Common hash giati pleon den ta xriazome
   FreeLHash(LEK);
   printf("The creation of vectors just finished\n");
   char* first;
   char* second;
   int match=0,tcount,z=0,a=0;

   while (fgets(line,sizeof(line),dataw)){//Diavazei to csv file grami grami
     token=strtok(line,",");
     token=strtok(NULL,",");
     token=strtok(NULL,",");
     match=atoi(token);
     if(match){
       a++; //arithmos twn thetikwn sisxetisewn
     }else{
       z++; //arithmos twn arnitikwn sisxetisewn
     }
	 }
   fclose(dataw);
   int sa,ta,sz,tz,trz=0,tra=0,tez=0,tea=0,vz=0,va=0;

   sa=a*0.6;  //to 60% twn thetikwn sisxetisewn tou dataw
   ta=(a-sa)/2; ////to 20% twn thetikwn sisxetisewn tou dataw
   sz=z*0.6; //to 60% twn arnitikwn sisxetisewn tou dataw
   tz=(z-sz)/2; //to 20% twn arnitikwn sisxetisewn tou dataw
   if((dataw=fopen(wfile,"r"))==NULL){//ean den iparxei to arxeio pou dothike san input file vgenei error kai termatizei h efarmogh
     printf("ERROR:There no such wfile\n");
     free(dataw);
     return 0;
   }

   if( (testing=fopen("Testing.csv","w"))==NULL){
     perror("Fopen");
     return 0;
   }
   if( (validation=fopen("Validation.csv","w"))==NULL){
     perror("Fopen");
     return 0;
   }
	 while (fgets(line,sizeof(line),dataw)){//Diavazei to csv file grami grami
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

    if(match){ //Ean einai thetikh sisxetish
      if(tra<sa){ //Ean den exw isagei to 60% twn thetikwn sisxetisewn sto trainig set
        HashConect(H,first,second,match);
        tra++;
      }else if(tea<ta){ //Ean den exw isagei to 20% twn thetikwn sisxetisewn sto testing set
        fprintf(testing,"%s,%s,%d\n",first,second,match );
        tea++;
      }else{ //To ipolipo 20% twn thetikwn sisxetisewn paei sto validation set
        fprintf(validation,"%s,%s,%d\n",first,second,match );
      }
    }else{  //Ean einai arnitilh sisxetish
      if(trz<sz){  //Ean den exw isagei to 60% twn arnitikwn sisxetisewn sto trainig set
        HashConect(H,first,second,match);
        trz++;
      }else if(tez<tz){ //Ean den exw isagei to 20% twn arnitikwn sisxetisewn sto testing set
        fprintf(testing,"%s,%s,%d\n",first,second,match );
        tez++;
      }else{  //To ipolipo 20% twn arnitikwn sisxetisewn paei sto validation set
        fprintf(validation,"%s,%s,%d\n",first,second,match );
      }
    }
    free(first);
    free(second);
	}
  fclose(dataw);
  fclose(testing);
  fclose(validation);
  printf("The reading of the datasetW just finished\n");

  scsv=fopen("Same.csv","w+"); //Dimiourgw ena neo csv arxio
  fprintf(scsv,"left_spec_id, right_spec_id\n");

  dcsv=fopen("Diffrend.csv","w+"); //Dimiourgw ena neo csv arxio
  fprintf(dcsv,"left_spec_id, right_spec_id\n");

  HashTransfer(H,scsv); //Pernaw ta teriasmata sto csv arxio
  HashDiff(H,dcsv);

  fclose(scsv);
  fclose(dcsv);
  printf("The creation of the Same.csv and Diffrend.csv files just finished\n");

  Input* input=InputMake("Same.csv","Diffrend.csv", H);
  Model model;
  // model=Training(input,H);
  model = RepetitiveTaining(input, H, NULL);
  printf("The training of the model just finished\n");

  Testing("Testing.csv",model,H);

  if(oe){//Ean iparxi epilogh tou elegxou olwn twn dedomenwn tou datasetX metaksi tous
    t2 = (double) times(&tb2);
    TestAllData(H,model);
    t3=t2;
    t2 = (double) times(&tb2);
    time=((t2 - t3) / ticspersec);  //O xrronos pou perase gia na vrethei o arithmos
    printf("Ola gia ola was %f secs\n",time);
  }

  //apodesmefsh twn domwn
  FreeInput(input);
  FreeHash(H);
  free(temp);
  free(model.weight_array);
  free(wfile);
  free(dd);
  closedir(directory);
  remove("Testing.csv");
  remove("Validation.csv");

  t2 = (double) times(&tb2);
  time=((t2 - t1) / ticspersec);  //O xrronos pou perase gia na vrethei o arithmos
  printf("Time was %f secs\n",time);


}
