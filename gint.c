#include "gint.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void gclone(gint *a, gint *b) {
  memcpy(b->value, a->value, sizeof(long long) * GINT_LENGTH);
  b->length = a->length;
}

/// This function is to deal with carry when calculating, will change the value
/// of inputed gint.
void update(gint *a) {
  int temp, i;
  for (i = 0; i < GINT_LENGTH - 1; i++) {
    temp = a->value[i] >> GINT_DIGIT;
    if (temp) {
      a->value[i + 1] += temp;
    }
    a->value[i] = a->value[i] & GINT_DIGIT_MAX;
  }
  int length = GINT_LENGTH - 1;
  while (!a->value[length]) {
    length--;
  }
  a->length = length + 1;
  if (a->length <= 0) {
    a->length = 1;
  }
}

/// To judge whether a gint is equal to a int.
int ginteqint(gint *a, int b) {
  if (a->value[0] != b)
    return FALSE;
  for (int i = 1; i < GINT_LENGTH; i++) {
    if (a->value[i])
      return FALSE;
  }
  return TRUE;
}

/// To judge whether two gint is equal.
int gequal(gint *a, gint *b) {
  for (int i = 0; i < GINT_LENGTH; i++) {
    if (a->value[i] != b->value[i]) {
      return FALSE;
    }
  }
  return TRUE;
}

/// This function is to shift one right digit in gint `a`.
/// will change the inputed gint.
void gshiftright(gint *a) {
  a->value[0] = (a->value[0]) >> 1;
  unsigned long long temp;
  for (int i = 0; i < GINT_LENGTH - 1; i++) {
    temp = (a->value[i + 1]) & 1;
    a->value[i + 1] = (a->value[i + 1]) >> 1;
    a->value[i] = a->value[i] + (temp << (GINT_DIGIT - 1));
  }
  update(a);
}

/// This function is to shift one left digit in gint a.
/// will change the inputed gint.
void gshiftleft(gint *a) {
  for (int i = 0; i < GINT_LENGTH; i++) {
    a->value[i] <<= 1;
  }
  update(a);
}

/// This function is to shift `i*GINT_DIGIT` digits right in gint a.
/// will change the inputed gint.
void gShiftRight(gint *a, int i) {
  if (i == 0)
    return;
  int j = 0;
  for (j = 0; i < GINT_LENGTH; j++, i++) {
    a->value[j] = a->value[i];
  }
  while (j < GINT_LENGTH) {
    a->value[j] = 0;
    j++;
  }
  update(a);
}

/// This function is to shift `i*GINT_DIGIT` digits left in gint a.
/// will change the inputed gint.
void gShiftLeft(gint *a, int i) {
  if (i == 0)
    return;
  int j = 0;
  i = GINT_LENGTH - 1 - i;
  for (j = GINT_LENGTH - 1; i >= 0; j--, i--) {
    a->value[j] = a->value[i];
  }
  while (j >= 0) {
    a->value[j] = 0;
    j--;
  }
  update(a);
}

/// This function is to convert an unsigned long long number to string
/// characters by BASE64. The output will be stored in str.
/// `No EOF added` at the end, because the length is fixed as
/// `GINT_DIGIT_BASE64`.
void base64(unsigned long long a, char *str) {
  char chars[65] =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";
  int temp;
  for (int i = 0; i < GINT_DIGIT_BASE64; i++) {
    temp = a & GINT_BASE64_DIGIT_NORMALIZER;
    *str = chars[temp];
    str++;
    a = a >> 6;
  }
}
/// The reverse of base64.
unsigned long long unbase64(char *str) {
  char chars[65] =
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";
  unsigned long long num = 0;
  for (int j = 0; j < GINT_DIGIT_BASE64; j++) {
    for (int k = 0; k <= 64; k++) {
      if (k == 64) {
        printf("invaild input!%c\n", *str);
        exit(1);
      }
      if (!*str || *str == '\n') {
        break;
      }
      if (chars[k] == *str) {
        num += k << (j * 6);
        str++;
        break;
      }
    }
  }
  return num;
}

