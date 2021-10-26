echo "    __  ______    __    __    ____  _________  __________  ____ "
echo "   /  |/  /   |  / /   / /   / __ \\/ ____/   |/_  __/ __ \\/ __ \\"
echo "  / /|_/ / /| | / /   / /   / / / / /   / /| | / / / / / / /_/ /"
echo " / /  / / ___ |/ /___/ /___/ /_/ / /___/ ___ |/ / / /_/ / _, _/ "
echo "/_/  /_/_/  |_/_____/_____/\\____/\\____/_/  |_/_/  \\____/_/ |_|  "
echo
echo 'By tmatis <tmatis@student.42.fr>'
echo
                                                                

#CONFIG 
args=""


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



# need those
cd ..

echo "0x0" > ./.addr.tmp

make malloc_test # -fsanitize=undefined -rdynamic -g -L./ft_mallocator -lmallocator

./malloc_test $args &> ./ft_mallocator/logs/fetch_out.log

FETCH_OUT=$(cat ./ft_mallocator/logs/fetch_out.log | tail -n 2)

# we parse
nb_leaked_block=$(head -n 1 <<< "$FETCH_OUT" | cut -d ':' -f 2 | xargs)
address_list=$(head -n 2 <<< "$FETCH_OUT" | tail -n 1)
is_leaking=0
echo -e "${GREEN}done${NC}"
echo

# check if nb_free == nb_malloc
if [ $nb_leaked_block -eq 0 ]; then
	echo -e "${BOLD}leak: ${GREEN}[OK]${NC} (./ft_mallocator/logs/fetch_out.log)"
else
	echo -e "${BOLD}leak: ${RED}[KO]${NC} (./ft_mallocator/logs/fetch_out.log) ${CYAN}<<<${NC} check with ${BOLD}valgrind${NC}"
	is_leaking=1
fi

echo

# for each address
for address in $address_list; do
	echo -en "${BOLD}Checking malloc at address: ${CYAN}$address${NC}"
	echo $address > ./addr.tmp
	./malloc_test $args &> ./ft_mallocator/logs/log_$address.log
	ret=$?
	if grep -q "ERROR: UndefinedBehaviorSanitizer" ./logs/log_$address.log; then
		echo -e " ${RED}[KO]${NC} ./ft_mallocator/logs/log_$address.log"
	else
		echo -e " ${GREEN}[OK]${NC}"
		log_content=$(cat ./ft_mallocator/logs/log_$address.log | tail -n 1)
		nb_leaked_block=$(head -n 1 <<< "$log_content" | cut -d ':' -f 2 | xargs)
		# $nb_free != $nb_malloc && is_leaking == 1
		if [ $nb_leaked_block -ne 0 ]; then
			if [ $is_leaking -eq 0 ]; then
				echo -ne " ${YELLOW} Warning >>> ${NC} "
				echo "you do not free everything when this malloc crash (see ./ft_mallocator/logs/log_$address.log)"
			fi
		fi

	fi
done

rm ./.addr.tmp

