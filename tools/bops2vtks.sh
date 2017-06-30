#!/bin/sh

usg() {
    echo "usage: ./bop2ids <0.bop> <1.bop> ... -- <i0.bop> <i1.bop> ..."
    echo "usage: ./bop2ids <0.bop> <1.bop> ..."
    exit
}

if test $# -ne 0 -a "$1" = -h; then usg; fi

declare -a fbop
declare -a ibop

WF="true"
C=0

for a in $@; do
    if test "$a" = "--"; then WF="false"; C=0; continue; fi

    if test "$WF" = "true"; then 
        fbop[$C]=$a
    else
        ibop[$C]=$a
    fi
    C=$((C+1))
done

for ((i=0;i<${#fbop[@]};++i)); do
    fin="${fbop[i]}"
    if test "$WF" = "true"; then
        in=$fin
    else
        in="$fin -- \"${ibop[i]}\""
    fi
    out="${fin%.bop}.vtk"
    echo bop2vtk $out $in
done
