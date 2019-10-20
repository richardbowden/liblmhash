#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lm_hash.h"
#include <assert.h>

#define hash_bytes hash_sdbm

__thread int lm_hash_errno;

lm_hash_t hash_djb2(const char *str){
        unsigned long hash = 5381;
        int c;
        while ((c = *str++)){
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash;
}

static inline lm_hash_t hash_sdbm(const char *str){
        lm_hash_t hash = 0;
        int c;
        while ((c = *str++)) {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }
        return hash;
}


float lm_hash_how_full(lm_hash_table_t *t){
    size_t max = t->capacity;
    size_t used = t->consumed;
    
    float r = (float)used / (float)max * 100;
    return r;
}

lm_hash_table_t *lm_hash_table_new_with_size(size_t count){
    
    lm_hash_table_t *ht = NULL;
    ht = calloc(1, sizeof(*ht));// + count * sizeof(lm_hash_entry_t));
    
    ht->modulus = count;
    ht->capacity = count;
    ht->consumed = 0;
//    ht->entry_size = sizeof(lm_hash_entry_t);
    
    lm_hash_entry_t **e;
    e = calloc(count, sizeof(*e));
    ht->entry = e;
//    ht->should_resize = 0;
    return ht;
}

lm_hash_table_t *lm_hash_table_new(){
    return lm_hash_table_new_with_size(LM_HASH_INIT_SIZE);
}

lm_hash_entry_t* lm_hash_entry_new(){
    lm_hash_entry_t *e = NULL;
    size_t s = (sizeof(lm_hash_entry_t) + LM_HASH_INIT_ENTRY_ITEM_SIZE * sizeof(lm_hash_item_t*));
    e = calloc(1, s);
    e->consumed = 0;
    e->size =LM_HASH_INIT_ENTRY_ITEM_SIZE;
    return e;
}

lm_hash_item_t* lm_hash_item_new(){
    lm_hash_item_t *i = NULL;
    i = calloc(1, sizeof(lm_hash_item_t));
    return i;
}

//lm_hash_entry_t lm_hash_entry_resize(lm_hash_entry_t *e, int new_size){
//
//}

void lm_hash_add_item_to_entry(lm_hash_item_t *item, lm_hash_entry_t *entry){
    entry->item[entry->consumed] = item;
    entry->consumed++;
}

void lm_hash_add_entry_to_table(lm_hash_entry_t *entry, lm_slot_t slot, lm_hash_table_t *table){
    
    float used = lm_hash_how_full(table);
    // i think this is bullshit
    if (used > 75){
        printf("it's nearly fucking full, grow the array by Harry Potter magic");
    }
    table->entry[slot] = entry;
    table->consumed++;
}

lm_slot_t lm_hash_calc_entry_slot(lm_hash_table_t *table, lm_hash_t hash){
    return hash % table->modulus;
}

lm_hash_item_t* lm_hash_search_entry_for_key(lm_hash_entry_t *entry, const char *key){
    int max = (int)entry->consumed;
    for (int i = 0; i <= max -1; i++) {
        lm_hash_item_t *item = entry->item[i];
        int r = strcmp(item->key, key);
        if (r == 0){
            return item;
        }
    }
    return NULL;
}

lm_hash_entry_t * get_entry_for_key(lm_hash_table_t *t, const char *key){
    lm_hash_t key_hash = hash_bytes(key);
    lm_slot_t slot = 0;
    slot = lm_hash_calc_entry_slot(t, key_hash);
    return t->entry[slot];
}

const char* lm_hash_get_str_value_key(lm_hash_table_t *table, const char *key){
    
    printf("this is the key value, this should be a const char, local stack: %s", key);
    
    return NULL;
}

int lm_hash_add_string(lm_hash_table_t *table, const char *key, const char *value){
    assert(table->modulus > 0);

    lm_hash_t key_hash = 0;
    key_hash = hash_bytes(key);
    
    printf("key=%s, value=%s, hash=%lu\n", key, value, key_hash);
    
    assert(key_hash != 0);
    
    lm_slot_t entry_slot = lm_hash_calc_entry_slot(table, key_hash);

    lm_hash_entry_t *retrieved_entry = NULL;
    retrieved_entry = table->entry[entry_slot];

    if (retrieved_entry == NULL){
        lm_hash_entry_t *entry = lm_hash_entry_new();

        lm_hash_item_t *item = lm_hash_item_new();

        item->hash = key_hash;
        item->key = strdup(key);
        item->size = sizeof(key);
        item->value.str = strdup(value);
        
        lm_hash_add_item_to_entry(item, entry);
        lm_hash_add_entry_to_table(entry, entry_slot, table);
        
    }else {
        lm_hash_item_t *item = lm_hash_search_entry_for_key(retrieved_entry, key);
        //update an existing item the key is the same, not other checks required
        if (item != NULL){
            item->value.str = strdup(value);
            return 0;
//            set_lm_hash_errno(LM_HERR_EXISTS_NO_OVER_WRITE);
//            return LM_HERR;
        }
        
        lm_hash_item_t *new_item = NULL;
        new_item = lm_hash_item_new();
        new_item->hash = key_hash;
        new_item->key = strdup(key);
        new_item->size = sizeof(key);
        new_item->value.str = strdup(value);
        
        lm_hash_add_item_to_entry(new_item, retrieved_entry);
        
        
//        printf("we do not support duplicate hashes at the moment new key=%s value=%s, existing key=%s value=%s\n", key, value, t[0].item[0]->key, t[0].item[0]->value.str);
//        exit(1);
    }
    
    printf("key: %s, value: %s, hash: %lu, slot: %lu\n", key, value, key_hash, entry_slot);
    return 0;
}

const char* lm_hash_get_string_for_key(lm_hash_table_t *table, const char *key){
//    lm_hash_t key_hash = hash_bytes(key);
//    lm_slot_t entry_slot = lm_hash_calc_entry_slot(table, key_hash);
//    lm_hash_entry_t *e = table->entry[entry_slot];
    
    lm_hash_entry_t *e = NULL;
    e = get_entry_for_key(table, key);
    
    if (table->consumed == 0) {
        return NULL;
    }
    
    lm_hash_item_t *item = lm_hash_search_entry_for_key(e, key);
    return strdup(item->value.str);
    //    for (int i = 0; i <= e->consumed -1; ++i) {
//        if (strcmp(e->item[i]->key, key) == 0){
//            return strdup(e->item[i]->value.str);
//        }
//    }
    return NULL;
}

int lm_hash_does_key_exist(lm_hash_table_t *t, const char *key){
    lm_hash_entry_t *e = NULL;
    e = get_entry_for_key(t, key);
    
    if (e == NULL){
        return 0;
    }

    return 1;
}

void lm_hash_print_hash_table(lm_hash_table_t *t){
    
}

// Quadratic Probing
//find a position to set the key
// int findPos( int key, YourHashTable h )
// {
//     int curPos;
//     int collisionNum = 0;    
//     curPos = key %  h.TableSize;
//     //while find a collision
//     while( h[curPos] != null && h[curPos] != key )
//     {
//         //f(i) = i*i = f(i-1) + 2*i -1
//         curPos += 2 * ++collisionNum - 1;
//         //do the mod only use - for efficiency
//         if( curPos >= h.TableSize )
//             curPos -= h.TableSize;
//     }
//     return curPos;
// }

//int lm_hash_get_errno(){
//    reutrn lm_hash_errno;
//}
