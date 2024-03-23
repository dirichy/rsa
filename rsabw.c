#include <stdlib.h>
#include <stdio.h>
#include "gint.h"
char Flags[9] = "oigedsrh";
void help(){
  printf("Usage : rsabw -g [<name>]\n");
  printf("   or : rsabw -h\n");
  printf("   or : rsabw -<e|d|s|r> [<name>] [-i <file> | -] [-o <file> | -]\n");
  printf("-g [<name>] : to generate a pair of public key and secret key with corresponding n and phi(n) \n");
  printf("              and you can name the pair of key by the optional parameter <name>, otherwise it will be default.\n");
  printf("-e [<name>] : to encode content by public key, you can appoint one pair of your keys by optional parameter <name>,\n");
  printf("              otherwise it will be encoded by the default key. If you use flag -e with an intput flag -i,\n");
  printf("              then you can appoint the source of intput, otherwise the intput form is stdin.\n");
  printf("              And you can also appoint a position to store the encoded content by an output flag -o, if not \n");
  printf("              your output will be a form of stdin.\n");
  printf("-d [<name>] : to decode content by secret key, same as -e.\n");
  printf("-s [<name>] : to encoded content by secret key, same as -e.\n");
  printf("-r [<name>] : to decode content by public key, same as -e.\n");
  printf("[-i <file>|-] : to appoint the source of intput content, it is an optional parameter, if you don't use it,\n");
  printf("                the intput will be a stdin form.\n");
  printf("[-o <file>|-] : to appoint the aim of output content, it is an optional parameter, if you don't use it,\n");
  printf("                the output will be a stdin form.\n");
  printf("-h : to show the help document of command rsabw\n");
}

void work(char option,char ** args,int argc){
  char input[100]={0},output[100]={0},name[100]={0};
  char instr[GINT_LENGTH*GINT_DIGIT_BASE64+128]={0},outstr[GINT_LENGTH*GINT_DIGIT_BASE64+128]={0},temp[100]={0};
  gint n,phin,e,d;
  int i=2;
  if((argc>2)&&(args[2][0]!='-')){
    sprintf(name,"%s",args[2]);
    i++;
  }else{
    sprintf(name,"rsabw");
  }
  while(i<argc){
    switch(args[i][0]){
      case '-': 
        switch(args[i][1]){
          case 'i':
            if(i+1>=argc||*input){
              help();
              exit(1);
            }
            sprintf(input,"%s",args[i+1]);break;
          case 'o':
            if(i+1>=argc||*output){
              help();
              exit(1);
            }
            sprintf(output,"%s",args[i+1]);break;
          default:
            help();
            exit(1);
        };
        break;
      default:
        if(i+1<argc&&args[i+1][0]!='-'){
          help();
          exit(1);
        }
    }
    i++;
  }
  readrsa(name,&n,&phin,&d,&e);
  if(*input){
    FILE* fp = NULL;
    int ret;
    fp = fopen(input,"r");
    if(NULL == fp){
      printf("open file err!\n");
      exit(1);
    }
    ret = fread(instr,1,GINT_LENGTH*GINT_DIGIT_BASE64+128,fp);
  } else {
    fgets(instr,GINT_LENGTH*GINT_DIGIT_BASE64+128,stdin);
  }
  gint ingint,outgint,key;
  str2gint(instr,&ingint);
  if(option=='d'||option=='s'){
    gclone(&e,&key);
  }else{
    if(option=='e'||option=='r'){
      gclone(&d,&key);
    }else{
      printf("error option!\n");
      exit(1);
    }}
  gcode(&n,&ingint,&key,&outgint);
  gprint(outgint,outstr);
  if(*output){
    FILE*file=fopen(output,"w");
    if (file != NULL){
      fprintf(file,"%s",outstr);
      fclose(file);
    }
    else{
      perror("Error opening file");
    }
  }else{
    printf("%s\n",outstr);
  }
}
int main(int argc,char *argv[]){
  if(argc==1){
    help();
    exit(0);
  }
  if(argv[1][0]=='-'){
    switch(argv[1][1]){
      case 'g': gen(128,argc>2?argv[2]:"rsabw");break;
      case 'h': help();break;
      case 'e': 
      case 'd':
      case 's':
      case 'r': work(argv[1][1],argv,argc);break;
      default: help();
    }
  }
}
// int main(){
//   gen(1000,"byl");
//   gint e,d,n,phin,message,output,m;
//   readrsa("byl",&n,&phin,&d,&e);
//   int2gint(&message,10);
//   gdisplay(message);
//   gclone(&n,&m);
//   gcode(&m,&message,&e,&output);
//   gdisplay(output);
//   gclone(&n,&m);
//   gcode(&m,&output,&d,&message);
//   gdisplay(message);
//   // long long begin=(long long)(time(NULL));
//   // gen(1000,"byl");
//   // long long endtime=(long long)(time(NULL));
//   // printf("Used %lld seconds to generate\n",endtime-begin);
//   // unsigned long long a=unbase64("00Z0");
//   // printf("%lld",a);
// }
