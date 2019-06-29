#include <string.h>
#include "mbuf.h"

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