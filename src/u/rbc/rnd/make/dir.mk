D = @d () { test -d "$$1" || mkdir -p -- "$$1"; } && \
    d $B/d                 && \
    d $B/mpi               && \
    d $B/rbc/adj           && \
    d $B/rbc/adj/edg       && \
    d $B/rbc/force/rnd     && \
    d $B/rbc/force/rnd/api && \
    d $B/u/rbc/rnd         && \
    d $B/utils            
