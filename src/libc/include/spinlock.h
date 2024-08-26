#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "type.h"

typedef struct spinlock
{
	volatile u32_t lock;
} spinlock_t;

#define SPINLOCK_INIT(name) \
	struct spinlock name = {.lock = 0}

void spinlock_init(spinlock_t *lock);
void spinlock_lock(spinlock_t *lock);
void spinlock_unlock(spinlock_t *lock);

#endif