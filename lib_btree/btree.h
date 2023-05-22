#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdio.h>
/*
 * B+Tree node format:
 * [key0, key1, ..., keyN] [val0, val1, ..., valN]
 * Each key is an array of unsigned longs, head->no_longs in total.
 * Total number of keys and vals (N) is head->no_pairs.
 */

#define BUG_ON(c) do { if (c) abort(); } while (0)
#define BUG() abort()
#define BITS_PER_LONG (sizeof(long) * 8)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned int __u32;
typedef unsigned long long u64;
typedef char s8;
typedef short s16;
typedef int s32;
typedef int __s32;
typedef long long s64;

struct btree_head {
	unsigned long *node;
	int height;
};

struct btree_geo {
	int keylen;
	int no_pairs;
};
extern struct btree_geo btree_geo32;
extern struct btree_geo btree_geo64;
extern struct btree_geo btree_geo128;

struct btree_headl { struct btree_head h; };
struct btree_head32 { struct btree_head h; };
struct btree_head64 { struct btree_head h; };
struct btree_head128 { struct btree_head h; };

/*
 * These couple of functions are all there is to it.  The rest of this header
 * consists only of wrappers that try to add some typesafety, make the code
 * a little self-documenting and generally be nice to people.
 */
void btree_free(void *element, void *pool_data);
void btree_init(struct btree_head *head);
void *btree_lookup(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key);
int btree_insert(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, void *val);
void *btree_remove(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key);
int btree_merge(struct btree_head *target, struct btree_head *victim,
		struct btree_geo *geo, unsigned long *duplicate);
unsigned long *btree_last(struct btree_head *head, struct btree_geo *geo);
size_t btree_visitor(struct btree_head *head, struct btree_geo *geo,
		long opaque,
		void (*func)(void *elem, long opaque, unsigned long *key,
			size_t index, void *func2), void *func2);
size_t btree_grim_visitor(struct btree_head *head, struct btree_geo *geo,
		long opaque,
		void (*func)(void *elem, long opaque, unsigned long *key,
			size_t index, void *func2), void *func2);

/* key is unsigned long */
static inline void btree_initl(struct btree_headl *head)
{
	btree_init(&head->h);
}

static inline void *btree_lookupl(struct btree_headl *head, unsigned long key)
{
	return btree_lookup(&head->h, &btree_geo32, &key);
}

static inline int btree_insertl(struct btree_headl *head, unsigned long key,
		void *val)
{
	return btree_insert(&head->h, &btree_geo32, &key, val);
}

static inline void *btree_removel(struct btree_headl *head, unsigned long key)
{
	return btree_remove(&head->h, &btree_geo32, &key);
}

static inline int btree_mergel(struct btree_headl *target,
		struct btree_headl *victim)
{
	unsigned long scratch;

	return btree_merge(&target->h, &victim->h, &btree_geo32, &scratch);
}

void visitorl(void *elem, long opaque, unsigned long *key, size_t index,
		void *__func);

typedef void (*visitorl_t)(void *elem, long opaque, unsigned long key,
		size_t index);

static inline size_t btree_visitorl(struct btree_headl *head, long opaque,
		visitorl_t func2)
{
	return btree_visitor(&head->h, &btree_geo32, opaque, visitorl, func2);
}

static inline size_t btree_grim_visitorl(struct btree_headl *head, long opaque,
		visitorl_t func2)
{
	return btree_grim_visitor(&head->h, &btree_geo32, opaque, visitorl, func2);
}

/* key is u32 */
static inline void btree_init32(struct btree_head32 *head)
{
	btree_init(&head->h);
}

static inline void *btree_lookup32(struct btree_head32 *head, u32 key)
{
	return btree_lookup(&head->h, &btree_geo32, (unsigned long *)&key);
}

static inline int btree_insert32(struct btree_head32 *head, u32 key,
		void *val)
{
	return btree_insert(&head->h, &btree_geo32, (unsigned long *)&key, val);
}

static inline void *btree_remove32(struct btree_head32 *head, u32 key)
{
	return btree_remove(&head->h, &btree_geo32, (unsigned long *)&key);
}

static inline int btree_merge32(struct btree_head32 *target,
		struct btree_head32 *victim)
{
	unsigned long scratch;

	return btree_merge(&target->h, &victim->h, &btree_geo32, &scratch);
}

