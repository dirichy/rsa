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
#define GINT_LENGTH 100
#define GINT_DIGIT 24
#define GINT_DIGIT_BASE64 4
#define GINT_DIGIT_MAX 0xffffff
#define GINT_BASE64_DIGIT_NORMALIZER 0x3f
///This is to define a type for great numbers, we call it gint.
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

int ginteqint(gint*a,int b){
  if(a->value[0]!=b) return 0;
  for(int i=1;i<a->length;i++){
    if(a->value[i]) return 0;
  }
  return 1;
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
  *str='\0';
}
void gdisplay(gint a){
  char str[GINT_LENGTH*GINT_DIGIT_BASE64+1]={"\0"};
  gprint(a,str);
  printf("%s\n",str);
}
///This function is to generate a random gint number.
///It will return the gint number.
void grandom(gint*a,int digit){
  int length = digit/GINT_DIGIT;
  for(int i=0;i<length;i++){
    a->value[i]=rand()&GINT_DIGIT_MAX;
  }
  int remainder = digit%GINT_DIGIT;
  long long temp =1;
  long long normalizer = 0;
  a->length=remainder?length+1:length;
  int i=0;
  while(i<remainder){
    normalizer+=temp;
    temp=temp<<1;
    i++;
  }
  a->value[length]=rand()&normalizer;
  normalizer = (normalizer+1)>>1;
  a->value[length]|=normalizer;
  i=length+1;
  while(i<GINT_LENGTH){
    a->value[i]=0;
    i++;
  }
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
  int length=a->length>b->length?a->length:b->length;
  for(int i =0;i < length;i++ ){
    a->value[i]+=b->value[i];
  }
  update(a);
}
void gintaddint(gint* a, int b){
    a->value[0]+=b;
  update(a);
}


///This fuction is to use a minus b 
///And a will be the result of a-b
void gminus(gint* a, gint* b){
  if(gintlegint(a,b)){
    printf("minus erroe!\n");
    exit(1);
  }
  int k,length=a->length;
  for(int i =0;i < length;i++ ){
    if(a->value[i]>=b->value[i]){
      a->value[i]-=b->value[i];
      continue;
    }
    k=i+1;
    while(k<length && !(a->value[k])){
      a->value[k]=GINT_DIGIT_MAX;
      k++;
    }
    a->value[k]-=1;
    a->value[i]+=GINT_DIGIT_MAX+1;
    a->value[i]-=b->value[i];
  }
  update(a);
}

