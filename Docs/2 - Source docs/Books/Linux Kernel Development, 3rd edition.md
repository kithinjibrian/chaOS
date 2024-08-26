# Linux Kernel Development, 3rd edition

## Interrupts and Interrupt Handlers (chapter 7)
A primary responsibility of an operating system kernel is managing hardware devices, such as hard drives, keyboards, and mice. To do this, the kernel needs to communicate with these devices. Since processors are much faster than the hardware they control, it is inefficient for the kernel to issue a request and then wait for a response from a slower device. Instead, because hardware devices take longer to respond, the kernel should be free to perform other tasks, returning to handle the hardware only after it has finished its work.

To manage this without being held up by slower hardware, the kernel can use one of the following methods:

1. **Polling**: The kernel repeatedly checks the status of the hardware to see if it needs attention.
2. **Interrupts**: The hardware signals the kernel when it requires attention, allowing the kernel to continue other tasks until then.

### Interrupts
Different devices have distinct interrupts, each represented by a unique value. This allows the kernel to differentiate between interrupts from different devices, such as a keyboard versus a hard drive, and determine which hardware caused each interrupt. Consequently, the kernel can use the appropriate handler to service each interrupt correctly.

These interrupt values are commonly known as interrupt request (IRQ) lines, with each line assigned a specific numeric value. For example, in classic PC architecture, IRQ zero is designated for the timer interrupt, while IRQ one is assigned to the keyboard interrupt. However, not all interrupt numbers are fixed. Devices on the PCI bus, for instance, usually have dynamically assigned interrupts. Similarly, other non-PC architectures often use dynamic assignments for their interrupt values. The key idea is that each interrupt is associated with a specific device, and the kernel is aware of this association.

An interrupt can occur at any moment, so the interrupt handler should execute quickly and then return control to the interrupted code. However, interrupt handlers often have a significant amount of work to complete.

These two requirements - having the interrupt handler run quickly and handling substantial tasks - are inherently in conflict. To address this, interrupt processing is divided into two parts, known as halves. The interrupt handler represents the top half, which runs immediately, while the bottom half defers the remaining work to a later time when all interrupts are enabled and it's more convenient to process them.

To register an interrupt handler for a specific interrupt line, use the `request_irq()` function, which is declared in the Linux kernel header file [interrupt.h](https://github.com/torvalds/linux/blob/master/include/linux/interrupt.h):

```c
/**
 * request_irq - Add a handler for an interrupt line
 * @irq:	The interrupt line to allocate
 * @handler:	Function to be called when the IRQ occurs.
 *		Primary handler for threaded interrupts
 *		If NULL, the default primary handler is installed
 * @flags:	Handling flags
 * @name:	Name of the device generating this interrupt
 * @dev:	A cookie passed to the handler function
 *
 * This call allocates an interrupt and establishes a handler; see
 * the documentation for request_threaded_irq() for details.
 */

typedef irqreturn_t (*irq_handler_t)(int, void *);

request_irq(
	unsigned int irq, 
	irq_handler_t handler, 
	unsigned long flags,
	const char *name, 
	void *dev)
```

Interrupt handlers in Linux do not need to be re-entrant. When an interrupt handler is executing, the corresponding interrupt line is masked on all processors, which prevents another interrupt on the same line from being received. Typically, all other interrupts remain enabled, allowing the system to handle other interrupts. However, the current interrupt line stays disabled, ensuring that the same interrupt handler is not called concurrently to handle a nested interrupt.

When executing an interrupt handler, the kernel is in interrupt context. 
# Authors
Kithinji Brian

# References
[Linux kernel development - free version](https://github.com/jyfc/ebook/blob/master/03_operating_system/Linux.Kernel.Development.3rd.Edition.pdf)
[Linux kernel development - Amazon link](https://www.amazon.com/Linux-Kernel-Development-Robert-Love/dp/0672329468)