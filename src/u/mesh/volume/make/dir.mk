D = @d () { test -d "$$1" || mkdir -p -- "$$1"; } && \
    d $B/algo/kahan_sum && \
    d $B/conf           && \
    d $B/coords         && \
    d $B/d              && \
    d $B/io/mesh_read   && \
    d $B/math/tform     && \
    d $B/mesh/vectors   && \
    d $B/mesh/volume    && \
    d $B/mpi            && \
    d $B/u/mesh/volume  && \
    d $B/utils         
