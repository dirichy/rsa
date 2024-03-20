#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define GINT_LENGTH 35
#define GINT_DIGIT 24
#define GINT_DIGIT_BASE64 4
#define GINT_DIGIT_MAX 0xffffff
#define GINT_BASE64_DIGIT_NORMALIZER 0x3f
typedef struct {
  unsigned long long value[GINT_LENGTH];
  int length;
} gint;
void gclone(gint* a,gint* b){
  for(int i=0;i<GINT_LENGTH;i++){
    b->value[i]=a->value[i];
  }
  b->length=a->length;
}
void update(gint* a){
  int temp,i;
  for(i=0;i<GINT_LENGTH-1;i++){
    temp=a->value[i]>>GINT_DIGIT;
    if(temp){
      a->value[i+1]+=temp;
    }
    a->value[i]=a->value[i] & GINT_DIGIT_MAX;
  }
  int length=GINT_LENGTH-1;
  while(!a->value[length]){
    length--;
  }
  a->length=length+1;
}
void gshiftright(gint* a){
  a->value[0]>>=1;
  int temp;
  for(int i=1;i<GINT_LENGTH;i++){
    temp=a->value[i]&1;
    a->value[i]>>=1;
    a->value[i-1]+=temp<<GINT_DIGIT;
  }
  update(a);
}
void gshiftleft(gint* a){
  for(int i=0;i<GINT_LENGTH;i++){
    a->value[i]<<=1;
  }
  update(a);
}
void gShiftRight(gint* a,int i){
  if(i==0) return;
  int j=0;
  for(j=0;i<GINT_LENGTH;j++,i++){
    a->value[j]=a->value[i];
  }
  while(j<GINT_LENGTH){
    a->value[j]=0;
    j++;
  }
  update(a);
}
void gShiftLeft(gint* a,int i){
  if(i==0)return;
  int j=0;
  i=GINT_LENGTH-1-i;
  for(j=GINT_LENGTH-1;i>=0;j--,i--){
    a->value[j]=a->value[i];
  }
  while(j>=0){
    a->value[j]=0;
    j--;
  }
  update(a);
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
  for(int i=0;i<a.length;i++){
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
  int remainder = digit%GINT_DIGIT;
  long long temp =1;
  long long normalizer = 0;
  a.length=remainder?length+1:length;
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
void int2gint(gint* b,int a){
  b->value[0]=(unsigned long long)a;
  for(int i=1;i <GINT_LENGTH;i++){
    b->value[i]=(unsigned long long)0;
  }
  update(b);
}
int gint_le_or_leq_gint(gint* a,gint* b,int flag){
  for(int i=0;i<GINT_LENGTH;i++){
    if(a->value[i]<b->value[i]) return 1;
    if(a->value[i]>b->value[i]) return 0;
  }
  return flag;
}
int gintlegint(gint* a,gint* b){
  return gint_le_or_leq_gint(a,b,0);
}
int gintleqgint(gint* a,gint* b){
  return gint_le_or_leq_gint(a,b,1);
}
int gintgegint(gint* a,gint* b){
  return !gintleqgint(a,b);
}
int gintgeqgint(gint* a,gint* b){
  return !gintleqgint(a,b);
}
void gadd(gint* a, gint* b){
  for(int i =0;i < GINT_LENGTH;i++ ){
    a->value[i]+=b->value[i];
  }
  update(a);
}
void gminus(gint* a, gint* b){
  int k;
  for(int i =0;i < GINT_LENGTH;i++ ){
    if(a->value[i]>=b->value[i]){
      a->value[i]-=b->value[i];
      break;
    }
    k=i+1;
    while(k<GINT_LENGTH && !(a->value[k])){
      a->value[k]=GINT_DIGIT_MAX;
      k++;
    }
    a->value[k]-=1;
    a->value[i]+=GINT_DIGIT_MAX+1;
    a->value[i]-=b->value[i];
  }
  update(a);
}
void gmutiply(gint* a,gint* b){
  for(int i=GINT_LENGTH-1;i>=0;i--){
    a->value[i]*=b->value[0];
    for(int j=0;j<i;j++){
      a->value[i]+=a->value[j]*b->value[i-j];
    }
  }
  update(a);
}
void gdivide(gint* a,gint* b,gint* q){
  int2gint(q,0);
  if(gintleqgint(b,q)){
    printf("devide 0");
    exit(1);
  }
  if(gintlegint(a,b)){
    int2gint(q,0);
    return;
  }
  int Scale=a->length-b->length;
  if(Scale>0){
    gShiftLeft(b,Scale-1);
  }
  int scale=0;
  while(gintlegint(b,a)&((b->value[GINT_LENGTH-1])<=0x8000000000000000)){
    gshiftleft(b);
    scale++;
  }
  while(scale>=30){
    scale-=30;
    Scale++;
  }
  while(Scale || scale){
  printf("%d %d\n",Scale,scale);
    if(gintleqgint(b,a)){
      gminus(a,b);
      q->value[Scale]|=1<<scale;
    }
    scale--;
    gshiftright(b);
    if(scale<0){
      scale=GINT_LENGTH-1;
      Scale--;
    }
  }
  if(gintleqgint(b,a)){
    gminus(a,b);
    q->value[Scale]|=1<<scale;
  }
}
int main(){
  srand((unsigned)time(NULL));
  // printf("%lld",a.value[1]);
  // gmutiply(&a,&b);
  // gshiftright(&a);
  // printf("%lld",a.value[0]);
  // update(&a);
  gint a;
  gint b;
  int2gint(&a,14);
  int2gint(&b,2);
  char str[GINT_DIGIT_BASE64*GINT_LENGTH+1]={"\0"};
  gprint(a,str);
  printf("%s\n",str);
  gint q;
  gdivide(&a,&b,&q);
  gprint(a,str);
  printf("%s\n",str);
  gprint(q,str);
  printf("%s",str);
  // int a = 1073741822;
  // a=a>>30;
  // printf("%d",a);
}
