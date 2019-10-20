
#ifndef lm_hash_h
#define lm_hash_h

#include <stdlib.h>
#include <stdbool.h>


/* __thread int lm_hash_errno; */
#define set_lm_hash_errno(err) (lm_hash_errno = (err))
#define lm_hash_errno (lm_hash_errno)

#define SQLITE_ERROR_MISSING_COLLSEQ   (SQLITE_ERROR | (1<<8))

typedef enum lm_hash_err {
    LM_HERR_OK                      = 0,
    LM_HERR                         = 1,
    
    
    LM_HERR_EXISTS_NO_OVER_WRITE = (LM_HERR | (1<<8))
} lm_hash_err_t;

#define LM_HASH_INIT_SIZE               23
#define LM_HASH_INIT_ENTRY_ITEM_SIZE    4

typedef unsigned long lm_hash_t;
typedef unsigned long lm_slot_t;

typedef struct lm_hash_item_s {
    lm_hash_t hash;
    
    /* TODO(rich) cannot remember why i added this, remove if not used */
    size_t size;

    char *key;
    union {
        char* str;
        
    }value;
}lm_hash_item_t;


/* lm_hash_entry_t represents a key as a hashed value */
typedef struct lm_hash_entry_s {
    size_t consumed;
    size_t size;
    lm_hash_item_t* item[];
}lm_hash_entry_t;

typedef struct lm_hash_table_s {
    size_t entry_size;
    size_t consumed;
    size_t modulus;
    size_t capacity;
    bool must_grow;
//    char should_resize;
    
    lm_hash_entry_t **entry; //array
}lm_hash_table_t;

// Creation
lm_hash_table_t *lm_hash_table_new(void);
lm_hash_table_t *lm_hash_table_new_with_size(size_t count);

// Search

// Error
int lm_hash_get_errn(void);

// void lm_hash_add_string(char* key, )

//TODO(rich): these could be private, maybe
lm_hash_t hash_djb2(const char *str);
static inline lm_hash_t hash_sdbm(const char *str);

int lm_hash_add_string(lm_hash_table_t *table, const char *key, const char *value);
// int lm_hash_add_int(const char *key, int*value);

const char* lm_hash_get_string_for_key(lm_hash_table_t *table, const char *key);
const char* lm_hash_get_str_value_key(lm_hash_table_t *table, const char *key);
int lm_hash_get_int_for_key(const char *key);
int lm_hash_does_key_exist(lm_hash_table_t *t, const char *key);

void lm_hash_print_hash_table(lm_hash_table_t *t);


#endif
