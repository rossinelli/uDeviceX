$B/algo/minmax.o: $S/algo/minmax.cu; $N -I$S/. -I$S/algo
$B/algo/scan/imp.o: $S/algo/scan/imp.cu; $N -I$S/. -I$S/algo/scan
$B/clist/imp.o: $S/clist/imp.cu; $N -I$S/. -I$S/clist
$B/cnt/imp.o: $S/cnt/imp.cu; $N -I$S/. -I$S/cnt
$B/color/flux.o: $S/color/flux.cu; $N -I$S/. -I$S/color
$B/comm/imp.o: $S/comm/imp.cu; $N -I$S/. -I$S/comm
$B/comm/oc/imp.o: $S/comm/oc/imp.cpp; $X -I$S/. -I$S/comm/oc
$B/d/api.o: $S/d/api.cu; $N -I$S/. -I$S/d
$B/dbg/imp.o: $S/dbg/imp.cu; $N -I$S/. -I$S/dbg
$B/distr/flu/imp.o: $S/distr/flu/imp.cu; $N -I$S/. -I$S/distr/flu
$B/distr/rbc/imp.o: $S/distr/rbc/imp.cu; $N -I$S/. -I$S/distr/rbc
$B/distr/rig/imp.o: $S/distr/rig/imp.cu; $N -I$S/. -I$S/distr/rig
$B/dpdr/imp.o: $S/dpdr/imp.cu; $N -I$S/. -I$S/dpdr
$B/dpdr/int.o: $S/dpdr/int.cpp; $X -I$S/. -I$S/dpdr
$B/exch/flu/imp.o: $S/exch/flu/imp.cu; $N -I$S/. -I$S/exch/flu
$B/exch/mesh/imp.o: $S/exch/mesh/imp.cu; $N -I$S/. -I$S/exch/mesh
$B/exch/obj/imp.o: $S/exch/obj/imp.cu; $N -I$S/. -I$S/exch/obj
$B/field.o: $S/field.cpp; $X -I$S/.
$B/flu/imp.o: $S/flu/imp.cpp; $X -I$S/. -I$S/flu
$B/frag/imp.o: $S/frag/imp.cpp; $X -I$S/. -I$S/frag
$B/fsi/imp.o: $S/fsi/imp.cu; $N -I$S/. -I$S/fsi
$B/fsolvent/bulk/imp.o: $S/fsolvent/bulk/imp.cu; $N -I$S/. -I$S/fsolvent/bulk
$B/fsolvent/bulk/transpose/imp.o: $S/fsolvent/bulk/transpose/imp.cu; $N -I$S/. -I$S/fsolvent/bulk/transpose
$B/fsolvent/halo/imp.o: $S/fsolvent/halo/imp.cu; $N -I$S/. -I$S/fsolvent/halo
$B/fsolvent/imp.o: $S/fsolvent/imp.cu; $N -I$S/. -I$S/fsolvent
$B/generate/rig/imp.o: $S/generate/rig/imp.cu; $N -I$S/. -I$S/generate/rig
$B/glb/imp.o: $S/glb/imp.cu; $N -I$S/. -I$S/glb
$B/glb/wvel/imp.o: $S/glb/wvel/imp.cu; $N -I$S/. -I$S/glb/wvel
$B/inter/imp.o: $S/inter/imp.cu; $N -I$S/. -I$S/inter -I$S/inter/_bangladesh
$B/io/bop/imp.o: $S/io/bop/imp.cpp; $X -I$S/. -I$S/io/bop
$B/io/com.o: $S/io/com.cpp; $X -I$S/. -I$S/io
$B/io/diag.o: $S/io/diag.cpp; $X -I$S/. -I$S/io
$B/io/field/h5/imp.o: $S/io/field/h5/imp.cpp; $X -I$S/. -I$S/io/field/h5
$B/io/field/imp.o: $S/io/field/imp.cpp; $X -I$S/. -I$S/io/field
$B/io/field/xmf/imp.o: $S/io/field/xmf/imp.cpp; $X -I$S/. -I$S/io/field/xmf
$B/io/fields_grid.o: $S/io/fields_grid.cpp; $X -I$S/. -I$S/io
$B/io/mesh/imp.o: $S/io/mesh/imp.cpp; $X -I$S/. -I$S/io/mesh
$B/io/mesh/write/imp.o: $S/io/mesh/write/imp.cpp; $X -I$S/. -I$S/io/mesh/write
$B/io/off.o: $S/io/off.cpp; $X -I$S/. -I$S/io
$B/io/ply.o: $S/io/ply.cpp; $X -I$S/. -I$S/io
$B/io/restart.o: $S/io/restart.cpp; $X -I$S/. -I$S/io
$B/io/rig.o: $S/io/rig.cpp; $X -I$S/. -I$S/io
$B/main.o: $S/main.cpp; $X -I$S/.
$B/math/linal.o: $S/math/linal.cpp; $X -I$S/. -I$S/math
$B/mesh/bbox.o: $S/mesh/bbox.cu; $N -I$S/. -I$S/mesh
$B/mesh/collision.o: $S/mesh/collision.cu; $N -I$S/. -I$S/mesh
$B/mesh/dist.o: $S/mesh/dist.cpp; $X -I$S/. -I$S/mesh
$B/mesh/props.o: $S/mesh/props.cpp; $X -I$S/. -I$S/mesh
$B/meshbb/imp.o: $S/meshbb/imp.cu; $N -I$S/. -I$S/meshbb
$B/mpi/glb.o: $S/mpi/glb.cpp; $X -I$S/. -I$S/mpi
$B/mpi/type.o: $S/mpi/type.cpp; $X -I$S/. -I$S/mpi
$B/mpi/wrapper.o: $S/mpi/wrapper.cpp; $X -I$S/. -I$S/mpi
$B/msg.o: $S/msg.cpp; $X -I$S/.
$B/rbc/adj/imp.o: $S/rbc/adj/imp.cpp; $X -I$S/. -I$S/rbc/adj
$B/rbc/com/imp.o: $S/rbc/com/imp.cu; $N -I$S/. -I$S/rbc/com
$B/rbc/edg/imp.o: $S/rbc/edg/imp.cpp; $X -I$S/. -I$S/rbc/edg
$B/rbc/force/area_volume/imp.o: $S/rbc/force/area_volume/imp.cu; $N -I$S/. -I$S/rbc/force/area_volume
$B/rbc/force/imp.o: $S/rbc/force/imp.cu; $N -I$S/. -I$S/rbc/force
$B/rbc/gen/imp.o: $S/rbc/gen/imp.cpp; $X -I$S/. -I$S/rbc/gen
$B/rbc/main/anti/imp.o: $S/rbc/main/anti/imp.cpp; $X -I$S/. -I$S/rbc/main/anti
$B/rbc/main/imp.o: $S/rbc/main/imp.cpp; $X -I$S/. -I$S/rbc/main
$B/rbc/rnd/api/imp.o: $S/rbc/rnd/api/imp.cpp; $X -I$S/. -I$S/rbc/rnd/api
$B/rbc/rnd/imp.o: $S/rbc/rnd/imp.cpp; $X -I$S/. -I$S/rbc/rnd
$B/rbc/stretch/imp.o: $S/rbc/stretch/imp.cu; $N -I$S/. -I$S/rbc/stretch
$B/rig/imp.o: $S/rig/imp.cu; $N -I$S/. -I$S/rig
$B/rigid/imp.o: $S/rigid/imp.cu; $N -I$S/. -I$S/rigid
$B/rnd/imp.o: $S/rnd/imp.cpp; $X -I$S/. -I$S/rnd
$B/scheme/force/imp.o: $S/scheme/force/imp.cu; $N -I$S/. -I$S/scheme/force
$B/scheme/move/imp.o: $S/scheme/move/imp.cu; $N -I$S/. -I$S/scheme/move
$B/scheme/restrain/imp.o: $S/scheme/restrain/imp.cpp; $X -I$S/. -I$S/scheme/restrain
$B/scheme/restrain/sub/imp.o: $S/scheme/restrain/sub/imp.cu; $N -I$S/. -I$S/scheme/restrain/sub
$B/scheme/restrain/sub/stat/imp.o: $S/scheme/restrain/sub/stat/imp.cpp; $X -I$S/. -I$S/scheme/restrain/sub/stat
$B/scheme/restrain/sub/sum/imp.o: $S/scheme/restrain/sub/sum/imp.cpp; $X -I$S/. -I$S/scheme/restrain/sub/sum
$B/sdf/imp.o: $S/sdf/imp.cu; $N -I$S/. -I$S/sdf
$B/sdf/int.o: $S/sdf/int.cu; $N -I$S/. -I$S/sdf
$B/sim/imp.o: $S/sim/imp.cu; $N -I$S/. -I$S/sim
$B/utils/cc.o: $S/utils/cc.cpp; $X -I$S/. -I$S/utils
$B/utils/error.o: $S/utils/error.cpp; $X -I$S/. -I$S/utils
$B/utils/halloc.o: $S/utils/halloc.cpp; $X -I$S/. -I$S/utils
$B/utils/mc.o: $S/utils/mc.cpp; $X -I$S/. -I$S/utils
$B/utils/os.o: $S/utils/os.cpp; $X -I$S/. -I$S/utils
$B/vcontroller/imp.o: $S/vcontroller/imp.cu; $N -I$S/. -I$S/vcontroller
$B/wall/exch/imp.o: $S/wall/exch/imp.cpp; $X -I$S/. -I$S/wall/exch
$B/wall/force/imp.o: $S/wall/force/imp.cu; $N -I$S/. -I$S/wall/force
$B/wall/imp.o: $S/wall/imp.cu; $N -I$S/. -I$S/wall
