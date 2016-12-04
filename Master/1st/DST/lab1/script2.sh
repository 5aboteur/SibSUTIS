#!/bin/bash

clear

ssh tereshkov@jet.cpct.sibsutis.ru "./DST/lab1/script1.sh"
scp tereshkov@jet.cpct.sibsutis.ru:~/DST/lab1/.pbsnodes_info ./

cat .pbsnodes_info
