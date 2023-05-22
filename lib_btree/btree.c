#include <errno.h>
#include <stddef.h>
#include "btree.h"

/*
 * Depending on the ratio of lookups vs. insert and removes, it may be
 * beneficial to spend more work trying to keep the tree as compact as
 * possible.  With roughly 50 lookups for every insert/remove, stealing
 * from neighbours becomes more effective.  If that is the case, please
 * define AGGRESSIVE_COMPACTION below
 */
#define AGGRESSIVE_COMPACTION

#ifndef L1_CACHE_BYTES
#define L1_CACHE_BYTES 128
#endif

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define NODESIZE MAX(L1_CACHE_BYTES, 128)

struct btree_geo btree_geo32 = {
	.keylen = 1,
	.no_pairs = NODESIZE / sizeof(long) / 2,
};

#define LONG_PER_U64 (64 / BITS_PER_LONG)
struct btree_geo btree_geo64 = {
	.keylen = LONG_PER_U64,
	.no_pairs = NODESIZE / sizeof(long) / (1 + LONG_PER_U64),
};

struct btree_geo btree_geo128 = {
	.keylen = 2 * LONG_PER_U64,
	.no_pairs = NODESIZE / sizeof(long) / (1 + 2 * LONG_PER_U64),
};

static unsigned long *btree_node_alloc(struct btree_head *head)
{
	return calloc(1, NODESIZE);
}

static int longcmp(const unsigned long *l1, const unsigned long *l2, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		if (l1[i] < l2[i])
			return -1;
		if (l1[i] > l2[i])
			return 1;
	}
	return 0;
}

static unsigned long *longcpy(unsigned long *dest, const unsigned long *src,
		size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		dest[i] = src[i];
	return dest;
}

static unsigned long *longset(unsigned long *s, unsigned long c, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		s[i] = c;
	return s;
}

/*
 * B+Tree node format:
 * [key0, key1, ..., keyN] [val0, val1, ..., valN]
 * Each key is an array of unsigned longs, head->keylen in total.
 * Total number of keys and vals (N) is head->no_pairs.
 */

static unsigned long *bkey(struct btree_geo *geo, unsigned long *node, int n)
{
	return &node[n * geo->keylen];
}

static unsigned long bval(struct btree_geo *geo, unsigned long *node, int n)
{
	return node[geo->no_pairs * geo->keylen + n];
}

static void setkeyb(struct btree_geo *geo, unsigned long *node,
		unsigned long *key, int n)
{
	longcpy(bkey(geo, node, n), key, geo->keylen);
}

static void setval(struct btree_geo *geo, unsigned long *node,
		unsigned long val, int n)
{
	node[geo->no_pairs * geo->keylen + n] = val;
}

static void clearpair(struct btree_geo *geo, unsigned long *node, int n)
{
	longset(bkey(geo, node, n), 0, geo->keylen);
	node[geo->no_pairs * geo->keylen + n] = 0;
}

static void dumpkey(struct btree_geo *geo, unsigned long *key)
{
	int k;

	printf("k(%lx", key[0]);
	for (k = 1; k < geo->keylen; k++)
		printf(",%lx", key[k]);
	printf(")");
}

static void dumpnode(struct btree_geo *geo, unsigned long *node)
{
	int i;
	unsigned long *key;

	printf("N:%p: ", node);
	for (i = 0; i < geo->no_pairs; i++) {
		key = bkey(geo, node, i);
		dumpkey(geo, key);
		printf("v(%lx) ", bval(geo, node, i));
	}
	printf("\n");
}

static void __dumptree(struct btree_head *head, struct btree_geo *geo,
		unsigned long *node, int height)
{
	int i;
	unsigned long *child;

	if (!height)
		return;

	printf("h:%2x ", height);
	dumpnode(geo, node);
	for (i = 0; i < geo->no_pairs; i++) {
		child = (void *)bval(geo, node, i);
		if (!child)
			return;
		__dumptree(head, geo, child, height - 1);
	}
}

void dumptree(struct btree_head *head, struct btree_geo *geo)
{
	__dumptree(head, geo, head->node, head->height);
}

