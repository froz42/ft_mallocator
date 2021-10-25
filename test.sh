# COLORS VAR

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[1;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m'


# FETCH MALLOC RUN
echo -ne "${BOLD}Fetching mallocs ... ${NC}"

make re FETCH_ADDR=0 > /dev/null

echo "0x0" > ./addr.tmp

clang -fsanitize=undefined -Wall -Wextra -Werror -o malloc_test main.c -L. -lmallocator

FETCH_OUT=$(./malloc_test)

# format is
# <address> <address> <address> <address> etc..
# free: nb_free
# malloc: nb_malloc

# we parse

address_list=$(head -n 1 <<< "$FETCH_OUT")
nb_free=$(tail -n 1 <<< "$FETCH_OUT" | cut -d ':' -f 2 | xargs)
nb_malloc=$(head -n 2 <<< "$FETCH_OUT" | tail -n 1 | cut -d ':' -f 2 | xargs)

echo -e "${GREEN}done${NC}"

# check if nb_free == nb_malloc
if [ $nb_free -eq $nb_malloc ]; then
	echo -e "${BOLD}leak: ${GREEN}[OK]${NC} $nb_free free / $nb_malloc malloc ${NC}"
else
	echo -e "${BOLD}leak: ${RED}[KO]${NC} $nb_free free / $nb_malloc malloc ${CYAN}<<<${NC} check with ${BOLD}valgrind${NC}"
fi

echo -e "${BOLD}Address list:${NC}"
echo "$address_list"

# for each address
for address in $address_list; do
	echo -e "${BOLD}Checking address: ${CYAN}$address${NC}"
	echo $address > ./addr.tmp
	./malloc_test
done

rm ./addr.tmp

