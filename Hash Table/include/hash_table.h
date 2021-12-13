#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdbool.h>

/**
 * A struct to represent the item of a hash table.
 */
typedef struct hash_table_t hash_table_t;

/**
 * Creates and configures a new hash table.
 * @return The initialized hash table
 */
hash_table_t *hash_table_create();

/**
 * Gets the value of the item.
 * @param hash_table the hash table to get the value from
 * @param key the key to look for in the hash table
 * @return The value of the corresponding item or -1 if not found
 */
void *hash_table_get(hash_table_t *hash_table, int key);

/**
 * Sets the value of an item in the hash table or inserts a new one if none found.
 * @param hash_table the hash table to set the value or insert the item into
 * @param key the key of the item
 * @param value the value to set
 * @return Whether the setting was successful 
 */
bool hash_table_set(hash_table_t *hash_table, int key, void *value);

/**
 * Deletes an item from the hash table.
 * @param hash_table the hash table to delete the item from
 * @param key the key of the item to delete
 * @return Whether the deletion was successful
 */
bool hash_table_delete(hash_table_t *hash_table, int key);

#endif
