## Scalloc
A simple allocator for Linux, written in the C programming language.

## Compiling 
```bash
gcc scalloc.h scalloc.c your_file.c -o your_binary_name
```
## Use
**Allocating memory:**
```c
int* pointer_name = scalloc(size_in_bytes);
```
**Free memory:**
```c
scallocfree();
```
