#ifndef _BASIC_HASH_TABLE_H
#define _BASIC_HASH_TABLE_H

#include "hashtable.h"

#define SMALL_HASH_TABLE_SIZE 4

class BasicHashTable: public HashTable {
private:
    class TableEntry;

public:
    BasicHashTable(int keyType);
    virtual ~BasicHashTable();

    class Iterator; friend class Iterator;
    class Iterator: public HashTable::Iterator {
    public:
        Iterator(BasicHashTable const& table);

    private:
        void* next(char const*& key);
    
    private:
        BasicHashTable const& fTable;
        unsigned fNextIndex;
        TableEntry* fNextEntry;
    };

private:
    virtual void* Add(char const* key, void* value);
    virtual bool Remove(char const* key);
    virtual void* Lookup(char const* key) const;
    virtual unsigned numEntries() const;

private:
    class TableEntry {
    public:
        TableEntry* fNext;
        char const* key;
        void* value;
    };

    TableEntry* lookupKey(char const* key, unsigned& index) const;
    bool keyMatches(char const* key1, char const* key2) const;

    TableEntry* insertNewEntry(unsigned index, char const* key);
    void assignKey(TableEntry* entry, char const* key);

    void deleteEntry(unsigned index, TableEntry* entry);
    void deleteKey(TableEntry* entry);

    void rebuild();

    unsigned hashIndexFromKey(char const* key) const;

    unsigned randomIndex(uintptr_t i) const {
        return (unsigned)(((i*1103515245) >> fDownShift) & fMask);
    }

private:
    TableEntry** fBuckets;
    TableEntry* fStaticBuckets[SMALL_HASH_TABLE_SIZE];
    unsigned fNumBuckets, fNumEntries, fRebuildSize, fDownShift, fMask;
    int fKeyType;
};

#endif