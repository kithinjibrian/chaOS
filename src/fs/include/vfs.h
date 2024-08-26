#ifndef VFS_H
#define VFS_H

#include "vga.h"
#include "type.h"
#include "string.h"
#include "spinlock.h"

#define DECLARE_FSTYPE(name, mount) \
	struct fs_type name = {mount, #name, NULL}

typedef struct inode_ops
{
	int (*mkdir)(struct vnode *vn, struct dentry *dn);
	struct dentry *(*lookup)(struct vnode *vn, struct dentry *dn, u32_t name_len);
} inode_ops_t;

typedef struct vnode
{
	spinlock_t lock;
	struct superblock *sb;
	const struct inode_ops *ops;
} vnode_t;

typedef struct dentry_ops
{

} dentry_ops_t;

typedef struct dentry
{

} dentry_t;

typedef struct superblock_ops
{
	struct inode *(*inode_alloc)(struct superblock *sb);
	void (*inode_free)(struct inode *in);
	void (*inode_write)(struct inode *in);
} superblock_ops_t;

typedef struct superblock
{
	list_t list;
	u32_t magic;
	u32_t blocksize;
	struct dentry *root;
	const struct superblock_ops *ops;
	const struct dentry_ops *dentry_ops;
} superblock_t;

typedef struct fs_type
{
	const char *name;
	struct fs_type *next;
	dentry_t *(*mount)(struct fs_type *fs);
} fs_type_t;

typedef struct file_ops
{
	int (*open)(struct vnode *vn, struct file *fl);
	size_t (*read)(struct file *fl, size_t count, void *buf);
	size_t (*write)(struct file *fl, size_t count, void *buf);
} file_ops_t;

typedef struct file
{
	spinlock_t lock;
	vnode_t *vnode;
	const struct file_ops *ops;
} file_t;

int fs_register(fs_type_t *fs);

#endif