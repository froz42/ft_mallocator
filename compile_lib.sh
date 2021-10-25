
fail_number=0
clang -Wall -Werror -Wextra -g -c vector.c malloc_hook.c -D FAIL=$fail_number \
&& ar -rcs libmallocator.a malloc_hook.o vector.o
rm malloc_hook.o vector.o
clang -Wall -Wextra -Werror -o out main.c -L. -lmallocator