static inline void __btree_init(struct btree_head *head)
{
	head->node = NULL;
	head->height = 0;
}

void btree_init(struct btree_head *head)
{
	__btree_init(head);
}

unsigned long *btree_last(struct btree_head *head, struct btree_geo *geo)
{
	int height = head->height;
	unsigned long *node = head->node;

	if (height == 0)
		return NULL;

	for ( ; height > 1; height--)
		node = (unsigned long *)bval(geo, node, 0);

	return bkey(geo, node, 0);
}

static int keycmp(struct btree_geo *geo, unsigned long *node, int pos,
		unsigned long *key)
{
	return longcmp(bkey(geo, node, pos), key, geo->keylen);
}

void *btree_lookup(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key)
{
	int i, height = head->height;
	unsigned long *node = head->node;

	if (height == 0)
		return NULL;

	for ( ; height > 1; height--) {
		for (i = 0; i < geo->no_pairs; i++)
			if (keycmp(geo, node, i, key) <= 0)
				break;
		if (i == geo->no_pairs)
			return NULL;
		node = (unsigned long *)bval(geo, node, i);
		if (!node)
			return NULL;
	}

	if (!node)
		return NULL;

	for (i = 0; i < geo->no_pairs; i++)
		if (keycmp(geo, node, i, key) == 0)
			return (void *)bval(geo, node, i);
	return NULL;
}

static int getpos(struct btree_geo *geo, unsigned long *node,
		unsigned long *key)
{
	int i;

	for (i = 0; i < geo->no_pairs; i++) {
		if (keycmp(geo, node, i, key) <= 0)
			break;
	}
	return i;
}

static int getfill(struct btree_geo *geo, unsigned long *node, int start)
{
	int i;

	for (i = start; i < geo->no_pairs; i++)
		if (bval(geo, node, i) == 0)
			break;
	return i;
}

/*
 * locate the correct leaf node in the btree
 */
static unsigned long *find_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level)
{
	unsigned long *node = head->node;
	int i, height;

	for (height = head->height; height > level; height--) {
		for (i = 0; i < geo->no_pairs; i++)
			if (keycmp(geo, node, i, key) <= 0)
				break;

		if ((i == geo->no_pairs) || !bval(geo, node, i)) {
			/* right-most key is too large, update it */
			/* FIXME: If the right-most key on higher levels is
			 * always zero, this wouldn't be necessary. */
			i--;
			setkeyb(geo, node, key, i);
		}
		BUG_ON(i < 0);
		node = (unsigned long *)bval(geo, node, i);
	}
	BUG_ON(!node);
	return node;
}

static int btree_grow(struct btree_head *head, struct btree_geo *geo)
{
	unsigned long *node;
	int fill;

	node = btree_node_alloc(head);
	if (!node)
		return -ENOMEM;
	printf("node %p\n", node);
	if (head->node) {
		fill = getfill(geo, head->node, 0);
		setkeyb(geo, node, bkey(geo, head->node, fill - 1), 0);
		setval(geo, node, (unsigned long)head->node, 0);
	}
	head->node = node;
	head->height++;
	return 0;
}

static void btree_shrink(struct btree_head *head, struct btree_geo *geo,
		int fill)
{
	unsigned long *node;

	if ((fill == 0) || ((fill == 1) && (head->height > 1))) {
		node = head->node;
		head->node = (unsigned long *)bval(geo, node, 0);
		head->height--;
		free(node);
	}
}

static void steal_l(struct btree_head *head, struct btree_geo *geo, int level,
		unsigned long *left, int lfill,
		unsigned long *right, int rfill,
		unsigned long *parent, int lpos,
		int no_entries)
{
	int i;

	for (i = rfill - 1; i >= 0; i--) {
		/* Shift entries on the right */
		setkeyb(geo, right, bkey(geo, right, i), i + no_entries);
		setval(geo, right, bval(geo, right, i), i + no_entries);
	}
	for (i = 0; i < no_entries; i++) {
		/* Move some entries to the right */
		setkeyb(geo, right, bkey(geo, left, lfill - no_entries + i), i);
		setval(geo, right, bval(geo, left, lfill - no_entries + i), i);
	}
	/* Set parent key */
	setkeyb(geo, parent, bkey(geo, left, lfill - no_entries - 1), lpos);
	for (i = lfill - no_entries; i < lfill; i++)
		clearpair(geo, left, i);
}

