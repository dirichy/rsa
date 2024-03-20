#include <stdio.h>
char Flags[9] = "oigedsrh";
void gen(){
  printf("gen\n");
}
void help(){
printf("-g [<name>] : to generate a pair of public key and secret key with corresponding n and phi(n) \n");
printf("              and you can name the pair of key by the optional parameter <name>, otherwise it will be default.\n");
printf("-e [<name>] : to encode context by public key, you can appoint one pair of your keys by optional parameter <name>,\n");
printf("              otherwise it will be encoded by the default key. If you use flag -e with an intput flag -i,\n");
printf("              then you can appoint the source of intput, otherwise the intput form is stdin.\n");
printf("              And you can also appoint a position to store the encoded context by an output flag -o, if not \n");
printf("              your output will be a form of stdin.\n");
printf("-d [<name>] : to decode context by secret key, same as -e.\n");
printf("-s [<name>] : to encoded context by ");
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
