$B/algo/minmax.o: $S/inc/conf.h $S/inc/type.h $S/d/q.h $S/utils/cc.h $S/inc/def.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/utils/kl.h $S/algo/minmax.h $S/d/ker.h
$B/algo/scan/imp.o: $S/algo/scan/int.h $S/inc/conf.h $S/d/q.h $S/utils/cc.h $S/inc/def.h $S/algo/scan/dev.h $S/algo/scan/cpu/imp.h $S/d/api.h $S/msg.h $S/inc/dev.h $B/conf.h $S/utils/kl.h $S/algo/scan/cuda/imp.h $S/d/ker.h
$B/clist/imp.o: $S/clist/imp/fin.h $S/clist/code.h $S/algo/scan/int.h $S/inc/conf.h $S/inc/type.h $S/clist/imp.h $S/utils/cc.h $S/inc/def.h $S/clist/dev.h $S/clist/imp/ini.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/clist/imp/main.h $S/utils/kl.h
$B/cnt/imp.o: $S/cnt/dev/pair.h $S/algo/scan/int.h $S/inc/conf.h $S/forces/imp.h $S/inc/type.h $S/d/q.h $S/cnt/imp/halo.h $S/cnt/dev/map/common.h $S/dbg/imp.h $S/cnt/imp.h $S/inc/dev/common.h $S/cnt/imp/bulk.h $S/cnt/dev/map/bulk.h $S/utils/cc.h $S/inc/def.h $S/rnd/dev.h $S/cnt/dev/halo.h $S/cnt/dev/map/halo.h $S/inc/dev.h $S/msg.h $S/rnd/imp.h $S/d/api.h $B/conf.h $S/cnt/imp/main.h $S/forces/pack.h $S/cnt/dev/bulk.h $S/utils/kl.h $S/clist/code.h $S/clist/imp.h $S/forces/type.h $S/frag/imp.h $S/forces/use.h $S/mpi/glb.h $S/d/ker.h
$B/color/flux.o: $S/inc/conf.h $S/inc/type.h $S/utils/cc.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/color/flux.h $S/utils/kl.h $S/mpi/glb.h
$B/comm/imp.o: $S/comm/imp/fin.h $S/inc/conf.h $S/utils/error.h $S/mpi/basetags.h $S/mpi/wrapper.h $S/comm/imp.h $S/utils/cc.h $S/comm/imp/ini.h $S/d/api.h $S/utils/mc.h $S/msg.h $B/conf.h $S/comm/imp/main.h $S/frag/imp.h $S/mpi/glb.h
$B/d/api.o: $S/d/cpu/imp.h $S/inc/conf.h $S/d/common.h $S/d/api.h $S/msg.h $B/conf.h $S/d/cuda/imp.h
$B/dbg/imp.o: $S/dbg/dev/force.h $S/inc/conf.h $S/dbg/error.h $S/inc/type.h $S/dbg/imp.h $S/dbg/dev/vel.h $S/dbg/dev/clist.h $S/dbg/dev/common.h $S/utils/cc.h $S/inc/def.h $S/d/api.h $S/msg.h $S/inc/dev.h $B/conf.h $S/dbg/macro/switch.h $S/utils/kl.h $S/dbg/dev/pos.h $S/dbg/dev/color.h
$B/distr/flu/imp.o: $S/distr/flu/imp/fin.h $S/comm/imp.h $S/algo/scan/int.h $S/inc/conf.h $S/flu/imp.h $S/utils/error.h $S/distr/flu/imp/com.h $S/comm/utils.h $S/mpi/basetags.h $S/inc/type.h $S/distr/map/type.h $S/distr/flu/imp.h $S/inc/dev/common.h $S/distr/flu/imp/gather.h $S/distr/map/imp.h $S/utils/cc.h $S/distr/flu/dev.h $S/distr/flu/imp/ini.h $S/distr/map/dev.h $S/rnd/imp.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/distr/flu/imp/unpack.h $S/distr/flu/imp/type.h $S/distr/flu/type.h $S/utils/kl.h $S/distr/flu/imp/pack.h $S/distr/flu/imp/map.h $S/distr/common/dev.h $S/clist/imp.h $S/frag/imp.h
$B/distr/rbc/imp.o: $S/distr/rbc/imp/fin.h $S/comm/imp.h $S/inc/conf.h $S/rbc/type.h $S/utils/error.h $S/distr/rbc/imp/com.h $S/comm/utils.h $S/mpi/basetags.h $S/inc/type.h $S/distr/map/type.h $S/distr/rbc/imp.h $S/inc/dev/common.h $S/distr/map/imp.h $S/inc/def.h $S/utils/cc.h $S/distr/rbc/dev.h $S/distr/rbc/imp/ini.h $S/distr/map/dev.h $S/algo/minmax.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/utils/texo.h $S/distr/rbc/imp/unpack.h $S/distr/rbc/type.h $S/utils/kl.h $S/distr/rbc/imp/pack.h $S/distr/rbc/imp/map.h $S/distr/common/dev.h $S/frag/imp.h $S/d/ker.h
$B/distr/rig/imp.o: $S/distr/rig/imp/fin.h $S/comm/imp.h $S/inc/conf.h $S/utils/error.h $S/distr/rig/imp/com.h $S/comm/utils.h $S/mpi/basetags.h $S/inc/type.h $S/distr/map/type.h $S/distr/rig/imp.h $S/inc/dev/common.h $S/distr/map/imp.h $S/inc/def.h $S/utils/cc.h $S/distr/rig/dev.h $S/distr/rig/imp/ini.h $S/distr/map/dev.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/rig/imp.h $S/distr/rig/imp/unpack.h $S/distr/rig/type.h $S/utils/kl.h $S/distr/rig/imp/pack.h $S/distr/rig/imp/map.h $S/distr/common/dev.h $S/frag/imp.h
$B/dpdr/imp.o: $S/inc/conf.h $S/dpdr/fin.h $S/inc/type.h $S/d/q.h $S/dpdr/dev/fill.h $S/dpdr/dev/common.h $S/dpdr/imp.h $S/mpi/wrapper.h $S/inc/dev/common.h $S/dpdr/buf.h $S/utils/cc.h $S/inc/def.h $S/mpi/type.h $S/utils/mc.h $S/inc/dev.h $S/rnd/imp.h $S/msg.h $S/d/api.h $B/conf.h $S/dpdr/ini.h $S/utils/kl.h $S/dpdr/type.h $S/frag/imp.h $S/dpdr/recv.h $S/mpi/glb.h $S/d/ker.h
$B/dpdr/int.o: $S/inc/conf.h $S/cloud/imp.h $S/inc/type.h $S/mpi/basetags.h $S/dpdr/imp.h $S/utils/cc.h $S/inc/def.h $S/inc/dev.h $S/rnd/imp.h $S/d/api.h $S/msg.h $B/conf.h $S/hforces/imp.h $S/dpdr/type.h $S/frag/imp.h $S/dpdr/int.h
$B/exch/flu/imp.o: $S/exch/flu/imp/fin.h $S/exch/map/dev.h $S/comm/imp.h $S/inc/conf.h $S/exch/flu/imp/com.h $S/exch/map/imp.h $S/exch/common/type.h $S/comm/utils.h $S/mpi/basetags.h $S/inc/type.h $S/exch/flu/imp.h $S/inc/dev/common.h $S/utils/cc.h $S/exch/flu/dev.h $S/exch/flu/imp/ini.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/exch/flu/imp/unpack.h $S/exch/flu/type.h $S/exch/map/type.h $S/utils/kl.h $S/exch/flu/imp/pack.h $S/exch/flu/imp/map.h $S/exch/common/dev.h $S/frag/imp.h
$B/exch/mesh/imp.o: $S/exch/mesh/imp/fin.h $S/exch/map/dev.h $S/comm/imp.h $S/inc/conf.h $S/utils/error.h $S/exch/mesh/imp/com.h $S/exch/map/imp.h $S/exch/common/type.h $S/comm/utils.h $S/mpi/basetags.h $S/inc/type.h $S/exch/mesh/imp.h $S/inc/dev/common.h $S/utils/cc.h $S/exch/mesh/dev.h $S/exch/mesh/imp/ini.h $S/algo/minmax.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/exch/mesh/imp/unpack.h $S/exch/mesh/type.h $S/exch/map/type.h $S/utils/kl.h $S/exch/mesh/imp/pack.h $S/exch/mesh/imp/map.h $S/exch/common/dev.h $S/frag/imp.h
$B/exch/obj/imp.o: $S/exch/obj/imp/fin.h $S/exch/map/dev.h $S/comm/imp.h $S/inc/conf.h $S/utils/error.h $S/exch/obj/imp/com.h $S/exch/map/imp.h $S/exch/common/type.h $S/comm/utils.h $S/mpi/basetags.h $S/inc/type.h $S/exch/obj/imp.h $S/inc/dev/common.h $S/utils/cc.h $S/exch/obj/dev.h $S/exch/obj/imp/ini.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/exch/obj/imp/unpack.h $S/exch/obj/type.h $S/exch/map/type.h $S/utils/kl.h $S/exch/obj/imp/pack.h $S/exch/obj/imp/map.h $S/exch/common/dev.h $S/frag/imp.h
$B/field.o: $S/inc/conf.h $S/io/field/imp.h $S/inc/type.h $S/msg.h $B/conf.h $S/field.h $S/mpi/glb.h
$B/flu/imp.o: $S/flu/imp/fin.h $S/algo/scan/int.h $S/inc/conf.h $S/inc/type.h $S/inter/color.h $S/io/restart.h $S/mpi/wrapper.h $S/flu/imp.h $S/utils/cc.h $S/inc/def.h $S/flu/dev.h $S/flu/imp/generate.h $S/flu/imp/ini.h $S/rnd/imp.h $S/utils/mc.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/utils/kl.h $S/clist/imp.h $S/flu/imp/start.h
$B/frag/imp.o: $S/inc/conf.h $B/conf.h $S/frag/imp.h
$B/fsi/imp.o: $S/sim/imp.h $S/fsi/imp/ini.h $S/utils/te.h $S/fsi/dev/map.common.h $S/cloud/imp.h $S/fsi/dev/pair.h $S/fsi/imp/halo.h $S/mpi/glb.h $S/fsi/dev/fetch.h $S/utils/texo.h $B/conf.h $S/d/api.h $S/fsi/imp.h $S/fsi/dev/map/bulk.h $S/fsi/dev/map/halo.h $S/fsi/imp/fin.h $S/inc/conf.h $S/fsi/type.h $S/inc/dev.h $S/dbg/imp.h $S/fsi/imp/setup.h $S/utils/mc.h $S/fsi/dev/type.h $S/forces/imp.h $S/d/q.h $S/inc/type.h $S/clist/imp.h $S/fsi/dev/decl.h $S/fsi/dev/bulk.h $S/utils/kl.h $S/fsi/dev/common.h $S/rnd/dev.h $S/fsi/imp/bind.h $S/utils/cc.h $S/fsi/dev/halo.h $S/forces/pack.h $S/cloud/dev.h $S/forces/type.h $S/algo/scan/int.h $S/inc/def.h $S/mpi/type.h $S/msg.h $S/d/ker.h $S/inc/dev/common.h $S/fsi/imp/decl.h $S/rnd/imp.h $S/fsi/imp/bulk.h $S/mpi/basetags.h $S/frag/imp.h
$B/generate/rig/imp.o: $S/mesh/props.h $S/inc/conf.h $S/inc/type.h $S/mpi/wrapper.h $S/generate/rig/imp/ini_props.h $S/generate/rig/imp.h $S/math/linal.h $S/generate/rig/imp/ic.h $S/utils/cc.h $S/inc/def.h $S/mpi/type.h $S/generate/rig/imp/ids.h $S/d/api.h $S/msg.h $S/utils/mc.h $B/conf.h $S/generate/rig/imp/main.h $S/generate/rig/imp/share.h $S/utils/texo.h $S/mesh/dist.h $S/mesh/bbox.h $S/mesh/collision.h $S/d/ker.h $S/mpi/glb.h
$B/glb/imp.o: $S/inc/conf.h $S/glb/imp/util.h $S/glb/set.h $S/glb/wvel/imp.h $S/d/api.h $B/conf.h $S/glb/imp/main.h $S/glb/imp/dec.h $S/glb/get.h $S/mpi/glb.h
$B/glb/wvel/imp.o: $S/inc/conf.h $S/glb/wvel/imp/flat.h $S/glb/wvel/imp.h $S/msg.h $B/conf.h $S/glb/wvel/imp/dupire/common.h $S/glb/wvel/imp/dupire/down.h $S/glb/wvel/imp/dupire/up.h $S/glb/wvel/imp/sin.h
$B/hforces/imp.o: $S/cloud/dev.h $S/inc/conf.h $S/hforces/dev.map.h $S/cloud/imp.h $S/forces/imp.h $S/inc/type.h $S/d/q.h $S/hforces/imp.h $S/inc/dev/common.h $S/rnd/dev.h $S/utils/cc.h $S/inc/def.h $S/hforces/dev.h $S/rnd/imp.h $S/inc/dev.h $S/msg.h $S/d/api.h $B/conf.h $S/forces/pack.h $S/utils/kl.h $S/forces/type.h $S/forces/use.h $S/mpi/glb.h $S/d/ker.h
$B/inter/imp.o: $S/algo/scan/int.h $S/inc/conf.h $S/rbc/type.h $S/flu/imp.h $S/cloud/imp.h $S/inc/type.h $S/mpi/basetags.h $S/d/q.h $S/dbg/imp.h $S/mpi/wrapper.h $S/inter/imp.h $S/inter/color.h $S/utils/cc.h $S/inc/def.h $S/wall/imp.h $S/mpi/type.h $S/inter/_korea/imp/color.h $S/sdf/type.h $S/rnd/imp.h $S/inc/dev.h $S/utils/mc.h $S/msg.h $S/d/api.h $B/conf.h $S/inter/imp/main.h $S/rig/imp.h $S/utils/texo.h $S/sdf/int.h $S/utils/kl.h $S/clist/imp.h $S/forces/type.h $S/frag/imp.h $S/mpi/glb.h $S/d/ker.h
$B/io/bop/imp.o: $S/utils/os.h $S/inc/conf.h $S/inc/type.h $S/mpi/wrapper.h $S/io/bop/imp.h $S/inc/def.h $S/mpi/type.h $S/msg.h $S/d/api.h $S/utils/mc.h $B/conf.h $S/mpi/glb.h
$B/io/com.o: $S/utils/os.h $S/inc/conf.h $S/utils/error.h $S/mpi/wrapper.h $S/msg.h $S/utils/mc.h $B/conf.h $S/mpi/glb.h
$B/io/diag.o: $S/inc/conf.h $S/inc/type.h $S/mpi/wrapper.h $S/msg.h $B/conf.h $S/io/diag.h $S/mpi/glb.h
$B/io/field/h5/imp.o: $S/mpi/wrapper.h $S/io/field/h5/imp.h $S/msg.h $S/mpi/glb.h
$B/io/field/imp.o: $S/io/field/imp/scalar.h $S/utils/os.h $S/inc/conf.h $S/io/field/xmf/imp.h $S/inc/type.h $S/mpi/wrapper.h $S/io/field/imp.h $S/io/field/h5/imp.h $S/io/field/imp/dump.h $S/utils/mc.h $B/conf.h $S/mpi/glb.h
$B/io/fields_grid.o: $S/inc/conf.h $S/inc/type.h $S/utils/cc.h $S/io/fields_grid.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/io/field/imp.h $S/io/fields_grid/solvent.h $S/io/fields_grid/all.h
$B/io/field/xmf/imp.o: $S/io/field/xmf/imp.h $S/mpi/glb.h
$B/io/mesh.o: $S/utils/os.h $S/inc/conf.h $S/io/mesh.h $S/inc/type.h $S/io/mesh/shift/edge.h $S/mpi/wrapper.h $S/utils/mc.h $B/conf.h $S/io/mesh/main.h $S/io/mesh/shift/center.h $S/mpi/glb.h
$B/io/off.o: $S/io/off/imp.h $S/io/off.h
$B/io/ply.o: $S/io/ply/ascii.h $S/inc/type.h $S/inc/def.h $S/io/ply.h $S/msg.h $S/io/ply/bin.h $S/io/ply/common.h
$B/io/restart.o: $S/inc/conf.h $S/inc/type.h $S/inc/def.h $S/msg.h $B/conf.h $S/io/restart.h $S/mpi/glb.h
$B/io/rig.o: $S/inc/conf.h $S/inc/type.h $B/conf.h
$B/lforces/imp.o: $S/inc/conf.h $S/lforces/imp/info.h $S/lforces/dev/merged.h $S/lforces/dev/core.h $S/lforces/dev/asm.h $S/lforces/dev/pack.h $S/lforces/dev/decl.h $S/forces/imp.h $S/inc/type.h $S/d/q.h $S/lforces/dev/fetch.h $S/lforces/imp.h $S/lforces/imp/setup.h $S/rnd/dev.h $S/utils/cc.h $S/inc/def.h $S/lforces/dev/tex.h $S/lforces/dev/dpd.h $S/inc/dev.h $S/rnd/imp.h $S/d/api.h $S/msg.h $B/conf.h $S/lforces/imp/main.h $S/lforces/imp/tex.h $S/forces/pack.h $S/lforces/transpose/imp.h $S/lforces/imp/type.h $S/utils/kl.h $S/cloud/lforces/int.h $S/cloud/lforces/get.h $S/lforces/dev/float.h $S/forces/type.h $S/d/ker.h
$B/lforces/transpose/imp.o: $S/inc/conf.h $S/inc/type.h $S/d/q.h $S/lforces/transpose/imp.h $S/utils/cc.h $S/inc/def.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/lforces/transpose/imp/main.h $S/utils/kl.h $S/lforces/transpose/dev/main.h $S/d/ker.h
$B/main.o: $S/inc/conf.h $S/glb/set.h $S/d/api.h $S/msg.h $B/conf.h $S/sim/imp.h $S/mpi/glb.h
$B/math/linal.o: $S/math/linal.h
$B/meshbb/imp.o: $S/meshbb/bbstates.h $S/inc/conf.h $S/meshbb/imp/find_collisions/log_root1.h $S/meshbb/dev/collect.h $S/inc/type.h $S/meshbb/dev/cubic_root/main.h $S/meshbb/imp.h $S/meshbb/dev/roots.h $S/utils/cc.h $S/meshbb/dev/cubic_root/log_root0.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/meshbb/imp/main.h $S/meshbb/type.h $S/utils/kl.h $S/meshbb/imp/find_collisions/log_root0.h $S/math/dev.h $S/meshbb/dev/intersection.h $S/meshbb/dev/utils.h $S/meshbb/dev/main.h $S/meshbb/dev/cubic_root/log_root1.h
$B/mesh/bbox.o: $S/inc/type.h $S/algo/minmax.h $S/mesh/bbox.h
$B/mesh/collision.o: $S/inc/conf.h $S/inc/type.h $S/d/q.h $S/utils/cc.h $S/inc/def.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/utils/texo.h $S/utils/kl.h $S/utils/texo.dev.h $S/mesh/collision.h $S/d/ker.h
$B/mesh/dist.o: $S/inc/type.h $S/mesh/dist.h
$B/mesh/props.o: $S/mesh/props.h $S/inc/type.h
$B/mpi/glb.o: $S/inc/conf.h $S/mpi/wrapper.h $S/utils/mc.h $B/conf.h $S/mpi/glb.h
$B/mpi/type.o: $S/inc/conf.h $S/inc/type.h $S/mpi/type.h $S/utils/mc.h
$B/mpi/wrapper.o: $S/mpi/wrapper.h
$B/msg.o: $S/msg.h $S/mpi/glb.h
$B/rbc/adj/imp.o: $S/rbc/adj/imp.h $S/rbc/adj/imp/ini.h $S/msg.h $S/rbc/adj/type.h $S/rbc/adj/imp/map.h
$B/rbc/com/imp.o: $S/rbc/com/imp/fin.h $S/inc/conf.h $S/rbc/com/imp/com.h $S/inc/type.h $S/rbc/com/imp.h $S/utils/cc.h $S/inc/def.h $S/rbc/com/imp/ini.h $S/d/api.h $S/msg.h $S/inc/dev.h $B/conf.h $S/utils/kl.h $S/math/dev.h $S/rbc/com/dev/main.h $S/d/ker.h
$B/rbc/force/area_volume/imp.o: $S/inc/conf.h $S/rbc/force/area_volume/imp.h $S/utils/cc.h $S/inc/def.h $S/d/api.h $S/msg.h $S/inc/dev.h $B/conf.h $S/rbc/force/area_volume/imp/main.h $S/utils/texo.h $S/utils/texo.dev.h $S/utils/kl.h $S/math/dev.h $S/rbc/force/area_volume/dev/main.h $S/d/ker.h
$B/rbc/force/imp.o: $S/rbc/force/imp/fin.h $S/rbc/force/params/lina.h $S/rbc/force/params/test.h $S/rbc/force/area_volume/imp.h $S/inc/conf.h $S/rbc/force/dev/common.h $S/rbc/type.h $S/inc/type.h $S/rbc/force/dev/shape.h $S/rbc/force/imp.h $S/utils/cc.h $S/inc/def.h $S/rbc/force/imp/ini.h $S/rbc/adj/dev.h $S/d/api.h $S/msg.h $S/inc/dev.h $B/conf.h $S/rbc/force/dev/force.h $S/utils/texo.h $S/rbc/force/imp/forces.h $S/utils/texo.dev.h $S/utils/te.h $S/utils/kl.h $S/math/dev.h $S/rbc/adj/type.h $S/rbc/force/dev/main.h $S/d/ker.h
$B/rbc/gen/imp.o: $S/inc/conf.h $S/inc/type.h $S/mpi/wrapper.h $S/rbc/gen/imp.h $S/io/off.h $S/inc/def.h $S/utils/mc.h $S/msg.h $B/conf.h $S/rbc/gen/imp/main.h $S/mpi/glb.h
$B/rbc/main/imp.o: $S/rbc/main/imp/fin.h $S/inc/conf.h $S/rbc/type.h $S/inc/type.h $S/rbc/main/imp/util.h $S/rbc/gen/imp.h $S/io/restart.h $S/mpi/wrapper.h $S/rbc/main/imp.h $S/io/off.h $S/rbc/main/imp/setup.h $S/utils/cc.h $S/inc/def.h $S/rbc/main/imp/generate.h $S/rbc/main/imp/ini.h $S/utils/mc.h $S/d/api.h $S/msg.h $S/inc/dev.h $B/conf.h $S/rbc/adj/imp.h $S/rbc/main/imp/start.h $S/rbc/adj/type.h $S/mpi/glb.h
$B/rbc/stretch/imp.o: $S/inc/conf.h $S/inc/type.h $S/rbc/stretch/imp/util.h $S/rbc/stretch/imp.h $S/utils/cc.h $S/inc/def.h $S/d/api.h $S/msg.h $S/inc/dev.h $B/conf.h $S/rbc/stretch/imp/main.h $S/rbc/stretch/imp/type.h $S/utils/kl.h $S/rbc/stretch/dev/main.h $S/d/ker.h
$B/restrain/imp.o: $S/inc/conf.h $S/restrain/dev/grey/map.h $S/restrain/imp/color/main.h $S/restrain/imp/main0.h $S/restrain/dev/main0.h $S/inc/type.h $S/d/q.h $S/restrain/imp.h $S/utils/cc.h $S/restrain/imp/grey/main.h $S/restrain/imp/common.h $S/restrain/dev/util.h $S/inc/dev.h $S/msg.h $S/d/api.h $B/conf.h $S/restrain/sum/imp.h $S/restrain/dev/color/map.h $S/utils/kl.h $S/restrain/dev/dec.h $S/restrain/stat/imp.h $S/restrain/dev/main.h $S/d/ker.h
$B/restrain/stat/imp.o: $S/restrain/stat/imp.h $S/restrain/stat/imp/main.h $S/restrain/stat/imp/dec.h
$B/restrain/sum/imp.o: $S/inc/conf.h $S/mpi/wrapper.h $S/restrain/sum/imp.h $S/utils/mc.h $B/conf.h $S/restrain/sum/imp/main.h
$B/rigid/imp.o: $S/inc/conf.h $S/inc/type.h $S/rigid/imp.h $S/utils/cc.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/rigid/imp/main.h $S/utils/kl.h $S/rigid/dev/utils.h $S/rigid/dev/main.h
$B/rig/imp.o: $S/rig/imp/fin.h $S/inc/conf.h $S/inc/type.h $S/io/restart.h $S/rig/imp.h $S/io/ply.h $S/utils/cc.h $S/inc/def.h $S/rig/imp/generate.h $S/rig/imp/common.h $S/rig/imp/ini.h $S/msg.h $S/inc/dev.h $S/d/api.h $B/conf.h $S/rig/imp/start.h $S/generate/rig/imp.h
$B/rnd/imp.o: $S/rnd/imp.h
$B/scheme/imp.o: $S/scheme/dev/force/double_poiseuille.h $S/scheme/dev/force/none.h $S/inc/conf.h $S/inc/type.h $S/d/q.h $S/scheme/dev/force/constant.h $S/mpi/wrapper.h $S/scheme/imp/vv.h $S/scheme/imp.h $S/utils/cc.h $S/inc/def.h $S/inc/dev.h $S/utils/mc.h $S/msg.h $S/d/api.h $B/conf.h $S/scheme/imp/main.h $S/scheme/imp/restrain/red_vel.h $S/scheme/dev/force/4roller.h $S/utils/kl.h $S/scheme/imp/euler.h $S/restrain/imp.h $S/glb/get.h $S/scheme/imp/restrain/rbc_vel.h $S/scheme/imp/restrain/none.h $S/scheme/dev/main.h $S/d/ker.h
$B/sdf/imp.o: $S/inc/conf.h $S/inc/dev/wvel.h $S/sdf/cheap.dev.h $S/inc/type.h $S/d/q.h $S/inc/macro.h $S/utils/cc.h $S/inc/def.h $S/sdf/type.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/sdf/imp.h $S/utils/kl.h $S/field.h $S/glb/get.h $S/sdf/dev.h $S/d/ker.h $S/mpi/glb.h
$B/sdf/int.o: $S/inc/conf.h $S/inc/type.h $S/inc/macro.h $S/utils/cc.h $S/inc/def.h $S/sdf/type.h $S/inc/dev.h $S/msg.h $S/d/api.h $B/conf.h $S/sdf/imp.h $S/sdf/int.h $S/mpi/glb.h $S/d/ker.h
$B/sim/imp.o: $S/rbc/stretch/imp.h $S/distr/map/type.h $S/scheme/imp.h $S/sim/imp/ini.h $S/io/bop/imp.h $S/utils/te.h $S/comm/imp.h $S/cloud/imp.h $S/flu/imp.h $S/mesh/collision.h $S/sim/imp/distr.h $S/io/com.h $S/lforces/imp.h $S/mpi/glb.h $S/sim/imp/step.h $S/sim/imp/main.h $S/rig/imp.h $S/utils/texo.h $S/rbc/main/imp.h $S/sim/imp/dec.h $S/exch/obj/type.h $B/conf.h $S/sim/imp.h $S/d/api.h $S/rbc/com/imp.h $S/io/restart.h $S/algo/minmax.h $S/inter/imp.h $S/rigid/imp.h $S/dpdr/int.h $S/distr/flu/imp.h $S/sim/imp/fin.h $S/sim/imp/force/imp.h $S/sim/imp/run.h $S/distr/rig/type.h $S/wall/imp.h $S/sdf/int.h $S/io/mesh.h $S/inc/conf.h $S/sim/imp/colors.h $S/io/rig.h $S/inc/dev.h $S/glb/set.h $S/dbg/imp.h $S/distr/rbc/type.h $S/sim/imp/dump.h $S/utils/mc.h $S/exch/obj/imp.h $S/sim/imp/force/common.h $S/sim/imp/vcont.h $S/inter/color.h $S/fsi/type.h $S/distr/rig/imp.h $S/inc/type.h $S/fsi/imp.h $S/clist/imp.h $S/distr/rbc/imp.h $S/rbc/force/imp.h $S/distr/flu/type.h $S/cnt/imp.h $S/sim/imp/force/dpd.h $S/utils/cc.h $S/exch/mesh/imp.h $S/exch/map/type.h $S/sim/imp/update.h $S/rbc/type.h $S/sim/imp/type.h $S/exch/mesh/type.h $S/io/diag.h $S/algo/scan/int.h $S/mesh/bbox.h $S/io/fields_grid.h $S/inc/def.h $S/vcontroller/imp.h $S/mpi/type.h $S/msg.h $S/dpdr/type.h $S/color/flux.h $S/d/ker.h $S/rnd/imp.h $S/sim/imp/force/objects.h $S/mpi/wrapper.h $S/mpi/basetags.h $S/meshbb/imp.h $S/sdf/type.h $S/utils/error.h
$B/utils/cc.o: $S/utils/cc/common.h $S/inc/conf.h $S/d/api.h $S/msg.h $B/conf.h
$B/utils/error.o: $S/msg.h $S/utils/error.h
$B/utils/mc.o: $S/inc/conf.h $S/utils/mc.h $B/conf.h
$B/utils/os.o: $S/utils/os.h
$B/vcontroller/imp.o: $S/inc/conf.h $S/inc/type.h $S/mpi/wrapper.h $S/vcontroller/imp.h $S/utils/cc.h $S/vcontroller/dev.h $S/utils/mc.h $S/d/api.h $S/inc/dev.h $S/msg.h $B/conf.h $S/utils/kl.h $S/math/dev.h $S/mpi/glb.h
$B/wall/exch/imp.o: $S/comm/imp.h $S/inc/conf.h $S/utils/error.h $S/inc/type.h $S/mpi/basetags.h $S/mpi/wrapper.h $S/msg.h $B/conf.h $S/frag/imp.h $S/mpi/glb.h
$B/wall/force/imp.o: $S/utils/te.h $S/cloud/imp.h $S/wall/force/imp/main.h $S/utils/texo.h $B/conf.h $S/d/api.h $S/wall/force/imp.h $S/wall/force/dev/main0.h $S/inc/macro.h $S/sdf/cheap.dev.h $S/sdf/int.h $S/inc/conf.h $S/inc/dev.h $S/forces/imp.h $S/wall/force/dev/fetch/color.h $S/wall/force/dev/fetch/grey.h $S/d/q.h $S/inc/type.h $S/clist/imp.h $S/wall/force/dev/map/use.h $S/utils/kl.h $S/wall/force/dev/map/ini.h $S/rnd/dev.h $S/forces/use.h $S/utils/cc.h $S/inc/dev/wvel.h $S/forces/pack.h $S/cloud/dev.h $S/forces/type.h $S/utils/texo.dev.h $S/wall/force/dev/main.h $S/algo/scan/int.h $S/inc/def.h $S/glb/get.h $S/msg.h $S/d/ker.h $S/rnd/imp.h $S/wall/force/dev/map/type.h $S/sdf/type.h
$B/wall/imp.o: $S/wall/imp/fin.h $S/wall/force/imp.h $S/wall/exch/imp.h $S/algo/scan/int.h $S/inc/conf.h $S/cloud/imp.h $S/sdf/cheap.dev.h $S/inc/type.h $S/d/q.h $S/io/restart.h $S/wall/imp/strt.h $S/wall/imp.h $S/inc/macro.h $S/utils/cc.h $S/inc/def.h $S/wall/dev.h $S/wall/imp/generate.h $S/wall/imp/ini.h $S/sdf/type.h $S/rnd/imp.h $S/inc/dev.h $S/d/api.h $S/msg.h $B/conf.h $S/wall/imp/force.h $S/wall/imp/main.h $S/utils/texo.h $S/sdf/int.h $S/utils/te.h $S/utils/kl.h $S/clist/imp.h $S/glb/get.h $S/forces/type.h $S/d/ker.h
