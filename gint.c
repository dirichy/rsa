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
void gclone(gint* a,gint* b){
  for(int i=0;i<GINT_LENGTH;i++){
    b->value[i]=a->value[i];
  }
  b->length=a->length;
}
///This function is to deal with carry when calculating and the size of a gint number when it comes to 
///some operation between int numbers and gint numbers.
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

int gequal(gint*a,gint*b){
  for(int i=0;i<GINT_LENGTH;i++){
    if(a->value[i]!=b->value[i]){
      return 0;
    }
  }
  return 1;
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
///This function is to realize a = qb +r. And it turns out that 
///a = r, q is q
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
int ginteqint(gint*a,int b){
  if(a->value[0]!=b) return 0;
  for(int i=1;i<GINT_LENGTH;i++){
    if(a->value[i]) return 0;
  }
  return 1;
}
int giseven(gint* a){
  return !(a->value[0]&1);
}
/// this function is to do the fast power mod n for gint. 
/// s= a^b mod n
void gmodpower(gint *n,gint *a,gint *b,gint* s){
  gint qq;
  gint *q =&qq;
  gdivide(a,n,q);
  int2gint(s,1);
  while(!ginteqint(b,0)){
    if(!giseven(b)) {
      gmutiply(s,a);
      gdivide(s,n,q);
    }
    gshiftright(b);
    gmutiply(a,a);
    gdivide(a,n,q);
  }
}
int checker[10]={2,3,5,7,11,13,17,19,23,29};
int gisprime(gint*a,int*knownprime,int sizeofknownprime){
  gint bb,cc,dd,ee,*b=&bb,*c=&cc,*d=&dd,*e=&ee;
  int i=0,s=0,j=0;
  for(i=0;i<sizeofknownprime;i++){
    int2gint(b,knownprime[i]);
    gclone(a,c);
    gdivide(c,b,d);
    if(ginteqint(c,0)){
      return 0;
    }
  }
  for(i=0;i<10;i++){
    s=0;
    gclone(a,c);
    int2gint(b,1);
    gminus(c,b);
    int2gint(b,checker[i]);
    while(giseven(c)){
      gshiftright(c);
      s++;
    }
    gmodpower(a,b,c,d);
    if(ginteqint(d,1)) continue;
    gclone(a,b);
    int2gint(c,1);
    gminus(b,c);
    for(j=0;j<s;j++){
      printf("%lld\n",d->value[0]);
      gmutiply(d,d);
      gdivide(d,a,c);
      gdisplay(*a);
      if(gequal(d,b)){
        continue;
      }
    }
      printf("%d\n",checker[i]);
      return 0;
  }
  return 1;
}
/// this function is to solve the eqution ed = 1 mod n 
/// And d satisfies n | ed-1
  ///if gcd(e,n)!=1 then int2gint(d,0);
void ginverse(gint* n,gint* e,gint* d){
  gclone(n,d);
  gint*temp, qqqq, p11,p22;
  gint* q=&qqqq,*pk_1=&p11,*pk_2=&p22;
  gdivide(n,e,q);
  int i=1;
  int2gint(pk_1,0);
  int2gint(pk_2,1);
  while(!ginteqint(n,0)){
    i=i + 1;
    gmutiply(q,pk_2);
    gadd(q,pk_1);
    gdivide(q,d,pk_1);
    temp = pk_1;
    pk_1=pk_2;
    pk_2=q;
    q=temp;
    temp = e;
    e = n;
    n = temp;
    gdivide(n,e,q);
  }
  if(!ginteqint(e,1)){
    int2gint(d,0);
  }
  else{
  if (i%2){
    gclone(pk_2,d);
  }
  else{
    gminus(d,pk_2);
  }
  }
}
int main(){
  gint a,b,c,d,e;
  int2gint(&a,37);
  // int2gint(&b,1023);
  // int2gint(&c,10);
  int test[3]={2,3,5};
  int n=3;
  printf("%d\n",gisprime(&a,test,3));
  // gmodpower(&b,&a,&c,&d);
  // gdisplay(d);
}
