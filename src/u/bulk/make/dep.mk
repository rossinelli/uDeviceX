$B/algo/scan/imp.o: $S/algo/scan/cpu/imp.h $S/algo/scan/cpu/type.h $S/algo/scan/cuda/imp.h $S/algo/scan/cuda/type.h $S/algo/scan/dev.h $S/algo/scan/imp.h $B/conf.h $S/d/api.h $S/d/ker.h $S/d/q.h $S/inc/conf.h $S/inc/def.h $S/inc/dev.h $S/utils/cc.h $S/utils/error.h $S/utils/imp.h $S/utils/kl.h
$B/clist/imp.o: $S/algo/scan/imp.h $S/clist/dev.h $S/clist/dev/main.h $S/clist/imp.h $S/clist/imp/fin.h $S/clist/imp/ini.h $S/clist/imp/main.h $S/clist/imp/type.h $B/conf.h $S/d/api.h $S/inc/conf.h $S/inc/def.h $S/inc/dev.h $S/inc/type.h $S/partlist/dev.h $S/partlist/type.h $S/utils/cc.h $S/utils/error.h $S/utils/imp.h $S/utils/kl.h $S/utils/msg.h
$B/coords/imp.o: $B/conf.h $S/coords/imp.h $S/coords/imp/main.h $S/coords/imp/type.h $S/coords/ini.h $S/coords/type.h $S/inc/conf.h $S/mpi/wrapper.h $S/utils/error.h $S/utils/imp.h $S/utils/mc.h
$B/d/api.o: $B/conf.h $S/d/api.h $S/d/common.h $S/d/cpu/imp.h $S/d/cuda/imp.h $S/inc/conf.h $S/utils/error.h $S/utils/imp.h
$B/fluforces/bulk/imp.o: $S/cloud/lforces/get.h $S/cloud/lforces/int.h $B/conf.h $S/d/api.h $S/d/ker.h $S/d/q.h $S/fluforces/bulk/dev/asm.h $S/fluforces/bulk/dev/core.h $S/fluforces/bulk/dev/decl.h $S/fluforces/bulk/dev/dpd.h $S/fluforces/bulk/dev/fetch.h $S/fluforces/bulk/dev/float.h $S/fluforces/bulk/dev/merged.h $S/fluforces/bulk/dev/pack.h $S/fluforces/bulk/dev/tex.h $S/fluforces/bulk/imp.h $S/fluforces/bulk/imp/info.h $S/fluforces/bulk/imp/main.h $S/fluforces/bulk/imp/setup.h $S/fluforces/bulk/imp/tex.h $S/fluforces/bulk/imp/type.h $S/fluforces/bulk/transpose/imp.h $S/forces/imp.h $S/forces/pack.h $S/forces/type.h $S/inc/conf.h $S/inc/def.h $S/inc/dev.h $S/inc/type.h $S/math/rnd/dev.h $S/math/rnd/imp.h $S/utils/cc.h $S/utils/error.h $S/utils/kl.h $S/utils/msg.h
$B/fluforces/bulk/transpose/imp.o: $B/conf.h $S/d/api.h $S/d/ker.h $S/d/q.h $S/fluforces/bulk/transpose/dev/main.h $S/fluforces/bulk/transpose/imp.h $S/fluforces/bulk/transpose/imp/main.h $S/inc/conf.h $S/inc/def.h $S/inc/dev.h $S/inc/type.h $S/utils/cc.h $S/utils/kl.h $S/utils/msg.h
$B/fluforces/halo/imp.o: $S/cloud/dev.h $S/cloud/imp.h $B/conf.h $S/d/api.h $S/d/ker.h $S/d/q.h $S/flu/type.h $S/fluforces/halo/dev/dbg.h $S/fluforces/halo/dev/main.h $S/fluforces/halo/dev/map.h $S/fluforces/halo/imp.h $S/fluforces/halo/imp/main.h $S/forces/imp.h $S/forces/pack.h $S/forces/type.h $S/forces/use.h $S/frag/dev.h $S/frag/imp.h $S/inc/conf.h $S/inc/def.h $S/inc/dev.h $S/inc/type.h $S/math/rnd/dev.h $S/mpi/glb.h $S/utils/cc.h $S/utils/kl.h
$B/fluforces/imp.o: $S/cloud/imp.h $B/conf.h $S/d/api.h $S/flu/type.h $S/fluforces/bulk/imp.h $S/fluforces/dev/main.h $S/fluforces/halo/imp.h $S/fluforces/imp.h $S/fluforces/imp/fin.h $S/fluforces/imp/ini.h $S/fluforces/imp/main.h $S/fluforces/imp/type.h $S/fluforces/xbulk/imp.h $S/frag/imp.h $S/inc/conf.h $S/inc/dev.h $S/inc/type.h $S/math/rnd/dev.h $S/math/rnd/imp.h $S/mpi/glb.h $S/mpi/wrapper.h $S/utils/cc.h $S/utils/error.h $S/utils/imp.h $S/utils/kl.h $S/utils/mc.h $S/utils/msg.h
$B/fluforces/xbulk/imp.o: $B/conf.h $S/d/api.h $S/fluforces/xbulk/dev/main.h $S/fluforces/xbulk/imp.h $S/fluforces/xbulk/imp/main.h $S/forces/imp.h $S/forces/pack.h $S/forces/type.h $S/inc/conf.h $S/inc/def.h $S/inc/dev.h $S/inc/type.h $S/math/rnd/dev.h $S/math/rnd/imp.h $S/utils/cc.h $S/utils/kl.h
$B/frag/imp.o: $B/conf.h $S/frag/imp.h $S/inc/conf.h
$B/io/txt/imp.o: $S/inc/type.h $S/io/txt/imp.h $S/io/txt/imp/dump.h $S/io/txt/imp/read.h $S/io/txt/imp/type.h $S/utils/error.h $S/utils/imp.h $S/utils/msg.h
$B/math/rnd/imp.o: $S/math/rnd/imp.h $S/utils/error.h $S/utils/imp.h
$B/mpi/glb.o: $B/conf.h $S/inc/conf.h $S/mpi/glb.h $S/mpi/wrapper.h $S/utils/mc.h
$B/mpi/type.o: $S/inc/conf.h $S/inc/type.h $S/mpi/type.h $S/utils/mc.h
$B/mpi/wrapper.o: $S/mpi/wrapper.h
$B/parser/imp.o: $S/parser/imp.h $S/utils/error.h $S/utils/imp.h $S/utils/msg.h
$B/u/bulk/main.o: $S/clist/imp.h $S/cloud/imp.h $B/conf.h $S/d/api.h $S/flu/type.h $S/fluforces/imp.h $S/inc/conf.h $S/inc/dev.h $S/inc/type.h $S/io/txt/imp.h $S/mpi/glb.h $S/parser/imp.h $S/partlist/type.h $S/utils/cc.h $S/utils/error.h $S/utils/imp.h $S/utils/msg.h
$B/utils/cc.o: $B/conf.h $S/d/api.h $S/inc/conf.h $S/utils/cc/common.h $S/utils/error.h
$B/utils/error.o: $S/utils/error.h $S/utils/msg.h
$B/utils/imp.o: $S/utils/error.h $S/utils/imp.h
$B/utils/mc.o: $B/conf.h $S/inc/conf.h $S/mpi/wrapper.h $S/utils/error.h $S/utils/mc.h
$B/utils/msg.o: $S/utils/msg.h
$B/utils/os.o: $S/utils/error.h $S/utils/msg.h $S/utils/os.h