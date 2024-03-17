#include<stdio.h>
#include<stdlib.h>
typedef struct {
  int value;
} gint;

void gdisplay(gint a){
  printf("%d",a.value);
}

gint gadd(gint a,gint b){
  gint c;
  c.value=a.value+b.value;
  return c;
}

gint gminus(gint a,gint b){
  gint c;
  c.value=a.value-b.value;
  return c;
}
gint gmultiply(gint a,gint b){
  gint c;
  c.value=a.value*b.value;
  return c;
}
gint gdivide(gint a,gint b){
  gint c;
  c.value=a.value/b.value;
  return c;
}
gint gmod(gint a,gint b){
  gint c;
  c.value=a.value%b.value;
  return c;
}
gint grandom(){
  gint a;
  a.value=rand();
  return a;
}
int gisprime(gint a){
  int b=a.value;
  if(b==1) return 0;
  for(int i=2;i<b;i++){
    if(b%i==0) return 0;
  }
  return 1;
}
gint int2gint(int a){
  gint b;
  b.value=a;
  return b;
}
gint gintaddint(gint a,int b){
  gint c;
  c.value=a.value+b;
  return c;
}
gint randomprime(){
  gint a;
  while(a.value<9999){
    a=grandom();
  }
  while(!gisprime(a)){
    a=gintaddint(a,2);
  }
  return a;
}
gint ginverse(gint n,gint e){
  gint d;
//ed \equiv 1 \mod n
  return d;
}
int main(){
  gint a=randomprime();
  gdisplay(a);
  return 0;
}
