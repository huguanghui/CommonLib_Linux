#include <ctype.h>
#include <fstream>
#include "zw_usbpipe_util.h"
#include "zw_aistick_util.h"

sem_t g_taskQueueSem;

void AS_TASK_QUEUE_INIT(struct taskQueue *tq)
{
	if (NULL == tq)
	{
		return;
	}
	pthread_mutex_init(&tq->task_mtx, NULL);
	sem_init(&tq->task_sem, 0, 0);
	tq->tqh_first = NULL;
	tq->tqh_last = &tq->tqh_first;
	return;
}

void AS_TASK_QUEUE_RELEASE(struct taskQueue *tq)
{
	struct taskNode* e = NULL;

	pthread_mutex_lock(&tq->task_mtx);
	while (!TAILQ_EMPTY(tq))
	{
		e = TAILQ_FIRST(tq);
		TAILQ_REMOVE(tq, e, nodes);
		free(e);
		e = NULL;
	}
	pthread_mutex_unlock(&tq->task_mtx);

	sem_destroy(&tq->task_sem);
	pthread_mutex_destroy(&tq->task_mtx);

	return;
}

void AS_TASK_QUEUE_INSERT(struct taskQueue *tq, struct taskNode *node)
{
	if (NULL == tq || NULL == node)
	{
		err("param error! tq[%p] node[%p]!\n", tq, node);
		return;
	}
	pthread_mutex_lock(&tq->task_mtx);
	TAILQ_INSERT_TAIL(tq, node, nodes);
	pthread_mutex_unlock(&tq->task_mtx);

	return;
}

struct taskNode* AS_TASK_QUEUE_GET(struct taskQueue *tq)
{
	if (TAILQ_EMPTY(tq))
	{
		return NULL;
	}

	struct taskNode* e = NULL;

	pthread_mutex_lock(&tq->task_mtx);
	e = TAILQ_FIRST(tq);
	TAILQ_REMOVE(tq, e, nodes);
	pthread_mutex_unlock(&tq->task_mtx);

	return e;
}

void AS_SHOW_TASK_QUEUE(struct taskQueue *tq)
{
	struct taskNode* e = NULL;
	TAILQ_FOREACH(e, tq, nodes)
	{
		// printf("%c ", e->c);
	}
	// printf("\n");
	return;
}

void AS_TASK_WAIT_QUEUE_INIT(struct waitQueue *tq)
{
	if (NULL == tq)
	{
		return;
	}
	pthread_mutex_init(&tq->mtx, NULL);
	tq->tqh_first = NULL;
	tq->tqh_last = &tq->tqh_first;

	return;
}

void AS_TASK_WAIT_QUEUE_RELEASE(struct waitQueue *tq)
{
	struct waitNode *e = NULL;

	pthread_mutex_lock(&tq->mtx);
	while (!TAILQ_EMPTY(tq))
	{
		e = TAILQ_FIRST(tq);
		TAILQ_REMOVE(tq, e, nodes);
		free(e);
		e = NULL;
	}
	pthread_mutex_unlock(&tq->mtx);
	pthread_mutex_destroy(&tq->mtx);

	return;
}

void AS_TASK_WAIT_QUEUE_INSERT(struct waitQueue *tq, struct waitNode *node)
{
	if (NULL == tq || NULL == node)
	{
		err("param error! tq[%p] node[%p]!\n", tq, node);
		return;
	}
	pthread_mutex_lock(&tq->mtx);
	TAILQ_INSERT_TAIL(tq, node, nodes);
	pthread_mutex_unlock(&tq->mtx);

	return;
}

struct waitNode* AS_TASK_WAIT_QUEUE_GET(struct waitQueue *tq)
{
	if (TAILQ_EMPTY(tq))
	{
		return NULL;
	}
	pthread_mutex_lock(&tq->mtx);
	struct waitNode *e = NULL;
	e = TAILQ_FIRST(tq);
	TAILQ_REMOVE(tq, e, nodes);
	pthread_mutex_unlock(&tq->mtx);

	return e;
}

struct waitNode* AS_TASK_WAIT_QUEUE_SEARCH_BY_SEQ(struct waitQueue *tq, int seq)
{
	if (TAILQ_EMPTY(tq))
	{
		return NULL;
	}

	pthread_mutex_lock(&tq->mtx);
	struct waitNode *e = NULL;
	TAILQ_FOREACH(e, tq, nodes)
	{
		if (e->req == seq)
		{
			TAILQ_REMOVE(tq, e, nodes);
			break;
		}
	}
	pthread_mutex_unlock(&tq->mtx);

	return e;
}

