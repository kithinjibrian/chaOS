#ifndef ARRAY_H
#define ARRAY_H

#include "type.h"
#include "math.h"
#include "macro.h"
#include "string.h"
#include "kmalloc.h"

#define new_array(T) array_create(sizeof(T));

array_t *array_create(size_t size);

bool_e array_push(array_t *array, void *data);

void __array_free__(array_t **array);

#define array_free(array) __array_free__(&array);

static inline void *array_at(array_t *array, size_t pos)
{
	if (array == NULL || pos >= array->length)
		return NULL;

	return (void *)(array->array + pos * array->size);
}

static inline void *array_current(array_t *array)
{
	return (void *)(array->array + array->index * array->size);
}

static inline array_t *array_share(array_t *array)
{
	if (array == NULL)
		return NULL;

	array->ref++;

	return array;
}

static inline bool_e array_next(array_t *array)
{
	if (array->index > array->length - 1)
	{
		return FALSE;
	}

	array->index++;

	return TRUE;
}

static inline bool_e array_prev(array_t *array)
{
	if (array->index == 0)
		return FALSE;

	array->index--;

	return TRUE;
}

static inline void array_rewind(array_t *array)
{
	array->index = 0;
}

#define array_for_each(pos, array) \
	for (pos = (array)->index; pos < (array)->count; pos++, array_next(array))

#endif