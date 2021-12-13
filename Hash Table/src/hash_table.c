#include "hash_table.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

/**
 * A struct to represent a hash table.
 */
typedef struct {
	/// The value the entry holds.
	void *value;

	/// Uniquely identifies the entry.
	int key;
} hash_table_item_t;

struct hash_table_t {
	/// Pointer to array of entries.
	hash_table_item_t **items;

	/// Number of bins.
	size_t m;

	/// The power of two that results in `m`.
	size_t M;

	/// Random odd positive integer.
	size_t a;

	/// Random non-negative integer.
	size_t b;

	/// Number of entries.
	size_t size;
};

static const double min_load_factor = 1.0 / 3.0, max_load_factor = 2.0 / 3.0;
static const size_t w = sizeof(long) * CHAR_BIT;

static size_t hash(int, const hash_table_t *);
static void quad_probe(size_t *, size_t, const hash_table_t *);
static int hash_table_search(const hash_table_t *, int);
static bool hash_table_create_item(hash_table_t *, int, void *);
static bool hash_table_delete_item_at_index(hash_table_t *, size_t);
static bool hash_table_resize(hash_table_t *, size_t);
static hash_table_item_t **hash_table_move_items_out(hash_table_t *);
static bool hash_table_move_item_out_at_index(hash_table_t *, size_t, hash_table_item_t **);
static bool hash_table_move_items_in(hash_table_t *, hash_table_item_t **, size_t);
static bool hash_table_set_capacity(hash_table_t *, size_t);
static void hash_table_set_variables(hash_table_t *, size_t);
static void clean_up_hash_table_items(hash_table_item_t **, size_t);

hash_table_t *hash_table_create() {
	hash_table_t *hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
	
	if (!hash_table) {
		return NULL;
	}

	size_t capacity = 2;

	if (!(hash_table->items = (hash_table_item_t **)calloc(capacity, sizeof(hash_table_item_t *)))) {
		free(hash_table);
		return NULL;
	}

	hash_table_set_variables(hash_table, capacity);
	hash_table->size = 0;

	return hash_table;
}

void *hash_table_get(hash_table_t *hash_table, int key) {
	if (!hash_table) {
		return NULL;
	}

	size_t idx = hash_table_search(hash_table, key);

	if (idx != -1) {
		return hash_table->items[idx]->value;
	}

	return NULL;
}

bool hash_table_set(hash_table_t *hash_table, int key, void *value) {
	if (!hash_table) {
		return false;
	}

	size_t idx = hash_table_search(hash_table, key);

	if (idx != -1) {
		hash_table->items[idx]->value = value;
		return true;
	} else {
		return hash_table_create_item(hash_table, key, value);
	}
}

bool hash_table_delete(hash_table_t *hash_table, int key) {
	if (!hash_table) {
		return false;
	}

	size_t idx = hash_table_search(hash_table, key);

	if (idx != -1) {
		return hash_table_delete_item_at_index(hash_table, idx);
	}

	return false;
}

inline size_t hash(int x, const hash_table_t *hash_table) {
	return ((hash_table->a * x) + hash_table->b) >> (w - hash_table->M);
}

inline void quad_probe(size_t *idx, size_t i, const hash_table_t *hash_table) {
	*idx = (*idx + (i * i)) % hash_table->m;
}

int hash_table_search(const hash_table_t *hash_table, int key) {
	if (!hash_table) {
		return -1;
	}

	for (size_t idx = hash(key, hash_table), i = 1; hash_table->items[idx]; quad_probe(&idx, i++, hash_table)) {
		if (hash_table->items[idx]->key == key) {
			return idx;
		}
	}

	return -1;
}

bool hash_table_create_item(hash_table_t *hash_table, int key, void *value) {
	hash_table_item_t *item = (hash_table_item_t *)malloc(sizeof(hash_table_item_t));

	if (!item) {
		return false;
	}

	item->key = key;
	item->value = value;

	size_t idx = hash(key, hash_table);

	for (size_t i = 1; hash_table->items[idx]; i++) {
		quad_probe(&idx, i, hash_table);
	}

	hash_table->items[idx] = item;
	hash_table->size++;

	if ((double)hash_table->size / hash_table->m > max_load_factor) {
		return hash_table_resize(hash_table, hash_table->m * 2);
	}

	return true;
}

bool hash_table_delete_item_at_index(hash_table_t *hash_table, size_t idx) {
	free(hash_table->items[idx]);

	hash_table->items[idx] = NULL;
	hash_table->size--;

	if ((double)hash_table->size / hash_table->m < min_load_factor) {
		return hash_table_resize(hash_table, hash_table->m / 2);
	}

	return true;
}

bool hash_table_resize(hash_table_t *hash_table, size_t new_capacity) {
	if (!hash_table || new_capacity % 2) {
		return false;
	}

	hash_table_item_t **items = hash_table_move_items_out(hash_table);

	if (!items) {
		return false;
	}

	size_t old_capacity = hash_table->m;

	if (!hash_table_set_capacity(hash_table, new_capacity)) {
		clean_up_hash_table_items(items, old_capacity);
		free(items);

		return false;
	}

	return hash_table_move_items_in(hash_table, items, old_capacity);
}

hash_table_item_t **hash_table_move_items_out(hash_table_t *hash_table) {
	hash_table_item_t **items = (hash_table_item_t **)calloc(hash_table->m, sizeof(hash_table_item_t *));

	if (!items) {
		return NULL;
	}

	for (size_t idx = 0; idx < hash_table->m; idx++) {
		if (hash_table->items[idx]) {
			if (!hash_table_move_item_out_at_index(hash_table, idx, items)) {
				return NULL;
			}
		}
	}

	free(hash_table->items);

	return items;
}

bool hash_table_move_item_out_at_index(hash_table_t *hash_table, size_t idx, hash_table_item_t **items) {
	hash_table_item_t *item = (hash_table_item_t *)malloc(sizeof(hash_table_item_t));

	if (!item) {
		clean_up_hash_table_items(items, hash_table->m);
		free(items);

		return false;
	}

	item->key = hash_table->items[idx]->key;
	item->value = hash_table->items[idx]->value;

	items[idx] = item;

	free(hash_table->items[idx]);

	return true;
}

bool hash_table_move_items_in(hash_table_t *hash_table, hash_table_item_t **items, size_t items_size) {
	for (size_t i = 0; i < items_size; i++) {
		if (items[i]) {
			if (!hash_table_set(hash_table, items[i]->key, items[i]->value)) {
				clean_up_hash_table_items(items, items_size);
				free(items);

				return false;
			}

			free(items[i]);
		}
	}

	free(items);

	return true;
}

bool hash_table_set_capacity(hash_table_t *hash_table, size_t capacity) {
	hash_table_set_variables(hash_table, capacity);

	if (!(hash_table->items = (hash_table_item_t **)calloc(hash_table->m, sizeof(hash_table_item_t *)))) {
		return false;
	}

	return true;
}

void hash_table_set_variables(hash_table_t *hash_table, size_t capacity) {
	if (!hash_table) {
		return;
	}

	hash_table->m = capacity;
	hash_table->M = log2(hash_table->m);

	srand(time(NULL));

	hash_table->a = (size_t)(((double)rand() / RAND_MAX) * pow(2, w - 2)) | 1;
	hash_table->b = (size_t)(((double)rand() / RAND_MAX) * pow(2, (w - hash_table->M) - 1));
}

void clean_up_hash_table_items(hash_table_item_t **items, size_t items_size) {
	for (size_t i = 0; i < items_size; i++) {
		if (items[i]) {
			free(items[i]);
			items[i] = NULL;
		}
	}
}