/// This function is to convert an int number to a gint number.
/// It will change gint number `b`'s value into int `a`
void int2gint(gint *b, int a) {
  b->value[0] = a & GINT_DIGIT_MAX;
  b->value[1] = a >> GINT_DIGIT;
  memset(b->value + 2, 0, sizeof(long long) * (GINT_LENGTH - 2));
  update(b);
}
/// Use BASE64 to convert a string into a gint, the length of str should less
/// than `GINT_DIGIT_BASE64*GINT_LENGTH`
void str2gint(char *str, gint *a) {
  int2gint(a, 0);
  int i = 0;
  for (i = 0; i < GINT_LENGTH; i++) {
    if (!*str) {
      break;
    }
    a->value[i] = unbase64(str);
    for (int j = 0; j < GINT_DIGIT_BASE64; j++) {
      if (*str && *str != '\n') {
        str++;
      }
    }
  }
  update(a);
}

/// This function is to print out a gint by BASE64 codes
/// The output will be stored in `str`, and if you want to print it out
/// you will need the `printf` function to assist you.
void gprint(gint a, char *str) {
  for (int i = 0; i < a.length; i++) {
    base64(a.value[i], str);
    str = str + GINT_DIGIT_BASE64;
  }
  *str = '\0';
}

/// Display a gint in terminal, for debugging.
void gdisplay(gint a) {
  char str[GINT_LENGTH * GINT_DIGIT_BASE64 + 1] = {"\0"};
  gprint(a, str);
  printf("%s\n", str);
}

/// This function is to generate a random gint number with digit `n`.
/// It will change `a` into the random number.
void grandom(gint *a, int digit) {
  int length = digit / GINT_DIGIT;
  for (int i = 0; i < length; i++) {
    a->value[i] = rand() & GINT_DIGIT_MAX;
  }
  int remainder = digit % GINT_DIGIT;
  long long temp = 1;
  long long normalizer = 0;
  a->length = remainder ? length + 1 : length;
  int i = 0;
  while (i < remainder) {
    normalizer += temp;
    temp = temp << 1;
    i++;
  }
  a->value[length] = rand() & normalizer;
  normalizer = (normalizer + 1) >> 1;
  a->value[length] |= normalizer;
  i = length + 1;
  while (i < GINT_LENGTH) {
    a->value[i] = 0;
    i++;
  }
}
/// Inner function, don't use it manually, to generate `le` and `leq` function
/// for gint.
int gint_le_or_leq_gint(gint *a, gint *b, int flag) {
  for (int i = GINT_LENGTH - 1; i >= 0; i--) {
    if (a->value[i] < b->value[i])
      return 1;
    if (a->value[i] > b->value[i])
      return 0;
  }
  return flag;
}

/// This fuction is to judge wheter `a<b` or not for gint `a`,`b`.
int gintlegint(gint *a, gint *b) { return gint_le_or_leq_gint(a, b, 0); }
/// This fuction is to judge wheter `a<=b` or not for gint `a`,`b`.
int gintleqgint(gint *a, gint *b) { return gint_le_or_leq_gint(a, b, 1); }
/// This fuction is to judge wheter `a>b` or not for gint `a`,`b`.
int gintgegint(gint *a, gint *b) { return !gintleqgint(a, b); }
/// This fuction is to judge wheter `a>=b` or not for gint `a`,`b`.
int gintgeqgint(gint *a, gint *b) { return !gintlegint(a, b); }
/// This fuction is to add two gint `a`, `b`
gint gadd(gint a, gint b) {
  gint c;
  int length = a.length > b.length ? a.length : b.length;
  for (int i = 0; i < length; i++) {
    c.value[i] = a.value[i] + b.value[i];
  }
  for (int i = length; i < GINT_LENGTH; i++) {
    c.value[i] = 0;
  }
  update(&c);
  return c;
}
/// This function is to calculate `a+b`, where `a` is gint and `b` is int.
/// Will return the result.
gint gintaddint(gint a, int b) {
  gint c;
  gclone(&a, &c);
  c.value[0] += b;
  update(&c);
  return c;
}

