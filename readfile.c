#include <stdio.h>
#include <stdio.h>
#define GINT_LENGTH 35
#define GINT_DIGIT 24
#define GINT_DIGIT_BASE64 4
#define GINT_DIGIT_MAX 0xffffff
///This is to define a type for great numbers, we call it gint.
#define GINT_BASE64_DIGIT_NORMALIZER 0x3f

int main(int argc, char **argv)
{
  FILE* fp = NULL;
  char buf[GINT_LENGTH*GINT_DIGIT_BASE64+128]={0};
  int ret;
  fp = fopen("text.txt","r");
  if(NULL == fp)
  {
    printf("open file err!\n");
    return 1;
  }
  ret = fread(buf,1,GINT_LENGTH*GINT_DIGIT_BASE64+128,fp);
  printf("ret = %d\n",ret);
  printf("read buf = %s\n",buf);
  fclose(fp);
  return 0;
}
