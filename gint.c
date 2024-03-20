#include<stdio.h>
#include<stdlib.h>
#include<time.h>
///Brief introduction: We define a type for a great number and design a series number to operat numbers in the type. 
///Consider a great number as an array with size GINT_LENGTH. 
///Each element in the array is long long int type. We only use GINT_DIGIT digits of element 
///to store some of digits of the great number. 
///Each element in the array represent GINT_DIGIT_BASE64 digits in BASE64 coding.
///In some case, we may need to abstract particular digits, and we use GINT_BASE64_DIGIT_NORMALIZER to 
///obtain the lowest GINT_BASE64_DIGIT_NORMALIZER digits of a number.
#define GINT_LENGTH 35
#define GINT_DIGIT 24
#define GINT_DIGIT_BASE64 4
#define GINT_DIGIT_MAX 0xffffff
///This is to define a type for great numbers, we call it gint.
#define GINT_BASE64_DIGIT_NORMALIZER 0x3f
typedef struct {
  unsigned long long value[GINT_LENGTH];
  int length;
} gint;
///This function is to deal with carry when calculating and the size of a gint number when it comes to 
///some operation between int numbers and gint numbers.
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
  if(a->length<=0){
    a->length=1;
  }
}
///This function is to shift one right digit in gint a.
///a will convert to the right-shifted version.
void gshiftright(gint* a){
  a->value[0]=(a->value[0])>>1;
  unsigned long long temp;
  for(int i=0;i<GINT_LENGTH-1;i++){
    temp=(a->value[i+1])&1;
    a->value[i+1]=(a->value[i+1])>>1;
    a->value[i]=a->value[i]+(temp<<(GINT_DIGIT-1));
  }
  update(a);
}
///This function is to shift one left digit in gint a.
///a will convert to the left-shifted version.
void gshiftleft(gint* a){
  for(int i=0;i<GINT_LENGTH;i++){
    a->value[i]<<=1;
  }
  update(a);
}
///This function is to shift i right digits in gint a.
///a will convert to the right-shifted version.
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
///This function is to shift i left digits in gint a.
///a will convert to the left-shifted version.
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
///This function is to convert an unsigned long long number to string characters by BASE64.
///The output will be stored in str.
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
///This function is to print out a great number by BASE64 codes
///The output will be stored in str, and if you want to print str out 
///you will need printf function to assist you.
void gprint(gint a,char* str){
  for(int i=0;i<a.length;i++){
    base64(a.value[i],str);
    str=str+GINT_DIGIT_BASE64;
  }
}
void gdisplay(gint a){
  char str[GINT_LENGTH*GINT_DIGIT_BASE64+1]={"\0"};
  gprint(a,str);
  printf("%s\n",str);
}
///This function is to generate a random gint number.
///It will return the gint number.
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
///This function is to convert an int number to a gint number.
///It will return gint number b converted by int a
void int2gint(gint* b,int a){
  b->value[0]=(unsigned long long)a;
  for(int i=1;i <GINT_LENGTH;i++){
    b->value[i]=(unsigned long long)0;
  }
  update(b);
}
///Inner function, don't use it manually
int gint_le_or_leq_gint(gint* a,gint* b,int flag){
  for(int i=GINT_LENGTH-1;i>=0;i--){
    if(a->value[i]<b->value[i]) return 1;
    if(a->value[i]>b->value[i]) return 0;
  }
  return flag;
}
///This fuction is to judge wheter a<b or not.
///If a < b, then return 1, else 0 
int gintlegint(gint* a,gint* b){
  return gint_le_or_leq_gint(a,b,0);
}
///This fuction is to judge wheter a>=b or not.
///If a>=b, then return 1, else 0 
int gintleqgint(gint* a,gint* b){
  return gint_le_or_leq_gint(a,b,1);
}
///This fuction is to judge wheter a>b or not.
///If a>b, then return 1, else 0 
int gintgegint(gint* a,gint* b){
  return !gintleqgint(a,b);
}
///This fuction is to judge wheter a<=b or not.
///If a<=b, then return 1, else 0 
int gintgeqgint(gint* a,gint* b){
  return !gintlegint(a,b);
}
///This fuction is to add two great numbers a ,b
///And a will be the result of a+b
void gadd(gint* a, gint* b){
  for(int i =0;i < GINT_LENGTH;i++ ){
    a->value[i]+=b->value[i];
  }
  update(a);
}
///This fuction is to use a minus b 
///And a will be the result of a-b
void gminus(gint* a, gint* b){
  int k;
  for(int i =0;i < GINT_LENGTH;i++ ){
    if(a->value[i]>=b->value[i]){
      a->value[i]-=b->value[i];
      continue;
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
///This function is to use a mutiply b
///And a will be the result of a*b
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
    Scale-=1;
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
    if(gintleqgint(b,a)){
      gminus(a,b);
      q->value[Scale]+=1<<scale;
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
    q->value[0]+=1;
  }
  update(a);
  update(q);
  update(b);
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
  int2gint(&a,1);
  int2gint(&b,2);
  a.value[1]=1;
  update(&a);
  gint q;
  gdivide(&a,&b,&q);
  gdisplay(a);
  gdisplay(b);
  gdisplay(q);
  // int a = 1073741822;
  // a=a>>30;
  // printf("%d",a);

}
