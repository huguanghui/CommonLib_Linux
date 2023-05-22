#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btree.h"

static void free_nodes(void *elem, long opaque, u64 key, size_t index)
{
    if (elem) {
        free(elem);
    }
    return;
}

static void free_keys(void *elem, long opaque, u64 key, size_t index)
{
    btree_remove64((struct btree_head64 *)opaque, key);
    return;
}

static void delete_tree(struct btree_head64 *btree)
{
    btree_visitor(&btree->h, &btree_geo64, (long)&btree, visitor64, free_nodes);
    btree_visitor(&btree->h, &btree_geo64, (long)&btree, visitor64, free_keys);
}

int main(int argc, char *argv[])
{
    int idx;
    struct btree_head64 tree;

    srand(time(NULL));
    btree_init64(&tree);
    for (idx = 0; idx < 100; idx++) {
        int *p = malloc(sizeof(int));
        *p = rand() % 1000;
        *p = idx;
        btree_insert64(&tree, idx, p);
    }
    printf("height: %d\n", tree.h.height);
    printf("%llu\n", btree_last64(&tree));
    printf("STORED\n");
    // delete_tree(&tree);

    return 0;
}
