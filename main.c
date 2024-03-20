#include<stdio.h>
#include<stdlib.h>
/// the type gint is to restore a great number. 
/// Although now it's only long long, it will be better in the future
typedef struct {
  long long value;
} gint;
/// this function is to display a gint
void gdisplay(gint a){
  printf("%lld",a.value);
}
/// this function is to convert a int into gint
gint int2gint(int a){
  gint b;
  b.value=a;
  return b;
}
///this function is to add two gint
gint gadd(gint a,gint b){
  gint c;
  c.value=a.value+b.value;
  return c;
}
///this function is to minus two gint
gint gminus(gint a,gint b){
  gint c;
  c.value=a.value-b.value;
  return c;
}
///this function is to multiply two gint
gint gmultiply(gint a,gint b){
  gint c;
  c.value=a.value*b.value;
  return c;
}
///this function is to divide two gint
gint gdivide(gint a,gint b){
  gint c;
  c.value=a.value/b.value;
  return c;
}
///this function is to mod two gint
gint gmod(gint a,gint b){
  gint c;
  c.value=a.value%b.value;
  return c;
}
/// this function is to do the fast power mod n for gint. 
/// will return a^b mod n
gint gmodpower(gint n,gint a,gint b){
  long long c=a.value%n.value;
  long long d=b.value;
  long long s=1;
  while(d){
    if(d&1) s=s*c%n.value;
    d=d >> 1;
    c=c*c%n.value;
  }
  gint t=int2gint(s);
  return t;
}
/// this function is to genetate a random gint
gint grandom(){
  gint a;
  a.value=rand();
  return a;
}
/// this function is to judge whether a gint is a prime 
int gisprime(gint a){
  long long b=a.value;
  if(b<=1) return 0;
  for(long long i=2;i*i<=b;i++){
    if(b%i==0) return 0;
  }
  return 1;
}
/// this function is to add a gint and an int
gint gintaddint(gint a,int b){
  gint c;
  c.value=a.value+b;
  return c;
}
/// this function is to minus a gint by an int
gint gintminusint(gint a,int b){
  gint c;
  c.value=a.value-b;
  return c;
}
/// this function returns (gint)0
gint gzero(){
  gint a;
  a.value=0;
  return a;
}
int geven(gint a){
  return !(a.value&1);
}
/// this function is to genetate a randomprime
gint randomprime(){
  gint a;
  while(a.value<9999){
    a=grandom();
  }
  if(geven(a)){
    a=gintaddint(a,1);
  }
  while(!gisprime(a)){
    a=gintaddint(a,2);
  }
  return a;
}
int gle(gint a,gint b){
  return a.value<b.value;
}
int gequal(gint a, gint b){
  return a.value == b.value;
}
int giszero(gint a){
  return a.value==0;
}
int gintequalint(gint a,int b){
  return a.value==b;
}
gint gclone(gint a){
  gint b;
  b.value=a.value;
  return b;
}
/// this function is to solve the eqution ed = 1 mod n 
gint ginverse(gint n,gint e){
  gint norigin = gclone(n);
  //ed \equiv 1 \mod n
  //if gcd(e,n)!=1 then return (gint)0;
  gint r=imod(n,e);
  int i=1;
  gint pk_1=gzero();
  gint q,a;
  gint pk_2 =int2gint(1);  
  q = gdivide(n,e); 
  while(!giszero(r)){
    i=i + 1;
    a = pk_2 ;
    pk_2 = gadd(gmultiply(q,pk_2),pk_1);
    pk_2=gmod(pk_2,norigin);
    pk_1 = a;
    n = e;
    e = r;
    q = gdivide(n,e);
    r = gmod (n,e);
  }
  if(!gintequalint(e,1)){
    return gzero();
  }
  return i%2?pk_2:gminus(norigin,pk_2);
}
/// this function is to do the encode or decode
/// will return message^secret mod n
gint gcode(gint n,gint message,gint secret){
  return gmodpower(n,message,secret);
}

int main(){
  //gdisplay(ginverse(int2gint(211),int2gint(11)));
   gint a=randomprime();
   gdisplay(a);
  return 0;
}
