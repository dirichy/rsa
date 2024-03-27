///Brief introduction: We define a type for a great number and design a series number to operat numbers in the type. 
///Consider a great number as an array with size GINT_LENGTH. 
///Each element in the array is long long int type. We only use GINT_DIGIT digits of element 
///to store some of digits of the great number. 
///Each element in the array represent GINT_DIGIT_BASE64 digits in BASE64 coding.
///In some case, we may need to abstract particular digits, and we use GINT_BASE64_DIGIT_NORMALIZER to 
///obtain the lowest GINT_BASE64_DIGIT_NORMALIZER digits of a number.
#define GINT_LENGTH 256
#define GINT_DIGIT 16
#define GINT_DIGIT_BASE64 3
#define GINT_DIGIT_CHAR 2
#define GINT_DIGIT_MAX 0xffff
#define GINT_BASE64_DIGIT_NORMALIZER 0x3f
///This is to define a type for great numbers, we call it gint.
typedef struct {
  unsigned long long value[GINT_LENGTH];
  int length;
} gint;
void gen(int digit,char* name);
void readrsa(char* name,gint*n,gint*phin,gint*d,gint*e);
char* stradd(char* a,char*b,char*c);
void str2gint(char* str,gint* a);
void gclone(gint* a,gint* b);
void gcode(gint* n,gint* message,gint* key,gint *output);
void gprint(gint a,char* str);
void gdisplay(gint a);
