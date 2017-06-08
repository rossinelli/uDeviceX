#### Couette pinned sphere
# nTEST: solid.t1
# export PATH=../tools:$PATH
# rm -rf h5 bop diag.txt solid_diag.txt
# cp sdf/wall1/wall.dat sdf.dat
# echo -e "16 16 16" > ic_solid.txt
# cp data/sphere.ply mesh_solid.ply
# :
# argp .conf.test.h                                                \
#   -tend=2.01 -part_freq=1000 -walls -wall_creation=1 \
#   -field_dumps -part_dumps -field_freq=1000          \
#   -solids -sbounce_back -pin_com -dt=1e-3 -shear_z > .conf.h
# :
# (make clean && make -j && make -C ../tools) > /dev/null
# ./udx
# avg_h5.m h5/flowfields-0001.h5 | sed -n '4,29p' | uscale 0.5 > h5.out.txt

#### Double poiseuille non pinned sphere
# nTEST: solid.t2
# export PATH=../tools:$PATH
# rm -rf h5 bop diag.txt solid_diag.txt
# echo -e "8 16 8" > ic_solid.txt
# cp data/sphere.ply mesh_solid.ply
# :
# argp .conf.double.poiseuille.h      \
#   -solids -sbounce_back             \
#   -tend=0.5 -part_freq=100     \
#   -pushflow -doublepoiseuille    \
#   -field_dumps -part_dumps      \
#   -wall_creation=0           \
#   -field_freq=100 > .conf.h
# :
# (make clean && make -j && make -C ../tools) > /dev/null
# ./udx
# bop2txt bop/solid-00004.bop | awk '{print $1, $2}' | uscale 10 > bop.out.txt
#

#### Couette pinned cylinder
# nTEST: solid.t3
# export PATH=../tools:$PATH
# rm -rf h5 bop diag.txt solid_diag.txt
# cp sdf/yplates1/yplates.dat sdf.dat
# echo -e "16 16 16" > ic_solid.txt
# cp data/cyl.ply mesh_solid.ply
# :
# argp .conf.test.h  \
#   -tend=2.01 -part_freq=1000 -walls -wall_creation=100 \
#   -field_dumps -part_dumps -field_freq=1000            \
#   -solids -spdir=2 -sbounce_back -dt=1e-3 -shear_y > .conf.h
# :
# (make clean && make -j && make -C ../tools) > /dev/null
# ./udx
# avg_h5.m h5/flowfields-0001.h5 | sed -n '4,29p' | uscale 0.5 > h5.out.txt

#### two solids around cylinder with one solid removed by the wall
# nTEST: solid.t4
# export PATH=../tools:$PATH
# rm -rf h5 bop diag.txt solid_diag.txt
# cp sdf/cyl1/cyl.dat sdf.dat
# cp data/sphere_R1.ply mesh_solid.ply
# x=0.75 y=3 z=9; echo $x $y $z >  ic_solid.txt
# x=8    y=8 z=8; echo $x $y $z >> ic_solid.txt
# :
# argp .conf.around.h -solids -tend=1.0 -part_freq=500 \
#     -walls -wall_creation=100 -kBT=1e-7              \
#     -field_dumps -part_dumps                         \
#     -field_freq=500 -pushflow > .conf.h
# :
# { make clean && make -j && make -C ../tools; } > /dev/null
# ./udx
# bop2txt bop/solid-00001.bop | awk '{print $1, $2}' | uscale 10 > bop.out.txt
