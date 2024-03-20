#include <stdio.h>
char Flags[9] = "oigedsrh";
void gen(){
  printf("gen\n");
}
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
void work(){
  printf("work\n");
}
int main(int argc,const char *argv[]){
  if(argv[1][0]=='-'){
    switch(argv[1][1]){
      case 'g': gen();break;
      case 'h': help();break;
      case 'e':
      case 'd':
      case 's':
      case 'r': work();break;
      default: help();
    }
  }
}
