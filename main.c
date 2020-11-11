#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <dirent.h>
#include "list.h"

int main( int argc, char *argv[] ){

   DIR* directory;
   DIR* indirectory;
   struct dirent* new_directory;
   struct dirent* curfile;
   FILE *dataw;
   FILE *csvfile;
   char* wfile;
   char* dd;
   char* token=NULL;
   char line[300];
   int charcount=0,wordcount=0,inputexist=0,dexist=0,files=0;
   char c;


   for(int i=1 ; i<argc ; i++){//Elegxos gia to an iparxei arxio eisodou
     if(strcmp(argv[i],"-w")==0){//Efoson iparxi inputfile apothikefsi tou onomatos tou
       wfile=malloc(sizeof(char)*(strlen(argv[i+1])+1));
       strcpy(wfile,argv[i+1]);
       inputexist=1;
     }if(strcmp(argv[i],"-d")==0){//Efoson iparxi inputfile apothikefsi tou onomatos tou
       dd=malloc(sizeof(char)*(strlen(argv[i+1])+1));
       strcpy(dd,argv[i+1]);
       dexist=1;
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
   SList* S=CreateSList();

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

                      Camera *camera = Camera_Init(newid); //Dimiourgo mia kamera me id newid
                      Read_from_JSON_file(newfile, camera); //Eisagw sthn kamera ta stixeia tou json arxeiou

                      InsertSList(S,camera); //Eisagw thn kamera sthn domh mou

                      free(newfile);
                      free(newid);

                    }
                }
                closedir(indirectory); // klinw ton ipofakelo
            }
            free(newdir);
        }
    }

   char* first;
   char* second;
   int match=0,tcount,count=0;
	 while (fgets(line,sizeof(line),dataw)){//Diavazei to csv file grami grami
     token=strtok(line,",");
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
    if(match){//Ean match==1 tote ta proionta teriazounopote isagonte sthn domh
      ConectSList(S,first,second);
      count++;
    }
    free(first);
    free(second);
	}

  csvfile=fopen("Data.csv","w+"); //Dimiourgw ena neo csv arxio
  fprintf(csvfile,"left_spec_id, right_spec_id\n");

  TransferSList(S,csvfile); //Pernaw ta teriasmata sto csv arxio

  //Apodesmevi twn domwn

  FreeSList(S);
  free(wfile);
  free(dd);
  fclose(dataw);
  fclose(csvfile);
  closedir(directory);

}
