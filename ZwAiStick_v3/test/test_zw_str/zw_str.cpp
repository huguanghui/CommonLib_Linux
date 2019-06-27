#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "zw_str.h"

static int str_util_lowercase(const char *s) {
  return tolower(*(const unsigned char *) s);
}

int mg_ncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;

  if (len > 0) do {
      diff = str_util_lowercase(s1++) - str_util_lowercase(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);

  return diff;
}

struct mg_str mg_mk_str(const char *s) 
{
  struct mg_str ret = {s, 0};
  if (s != NULL) ret.len = strlen(s);
  return ret;
}

struct mg_str mg_mk_str_n(const char *s, size_t len) 
{
  struct mg_str ret = {s, len};
  return ret;
}

int mg_vcmp(const struct mg_str *str1, const char *str2) 
{
  size_t n2 = strlen(str2), n1 = str1->len;
  int r = strncmp(str1->p, str2, (n1 < n2) ? n1 : n2);
  if (r == 0) 
  {
    return n1 - n2;
  }
  return r;
}

int mg_vcasecmp(const struct mg_str *str1, const char *str2) 
{
  size_t n2 = strlen(str2), n1 = str1->len;
  int r = mg_ncasecmp(str1->p, str2, (n1 < n2) ? n1 : n2);
  if (r == 0) {
    return n1 - n2;
  }
  return r;
}

static struct mg_str mg_strdup_common(const struct mg_str s,
                                      int nul_terminate) 
{
  struct mg_str r = {NULL, 0};
  if (s.len > 0 && s.p != NULL) {
    char *sc = (char *) malloc(s.len + (nul_terminate ? 1 : 0));
    if (sc != NULL) {
      memcpy(sc, s.p, s.len);
      if (nul_terminate) sc[s.len] = '\0';
      r.p = sc;
      r.len = s.len;
    }
  }
  return r;
}

struct mg_str mg_strdup(const struct mg_str s) {
  return mg_strdup_common(s, 0 /* NUL-terminate */);
}

struct mg_str mg_strdup_nul(const struct mg_str s) {
  return mg_strdup_common(s, 1 /* NUL-terminate */);
}

const char *mg_strchr(const struct mg_str s, int c) {
  size_t i;
  for (i = 0; i < s.len; i++) {
    if (s.p[i] == c) return &s.p[i];
  }
  return NULL;
}

int mg_strcmp(const struct mg_str str1, const struct mg_str str2) {
  size_t i = 0;
  while (i < str1.len && i < str2.len) {
    if (str1.p[i] < str2.p[i]) return -1;
    if (str1.p[i] > str2.p[i]) return 1;
    i++;
  }
  if (i < str1.len) return 1;
  if (i < str2.len) return -1;
  return 0;
}

int mg_strncmp(const struct mg_str str1, const struct mg_str str2, size_t n) {
  struct mg_str s1 = str1;
  struct mg_str s2 = str2;

  if (s1.len > n) {
    s1.len = n;
  }
  if (s2.len > n) {
    s2.len = n;
  }
  return mg_strcmp(s1, s2);
}

void mg_strfree(struct mg_str *s) 
{
  char *sp = (char *) s->p;
  s->p = NULL;
  s->len = 0;
  if (sp != NULL) free(sp);
}

const char *mg_strstr(const struct mg_str haystack,
                      const struct mg_str needle) 
{
  size_t i;
  if (needle.len > haystack.len) return NULL;
  for (i = 0; i <= haystack.len - needle.len; i++) 
  {
    if (memcmp(haystack.p + i, needle.p, needle.len) == 0) 
    {
      return haystack.p + i;
    }
  }
  return NULL;
}

struct mg_str mg_strstrip(struct mg_str s) 
{
  while (s.len > 0 && isspace((int) *s.p)) 
  {
    s.p++;
    s.len--;
  }
  while (s.len > 0 && isspace((int) *(s.p + s.len - 1))) 
  {
    s.len--;
  }
  return s;
}

int mg_str_starts_with(struct mg_str s, struct mg_str prefix) 
{
  const struct mg_str sp = MG_MK_STR_N(s.p, prefix.len);
  if (s.len < prefix.len) return 0;
  return (mg_strcmp(sp, prefix) == 0);
}
