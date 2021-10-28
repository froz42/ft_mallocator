#!/bin/bash

#CONFIG

PROJECT_PATH=".."

ARGS="4 800 200 200 6"


# compute config

# remove last / of PROJECT_PATH if exist
if [ "${PROJECT_PATH: -1}" == "/" ]; then
    PROJECT_PATH=${PROJECT_PATH:0:${#PROJECT_PATH}-1}
fi

# usefull vars
WORK_PATH=$(pwd)

# COLORS VAR

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[1;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m'

echo -ne "${CYAN}"
echo "    __  ______    __    __    ____  _________  __________  ____ "
echo "   /  |/  /   |  / /   / /   / __ \\/ ____/   |/_  __/ __ \\/ __ \\"
echo "  / /|_/ / /| | / /   / /   / / / / /   / /| | / / / / / / /_/ /"
echo " / /  / / ___ |/ /___/ /___/ /_/ / /___/ ___ |/ / / /_/ / _, _/ "
echo "/_/  /_/_/  |_/_____/_____/\\____/\\____/_/  |_/_/  \\____/_/ |_|  "
echo -e "${NC}"
echo 'By tmatis <tmatis@student.42.fr>'
echo

if [ ! -n "$BASH" ] ;
then
    echo "Please run this script using bash"
    exit 1
fi

rm -rf ./logs
mkdir ./logs

echo -ne "${BOLD}Make malloc hook ... ${NC}"

make re &> ./logs/make_malloc_hook.log
return_value=$?

if [ $return_value -ne 0 ]; then
    echo -e "${RED}${BOLD}fail${NC} check ./ft_mallocator/logs/make_malloc_hook.log"
    exit 1
else
    echo -e "${GREEN}${BOLD}done${NC}"
fi

echo -ne "${BOLD}Compiling ... ${NC}"

make -C $PROJECT_PATH malloc_test &> ./logs/make.log
#clang -Wall -Werror -Wextra -fsanitize=undefined -rdynamic -g main.c -o malloc_test -L. -lmallocator
return_value=$?

if [ $return_value -ne 0 ]; then
    echo -e "${RED}${BOLD}fail${NC} check ./logs/make.log"
    exit 1
else
    echo -e "${GREEN}${BOLD}done${NC}"
fi

echo -ne "${BOLD}Fetching malloc routes ... ${NC}"

cd $PROJECT_PATH
./malloc_test $ARGS &> $WORK_PATH/logs/fetch_routes.log
cd $WORK_PATH

echo -e "${GREEN}${BOLD}done${NC}"
echo

routes=()
readarray -t routes < $PROJECT_PATH/routes.tmp

echo -ne "${BOLD}Leaks ... ${NC}"
is_leaking=0
LEAKS=$(head -n 1 $PROJECT_PATH/leaks.tmp | cut -d ':' -f 2 | sed 's/ //g')
if [ $LEAKS -ne 0 ]; then
    echo -e "${RED}${BOLD}fail${NC} check ./logs/fetch_routes.log"
    is_leaking=1
else
    echo -e "${GREEN}${BOLD}ok${NC}"
fi
echo

# get size of routes
size=${#routes[@]}

echo -e "${CYAN}${BOLD}$size${NC} routes to check:"

for route in "${routes[@]}"
do
    # we need to parse route the format is :
    # addrr:name addr:name addr:name #iteration
    
    #parse the iteration
    iteration=$(echo $route | cut -d '#' -f 2)
    
    #get the addresses
    addresses_names=$(echo $route | cut -d '#' -f 1)
    
    names=""
    addresses=""
    for address_name in $addresses_names
    do
        names="$names $(echo $address_name | cut -d ':' -f 2)"
        addresses="$addresses $(echo $address_name | cut -d ':' -f 1)"
    done
    
    reversed_names=""
    for name in $names
    do
        reversed_names="$name $reversed_names"
    done
    
    size_names=$(echo $names | wc -w)
    echo -ne "${BOLD}Testing route: ${NC}"
    for name in $reversed_names
    do
        echo -ne "${CYAN}$name${NC}"
        # if its not the last one, we need to add a space
        if [ $((size_names--)) -ne 1 ]; then
            echo -ne "${BLUE} -> ${NC}"
        fi
    done
    echo -n " ... "
    echo $addresses > $PROJECT_PATH/addresses.tmp
    
    #replace spaces with / in names
    path_names=$(echo $reversed_names | sed 's/ /\//g')
    mkdir -p ./logs/$path_names
    count=$(ls ./logs/$path_names | wc -l)
    echo '### FT MALLOCATOR TEST ###' > ./logs/$path_names/$count.log
    echo "route is: $addresses_names" >> ./logs/$path_names/$count.log
    echo >> ./logs/$path_names/$count.log

	cd $PROJECT_PATH
    ./malloc_test $ARGS &>> $WORK_PATH/logs/$path_names/$count.log
	cd $WORK_PATH


    rm -rf $PROJECT_PATH/addresses.tmp
    if grep -q "ERROR: UndefinedBehaviorSanitizer" ./logs/$path_names/$count.log; then
        echo -e "${RED}fail${NC}"
        echo -e "${RED}  >>>${NC} this malloc is not protected, check: ${BOLD}./logs/$path_names/$count.log${NC}"
    else
        LEAKS=$(head -n 1 $PROJECT_PATH/leaks.tmp | cut -d ':' -f 2 | sed 's/ //g')
        if [ $LEAKS -eq 0 ] || [ $is_leaking -eq 1 ]; then
            if [ $iteration -ne 1 ]; then
                echo $addresses > $PROJECT_PATH/addresses.tmp
                echo 1 > $PROJECT_PATH/iteration.tmp
                echo "### SECOND TEST ###" >> ./logs/$path_names/$count.log
				
				cd $PROJECT_PATH
                ./malloc_test $ARGS &>> $WORK_PATH/logs/$path_names/$count.log
				cd $WORK_PATH

                rm -rf $PROJECT_PATH/addresses.tmp $PROJECT_PATH/iteration.tmp
                if grep -q "ERROR: UndefinedBehaviorSanitizer" ./logs/$path_names/$count.log; then
                    echo -e "${RED}${BOLD}fail${NC}"
                    echo -e "${RED}${BOLD}  >>>${NC} this malloc is not protected, check: ${BOLD}./logs/$path_names/$count.log${NC}"
                else
                    LEAKS=$(head -n 1 $PROJECT_PATH/leaks.tmp | cut -d ':' -f 2 | sed 's/ //g')
                    if [ $LEAKS -eq 0 ] || [ $is_leaking -eq 1 ]; then
                        echo -e "${GREEN}${BOLD}ok${NC}"
                    else
                        echo -e "${YELLOW}${BOLD}warn${NC}"
                        echo -e "${YELLOW}${BOLD}  >>>${NC} you don't free everything when this malloc crash, check: ${BOLD}./logs/$path_names/$count.log${NC}"
                        cat $PROJECT_PATH/leaks.tmp >> ./logs/$path_names/$count.log
                    fi
                fi
            else
                echo -e "${GREEN}${BOLD}ok${NC}"
            fi
        else
            echo -e "${YELLOW}${BOLD}warn${NC}"
            echo -e "${YELLOW}${BOLD}  >>>${NC} you don't free everything when this malloc crash, check: ${BOLD}./logs/$path_names/$count.log${NC}"
            cat $PROJECT_PATH/leaks.tmp >> ./logs/$path_names/$count.log
        fi
    fi
    rm -rf $PROJECT_PATH/leaks.tmp $PROJECT_PATH/routes.tmp
done