static void steal_r(struct btree_head *head, struct btree_geo *geo, int level,
		unsigned long *left, int lfill,
		unsigned long *right, int rfill,
		unsigned long *parent, int lpos,
		int no_entries)
{
	int i;

	for (i = 0; i < no_entries; i++) {
		/* Move some entries to the left */
		setkeyb(geo, left, bkey(geo, right, i), lfill + i);
		setval(geo, left, bval(geo, right, i), lfill + i);
	}
	/* Set parent key */
	setkeyb(geo, parent, bkey(geo, right, no_entries - 1), lpos);
	/* Shift entries on the right */
	for ( ; i < rfill; i++) {
		setkeyb(geo, right, bkey(geo, right, i), i - no_entries);
		setval(geo, right, bval(geo, right, i), i - no_entries);
	}
	for (i = rfill - no_entries; i < rfill; i++)
		clearpair(geo, right, i);
}

static int btree_insert_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, unsigned long val, int level);
static int split(struct btree_head *head, struct btree_geo *geo,
		unsigned long *node, int level)
{
	unsigned long *new;
	int i, err, fill = geo->no_pairs;

	new = btree_node_alloc(head);
	if (!new)
		return -ENOMEM;
	err = btree_insert_level(head, geo,
			bkey(geo, node, fill / 2 - 1),
			(unsigned long)new, level + 1);
	if (err) {
		free(new);
		return err;
	}
	for (i = 0; i < fill / 2; i++) {
		setkeyb(geo, new, bkey(geo, node, i), i);
		setval(geo, new, bval(geo, node, i), i);
		setkeyb(geo, node, bkey(geo, node, i + fill / 2), i);
		setval(geo, node, bval(geo, node, i + fill / 2), i);
		clearpair(geo, node, i + fill / 2);
	}
	if (fill & 1) {
		setkeyb(geo, node, bkey(geo, node, fill - 1), i);
		setval(geo, node, bval(geo, node, fill - 1), i);
		clearpair(geo, node, fill - 1);
	}
	return 0;
}

static int rebalance_insert(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, unsigned long *child, int level)
{
#ifdef AGGRESSIVE_COMPACTION
	unsigned long *parent, *left, *right;
	int child_no, no_left, no_right, delta;

	if (level == head->height)
		goto split;

	parent = find_level(head, geo, key, level + 1);
	child_no = getpos(geo, parent, key);
	BUG_ON(bval(geo, parent, child_no) != (unsigned long)child);

	if (child_no > 0) {
		left = (unsigned long *)bval(geo, parent, child_no - 1);
		no_left = getfill(geo, left, 0);
		delta = geo->no_pairs - no_left;
		if (delta >= 2) {
			steal_r(head, geo, level,
					left, no_left,
					child, geo->no_pairs,
					parent, child_no - 1, delta / 2);
			return 0;
		}
	}
	if (child_no + 1 < getfill(geo, parent, child_no)) {
		right = (unsigned long *)bval(geo, parent, child_no + 1);
		no_right = getfill(geo, right, 0);
		delta = geo->no_pairs - no_right;
		if (delta >= 2) {
			steal_l(head, geo, level,
					child, geo->no_pairs,
					right, no_right,
					parent, child_no, delta / 2);
			return 0;
		}
	}
split:
#endif
	return split(head, geo, child, level);
}

