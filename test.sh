# FETCH MALLOC RUN

make re FETCH_RUN=1

clang -Wall -Wextra -Werror -o malloc_test main.c -L. -lmallocator

FETCH_OUT=$(./malloc_test)

# format is
# <address> <address> <address> <address> etc..
# free: nb_free
# malloc: nb_malloc

# we parse

address_list=$(head -n 1 <<< "$FETCH_OUT")
nb_free=$(tail -n 1 <<< "$FETCH_OUT" | cut -d ':' -f 2 | xargs)
nb_malloc=$(head -n 2 <<< "$FETCH_OUT" | tail -n 1 | cut -d ':' -f 2 | xargs)

make re FETCH_RUN=0

clang -Wall -Wextra -Werror -o malloc_test main.c -L. -lmallocator