void AS_SHOW_TASK_WAIT_QUEUE(struct mbuf *m, struct waitQueue *tq)
{
	struct waitNode *e = NULL;
	char tmpBuf[1024];
	int idx = 0;

	snprintf(tmpBuf, sizeof(tmpBuf), "#========= _TASK_WAIT_QUEUE START\n");
	mbuf_append(m, tmpBuf, strlen(tmpBuf));

	pthread_mutex_lock(&tq->mtx);
	TAILQ_FOREACH(e, tq, nodes)
	{
		snprintf(tmpBuf, sizeof(tmpBuf), 
		"#Idx-%d\n"
		"#req-%d\n"
		"#type-%d\n\n",
		idx, e->req, e->type);
		mbuf_append(m, tmpBuf, strlen(tmpBuf));
		idx++;
	}
	pthread_mutex_unlock(&tq->mtx);

	snprintf(tmpBuf, sizeof(tmpBuf), "#========= _TASK_WAIT_QUEUE END\n\n");
	mbuf_append(m, tmpBuf, strlen(tmpBuf));


	return;
}

void AS_RESPONSE_QUEUE_INIT(struct responseQueue *tq)
{
	if (NULL == tq)
	{
		return;
	}
	pthread_mutex_init(&tq->mtx, NULL);
	sem_init(&tq->sem, 0, 0);
	tq->tqh_first = NULL;
	tq->tqh_last = &tq->tqh_first;
	return;
}

void AS_RESPONSE_QUEUE_RELEASE(struct responseQueue *tq)
{
	struct responseNode* e = NULL;

	pthread_mutex_lock(&tq->mtx);
	while (!TAILQ_EMPTY(tq))
	{
		e = TAILQ_FIRST(tq);
		TAILQ_REMOVE(tq, e, nodes);
		free(e);
		e = NULL;
	}
	pthread_mutex_unlock(&tq->mtx);
	sem_destroy(&tq->sem);
	pthread_mutex_destroy(&tq->mtx);
	return;
}

void AS_RESPONSE_QUEUE_INSERT(struct responseQueue *tq, struct responseNode *node)
{
	if (NULL == tq || NULL == node)
	{
		err("param error! tq[%p] node[%p]!\n", tq, node);
		return;
	}
	pthread_mutex_lock(&tq->mtx);
	TAILQ_INSERT_HEAD(tq, node, nodes);
	pthread_mutex_unlock(&tq->mtx);
	return;
}

struct responseNode* AS_RESPONSE_QUEUE_GET(struct responseQueue *tq)
{
	if (TAILQ_EMPTY(tq))
	{
		return NULL;
	}

	struct responseNode* e = NULL;
	pthread_mutex_lock(&tq->mtx);
	e = TAILQ_FIRST(tq);
	pthread_mutex_unlock(&tq->mtx);
	return e;
}

struct responseNode* AS_RESPONSE_QUEUE_SEARCH_BY_SEQ(struct responseQueue *tq, int seq)
{
	if (TAILQ_EMPTY(tq))
	{
		return NULL;
	}

	struct responseNode* e = NULL;
	pthread_mutex_lock(&tq->mtx);
	TAILQ_FOREACH(e, tq, nodes)
	{
		if (e->seq == seq)
		{
				TAILQ_REMOVE(tq, e, nodes);
				break;
		}
	}
	pthread_mutex_unlock(&tq->mtx);
	return e;
}

void AS_SHOW_RESPONSE_QUEUE(struct responseQueue* tq)
{
	struct responseNode* e = NULL;
	pthread_mutex_lock(&tq->mtx);
	TAILQ_FOREACH(e, tq, nodes)
	{
		// printf("%c ", e->c);
	}
	pthread_mutex_unlock(&tq->mtx);
	// printf("\n");
	return;
}

/*************************************** mbuf ***************************************/

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

#define MG_BUFFER_SIZE 1000

static int mbuf_avprintf(char **buf, int size, const char *fmt, va_list ap)
{
	va_list ap_copy;
	int len;

	va_copy(ap_copy, ap);
	len = vsnprintf(*buf, size, fmt, ap_copy);
	if (len >= size)
	{
		*buf = (char *)malloc(len+1);
		va_copy(ap_copy, ap);
		len = vsnprintf(*buf, len+1, fmt, ap_copy);
		va_end(ap_copy);
	}
	return len;
}

static int mbuf_vprintf(struct mbuf *m, const char *fmt, va_list ap)
{
	int len;
	char mem[MG_BUFFER_SIZE], *buf = mem;

	if ((len = mbuf_avprintf(&buf, sizeof(mem), fmt, ap)) > 0)
	{
		mbuf_append(m, buf, len);
	}
	if (buf != mem && buf != NULL)
	{
		free(buf);
	}

	return len;
}