void visitor32(void *elem, long opaque, unsigned long *__key, size_t index,
		void *__func);

typedef void (*visitor32_t)(void *elem, long opaque, u32 key, size_t index);

static inline size_t btree_visitor32(struct btree_head32 *head, long opaque,
		visitor32_t func2)
{
	return btree_visitor(&head->h, &btree_geo32, opaque, visitor32, func2);
}

static inline size_t btree_grim_visitor32(struct btree_head32 *head, long opaque,
		visitor32_t func2)
{
	return btree_grim_visitor(&head->h, &btree_geo32, opaque, visitor32, func2);
}

/* key is u64 */
static inline void btree_init64(struct btree_head64 *head)
{
	btree_init(&head->h);
}

static inline void *btree_lookup64(struct btree_head64 *head, u64 key)
{
	return btree_lookup(&head->h, &btree_geo64, (unsigned long *)&key);
}

static inline int btree_insert64(struct btree_head64 *head, u64 key,
		void *val)
{
	return btree_insert(&head->h, &btree_geo64, (unsigned long *)&key, val);
}

static inline void *btree_remove64(struct btree_head64 *head, u64 key)
{
	return btree_remove(&head->h, &btree_geo64, (unsigned long *)&key);
}

static inline u64 btree_last64(struct btree_head64 *head)
{
	u64 *key;

	key = (u64 *)btree_last(&head->h, &btree_geo64);
	if (key)
		return *key;
	else
		return 0;
}

static inline int btree_merge64(struct btree_head64 *target,
		struct btree_head64 *victim)
{
	u64 scratch;

	return btree_merge(&target->h, &victim->h, &btree_geo64,
			(unsigned long *)&scratch);
}

void visitor64(void *elem, long opaque, unsigned long *__key, size_t index,
		void *__func);

typedef void (*visitor64_t)(void *elem, long opaque, u64 key, size_t index);

static inline size_t btree_visitor64(struct btree_head64 *head, long opaque,
		visitor64_t func2)
{
	return btree_visitor(&head->h, &btree_geo64, opaque, visitor64, func2);
}

static inline size_t btree_grim_visitor64(struct btree_head64 *head, long opaque,
		visitor64_t func2)
{
	return btree_grim_visitor(&head->h, &btree_geo64, opaque, visitor64, func2);
}

/* key is 128bit (two u64) */
static inline void btree_init128(struct btree_head128 *head)
{
	btree_init(&head->h);
}

static inline void *btree_lookup128(struct btree_head128 *head, u64 k1, u64 k2)
{
	u64 key[2] = {k1, k2};
	return btree_lookup(&head->h, &btree_geo128, (unsigned long *)&key);
}

static inline int btree_insert128(struct btree_head128 *head, u64 k1, u64 k2,
		void *val)
{
	u64 key[2] = {k1, k2};
	return btree_insert(&head->h, &btree_geo128, (unsigned long *)&key, val);
}

static inline void *btree_remove128(struct btree_head128 *head, u64 k1, u64 k2)
{
	u64 key[2] = {k1, k2};
	return btree_remove(&head->h, &btree_geo128, (unsigned long *)&key);
}

void dumptree(struct btree_head *head, struct btree_geo *geo);

static inline void btree_last128(struct btree_head128 *head, u64 *k1, u64 *k2)
{
	u64 *key = (u64 *)btree_last(&head->h, &btree_geo128);

	if (key) {
		*k1 = key[0];
		*k2 = key[1];
	} else {
		*k1 = 0;
		*k2 = 0;
	}
}

static inline int btree_merge128(struct btree_head128 *target,
		struct btree_head128 *victim)
{
	u64 scratch[2];

	return btree_merge(&target->h, &victim->h, &btree_geo128,
			(unsigned long *)scratch);
}

void visitor128(void *elem, long opaque, unsigned long *__key, size_t index,
		void *__func);

typedef void (*visitor128_t)(void *elem, long opaque, u64 key1, u64 key2,
		size_t index);

static inline size_t btree_visitor128(struct btree_head128 *head, long opaque,
		visitor128_t func2)
{
	return btree_visitor(&head->h, &btree_geo128, opaque, visitor128, func2);
}

static inline size_t btree_grim_visitor128(struct btree_head128 *head, long opaque,
		visitor128_t func2)
{
	return btree_grim_visitor(&head->h, &btree_geo128, opaque, visitor128, func2);
}

#endif
