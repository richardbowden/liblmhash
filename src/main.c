#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h> 
#include "lm_hash.h"
#include <unistd.h>
#include <time.h>

size_t load_file(const char *filename, char **result){
    size_t size;
    FILE *f = NULL;
    f = fopen(filename, "r");
    if (f == NULL){
        *result = NULL;
        return -1;
    }
    
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    
    /*add one for null terminator*/
    size += 1;
    
    fseek(f, 0L, SEEK_SET);
    *result = (char*)malloc(size * sizeof(char));
    fread(*result, sizeof(char),size, f);
/*    (*our_var_ptr)++; */
    (*result)[size] = '\0';
/*    
           if (size != rsize)
           {
               free(result);
               return -2; // -2 means file reading fail
           }
  */  
    fclose(f);
    /*        (result)[size] = 0;
        result[size] = 0; */
    return size;
}

struct Reader {
    char *contents;
    size_t c_size;
    size_t pos;
    int eof;
}reader;

char buffer[1024];

void my_reader(){
    size_t len = 0;
    
    char c = '\0';

    do {
        c = reader.contents[len];
        if (c != '\n' && c != '\n'){
            len++;
        }else{
            break;
        }
    } while (c != '\0');
    
    if (c == '\0'){
        reader.eof = 1;
    }
    
/*  
   while (c != '\r' && c != '\n'){
       ++len;
       reader.pos++;
       assert(reader.pos != reader.c_size);
       c = reader.contents[len];
   }
    
   printf("ssssssss: %zu\n", reader.pos);
   printf("length: %zu\n", len);
    
   char *str = (char*)malloc(len);
*/    
    memcpy(buffer, reader.contents, len);
    buffer[len] = '\0';
    
    reader.contents = reader.contents + (len +1);
/*
   reader.pos ++;
   return str;
*/
}

char* read_line(char **contents){
    size_t len = 0;
    
    char *old = NULL;
    old = *contents;
    
    char c = old[len];
    while (c != '\r' && c != '\n'){
        len++;
        c = old[len];
    }
    
    //add one byte for terminator
    //    char b[len+1];
    char *bb = calloc(len+1, sizeof(char));
    
    memcpy(bb, old, len);
    
    //    strncpy(b, old, len);
    bb[len+1] = '\0';
    
    //    int move_by = (int)();
    old = old + (len+1);
    
    //    for (int a = 1; a < len; a++) {
    //        b[a-1] = contents[a-1];
    //        ++contents;
    //    }
    
    //    printf("dddd: %s", contents);
    //    printf("processed: %s\n", b);
    *contents = old;
    return bb;
    
    
    //    switch (*contents) {
    //        case '\0':
    //            return NULL;
    //            break;
    //
    //        default:
    //            break;
    //    }
}







int main(int argc, char const *argv[])
{
    const char* p = "/Users/richard/development/c/liblmhash/words.txt";
    
    FILE *f;
    f = fopen(p, "r");
    
    reader.c_size = load_file(p, &reader.contents);
    reader.pos = 0;

    assert(reader.contents[reader.c_size] == '\0');

        lm_hash_table_t *t = lm_hash_table_new();
    
    
    
    
    //    if (res == LM_HERR){
    //        printf("%d\n", lm_hash_get_errno());
    //    }
    
    do {
        my_reader();
        printf("%s\n", buffer);
            int res = lm_hash_add_string(t, buffer, "a");
        
//            if (res == LM_HERR){
//                printf("%d\n", lm_hash_get_errno());
//            }
        
    }while (reader.eof != 1);
    



    

    //    do {
    //        char *a = NULL;
    //        a = read_line(&contents);
    //        printf("%s\n", a);
    //        free(a);
    //    }
    //    while(size > 0);
    
    
    //    const char *fff = "this is a new string";
    //    lm_hash_table_t *t = lm_hash_table_new();
    
    //    lm_hash_add_string(t, "name", fff);
    //    int res = lm_hash_add_string(t, "name", "richard");
    
    //    if (res == LM_HERR){
    //        printf("%d\n", lm_hash_get_errno());
    //    }
    
    //    lm_hash_add_string(t, "car", "my new car");
    
    //    lm_hash_add_string(t, "address", "401 Felix Point");
    
    //    lm_hash_get_str_value_key(t, "this is a test");
    
    
    //    const char* d = lm_hash_get_string_for_key(t, "name");
    
    return 0;
}
