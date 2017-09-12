#!/bin/bash

. ../generic.sh
. ./pre.sh

XS=24
YS=24
ZS=24

NX=$1; shift
NY=$1; shift
NZ=$1; shift

RUNDIR=${SCRATCH}/UDX/periodic_rbc/$NX.$NY.$NZ

copy() {
    all="udx launch.sh conf.h rbc.off rbcs-ic.txt"
    mkdir -p $RUNDIR
    for f in $all
    do cp $f $RUNDIR
    done
}

clean
setup
geom
echo "sub domain: $Domain"
echo "full domain: $FullDomain"

ic
u.strtdir $RUNDIR $NX $NY $NZ

u="u/x"
s="${GITROOT}/src"

u.conf $s $u conf.base.h <<EOF
    $Domain
    run
EOF

compile
copy

cd $RUNDIR

PROF="-o prof.%q{OMPI_COMM_WORLD_RANK}.nvprof"
u.batch $NX $NY $NZ ./udx 04:00
