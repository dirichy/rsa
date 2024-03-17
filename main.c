#include<stdio.h>
typedef struct {
  int value;
} gint;

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
gint ginverse(gint n,gint e){
  gint d;
//ed \equiv 1 \mod n
  return d;
}
int main(){
  return 0;
}
