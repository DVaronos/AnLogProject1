#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <unistd.h>
#include <sys/times.h>

int main( int argc, char *argv[] ){

  double t1, t2, cpu_time;
  struct tms tb1, tb2;
  double ticspersec;

  ticspersec = (double) sysconf(_SC_CLK_TCK);
  t1 = (double) times(&tb1);

   FILE *dataw;
   FILE *csvfile;
   char* wfile;
   char* token=NULL;
   char line[300];
   int charcount=0,wordcount=0,inputexist=0;
   char c;


   for(int i=1 ; i<argc ; i++){//Elegxos gia to an iparxei arxio eisodou
     if(strcmp(argv[i],"-w")==0){//Efoson iparxi inputfile apothikefsi tou onomatos tou
       wfile=malloc(sizeof(char)*(strlen(argv[i+1])+1));
       strcpy(wfile,argv[i+1]);
       inputexist=1;
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

   char* first;
   char* second;
   int match=0,tcount;
   SList* S=CreateSList();
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
      InsertSList(S,first,second);
    }
    free(first);
    free(second);
	}

  csvfile=fopen("new.csv","w+");
  fprintf(csvfile,"left_spec_id, right_spec_id\n");
  PrintSList(S,csvfile);

  //Apodesmevi twn domwn
  free(wfile);
  fclose(dataw);
  FreeSList(S);
  fclose(csvfile);

  t2 = (double) times(&tb2);
  cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) -
                           (tb1.tms_utime + tb1.tms_stime));
  printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time).\n", (t2 - t1) / ticspersec, cpu_time / ticspersec);
}
