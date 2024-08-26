#include "array.h"

array_t *array_create(size_t size)
{
	array_t *array = (array_t *)kmalloc(sizeof(array_t));

	if (array == NULL)
		return NULL;

	// start with an initial value of 4
	size_t length = 4 * size;
	array->array = (u8_t *)kmalloc(length);

	if (array->array == NULL)
	{
		kfree(array);
		return NULL;
	}

	array->size = size;
	array->length = 4;
	array->index = 0;
	array->count = 0;
	array->ref = 1;

	return array;
}

bool_e array_push(array_t *array, void *data)
{
	if (array == NULL || data == NULL)
		return FALSE;

	if (array->count >= array->length)
	{
		array->length *= 2;
		size_t len = array->length * array->size;
		array->array = (u8_t *)krealloc(array->array, len);

		if (array->array == NULL)
			return FALSE;
	}

	memcpy(array->array + array->count * array->size, data, array->size);
	array->count++;

	return TRUE;
}

void __array_free__(array_t **array)
{
	if (array == NULL)
		return;

	(*array)->ref--;

	if ((*array)->ref == 0)
	{
		kfree((*array)->array);
		kfree(*array);
	}

	*array = NULL;
}