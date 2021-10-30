# How it work

 ## What is a route ?

In ***mallocator*** a route is the path which is taken to go from the **main** to the allocation (malloc, calloc etc..) this is the call stack.
A route is an array of address that look like this:
```C
0x1234 (main)
0x1235 (other_func)
0x1236 (malloc)
```
Each address is where the function is called, it allow us to differentiate two alloc in the same function.

## How we get the route ?


This is pretty simple, unix have a function called **backtrace** that fetch the route to itself.
```c
void *routes[20]

size_t number_of_pointers = backtrace(routes, 20);
```
We can get symbol name using **dladdr** and the clang flag **-rdynamic**
that allow dladdr to find symbol's name

```c
char const *get_func_name(void *addr)
{
Dl_info info;

if (dladdr(addr, &info) && info.dli_sname)
	return (info.dli_sname);
else
	return ("cannot get name");
}
```
## How to replace malloc ?

You have to make a library that have a malloc function,
that will be the hook.
You can use __libc_malloc malloc to use the real malloc
```c
extern  void *__libc_malloc(size_t size);
```

Mallocator's malloc look like this:

```c
int g_malloc_hook_active = 1;

void *alloc_hook(size_t size)
{
	void *result;
	g_malloc_hook_active = 0;
	// here we can call malloc safefly
	if (some_condition)
		result = NULL;
	else
		result =.malloc(size);
	g_malloc_hook_active = 1;
}

void *malloc(size_t size)
{
	if (g_malloc_hook_active)
		return (alloc_hook(size));
	return (__libc_malloc(size));
}
```
## Steps

 - Fetch all routes by hooking allocs, and use backtrace
 - Run and block every route one by one
- If the route have multiple iteration run a second time the route but make the route fail the second time it is called
