#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <dirent.h>
#include "hash.h"

int main( int argc, char *argv[] ){

   DIR* directory;
   DIR* indirectory;
   struct dirent* new_directory;
   struct dirent* curfile;
   FILE *dataw;
   FILE *scsv;
   FILE *dcsv;
   char* wfile;
   char* dd;
   char* token=NULL;
   char line[300];
   int inputexist=0,dexist=0,files=0;
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
   char word[15];
   Hash* H=HashCreate(1000);
// DTS /LHash* LEK=LHashCreate(1000); //dimiourgo ena neo Leksilogio(LHash) 1000 theseon
   LHash* Common=LHashCreate(200);
   char* temp=strdup("start");
   int charcount=0;
   FILE *com;

   if( (com=fopen("common.txt","r"))==NULL){
     perror("Fopen");
     return 0;
   }

   while((c = fgetc(com))!= EOF){ //Apothikevo sto Common tis leksis tou common.txt
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

                      Camera *camera = Camera_Init(newid); //Dimiourgo mia kamera me id newid
                      Read_from_JSON_file(newfile, camera); //Eisagw sthn kamera ta stixeia tou json arxeiou
                      H=HashInsert(H,camera); //Eisagw thn kamera sthn domh mou
  //DEN TREXEI SOST// LEK=Camera_to_string(camera,LEK); //pernaw oles tis leksis ths cameras sto LHash
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
   int match=0,tcount,z=0,a=0;


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
    HashConect(H,first,second,match);
    free(first);
    free(second);
	}

  scsv=fopen("Same.csv","w+"); //Dimiourgw ena neo csv arxio
  fprintf(scsv,"left_spec_id, right_spec_id\n");

  dcsv=fopen("Diffrend.csv","w+"); //Dimiourgw ena neo csv arxio
  fprintf(dcsv,"left_spec_id, right_spec_id\n");

  HashTransfer(H,scsv); //Pernaw ta teriasmata sto csv arxio
  HashDiff(H,dcsv);

    Print_Camera_Count_TList(qlique_list);


  TrainSetList* train_sl;
  TestSetList* test_sl;
  ValidationSetList* validation_sl;


  SplitToTrainTestValidationSet(&train_sl, &test_sl, &validation_sl, qlique_list);

  PrintTrainSetCameraCount(train_sl);
  PrintTestSetCameraCount(test_sl);
  PrintValidationSetCameraCount(validation_sl);

  // PrintAllPairsTrainSet(train_sl);

  // int match_count= 0, not_match_count = 0,match__;
  //   TrainSetNode* tsn1_ptr, *tsn2_ptr;
  //   tsn1_ptr = train_sl->first;
  //   while(tsn1_ptr != NULL)
  //   {
  //     tsn2_ptr = tsn1_ptr->next;
  //     while(tsn2_ptr != NULL)
  //     {
  //       printf("\n");
  //       // PrintTrainSetNode(tsn1_ptr);
  //       // PrintTrainSetNode(tsn2_ptr);
  //       match__ = IsAMatch(tsn1_ptr->camera, tsn2_ptr->camera, H);
  //       if(match__)
  //       {
  //         printf("%s and %s : its a match\n", tsn1_ptr->camera->id, tsn2_ptr->camera->id);
  //         match_count++;
  //       }
  //       else
  //       {
  //         printf("%s and %s : is not a match\n", tsn1_ptr->camera->id, tsn2_ptr->camera->id);
  //         not_match_count++;
  //       }
  //       printf("==============================\n");
  //       tsn2_ptr = tsn2_ptr->next;
  //     }
  //     tsn1_ptr = tsn1_ptr->next;
  //   }
  // printf("matches: %d\n not matched: %d", match_count, not_match_count);

  // =====================

  FreeTrainSetList(train_sl);
  FreeTestSetList(test_sl);
  FreeValidationSetList(validation_sl);

  FreeTList(qlique_list);

//DTS  LHashTF(LEK);
//DTS  LEK=NMostLHash(LEK,100);
//DTS  LHashPrint(LEK);
  //Apodesmevi twn domwn

  FreeHash(H);
  FreeLHash(Common);
//DTS  FreeLHash(LEK);
  free(temp);
  free(wfile);
  free(dd);
  fclose(dataw);
  fclose(scsv);
  fclose(dcsv);
  closedir(directory);


}