/// This fuction is to use a minus b
gint gminus(gint a, gint b) {
  if (gintlegint(&a, &b)) {
    printf("minus erroe!\n");
    exit(1);
  }
  gint c;
  gclone(&a, &c);
  int k, length = c.length;
  for (int i = 0; i < length; i++) {
    if (c.value[i] >= b.value[i]) {
      c.value[i] -= b.value[i];
      continue;
    }
    k = i + 1;
    while (k < length && !(c.value[k])) {
      c.value[k] = GINT_DIGIT_MAX;
      k++;
    }
    c.value[k] -= 1;
    c.value[i] += GINT_DIGIT_MAX + 1;
    c.value[i] -= b.value[i];
  }
  update(&c);
  return c;
}

gint gintminusint(gint a, int b) {
  gint c;
  gclone(&a, &c);
  int k;
  if (c.value[0] >= b) {
    c.value[0] -= b;
    return c;
  }
  k = 1;
  while (k < GINT_LENGTH && !(c.value[k])) {
    c.value[k] = GINT_DIGIT_MAX;
    k++;
  }
  c.value[k] -= 1;
  c.value[0] += GINT_DIGIT_MAX + 1;
  c.value[0] -= b;
  update(&c);
  return c;
}

/// This function is to use a mutiply b
gint gmutiply(gint a, gint b) {
  gint c;
  gclone(&a, &c);
  int length = c.length + b.length - 1;
  if (length >= GINT_LENGTH) {
    gdisplay(a);
    gdisplay(b);
    printf("too big!");
    exit(1);
  }
  for (int i = length; i >= 0; i--) {
    c.value[i] *= b.value[0];
    if (i) {
      c.value[i] += c.value[0] * b.value[i];
    }
    for (int j = 1; j < i; j++) {
      c.value[i] += c.value[j] * b.value[i - j];
    }
  }
  update(&c);
  return c;
}

int gintmodint(gint a, int b) {
  int temp = 0;
  for (int i = a.length - 1; i >= 0; i--) {
    temp = temp << GINT_DIGIT;
    temp += a.value[i];
    temp %= b;
  }
  return temp;
}

gint gdivide(gint a, gint b) {
  if (ginteqint(&b, 0)) {
    printf("devide 0");
    exit(1);
  }
  if (gintlegint(&a, &b)) {
    int2gint(&a, 0);
    return a;
  }
  gint q;
  int2gint(&q, 0);
  int scale = a.length - b.length;
  if (scale > 0) {
    gShiftLeft(&b, scale - 1);
    scale -= 1;
  }
  scale *= GINT_DIGIT;
  while (gintlegint(&b, &a) &&
         ((b.value[GINT_LENGTH - 1]) <= GINT_DIGIT_MAX / 2 + 1)) {
    gshiftleft(&b);
    scale++;
  }
  while (scale > -1) {
    if (gintleqgint(&b, &a)) {
      a = gminus(a, b);
      q.value[scale / GINT_DIGIT] += 1 << (scale % GINT_DIGIT);
    }
    scale--;
    gshiftright(&b);
  }
  update(&q);
  return q;
}

gint gmodule(gint a, gint b) {
  if (ginteqint(&b, 0)) {
    printf("devide 0");
    exit(1);
  }
  if (gintlegint(&a, &b)) {
    return a;
  }
  int scale = a.length - b.length;
  if (scale > 0) {
    gShiftLeft(&b, scale - 1);
    scale -= 1;
  }
  scale *= GINT_DIGIT;
  while (gintlegint(&b, &a) &&
         ((b.value[GINT_LENGTH - 1]) <= GINT_DIGIT_MAX / 2 + 1)) {
    gshiftleft(&b);
    scale++;
  }
  while (scale) {
    if (gintleqgint(&b, &a)) {
      a = gminus(a, b);
    }
    scale--;
    gshiftright(&b);
  }
  if (gintleqgint(&b, &a)) {
    a = gminus(a, b);
  }
  update(&a);
  return a;
}

int giseven(gint *a) { return !(a->value[0] & 1); }

