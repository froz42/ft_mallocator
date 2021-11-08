#!/bin/bash

# default CONFIG

PROJECT_PATH=".."
ARGS=""
CHECK_LEAKS=1

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

# check if script is run on linux
if [ "$(uname)" != "Linux" ]; then
    echo "This script is only compatible with Linux"
    exit 1
fi

# usefull vars
WORK_PATH=$(pwd)

# COLORS VAR

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[1;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
UNDERLINE='\033[4m'
BOLD='\033[1m'
NC='\033[0m'

# check if config file exist
if [ ! -f "${WORK_PATH}/config.sh" ]; then
    echo -e "${BLUE}${BOLD}>>>${NC} No config file found configuring.."
    
    echo -e "to use ft_mallocator you need to setup a few things in your makefile"
    echo -e "1. add ${UNDERLINE}${BOLD}-g${NC} to your compilation flags"
    echo -e "CFLAGS = -Wall -Wextra -Werror ${UNDERLINE}${BOLD}-g${NC}"
    echo -e "2. add a rule that look like this in your Makefile"
    echo -e "${UNDERLINE}${BOLD}malloc_test:${NC} \$(OBJS) otherdispencies..."
    echo -ne "	\$(CC) \$(CFLAGS) ${UNDERLINE}${BOLD}-fsanitize=undefined${NC}"
    echo -ne " ${UNDERLINE}${BOLD}-rdynamic${NC} -o \$@ \${OBJS}"
    echo -e " -L./otherlib/ -lyourlib ${UNDERLINE}${BOLD}-L. -lmallocator${NC}"
    read -p "Press enter to continue... "
    
    echo -e "What is your ${UNDERLINE}${BOLD}project path${NC} ? (default is '$PROJECT_PATH')"
    echo -en "${CYAN}>>> ${NC}"
    read PROJECT_PATH_TEMP
    if [ -n "$PROJECT_PATH_TEMP" ]; then
        PROJECT_PATH=$PROJECT_PATH_TEMP
    fi
    
    echo -e "What ${UNDERLINE}${BOLD}arguments${NC} do you want ?"
    echo -e "Example :"
    echo -e "* ${UNDERLINE}4 800 200 200 6${NC} (for philosophers)"
    echo -e "* ${UNDERLINE}< ./file/to/stdin${NC} (for minishell)"
    echo -e "* ${UNDERLINE}./path/to/map.mapformat${NC}"
    echo -e "Note that the path is relative to your project folder"
    echo -en "${CYAN}>>> ${NC}"
    read ARGS
    
    echo -e "Do you want to check leaks when malloc crash ? [y/n] (default is yes)"
    echo -en "${CYAN}>>> ${NC}"
    read CHECK_LEAKS_TEMP
    if [ -n "$CHECK_LEAKS_TEMP" ]; then
        if [ "$CHECK_LEAKS_TEMP" == "y" ]; then
            CHECK_LEAKS=1
        else
            CHECK_LEAKS=0
        fi
    fi
    
    echo
    
    echo -en "${BLUE}${BOLD}>>>${NC} ${BOLD}Creating config file ...${NC} "
    echo -e "PROJECT_PATH=\"${PROJECT_PATH}\"" > config.sh
    echo -e "ARGS=\"${ARGS}\"" >> config.sh
    echo -e "CHECK_LEAKS=${CHECK_LEAKS}" >> config.sh
    
    echo -e "${GREEN}${BOLD}done${NC}"
else
    echo -ne "${BLUE}${BOLD}>>>${NC} ${BOLD}loading config file (config.sh) ... "
    # load config file
    source ./config.sh 2> /dev/null
    return_value=$?
    if [ $return_value -ne 0 ]; then
        echo -e "${RED}${BOLD}fail${NC}"
        exit 1
    else
        echo -e "${GREEN}${BOLD}done${NC}"
    fi
fi

echo

# compute config

# remove last / of PROJECT_PATH if exist
if [ "${PROJECT_PATH: -1}" == "/" ]; then
    PROJECT_PATH=${PROJECT_PATH:0:${#PROJECT_PATH}-1}
fi


rm -rf ./logs
mkdir ./logs

echo -ne "${BLUE}${BOLD}>>>${NC} ${BOLD}Make malloc hook ... ${NC}"

make re &> ./logs/make_malloc_hook.log
return_value=$?

if [ $return_value -ne 0 ]; then
    echo -e "${RED}${BOLD}fail${NC} check ./ft_mallocator/logs/make_malloc_hook.log"
    exit 1
else
    echo -e "${GREEN}${BOLD}done${NC}"
fi

cp libmallocator.a $PROJECT_PATH/libmallocator.a &> ./logs/make.log

while [ 1 ]
do
    echo -ne "${BLUE}${BOLD}>>>${NC} ${BOLD}Compiling ... ${NC}"
    
    make -C $PROJECT_PATH malloc_test &>> ./logs/make.log
    return_value=$?
    
    if [ $return_value -ne 0 ]; then
        echo -e "${RED}${BOLD}fail${NC} check ./logs/make.log"
        echo -e "to use ft_mallocator you need to setup a few things in your makefile"
        echo -e "1. add ${UNDERLINE}${BOLD}-g${NC} to your compilation flags"
        echo -e "CFLAGS = -Wall -Wextra -Werror ${UNDERLINE}${BOLD}-g${NC}"
        echo -e "2. add a rule that look like this in your Makefile"
        echo -e "${UNDERLINE}${BOLD}malloc_test:${NC} \$(OBJS) otherdispencies..."
        echo -ne "	\$(CC) \$(CFLAGS) ${UNDERLINE}${BOLD}-fsanitize=undefined${NC}"
        echo -ne " ${UNDERLINE}${BOLD}-rdynamic${NC} -o \$@ \${OBJS}"
    	echo -e " -L./otherlib/ -lyourlib ${UNDERLINE}${BOLD}-L. -lmallocator${NC}"
        read -p "Press enter to retry... "
    else
        echo -e "${GREEN}${BOLD}done${NC}"
        break
    fi
    make -C $PROJECT_PATH fclean &>> ./logs/make.log
done

rm -rf $PROJECT_PATH/libmallocator.a

echo -ne "${BLUE}${BOLD}>>>${NC} ${BOLD}Fetching malloc routes ... ${NC}"

cd $PROJECT_PATH
eval "./malloc_test $ARGS" &> $WORK_PATH/logs/fetch_routes.log
cd $WORK_PATH

echo -e "${GREEN}${BOLD}done${NC}"
echo

# check if $PROJECT_PATH/routes.tmp exist

if [ ! -f "$PROJECT_PATH/routes.tmp" ]; then
    echo -e "${BOLD}Fail to hook any malloc (check ./logs/fetch_routes.log), please do not use -fsanitize=address (use -fsanitize=undefined)${NC}"
	rm -rf $PROJECT_PATH/*.tmp
    exit 1
fi

routes=()
readarray -t routes < $PROJECT_PATH/routes.tmp

echo -ne "${BLUE}${BOLD}>>>${NC} ${BOLD}Leaks ... ${NC}"
LEAKS=$(head -n 1 $PROJECT_PATH/leaks.tmp | cut -d ':' -f 2 | sed 's/ //g')
if [ $LEAKS -ne 0 ]; then
    echo -e "${RED}${BOLD}fail${NC} check ./logs/fetch_routes.log"
	cat $PROJECT_PATH/leaks.tmp >> ./logs/fetch_routes.log
else
    echo -e "${GREEN}${BOLD}ok${NC}"
fi
echo


# get size of routes
size=${#routes[@]}
success_route=0
warn_route=0

if [ $size -eq 0 ]; then
	echo -e "${BOLD}Fail to process routes, please set -rdynamic flag at linking${NC}"
	rm -rf $PROJECT_PATH/*.tmp
	exit 1
fi

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
    echo -ne "${NC}${BOLD}Testing route: ${NC}"
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
    eval "./malloc_test $ARGS" &>> $WORK_PATH/logs/$path_names/$count.log
    cd $WORK_PATH
    
    
    rm -rf $PROJECT_PATH/addresses.tmp
    if grep -q "ERROR: UndefinedBehaviorSanitizer" ./logs/$path_names/$count.log || [ ! -f "$PROJECT_PATH/leaks.tmp" ]; then
        echo -e "${RED}${BOLD}fail${NC}"
        echo -e "${RED}${BOLD}  >>>${NC} this malloc is not protected, check: ${BOLD}./logs/$path_names/$count.log${NC}"
    else
        LEAKS=$(head -n 1 $PROJECT_PATH/leaks.tmp | cut -d ':' -f 2 | sed 's/ //g')
        if [ $LEAKS -eq 0 ] || [ $CHECK_LEAKS -eq 0 ]; then
            if [ $iteration -ne 1 ]; then
                echo $addresses > $PROJECT_PATH/addresses.tmp
                echo 1 > $PROJECT_PATH/iteration.tmp
                echo "### SECOND TEST ###" >> ./logs/$path_names/$count.log
                
                cd $PROJECT_PATH
                eval "./malloc_test $ARGS" &>> $WORK_PATH/logs/$path_names/$count.log
                cd $WORK_PATH
                
                rm -rf $PROJECT_PATH/addresses.tmp $PROJECT_PATH/iteration.tmp
                if grep -q "ERROR: UndefinedBehaviorSanitizer" ./logs/$path_names/$count.log; then
                    echo -e "${RED}${BOLD}fail${NC}"
                    echo -e "${RED}${BOLD}  >>>${NC} this malloc is not protected, check: ${BOLD}./logs/$path_names/$count.log${NC}"
                else
                    LEAKS=$(head -n 1 $PROJECT_PATH/leaks.tmp | cut -d ':' -f 2 | sed 's/ //g')
                    if [ $LEAKS -eq 0 ] || [ $CHECK_LEAKS -eq 0 ]; then
                        echo -e "${GREEN}${BOLD}ok${NC}"
                    else
                        echo -e "${YELLOW}${BOLD}warn${NC}"
                        echo -e "${YELLOW}${BOLD}  >>>${NC} you don't free everything when this malloc crash, check: ${BOLD}./logs/$path_names/$count.log${NC}"
                        cat $PROJECT_PATH/leaks.tmp >> ./logs/$path_names/$count.log
                        ((warn_route++))
                    fi
                    ((success_route++))
                fi
            else
                echo -e "${GREEN}${BOLD}ok${NC}"
                ((success_route++))
            fi
        else
            echo -e "${YELLOW}${BOLD}warn${NC}"
            echo -e "${YELLOW}${BOLD}  >>>${NC} you don't free everything when this malloc crash, check: ${BOLD}./logs/$path_names/$count.log${NC}"
            cat $PROJECT_PATH/leaks.tmp >> ./logs/$path_names/$count.log
			((warn_route++))
			((success_route++))
        fi
    fi
    rm -rf $PROJECT_PATH/leaks.tmp $PROJECT_PATH/routes.tmp
done
rm -rf $PROJECT_PATH/malloc_test

echo

echo -ne "${BLUE}${BOLD}>>>${NC} ${BOLD}Result:${NC} "
echo -ne "${CYAN}${BOLD}$success_route/$size${NC} routes are protected ... "

if [ $success_route -ne $size ]; then
    echo -e "${RED}${BOLD}fail${NC}"
    exit 1
else
    if [ $warn_route -ne 0 ]; then
        echo -e "${YELLOW}${BOLD}warn${NC} (not everything is free)"
    else
        echo -e "${GREEN}${BOLD}success${NC}"
    fi
fi

