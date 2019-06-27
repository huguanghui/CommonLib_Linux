#include <stdio.h>
#include "mg_util.h"

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

void mbuf_init(struct mbuf *mbuf, size_t initial_size) 
{
  mbuf->len = mbuf->size = 0;
  mbuf->buf = NULL;
  mbuf_resize(mbuf, initial_size);
  return;
}

void mbuf_free(struct mbuf *mbuf) 
{
  if (mbuf->buf != NULL) {
    free(mbuf->buf);
    mbuf_init(mbuf, 0);
  }
  return;
}

void mbuf_resize(struct mbuf *a, size_t new_size) 
{
  if (new_size > a->size || (new_size < a->size && new_size >= a->len)) 
  {
    char *buf = (char *) realloc(a->buf, new_size);
    /*
     * In case realloc fails, there's not much we can do, except keep things as
     * they are. Note that NULL is a valid return value from realloc when
     * size == 0, but that is covered too.
     */
    if (buf == NULL && new_size != 0)
    {
        return;
    }
    a->buf = buf;
    a->size = new_size;
  }
  return;
}

void mbuf_trim(struct mbuf *mbuf) 
{
  mbuf_resize(mbuf, mbuf->len);
  return;
}

size_t mbuf_insert(struct mbuf *a, size_t off, const void *buf, size_t len) 
{
  if ((NULL == a)||
    (a->len > a->size)||
    (off > a->len))
  {
      return 0;
  }

  char *p = NULL;
  /* check overflow */
  if (~(size_t) 0 - (size_t) a->buf < len)
  {
    return 0;
  }

  if (a->len + len <= a->size) 
  {
    memmove(a->buf + off + len, a->buf + off, a->len - off);
    if (buf != NULL) 
    {
      memcpy(a->buf + off, buf, len);
    }
    a->len += len;
  } 
  else 
  {
    size_t min_size = (a->len + len);
    size_t new_size = (size_t)(min_size * MBUF_SIZE_MULTIPLIER);
    if (new_size - min_size > MBUF_SIZE_MAX_HEADROOM) 
    {
      new_size = min_size + MBUF_SIZE_MAX_HEADROOM;
    }
    p = (char *) realloc(a->buf, new_size);
    if (p == NULL && new_size != min_size) 
    {
      new_size = min_size;
      p = (char *) realloc(a->buf, new_size);
    }
    if (p != NULL) 
    {
      a->buf = p;
      if (off != a->len) 
      {
        memmove(a->buf + off + len, a->buf + off, a->len - off);
      }
      if (buf != NULL) memcpy(a->buf + off, buf, len);
      a->len += len;
      a->size = new_size;
    } 
    else 
    {
      len = 0;
    }
  }

  return len;
}

size_t mbuf_append(struct mbuf *a, const void *buf, size_t len) 
{
  return mbuf_insert(a, a->len, buf, len);
}

size_t mbuf_append_and_free(struct mbuf *a, void *data, size_t len) 
{
  size_t ret;
  /* Optimization: if the buffer is currently empty,
   * take over the user-provided buffer. */
  if (a->len == 0) 
  {
    if (a->buf != NULL) free(a->buf);
    a->buf = (char *) data;
    a->len = a->size = len;
    return len;
  }
  ret = mbuf_insert(a, a->len, data, len);
  free(data);
  return ret;
}

void mbuf_remove(struct mbuf *mb, size_t n) 
{
  if (n > 0 && n <= mb->len) 
  {
    memmove(mb->buf, mb->buf + n, mb->len - n);
    mb->len -= n;
  }
  return;
}

void mbuf_clear(struct mbuf *mb) 
{
  mb->len = 0;
  return;
}

void mbuf_move(struct mbuf *from, struct mbuf *to) 
{
  memcpy(to, from, sizeof(*to));
  memset(from, 0, sizeof(*from));
  return;
}

