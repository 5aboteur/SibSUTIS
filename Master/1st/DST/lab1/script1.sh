#!/bin/bash

let flg=0
OUTSTREAM=""
prev_node=""
DEFAULT_IFS=$IFS


for word in $(pbsnodes) ; do

    if [[ $flg -ge 3 ]]
    then
        if [[ $flg -eq 4 ]]
        then
            let flg=$flg-1
            continue
        fi

        echo $word
        IFS=','
        let count_jobs=1
 
        for job in $word ; do
            OUTSTREAM=$OUTSTREAM'\t'$count_jobs") "$job'\n'
            let count_jobs=$count_jobs+1
        done

        OUTSTREAM=$OUTSTREAM"\n"
        IFS=$DEFAULT_IFS
        let flg=0
    elif [[ $flg -eq 2 ]]
    then
        OUTSTREAM=$OUTSTREAM$word' '
        let flg=$flg-1
    elif [[ $flg -eq 1 ]]
    then
        OUTSTREAM=$OUTSTREAM$word'\n'
        let flg=$flg-1
    fi

    if [[ $word = cn* && $word != $prev_node ]]
    then
        OUTSTREAM=$OUTSTREAM'\n'$word'\n'
        prev_node=$word
    elif [[ $word = "jobs" ]]
    then
        OUTSTREAM=$OUTSTREAM'\t'$word":\n"
        let flg=4
        echo $flg
    elif [[ $word = "state" || $word = "power_state" ]]
    then
        OUTSTREAM=$OUTSTREAM'\t'$word' '
        let flg=2
    fi

done

echo -e $OUTSTREAM
echo -e $OUTSTREAM > .pbsnodes_info
