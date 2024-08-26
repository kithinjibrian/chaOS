#include "wst.h"

static dentry_t *mount_wlt(struct fs_type *fs)
{
	return NULL;
}

static fs_type_t wlt = {
	.name = "wlt",
	.mount = mount_wlt,
};

static int __INIT__ wlt_init()
{
	fs_register(&wlt);
	return 10;
}

module_init(wlt_init);
module_author("Kithinji Brian");
module_description("The Wan Shi Tong (WST) file system driver.");