int mbuf_printf(struct mbuf *m, const char *fmt, ...)
{
	int len;
	va_list ap;
	va_start(ap, fmt);
	len = mbuf_vprintf(m, fmt, ap);
	va_end(ap);

	return len;
}

/*************************************** mg_str ***************************************/

static int str_util_lowercase(const char *s)
{
	return tolower(*(const unsigned char *) s);
}

int mg_ncasecmp(const char *s1, const char *s2, size_t len)
{
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
	if (r == 0)
	{
		return n1 - n2;
	}
	return r;
}

static struct mg_str mg_strdup_common(const struct mg_str s,
                                      int nul_terminate) 
{
	struct mg_str r = {NULL, 0};
	if (s.len > 0 && s.p != NULL)
	{
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

struct mg_str mg_strdup(const struct mg_str s)
{
	return mg_strdup_common(s, 0 /* NUL-terminate */);
}

struct mg_str mg_strdup_nul(const struct mg_str s)
{
	return mg_strdup_common(s, 1 /* NUL-terminate */);
}

const char *mg_strchr(const struct mg_str s, int c) {
	size_t i;
	for (i = 0; i < s.len; i++) 
	{
	if (s.p[i] == c) return &s.p[i];
	}
	return NULL;
}

int mg_strcmp(const struct mg_str str1, const struct mg_str str2)
{
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

	if (s1.len > n)
	{
		s1.len = n;
	}
	if (s2.len > n)
	{
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

/*************************************** base64 ***************************************/
/* ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ */

#define NUM_UPPERCASES ('Z' - 'A' + 1)
#define NUM_LETTERS (NUM_UPPERCASES * 2)
#define NUM_DIGITS ('9' - '0' + 1)

/*
 * Emit a base64 code char.
 *
 * Doesn't use memory, thus it's safe to use to safely dump memory in crashdumps
 */
static void cs_base64_emit_code(struct cs_base64_ctx *ctx, int v) 
{
	if (v < NUM_UPPERCASES) {
	ctx->b64_putc(v + 'A', ctx->user_data);
	} else if (v < (NUM_LETTERS)) {
	ctx->b64_putc(v - NUM_UPPERCASES + 'a', ctx->user_data);
	} else if (v < (NUM_LETTERS + NUM_DIGITS)) {
	ctx->b64_putc(v - NUM_LETTERS + '0', ctx->user_data);
	} else {
	ctx->b64_putc(v - NUM_LETTERS - NUM_DIGITS == 0 ? '+' : '/',
					ctx->user_data);
	}
}

static void cs_base64_emit_chunk(struct cs_base64_ctx *ctx) 
{
	int a, b, c;

	a = ctx->chunk[0];
	b = ctx->chunk[1];
	c = ctx->chunk[2];

	cs_base64_emit_code(ctx, a >> 2);
	cs_base64_emit_code(ctx, ((a & 3) << 4) | (b >> 4));
	if (ctx->chunk_size > 1) {
	cs_base64_emit_code(ctx, (b & 15) << 2 | (c >> 6));
	}
	if (ctx->chunk_size > 2) {
	cs_base64_emit_code(ctx, c & 63);
	}
}

void cs_base64_init(struct cs_base64_ctx *ctx, cs_base64_putc_t b64_putc,
                    void *user_data) {
	ctx->chunk_size = 0;
	ctx->b64_putc = b64_putc;
	ctx->user_data = user_data;
}

void cs_base64_update(struct cs_base64_ctx *ctx, const char *str, size_t len) {
	const unsigned char *src = (const unsigned char *) str;
	size_t i;
	for (i = 0; i < len; i++) {
	ctx->chunk[ctx->chunk_size++] = src[i];
	if (ctx->chunk_size == 3) {
		cs_base64_emit_chunk(ctx);
		ctx->chunk_size = 0;
	}
	}
}

void cs_base64_finish(struct cs_base64_ctx *ctx) {
	if (ctx->chunk_size > 0) {
	int i;
	memset(&ctx->chunk[ctx->chunk_size], 0, 3 - ctx->chunk_size);
	cs_base64_emit_chunk(ctx);
	for (i = 0; i < (3 - ctx->chunk_size); i++) {
		ctx->b64_putc('=', ctx->user_data);
	}
	}
}

#define BASE64_ENCODE_BODY                                                \
  static const char *b64 =                                                \
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; \
  int i, j, a, b, c;                                                      \
                                                                          \
  for (i = j = 0; i < src_len; i += 3) {                                  \
    a = src[i];                                                           \
    b = i + 1 >= src_len ? 0 : src[i + 1];                                \
    c = i + 2 >= src_len ? 0 : src[i + 2];                                \
                                                                          \
    BASE64_OUT(b64[a >> 2]);                                              \
    BASE64_OUT(b64[((a & 3) << 4) | (b >> 4)]);                           \
    if (i + 1 < src_len) {                                                \
      BASE64_OUT(b64[(b & 15) << 2 | (c >> 6)]);                          \
    }                                                                     \
    if (i + 2 < src_len) {                                                \
      BASE64_OUT(b64[c & 63]);                                            \
    }                                                                     \
  }                                                                       \
                                                                          \
  while (j % 4 != 0) {                                                    \
    BASE64_OUT('=');                                                      \
  }                                                                       \
  BASE64_FLUSH()

#define BASE64_OUT(ch) \
  do {                 \
    dst[j++] = (ch);   \
  } while (0)

#define BASE64_FLUSH() \
  do {                 \
    dst[j++] = '\0';   \
  } while (0)

void cs_base64_encode(const unsigned char *src, int src_len, char *dst) {
  BASE64_ENCODE_BODY;
}

#undef BASE64_OUT
#undef BASE64_FLUSH

#if CS_ENABLE_STDIO
#define BASE64_OUT(ch)      \
  do {                      \
    fprintf(f, "%c", (
      
    )); \
    j++;                    \
  } while (0)

#define BASE64_FLUSH()

void cs_fprint_base64(FILE *f, const unsigned char *src, int src_len) {
	BASE64_ENCODE_BODY;
}

#undef BASE64_OUT
#undef BASE64_FLUSH
#endif /* CS_ENABLE_STDIO */

/* Convert one byte of encoded base64 input stream to 6-bit chunk */
static unsigned char from_b64(unsigned char ch) {
	/* Inverse lookup map */
	static const unsigned char tab[128] = {
		255, 255, 255, 255,
		255, 255, 255, 255, /*  0 */
		255, 255, 255, 255,
		255, 255, 255, 255, /*  8 */
		255, 255, 255, 255,
		255, 255, 255, 255, /*  16 */
		255, 255, 255, 255,
		255, 255, 255, 255, /*  24 */
		255, 255, 255, 255,
		255, 255, 255, 255, /*  32 */
		255, 255, 255, 62,
		255, 255, 255, 63, /*  40 */
		52,  53,  54,  55,
		56,  57,  58,  59, /*  48 */
		60,  61,  255, 255,
		255, 200, 255, 255, /*  56   '=' is 200, on index 61 */
		255, 0,   1,   2,
		3,   4,   5,   6, /*  64 */
		7,   8,   9,   10,
		11,  12,  13,  14, /*  72 */
		15,  16,  17,  18,
		19,  20,  21,  22, /*  80 */
		23,  24,  25,  255,
		255, 255, 255, 255, /*  88 */
		255, 26,  27,  28,
		29,  30,  31,  32, /*  96 */
		33,  34,  35,  36,
		37,  38,  39,  40, /*  104 */
		41,  42,  43,  44,
		45,  46,  47,  48, /*  112 */
		49,  50,  51,  255,
		255, 255, 255, 255, /*  120 */
	};
	return tab[ch & 127];
}

int cs_base64_decode(const unsigned char *s, int len, char *dst, int *dec_len) {
	unsigned char a, b, c, d;
	int orig_len = len;
	char *orig_dst = dst;
	while (len >= 4 && (a = from_b64(s[0])) != 255 &&
					(b = from_b64(s[1])) != 255 && (c = from_b64(s[2])) != 255 &&
					(d = from_b64(s[3])) != 255) {
		s += 4;
		len -= 4;
		if (a == 200 || b == 200) break; /* '=' can't be there */
		*dst++ = a << 2 | b >> 4;
		if (c == 200) break;
		*dst++ = b << 4 | c >> 2;
		if (d == 200) break;
		*dst++ = c << 6 | d;
	}
	*dst = 0;
	if (dec_len != NULL) *dec_len = (dst - orig_dst);
	return orig_len - len;
}

void aistick_util_save2file(const char *filename, void *s, int len, int type)
{
	if (NULL == filename || NULL == s || len <= 0)
	{
		return;
	}

	std::ofstream openfile;
	if (0 == type)
	{
		openfile.open(filename);
	}
	else
	{
		openfile.open(filename, std::ios::app);
	}
	openfile.write((char*)s, len);
	openfile.close();
}
