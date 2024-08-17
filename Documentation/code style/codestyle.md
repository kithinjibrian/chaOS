# ChaOS Coding Style

Everyone's got their own way of coding, right? We're not gonna force our style on you or anything. Do your thing!

But hey, maybe you'll find some of our ideas useful. Worth a shot, right?

You should especially read about memory management, it's not a style but a rule.

## 1. Naming

In most code editors, autocompletion features can assist with longer variable and function names, so it's important to use descriptive names. Descriptive names not only make your code more readable but also simplify tasks like find and replace.

However, descriptive names should be concise and to the point. If a name is longer than one word, use underscores `_` to separate the words.

Examples:

```c
// password hash
❌ char *password_hash; 
✅ char *pw_hash; /* The name is brief but descriptive and understandable. */

❌ for(int counter; ;counter++)
✅ for(int i;;i++)

/* 
i, j, k: We all use these in simple loops
x, y, z: Prefered for coordinates
*/
```
### Global variables

Global variables should be distinguished from local variables by appending `_g` to their names. This makes them stand out and easily identifiable.
```c
int array_g[];
```

### Macros

Object-like macros should use uppercase letters.

```c
#define PI 3.14
```

Function-like macros should use lowercase letters.

```c
#define square(l) (l * l)
```

### Enums
All entries in an enum should be in uppercase.

```c
enum weekend
{
	FRIDAY,
	SATURDAY,
	SUNDAY,
}
```

## 2. Typedefs

When using typedef to create type aliases, follow these naming conventions:
1. Append `_t` for basic data types and structures.
2. Append `_ptr` for pointers to objects.
3. Append `_fun` for function pointers.
4. Append `_e` for enumerations.
5. Append `_u` for unions.
6. Append `_a` for arrays.

```c
typedef const char *string_t;

typedef struct rectangle
{
	int width;
	int length;
} rectangle_t, *rectangle_ptr;

/**
 * Also include the name of the parameters for clarity 
 */
typedef int (*area_fun)(int length, int width);

typedef enum
{
	FRIDAY,
	SATURDAY,
	SUNDAY,
} weekend_e;

typedef union
{
	int num;
	char ch;
} value_u;

typedef int array_a[5];
```

## 3. Comments
When you declare a function or a function-like macro, it should have a "JSDoc" comment.

```c
/**
 * Get the area of a rectangle.
 * @param length - The length of the rectangle
 * @param width - The width of the rectangle
 * @return A product of length and width
 */
 int area(int length, int width)
 {
	return length * width;
 }

 /**
 * Get the area of a rectangle.
 * @param length - The length of the rectangle
 * @param width - The width of the rectangle
 */
 #define area(length, width) (length * width)
 ```

 For comments inside a function, just place them where you think you did some voodoo magic, i.e:

 1. Did something very clever.
 2. Did something very ugly.

 Otherwise, aim for code that explains itself.

 ## 4. Memory management
Any object allocated on the heap by a function should be freed before the function returns. If the function has multiple exit points, it should centralize cleanup using goto statements to ensure that deallocation is handled consistently in one place.

```c
void cfree(void **obj)
{
	if(obj && *obj)
	{
		free(*obj);

		/* prevent a dangling pointer */
		*obj = NULL;
	}
}

void example(int bool)
{
	char *str = malloc(6);
	strncpy(str, "ChaOS", 6);

	if(bool)
	{
		goto jmp_to_exit;
	} else {
		got jmp_to_exit;
	}

jmp_to_exit:
	cfree((void *)&str);
	return;
}
```

If the allocated object should be shared between multiple functions or threads, the object should use reference counting.

Here is an example of an object being shared between functions.

```c
typedef struct string
{
	int ref;
	char str[10];
} string_t

string_t *retain(string_t *str)
{
	str->ref++;

	return str;
}

void release(string_t **str)
{
	(*str)->ref--;

	if((*str)->ref == 0)
	{
		free(*str);
	}

	/* prevent a dangling pointer */
	*str = NULL;
}

void fun1(string_t *str)
{
	int a = 1;
	if(a)
	{
		goto jmp_to_exit;
	}

jmp_to_exit:
	release(&str);
}

void fun2(string_t *str)
{
	release(&str);
}

void main()
{
	string_t *str = malloc(sizeof(string_t));
	str->ref = 1;
	strncpy(str->str, "ChaOS", 6);

	fun1(retain(str));
	fun2(retain(str));

	release(&str);
}
```

The above is not just a coding style; it's a rule. Under no circumstances should an object be passed to multiple functions without including a reference counter field. This prevents confusion over which function is responsible for freeing the object. The only exception to this rule is when using a memory arena, where all allocated objects can be freed simultaneously.





