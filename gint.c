#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define GINT_LENGTH 35
#define GINT_DIGIT 30
#define GINT_DIGIT_BASE64 5
#define GINT_DIGIT_MAX 0x3fffffff
#define GINT_BASE64_DIGIT_NORMALIZER 0x3f
typedef struct {
  unsigned long long value[GINT_LENGTH];
} gint;
void update(gint a){
  int temp,i;
  for(i=0;i<GINT_LENGTH-1;i++){
    temp=a.value[i]>>GINT_DIGIT;
    if(temp){
      a.value[i+1]+=temp;
    }
    a.value[i]=a.value[i] & GINT_DIGIT_MAX;
  }
}
void base64(unsigned long long a,char* str){
  char chars[65]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";
  int temp;
  for(int i=0;i<GINT_DIGIT_BASE64;i++){
    temp=a&GINT_BASE64_DIGIT_NORMALIZER;
    *str=chars[temp];
    str++;
    a=a>>6;
  }
}
void gprint(gint a,char* str){
  for(int i=0;i<GINT_LENGTH;i++){
    base64(a.value[i],str);
    str=str+GINT_DIGIT_BASE64;
  }
}
gint grandom(int digit){
  gint a;
  int length = digit/GINT_DIGIT;
  for(int i=0;i<length;i++){
    a.value[i]=rand()&GINT_DIGIT_MAX;
  }
  long long remainder = digit%GINT_DIGIT;
  long long temp =1;
  long long normalizer = 0;
  int i=0;
  while(i<remainder){
    normalizer+=temp;
    temp=temp<<1;
    i++;
  }
    a.value[length]=rand()&normalizer;
  normalizer = (normalizer+1)>>1;
  a.value[length]|=normalizer;
  i=length+1;
  while(i<GINT_LENGTH){
    a.value[i]=0;
    i++;
  }
  return a;
}
// gint int2gint(int a){
//
// }
int main(){
  // srand((unsigned)time(NULL));
  gint a=grandom(1024);
  char str[GINT_DIGIT_BASE64*GINT_LENGTH+1]={"\0"};
  gprint(a,str);
  printf("%s",str);
}
