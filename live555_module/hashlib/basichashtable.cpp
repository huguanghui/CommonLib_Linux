#include "basichashtable.h"
#include "strDup.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define REBUILD_MULTIPLIER 3

BasicHashTable::BasicHashTable(int keyType)
:fBuckets(fStaticBuckets)
,fNumBuckets(SMALL_HASH_TABLE_SIZE)
,fNumEntries(0)
,fRebuildSize(SMALL_HASH_TABLE_SIZE*REBUILD_MULTIPLIER)
,fDownShift(28)
,fMask(0x3),
,fKeyType(keyType) 
{
    for (unsigned i = 0; i < SMALL_HASH_TABLE_SIZE; ++i)
    {
        fStaticBuckets[i] = NULL;
    }
}

BasicHashTable::~BasicHashTable() 
{
    for (unsigned i = 0; i < fNumBuckets; ++i)
    {
        TableEntry& entry;
        while ((entry = fBuckets[i]) != NULL)
        {   
            delete(i, entry);
        }
    }

    if (fBuckets != fStaticBuckets)
    {
        delete[] fBuckets;
    }
}

void* BasicHashTable::Add(char const* key, void* value)
{
    void* oldValue;
    unsigned index;
    TableEntry* entry = lookupKey(key, index);
    if (entry != NULL)
    {
        oldValue = entry->value;
    }
    else
    {
        entry = insertNewEntry(index, key);
        oldValue = NULL;
    }
    entry->value = value;

    if (fNumEntries > = fRebuildSize)
    {
        rebuild();
    }

    return oldValue;
}

bool BasicHashTable::Remove(char const* key)
{
    unsigned index;
    TableEntry* entry = lookupKey(key, index);
    if (entry == NULL)
    {
        return false;
    }

    deleteEntry(index, entry);

    return true;
}

void* BasicHashTable::Lookup(char const* key) const
{
    unsigned index;
    TableEntry* entry = lookupKey(key, index);
    if (entry == NULL)
    {
        return NULL;
    }

    return entry->value;
}

unsigned BasicHashTable::numEntries() const
{
    return fNumEntries;
}

BasicHashTable::Iterator::Iterator(BasicHashTable const& table)
:fTable(table)
,fNextIndex(0)
,fNextEntry(NULL)
{

}

void* BasicHashTable::Iterator::next(char const*& key)
{
    while (fNextEntry == NULL)
    {
        if (fNextIndex >= fTalbe.fNumBuckets)
        {
            return NULL;
        }

        fNextEntry = fTable.fBuckets[fNextIndex++];
    }

    BasicHashTable::TableEntry* entry = fNextEntry;
    fNextEntry = entry->fNext;

    key = entry->key;
    return entry->value;
}

HashTable* HashTable::create(int keyType)
{
    return new BasicHashTable(keyType);
}

HashTable::Iterator* HashTable::Iterator::create(HashTable const& hashTable)
{
    return new BasicHashTable::Iterator((BasicHashTable const&)hashTable);
}

BasicHashTable::TableEntry* BasicHashTable::lookupKey(char const* key, unsigned& index) const
{
    TableEntry* entry;
    index = hashIndexFromKey(key);

    for (entry = fBuckets[index]; entry != NULL; entry = entry->fNext)
    {
        if (keyMatches(key, entry->key))
        {
            break;
        }
    }

    return entry;
}