void gintminusint(gint* a, int b){
  int k;
    if(a->value[0]>=b){
      a->value[0]-=b;
    return;
    }
    k=1;
    while(k<GINT_LENGTH && !(a->value[k])){
      a->value[k]=GINT_DIGIT_MAX;
      k++;
    }
    a->value[k]-=1;
    a->value[0]+=GINT_DIGIT_MAX+1;
    a->value[0]-=b;
  update(a);
}
///This function is to use a mutiply b
///And a will be the result of a*;b
void gmutiply(gint* a,gint* b){
  int length=a->length+b->length-1;
  for(int i=length;i>=0;i--){
    a->value[i]*=b->value[0];
    if(i){
      if(a==b){
        a->value[i]*=2;
      }else{
        a->value[i]+=a->value[0]*b->value[i];
      }
    }
    for(int j=1;j<i;j++){
      a->value[i]+=a->value[j]*b->value[i-j];
    }
  }
  update(a);
}
void gintmodint(gint*a,int b){
  for(int i=a->length-1;i>0;i--){
    a->value[i-1]+=(a->value[i]%b)<<GINT_DIGIT;
  }
  a->value[0]%=b;
  a->length=1;
}
///This function is to realize a = qb +r. And it turns out that 
///a = r, q is q
void gdivide(gint* a,gint* b,gint* q){
  int2gint(q,0);
  if(ginteqint(b,0)){
    printf("devide 0");
    exit(1);
  }
  if(gintlegint(a,b)){
    int2gint(q,0);
    return;
  }
  int scale=a->length-b->length;
  if(scale>0){
    gShiftLeft(b,scale-1);
    scale-=1;
  }
  scale*=GINT_DIGIT;
  while(gintlegint(b,a)&&((b->value[GINT_LENGTH-1])<=0x8000000000000000)){
    gshiftleft(b);
    scale++;
  }
  while(scale){
    if(gintleqgint(b,a)){
      gminus(a,b);
      q->value[scale/GINT_DIGIT]+=1<<(scale%GINT_DIGIT);
    }
    scale--;
    gshiftright(b);
  }
  if(gintleqgint(b,a)){
    gminus(a,b);
    q->value[0]+=1;
  }
  update(a);
  update(q);
  update(b);
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
  int blength = b->length * GINT_DIGIT;
  for(int i=0;i < blength;i++){
    int b_i=(b->value[i/GINT_DIGIT]>>(i%GINT_DIGIT))&1;
    if(b_i) {
      gmutiply(s,a);
      gdivide(s,n,q);
    }
    gmutiply(a,a);
    gdivide(a,n,q);
  }
}
int checker[10]={2,3,5,7,11,13,17,19,23,29};
int gisprime(gint*a,int*knownprime,int sizeofknownprime){
  gint bb,cc,dd,ee,ff,qq,*q=&qq,*b=&bb,*c=&cc,*d=&dd,*e=&ee,*f=&ff,gg,*g=&gg;
  int i=0,s=0,j=0;
  for(i=0;i<sizeofknownprime;i++){
    gclone(a,c);
    gintmodint(c,knownprime[i]);
    if(ginteqint(c,0)){
      return 0;
    }
  }
  s=0;
    gclone(a,c);
    int2gint(b,1);
    gminus(c,b);
  gclone(c,f);
    while(giseven(c)){
      gshiftright(c);
      s++;
    }
  for(i=0;i<10;i++){
    gclone(c,e);
    int2gint(b,checker[i]);
    gclone(a,g);
    gmodpower(g,b,e,d);
    if(ginteqint(d,1)) continue;
    int flag=0;
    for(j=0;j<s;j++){
      if(gequal(d,f)){
        flag=1;
        break;
      }
      gmutiply(d,d);
      gclone(a,g);
      gdivide(d,g,q);
    }
    if(!flag){
      return 0;
    };
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
void grandomprime(gint *p,int digits,int* knownprime,int sizeofknownprime){
  int i=0;
  grandom(p,digits);
  if(giseven(p)){
    gintaddint(p,1);
  }
  while(!gisprime(p,knownprime,sizeofknownprime)){
    gintaddint(p,2);
    i++;
      printf("tested %d numbers\n",i);
  }
}
char* stradd(char* a,char*b,char*c){
  int i=0;
  while(*a){
    c[i]=*a;
    i++;
    a++;
  }
  while(*b){
    c[i]=*b;
    i++;
    b++;
  }
  c[i]='\0';
  return c;
}
void gen(int digits,char* name){
  gint p,q,n,phin,e,d,temp1,temp2;
  int knownprime[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61};
  int sizeofknownprime=3;
  grandomprime(&p,digits,knownprime,sizeofknownprime);
  grandomprime(&q,digits,knownprime,sizeofknownprime);
  gclone(&p,&n);
  gmutiply(&n,&q);
  gintminusint(&p,1);
  gintminusint(&q,1);
  gclone(&p,&phin);
  gmutiply(&phin,&q);
  int2gint(&e,0);
  while(ginteqint(&e,0)){
    grandom(&d,digits);
    gclone(&n,&temp1);
    gclone(&d,&temp2);
    ginverse(&temp1,&temp2,&e);
  }
  char str[digits/3+128];
  if(!name[0]){
    name="rsabw";
  }
  FILE*file=fopen(stradd(name,".pub",str),"w");
  if (file != NULL){
    gprint(e,str);
    fprintf(file,"%s",str);
    fclose(file);
  }
  else{
    perror("Error opening file");
  }
  file=fopen(stradd(name,".sec",str),"w");
  if (file != NULL){
    gprint(d,str);
    fprintf(file,"%s",str);
    fclose(file);
  }
  else{
    perror("Error opening file");
  }
  file=fopen(stradd(name,".n",str),"w");
  if (file != NULL){
    gprint(n,str);
    fprintf(file,"%s",str);
    fclose(file);
  }
  else{
    perror("Error opening file");
  }
  file=fopen(stradd(name,".phin",str),"w");
  if (file != NULL){
    gprint(phin,str);
    fprintf(file,"%s",str);
    fclose(file);
  }
  else{
    perror("Error opening file");
  }
}

int main(){
  long long begin=(long long)(time(NULL));
  gen(1000,"byl");
  long long endtime=(long long)(time(NULL));
  printf("Used %lld seconds to generate\n",endtime-begin);
  gint a,b;
  int2gint(&a,0x3);
  int2gint(&b,0x3);
  gmutiply(&a,&b);
  gdisplay(a);
  gdisplay(b);
  // int2gint(&a,0xca1c45);
  // a.value[1]=0xd9f611;
  // a.value[2]=880;
  // update(&a);
  // int test[1]={2};
  // printf("%d",gisprime(&a,test,1));
  // gint b;
  // int2gint(&b,0);
  // b.value[10]=1;
  // gint r;
  // update(&b);
  // gdivide(&b,&a,&r);
  // gdisplay(b);
  // gShiftLeft(&a,2);
  // gdisplay(a);
  // int2gint(&b,1023);
  // int2gint(&c,10);
  // gmodpower(&b,&a,&c,&d);
  // gdisplay(d);
}
