#include "vfs.h"

fs_type_t *root_g = NULL;

static fs_type_t **fs_find(const char *name, int len)
{
	fs_type_t **fs;

	for (fs = &root_g; *fs; fs = &(*fs)->next)
		if (strncmp((*fs)->name, name, len) && !(*fs)->name[len])
			break;

	return fs;
}

int fs_register(fs_type_t *fs)
{
	int res = 0;
	fs_type_t **tmp = fs_find(fs->name, strlen(fs->name));

	if (*tmp)
		res = 1;
	else
		*tmp = fs;

	return res;
}