static int btree_insert_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, unsigned long val, int level)
{
	unsigned long *node;
	int i, pos, fill, err;

	BUG_ON(!val);
	if (head->height < level) {
		err = btree_grow(head, geo);
		if (err)
			return err;
	}

retry:
	node = find_level(head, geo, key, level);
	pos = getpos(geo, node, key);
	fill = getfill(geo, node, pos);
	/* two identical keys are not allowed */
	BUG_ON(pos < fill && keycmp(geo, node, pos, key) == 0);

	if (fill == geo->no_pairs) {
		/* need to split node */
		err = rebalance_insert(head, geo, key, node, level);
		if (err)
			return err;
		goto retry;
	}
	BUG_ON(fill >= geo->no_pairs);

	/* shift and insert */
	for (i = fill; i > pos; i--) {
		setkeyb(geo, node, bkey(geo, node, i - 1), i);
		setval(geo, node, bval(geo, node, i - 1), i);
	}
	setkeyb(geo, node, key, pos);
	setval(geo, node, val, pos);

	return 0;
}

int btree_insert(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, void *val)
{
	return btree_insert_level(head, geo, key, (unsigned long)val, 1);
}

static void *btree_remove_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level);
static void merge(struct btree_head *head, struct btree_geo *geo, int level,
		unsigned long *left, int lfill,
		unsigned long *right, int rfill,
		unsigned long *parent, int lpos)
{
	int i;

	for (i = 0; i < rfill; i++) {
		/* Move all entries to the left */
		setkeyb(geo, left, bkey(geo, right, i), lfill + i);
		setval(geo, left, bval(geo, right, i), lfill + i);
	}
	/* Exchange left and right child in parent */
	setval(geo, parent, (unsigned long)right, lpos);
	setval(geo, parent, (unsigned long)left, lpos + 1);
	/* Remove left (formerly right) child from parent */
	btree_remove_level(head, geo, bkey(geo, parent, lpos), level + 1);
	free(right);
}

static void rebalance(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level, unsigned long *child, int fill)
{
	unsigned long *parent, *left = NULL, *right = NULL;
	int child_no, no_left, no_right, i;

	parent = find_level(head, geo, key, level + 1);
	child_no = getpos(geo, parent, key);
	BUG_ON(bval(geo, parent, child_no) != (unsigned long)child);

	if (child_no > 0) {
		left = (unsigned long *)bval(geo, parent, child_no - 1);
		no_left = getfill(geo, left, 0);
		if (fill + no_left <= geo->no_pairs) {
			/* Merge with left neighbour */
			merge(head, geo, level,
					left, no_left,
					child, fill,
					parent, child_no - 1);
			return;
		}
	}
	if (child_no + 1 < getfill(geo, parent, child_no)) {
		right = (unsigned long *)bval(geo, parent, child_no + 1);
		no_right = getfill(geo, right, 0);
		if (fill + no_right <= geo->no_pairs) {
			/* Merge with right neighbour */
			merge(head, geo, level,
					child, fill,
					right, no_right,
					parent, child_no);
			return;
		}
	}
	/*
	 * Leaving the btree in a somewhat unbalanced state can improve
	 * performance.  Stealing entries from a neighbour is a fairly
	 * expensive operation.  In trees where reads completely dominate
	 * writes, the cost will be amortized sooner or later.  When the
	 * ratio of writes increases, they may never be amortized.
	 *
	 * So avoid stealing unless the tree would get _really_ unbalanced.
	 */
	if (fill > 1)
		return;
	if (left) {
		/* Steal from left neighbour */
		i = (no_left - fill) / 2;
		BUG_ON(i < 1);
		steal_l(head, geo, level,
				left, no_left,
				child, fill,
				parent, child_no - 1, i);
		return;
	}
	if (right) {
		/* Steal from right neighbour */
		i = (no_right - fill) / 2;
		BUG_ON(i < 1);
		steal_r(head, geo, level,
				child, fill,
				right, no_right,
				parent, child_no, i);
		return;
	}
	BUG(); /* We should never get here */
}

