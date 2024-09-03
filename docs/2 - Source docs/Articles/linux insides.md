Created at: 2024-08-27 01:13

# Linux insides
Interrupts can be split into 2 major classes.
1. External or hardware generated interrupts
2. Software generated interrupts

External interrupts are received through the `Local APIC` or pins on the processor which are connected to the `Local APIC`. Software generated interrupts are caused by an exceptional condition in the processor itself. A common example of an exceptional condition is `page fault`.

Exceptions are `synchronous` with program execution and can be classified into 3 categories:
+ Faults
+ Traps
+ Aborts

A fault is an exception reported before the execution of a "faulty" instruction (which can then be corrected). If correct, it allows the interrupted code to resume.

A trap is an exception, which is reported immediately following the execution of the trap instruction.

An abort is an exception that does not always report the exact instruction which caused the exception and does not allow the interrupted program to be resumed.

Interrupts can be classified as `maskable` and `non-maskable`. Maskable interrupts are interrupts which can be blocked with the two following instructions for `x86` - `sti` and `cli`. 

```c
static __always_inline void native_irq_disable(void)
{
	asm volatile("cli": : :"memory");
}

static __always_inline void native_irq_enable(void)
{
	asm volatile("sti": : :"memory");
}
```

# Authors
Kithinji Brian

# References
[Linux insides](https://0xax.gitbook.io/linux-insides/summary/interrupts)