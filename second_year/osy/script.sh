#!/usr/bin/env bash

#PATH./
#PATH./link.txt
#PATH./a.txt

H=0
Z=0

while getopts "hz" flags; do
    case flags in 
        h|H)
            H=1
            break
            ;;
        z|Z)
            Z=1
            ;;
    esac
done

if [[ H==1 ]] ; then
    
fi

LINE=0
while read LINE ; do
    if [[ ! "$LINE" == PATH* ]] ; then
        continue
    fi
    
    LINE="${LINE#PATH}"
    
    #link
    if [[ -L "$LINE" ]] ; then
        target=$(readlink "$LINE")
        echo "LINK $LINE $target"
        continue
    fi

    #file
    if [[ -f "$LINE" ]] ; then
        line_count=$(wc -l < "$LINE")
        first_line=\'$(head -n 1 "$LINE")\'
        if [[ "$first_line" == "''" ]]; then
            first_line=""
        fi
        echo "FILE $LINE $line_count $first_line"
        continue
    fi

    #directory
    if [[ -d "$LINE" ]] ; then
        echo "DIR $LINE"
        continue
    fi

    echo "ERROR $LINE" >&2
done