#include "idt.h"

static const struct idt_data early_idts[] = {
	INTG(TRAP_DB, asm_exc_debug),
	SYSG(TRAP_BP, asm_exc_int3),
	INTG(TRAP_PF, asm_exc_page_fault),
};

static gate_desc_t idt_table_g[IDT_ENTRIES];
static idt_ptr_t idt_ptr2_g = {
	.limit = IDT_TABLE_SIZE - 1,
	.base = (u32_t)&idt_table_g};

static __INLINE__ void
init_idt_data(idt_data_t *data, unsigned int n, void_fun addr)
{
	memset(data, 0, sizeof(*data));
	data->vector = n;
	data->addr = addr;
	data->segment = 0x08;
	data->bits.p = 1;
	data->bits.ist = 0;
	data->bits.dpl = 0;
	data->bits.zero = 0;
	data->bits.type = GATE_INTERRUPT;
}

static inline void idt_init_desc(gate_desc_t *gate, const struct idt_data *d)
{
	size_t addr = (size_t)d->addr;

	gate->base_lo = (u16_t)addr & 0xFFFF;
	gate->segment = (u16_t)d->segment;
	gate->bits = d->bits;
	gate->base_hi = (u16_t)(addr >> 16) & 0xFFFF;
}

static __INIT__ void
idt_setup_from_table(gate_desc_t *idt, const struct idt_data *t, int size /* ,bool_e sys */)
{
	gate_desc_t desc;

	for (; size > 0; t++, size--)
	{
		idt_init_desc(&desc, t);

		write_idt_entry(idt, t->vector, &desc);

		// if (sys)
		// 	set_bit(t->vector, system_vectors);
	}
}

__INIT__ __UNUSED__ static void
set_intr_gate(unsigned int n, void_fun addr)
{
	struct idt_data data;

	init_idt_data(&data, n, addr);

	idt_setup_from_table(idt_table_g, &data, 1 /* ,FALSE */);
}

void __INIT__ idt_setup_early_traps(void)
{
	idt_setup_from_table(idt_table_g, early_idts, array_size(early_idts) /*, TRUE */);

	load_idt(&idt_ptr2_g);
}
