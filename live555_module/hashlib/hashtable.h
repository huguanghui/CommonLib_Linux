#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

class HashTable {
public:
    virtual ~HashTable();

    static HashTable* create(int keyType);
    virtual void* Add(char const* key, void* value);
    virtual bool Remove(char const* key) = 0;
    virtual void* Lookup(char const* key) const = 0;
    
    virtual unsigned numEntries() const = 0;
    bool IsEmpty() const { return numEntries() == 0; }

    class Iterator {
    public:
        static Iterator* create(HashTable const& hashTable);

        virtual ~Iterator();

        virtual void* next(char const*& key) = 0;

    protected:
        Iterator();
    };

    void* RemoveNext();
    void* getFirst();
protected:
    HashTable();
};

int const STRING_HASH_KEYS = 0;
int const ONE_WORD_HASH_KEYS = 1;

#endif