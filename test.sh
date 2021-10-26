#!/bin/bash

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
	echo -e "${RED}fail${NC} check ./ft_mallocator/logs/make_malloc_hook.log"
	exit 1
else
	echo -e "${GREEN}done${NC}"
fi

