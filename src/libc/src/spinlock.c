#include "spinlock.h"

void spinlock_init(spinlock_t *lock)
{
	lock->lock = 0;
}

void spinlock_lock(spinlock_t *lock)
{
	while (TRUE)
	{
		int expected = 0;
		int desired = 1;

		__asm__ __volatile__(
			"xchg %0, %1"
			: "=r"(expected)
			: "m"(lock->lock), "0"(desired)
			: "memory");

		if (expected == 0)
		{
			return;
		}

		__builtin_ia32_pause();
	}
}

void spinlock_unlock(spinlock_t *lock)
{
	__asm__ __volatile__(
		"movl $0, %0"
		: "=m"(lock->lock)
		:
		: "memory");
}