static void *btree_remove_level(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key, int level)
{
	unsigned long *node;
	int i, pos, fill;
	void *ret;

	if (level > head->height) {
		/* we recursed all the way up */
		head->height = 0;
		head->node = NULL;
		return NULL;
	}

	node = find_level(head, geo, key, level);
	pos = getpos(geo, node, key);
	fill = getfill(geo, node, pos);
	if ((level == 1) && (keycmp(geo, node, pos, key) != 0))
		return NULL;
	ret = (void *)bval(geo, node, pos);

	/* remove and shift */
	for (i = pos; i < fill - 1; i++) {
		setkeyb(geo, node, bkey(geo, node, i + 1), i);
		setval(geo, node, bval(geo, node, i + 1), i);
	}
	clearpair(geo, node, fill - 1);

	if (fill - 1 < geo->no_pairs / 2) {
		if (level < head->height)
			rebalance(head, geo, key, level, node, fill - 1);
		else
			btree_shrink(head, geo, fill - 1);
	}

	return ret;
}

void *btree_remove(struct btree_head *head, struct btree_geo *geo,
		unsigned long *key)
{
	if (head->height == 0)
		return NULL;

	return btree_remove_level(head, geo, key, 1);
}

int btree_merge(struct btree_head *target, struct btree_head *victim,
		struct btree_geo *geo, unsigned long *duplicate)
{
	unsigned long *key;
	void *val;
	int err;

	BUG_ON(target == victim);

	if (!(target->node)) {
		/* target is empty, just copy fields over */
		target->node = victim->node;
		target->height = victim->height;
		__btree_init(victim);
		return 0;
	}

	for (;;) {
		key = btree_last(victim, geo);
		if (!key)
			break;
		val = btree_lookup(victim, geo, key);
		err = btree_insert(target, geo, key, val);
		if (err)
			return err;
		/* We must make a copy of the key, as the original will get
		 * mangled inside btree_remove. */
		longcpy(duplicate, key, geo->keylen);
		btree_remove(victim, geo, duplicate);
	}
	return 0;
}

static size_t __btree_for_each(struct btree_head *head, struct btree_geo *geo,
		unsigned long *node, long opaque,
		void (*func)(void *elem, long opaque,
			unsigned long *key, size_t index, void *func2),
		void *func2, int reap, int height, size_t count)
{
	int i;
	unsigned long *child;

	for (i = 0; i < geo->no_pairs; i++) {
		child = (void *)bval(geo, node, i);
		if (!child)
			break;
		if (height > 1)
			count = __btree_for_each(head, geo, child, opaque,
					func, func2, reap, height - 1, count);
		else
			func(child, opaque, bkey(geo, node, i), count++,
					func2);
	}
	if (reap)
		free(node);
	return count;
}

static void empty(void *elem, long opaque, unsigned long *key, size_t index,
		void *func2)
{
}

void visitorl(void *elem, long opaque, unsigned long *key, size_t index,
		void *__func)
{
	visitorl_t func = __func;

	func(elem, opaque, *key, index);
}

void visitor32(void *elem, long opaque, unsigned long *__key, size_t index,
		void *__func)
{
	visitor32_t func = __func;
	u32 *key = (void *)__key;

	func(elem, opaque, *key, index);
}

void visitor64(void *elem, long opaque, unsigned long *__key, size_t index,
		void *__func)
{
	visitor64_t func = __func;
	u64 *key = (void *)__key;

	func(elem, opaque, *key, index);
}

void visitor128(void *elem, long opaque, unsigned long *__key, size_t index,
		void *__func)
{
	visitor128_t func = __func;
	u64 *key = (void *)__key;

	func(elem, opaque, key[0], key[1], index);
}

size_t btree_visitor(struct btree_head *head, struct btree_geo *geo,
		long opaque,
		void (*func)(void *elem, long opaque, unsigned long *key,
			size_t index, void *func2), void *func2)
{
	size_t count = 0;

	if (!func2)
		func = empty;
	if (head->node)
		count = __btree_for_each(head, geo, head->node, opaque, func,
				func2, 0, head->height, 0);
	return count;
}

size_t btree_grim_visitor(struct btree_head *head, struct btree_geo *geo,
		long opaque,
		void (*func)(void *elem, long opaque, unsigned long *key,
			size_t index, void *func2), void *func2)
{
	size_t count = 0;

	if (!func2)
		func = empty;
	if (head->node)
		count = __btree_for_each(head, geo, head->node, opaque, func,
				func2, 1, head->height, 0);
	__btree_init(head);
	return count;
}
