#ifndef IDTENTRY_H
#define IDTENTRY_H

#include "trapno.h"

#ifndef __ASSEMBLER__ /* GCC builtin macro */
#include "type.h"
#include "macro.h"

#define DECLARE_IDTENTRY(vector, func) \
	void asm_##func(void);             \
	__VISIBLE__ void func(struct registers *regs)

#define DECLARE_IDTENTRY_ERRORCODE(vector, func) \
	void asm_##func(void);                       \
	__VISIBLE__ void func(struct registers *regs, unsigned long error_code)

#define DEFINE_IDTENTRY(func)                                \
	static __INLINE__ void __##func(struct registers *regs); \
                                                             \
	__VISIBLE__ void func(struct registers *regs)            \
	{                                                        \
		__##func(regs);                                      \
	}                                                        \
                                                             \
	static __INLINE__ void __##func(__UNUSED__ struct registers *regs)

#else /* __ASSEMBLER__ */

#define DECLARE_IDTENTRY(vector, func) \
	idtentry vector asm_##func func has_error_code = 0

#define DECLARE_IDTENTRY_ERRORCODE(vector, func) \
	idtentry vector asm_##func func has_error_code = 1

#endif /* __ASSEMBLER__ */

DECLARE_IDTENTRY(TRAP_BP, exc_int3);
DECLARE_IDTENTRY(TRAP_DB, exc_debug);
DECLARE_IDTENTRY(TRAP_PF, exc_page_fault);
DECLARE_IDTENTRY(TRAP_DE, exc_divide_error);

#endif
