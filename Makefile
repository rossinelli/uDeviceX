C = cmd cmd/argp cmd/par cmd/u cmd/build cmd/case poc/doc/convert/adoc2html poc/ply2vtk
D = tools cmd post/punto post/strt post/wall pre/placement pre/units pre/stretch post/ply/cmd post/rbc post/data conf

install:
	install0 () ( cd "$$d" && make install); \
	for d in $D $C; do install0; done

.PHONY: install
