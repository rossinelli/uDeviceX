D = @d () { test -d "$$1" || mkdir -p -- "$$1"; } && \
    d $B/algo              && \
    d $B/algo/scan         && \
    d $B/clist             && \
    d $B/cnt               && \
    d $B/comm              && \
    d $B/d                 && \
    d $B/dbg               && \
    d $B/distr/flu         && \
    d $B/distr/rbc         && \
    d $B/distr/rig         && \
    d $B/dpdr              && \
    d $B/exch/flu          && \
    d $B/exch/mesh         && \
    d $B/exch/obj          && \
    d $B/flu               && \
    d $B/frag              && \
    d $B/fsi               && \
    d $B/generate/rig      && \
    d $B/glb               && \
    d $B/glb/gdot          && \
    d $B/hforces           && \
    d $B/inter             && \
    d $B/io                && \
    d $B/io/bop            && \
    d $B/io/field          && \
    d $B/io/field/xmf      && \
    d $B/lforces           && \
    d $B/lforces/transpose && \
    d $B/math              && \
    d $B/mesh              && \
    d $B/meshbb            && \
    d $B/mpi               && \
    d $B/rbc               && \
    d $B/restrain          && \
    d $B/restrain/stat     && \
    d $B/restrain/sum      && \
    d $B/rig               && \
    d $B/rigid             && \
    d $B/rnd               && \
    d $B/scheme            && \
    d $B/sdf               && \
    d $B/sim               && \
    d $B/utils             && \
    d $B/vcontroller       && \
    d $B/wall              && \
    d $B/wall/exch         && \
    d $B/wall/force       
