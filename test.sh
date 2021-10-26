echo "    __  ______    __    __    ____  _________  __________  ____ "
echo "   /  |/  /   |  / /   / /   / __ \\/ ____/   |/_  __/ __ \\/ __ \\"
echo "  / /|_/ / /| | / /   / /   / / / / /   / /| | / / / / / / /_/ /"
echo " / /  / / ___ |/ /___/ /___/ /_/ / /___/ ___ |/ / / /_/ / _, _/ "
echo "/_/  /_/_/  |_/_____/_____/\\____/\\____/_/  |_/_/  \\____/_/ |_|  "
echo
echo 'By tmatis <tmatis@student.42.fr>'
echo
                                                                



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

make re FETCH_ADDR=0 > /dev/null

echo -ne "${BOLD}Fetching mallocs ... ${NC}"

echo "0x0" > ./addr.tmp


# need those
clang -fsanitize=undefined -Wall -Wextra -Werror -rdynamic -g -o malloc_test main.c -L. -lmallocator

./malloc_test &> ./logs/fetch_out.log

FETCH_OUT=$(cat ./logs/fetch_out.log | tail -n 3)
# format is
# malloc: nb_malloc
# free: nb_free
# <address> <address> <address> <address> etc..
# leaked
# leaked

# we parse
nb_malloc=$(head -n 1 <<< "$FETCH_OUT" | cut -d ':' -f 2 | xargs)
nb_free=$(head -n 2 <<< "$FETCH_OUT" | tail -n 1 | cut -d ':' -f 2 | xargs)
address_list=$(head -n 3 <<< "$FETCH_OUT" | tail -n 1)
is_leaking=0
echo -e "${GREEN}done${NC}"
echo

# check if nb_free == nb_malloc
if [ $nb_free -eq $nb_malloc ]; then
	echo -e "${BOLD}leak: ${GREEN}[OK]${NC} (./logs/fetch_out.log)"
else
	echo -e "${BOLD}leak: ${RED}[KO]${NC} (./logs/fetch_out.log) ${CYAN}<<<${NC} check with ${BOLD}valgrind${NC}"
	is_leaking=1
fi

echo

# for each address
for address in $address_list; do
	echo -en "${BOLD}Checking malloc at address: ${CYAN}$address${NC}"
	echo $address > ./addr.tmp
	./malloc_test &> ./logs/log_$address.log
	ret=$?
	if grep -q "ERROR: UndefinedBehaviorSanitizer" ./logs/log_$address.log; then
		echo -e " ${RED}[KO]${NC} ./logs/log_$address.log"
	else
		echo -e " ${GREEN}[OK]${NC}"
		log_content=$(cat ./logs/log_$address.log | tail -n 2)
		nb_malloc=$(head -n 1 <<< "$log_content" | cut -d ':' -f 2 | xargs)
		nb_free=$(head -n 2 <<< "$log_content" | tail -n 1 | cut -d ':' -f 2 | xargs)
		# $nb_free != $nb_malloc && is_leaking == 1
		if [ $nb_free -ne $nb_malloc ]; then
			if [ $is_leaking -eq 0 ]; then
				echo -ne " ${YELLOW} Warning >>> ${NC} "
				echo "you do not free everything when this malloc crash (see ./logs/log_$address.log)"
			fi
		fi

	fi
done

rm ./addr.tmp