/// this function is to do the fast power mod n for gint.
/// result is `a^b mod n`
gint gmodpower(gint n, gint a, gint b) {
  gint result;
  a = gmodule(a, n);
  int2gint(&result, 1);
  int bdigits = b.length * GINT_DIGIT;
  int b_i = 0;
  for (int i = 0; i < bdigits; i++) {
    b_i = (b.value[i / GINT_DIGIT] >> (i % GINT_DIGIT)) & 1;
    if (b_i) {
      result = gmutiply(result, a);
      result = gmodule(result, n);
    }
    a = gmutiply(a, a);
    a = gmodule(a, n);
  }
  return result;
}

int checker[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

/// This function is to judge whether a gint is prime or not by Miller Rabin
/// test. knownprime is an array of "little" primes, because Miller-Rabin test
/// is very slow, we will use knownprime to filter the given prime first.
int gisprime(gint n, int *knownprime, int sizeofknownprime) {
  if (giseven(&n)) {
    return ginteqint(&n, 2);
  }
  int i = 0, s = 0, j = 0;
  for (i = 0; i < sizeofknownprime; i++) {
    if (gintmodint(n, knownprime[i]) == 0) {
      return FALSE;
    }
  }
  s = 0;
  gint d, n_minus_1;
  n_minus_1 = gintminusint(n, 1);
  gclone(&n_minus_1, &d);
  while (giseven(&d)) {
    gshiftright(&d);
    s++;
  }
  int flag = FALSE;
  gint checker_i;
  for (i = 0; i < 10; i++) {
    int2gint(&checker_i, checker[i]);
    checker_i = gmodpower(n, checker_i, d);
    if (ginteqint(&checker_i, 1)) {
      continue;
    }
    flag = FALSE;
    for (j = 0; j < s; j++) {
      if (gequal(&checker_i, &n_minus_1)) {
        flag = TRUE;
        break;
      }
      checker_i = gmutiply(checker_i, checker_i);
      checker_i = gmodule(checker_i, n);
    }
    if (!flag) {
      return FALSE;
    }
  }
  return TRUE;
}
/// this function is to solve the eqution ed = 1 mod n
/// And d satisfies n | ed-1
/// if gcd(e,n)!=1 then int2gint(d,0);
gint ginverse(gint n, gint d) {
  gint r_ip, s_i, s_ip, q_ip, result;
  int2gint(&result, 1);
  gclone(&n, &r_ip);
  gint *rr_i, *rr_ip, *ss_i = &s_i, *ss_ip = &s_ip, *rresult = &result, *ttemp;
  int i = 0;
  rr_i = &d, rr_ip = &r_ip;
  int2gint(ss_ip, 0);
  while (TRUE) {
    q_ip = gdivide(*rr_i, *rr_ip);
    ttemp = rr_ip;
    rr_ip = ss_i;
    ss_i = rresult;
    rresult = ttemp;
    *rr_ip = gmodule(*rr_i, *rresult);
    ttemp = ss_ip;
    ss_ip = rr_i;
    rr_i = rresult;
    rresult = ttemp;
    *ss_ip = gadd(*ss_i, gmutiply(q_ip, *rresult));
    if (ginteqint(rr_ip, 0)) {
      if (i % 2 == 0) {
        *rresult = gminus(n, *rresult);
      }
      if (!ginteqint(rr_i, 1)) {
        int2gint(rresult, 0);
      }
      return *rresult;
    }
    i++;
  }
}

/// Will find the first prime greater or equal to `p`.
gint grandomprime(gint p, int *knownprime, int sizeofknownprime) {
  int i = 0;
  if (giseven(&p)) {
    p = gintaddint(p, 1);
  }
  int temp = 0;
  while (!gisprime(p, knownprime, sizeofknownprime)) {
    p = gintaddint(p, 2);
    i++;
    printf("tested %d numbers\n", i);
  }
  return p;
}

char *stradd(char *a, char *b, char *c) {
  int i = 0;
  while (*a) {
    c[i] = *a;
    i++;
    a++;
  }
  while (*b) {
    c[i] = *b;
    i++;
    b++;
  }
  c[i] = '\0';
  return c;
}
void gen(int digits, char *name) {
  srand((unsigned long long)time(NULL));
  gint p, q, n, phin, e, d, temp1, temp2;
  int knownprime[] = {2, 3, 5, 7, 11, 13};
  int sizeofknownprime = 6;
  grandom(&p, digits - 1);
  p = grandomprime(p, knownprime, sizeofknownprime);
  grandom(&q, digits + 1);
  q = grandomprime(q, knownprime, sizeofknownprime);
  printf("p:");
  gdisplay(p);
  printf("q:");
  gdisplay(q);
  n = gmutiply(p, q);
  p = gintminusint(p, 1);
  q = gintminusint(q, 1);
  phin = gmutiply(p, q);
  int2gint(&e, 0);
  while (ginteqint(&e, 0)) {
    grandom(&d, digits);
    e = ginverse(phin, d);
  }
  char str[GINT_LENGTH * GINT_DIGIT_BASE64 + 128];
  FILE *file = fopen(stradd(name, ".pub", str), "w");
  if (file != NULL) {
    gprint(e, str);
    fprintf(file, "%s", str);
    fclose(file);
  } else {
    perror("Error opening file");
  }
  file = fopen(stradd(name, ".sec", str), "w");
  if (file != NULL) {
    gprint(d, str);
    fprintf(file, "%s", str);
    fclose(file);
  } else {
    perror("Error opening file");
  }
  file = fopen(stradd(name, ".n", str), "w");
  if (file != NULL) {
    gprint(n, str);
    fprintf(file, "%s", str);
    fclose(file);
  } else {
    perror("Error opening file");
  }
  file = fopen(stradd(name, ".phin", str), "w");
  if (file != NULL) {
    gprint(phin, str);
    fprintf(file, "%s", str);
    fclose(file);
  } else {
    perror("Error opening file");
  }
}

/// this function is to do the encode or decode
/// will return message^secret mod n
void gcode(gint *n, gint *message, gint *key, gint *output) {
  *output = gmodpower(*n, *message, *key);
}
void readrsa(char *name, gint *n, gint *phin, gint *d, gint *e) {
  char str[100];
  FILE *fp = NULL;
  char buf[GINT_LENGTH * GINT_DIGIT_BASE64 + 128] = {0};
  int ret;
  fp = fopen(stradd(name, ".n", str), "r");
  if (NULL == fp) {
    printf("open file err!\n");
    exit(1);
  }
  ret = fread(buf, 1, GINT_LENGTH * GINT_DIGIT_BASE64 + 128, fp);
  str2gint(buf, n);
  fclose(fp);
  for (int i = 0; i < GINT_LENGTH * GINT_DIGIT_BASE64 + 128; i++) {
    buf[i] = '\0';
  }
  fp = fopen(stradd(name, ".phin", str), "r");
  if (NULL == fp) {
    printf("open file err!\n");
    exit(1);
  }
  ret = fread(buf, 1, GINT_LENGTH * GINT_DIGIT_BASE64 + 128, fp);
  str2gint(buf, phin);
  fclose(fp);

  for (int i = 0; i < GINT_LENGTH * GINT_DIGIT_BASE64 + 128; i++) {
    buf[i] = '\0';
  }
  fp = fopen(stradd(name, ".pub", str), "r");
  if (NULL == fp) {
    printf("open file err!\n");
    exit(1);
  }
  ret = fread(buf, 1, GINT_LENGTH * GINT_DIGIT_BASE64 + 128, fp);
  str2gint(buf, e);
  fclose(fp);
  for (int i = 0; i < GINT_LENGTH * GINT_DIGIT_BASE64 + 128; i++) {
    buf[i] = '\0';
  }
  fp = fopen(stradd(name, ".sec", str), "r");
  if (NULL == fp) {
    printf("open file err!\n");
    exit(1);
  }
  ret = fread(buf, 1, GINT_LENGTH * GINT_DIGIT_BASE64 + 128, fp);
  str2gint(buf, d);
  fclose(fp);
}
/* int main() { */
/*   gint a; */
/*   gint b; */
/*   int2gint(&a, 11); */
/*   int2gint(&b, 17); */
/*   gdisplay(ginverse(a, b)); */
/* } */
