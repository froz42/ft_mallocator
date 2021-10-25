# COLORS VAR

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[1;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m'

rm -rf ./logs
mkdir ./logs

# FETCH MALLOC RUN
echo -ne "${BOLD}Fetching mallocs ... ${NC}"

make re FETCH_ADDR=0 > /dev/null

echo "0x0" > ./addr.tmp

clang -fsanitize=undefined -Wall -Wextra -Werror -rdynamic -g -o malloc_test main.c -L. -lmallocator

FETCH_OUT=$(./malloc_test)

# format is
# <address> <address> <address> <address> etc..
# free: nb_free
# malloc: nb_malloc

# we parse

address_list=$(head -n 1 <<< "$FETCH_OUT")
nb_free=$(tail -n 1 <<< "$FETCH_OUT" | cut -d ':' -f 2 | xargs)
nb_malloc=$(head -n 2 <<< "$FETCH_OUT" | tail -n 1 | cut -d ':' -f 2 | xargs)
is_leaking=0
echo -e "${GREEN}done${NC}"

# check if nb_free == nb_malloc
if [ $nb_free -eq $nb_malloc ]; then
	echo -e "${BOLD}leak: ${GREEN}[OK]${NC} $nb_free free / $nb_malloc malloc ${NC}"
else
	echo -e "${BOLD}leak: ${RED}[KO]${NC} $nb_free free / $nb_malloc malloc ${CYAN}<<<${NC} check with ${BOLD}valgrind${NC}"
	is_leaking=1
fi

echo -e "${BOLD}Malloc list:${NC}"
echo "$address_list"

# for each address
for address in $address_list; do
	echo -en "${BOLD}Checking malloc at address: ${CYAN}$address${NC}"
	echo $address > ./addr.tmp
	./malloc_test &> ./logs/log_$address.log
	ret=$?
	# check if "ERROR: UndefinedBehaviorSanitizer" in ./logs/log_$address.log
	if grep -q "ERROR: UndefinedBehaviorSanitizer" ./logs/log_$address.log; then
		echo -e " ${RED}[KO]${NC}"
	else
		echo -e " ${GREEN}[OK]${NC}"
		log_content=$(cat ./logs/log_$address.log)
		nb_malloc=$(head -n 1 <<< "$log_content" | cut -d ':' -f 2 | xargs)
		nb_free=$(tail -n 1 <<< "$log_content" | tail -n 1 | cut -d ':' -f 2 | xargs)
		# $nb_free != $nb_malloc && is_leaking == 1
		if [ $nb_free -ne $nb_malloc ]; then
			if [ $is_leaking -eq 0 ]; then
				echo -e " ${YELLOW} Warning >>> ${NC} you do not free everything when this malloc crash"
			fi
		fi

	fi
done

rm ./addr.tmp

