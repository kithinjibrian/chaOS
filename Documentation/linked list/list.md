# Linked list
In ChaOS, we use a type of linked list called an intrusive linked list. This means the linked list structure is embedded directly into the data structure it links together.

Here is how the data structure look like.

```
typedef struct list
{
	struct list *next, *prev;
} list_t;

// Using the linked list
typedef struct embed
{
	int data;
	list_t list;
} embed_t;
```

This setup is different from a regular linked list, where the data is included inside the linked list node:

```
typedef struct data
{
	int num;
	struct data *next, *prev;
} data_t;
```

The intrusive linked list has a clear advantage: it can be used with any data structure without losing type safety.

To make a generic linked list with the data inside the list, you'd need to use a void pointer:

```
typedef struct list
{
	void *data;
	struct list *next, *prev;
} list_t;
```

This approach requires two memory allocations:

1. One for the data.
2. Another for `list_t`.

Given these points, the intrusive linked list is often a better choice. While the methods for using it can be a bit more complex, the Linux kernel also uses intrusive linked lists, so we can adopt their methods.

Back to `embed_t`, let's see how we can initialize the structure.

```
embed_t node = {
	.num = 1805,
	.list = LIST_HEAD_INIT(node.list)
};
```

`LIST_HEAD_INIT` is a macro that is defined as:

```
#define LIST_HEAD_INIT(name) {&(name), &(name)}
```

This creates a list structure that points to itself by setting the next and previous pointer to point to the head node:

```
embed_t node = {
	.num = 1805,
	.list = {
		&(node.list),
		&(node.list)
	}
};
```

Let's see how to initialize `embed_t` dynamically:

```
embed_t node2;

node2.num = 1805;
INIT_LIST_HEAD(&(node2.list));
```

`INIT_LIST_HEAD` is an inline function:

```
static inline void INIT_LIST_HEAD(list_t *list)
{
	list->next = list;
	list->prev = list;
}
```

Again, `node2.list` is made to point back to itself.

## Creating a linked list head
We initialized `node` but it doesn't have a head. 

To create a head node, we can use another macro:

```
LIST_HEAD(head_node);
```

This one is defined as:

```
#define LIST_HEAD(name) \
	struct list name = LIST_HEAD_INIT(name)
```

It simple to see what is happening here, so let's move on and add elements to our list.

```
list_add(&(node.list), &head_node);
list_add(&(node2.list), &head_node);
```

`list_add` is a function. Here is how it is defined.

```
static inline void __list_add__(list_t *new, list_t *prev, list_t *next)
{
	new->next = next;
	new->prev = prev;
	prev->next = new;
	next->prev = new;
}

static inline void list_add(list_t *new, list_t *head)
{
	__list_add__(new, head, head->next);
}
```

## Traversing the linked list
```
list_t *pos;
embed_t *entry;

list_for_each(pos, &head_node)
{
	entry = list_entry(pos, embed_t, list);
	printf("num: %d\n", entry->num);
}
```

The above code is so complicated than it may seem so let's go over each part separately.

### The list_for_each macro
`list_for_each` is defined as:

```
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)
```

You can now at least tell why we need `list_t pos`, it is a placeholder in the for loop.

### The list_entry macro
`list_entry` is defined as:

```
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
```

The macro is wrapper for another macro `container_of`, so let's see how that is defined as well.

```
#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) ); })

#define offsetof(type, member) ((size_t)(&((type *)0)->member))
```

This macro is straight from Linux's books.

It calculates the address of the enclosing data structure given the address of a member.

We can make even traversing the linked list less painful by combining the `list_for_each` and `list_entry` macros.

```
embed_t *entry = NULL;

list_for_each_entry(entry, &head_node, list)
{
	printf("num: %d\n", entry->num);
}
```

`list_for_each_entry` is defined as:

```
#define list_for_each_entry(pos, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos), member) \
		 &pos->member != (head);  \
		 pos = list_entry(pos->member.next, typeof(*pos), member))
```

That will do for now and I hope you've gotten an overview of how the linked list is implemented.

Obviously, we will add more documentation as we discover more ways to use the linked list in the OS.

