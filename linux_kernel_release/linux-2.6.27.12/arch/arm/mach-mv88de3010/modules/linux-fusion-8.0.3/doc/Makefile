all: fusion.ps

fusion.ps: fusion.dvi
	dvips fusion.dvi

fusion.dvi: fusion.texi
	texi2dvi fusion.texi

clean:
	rm -f fusion.aux fusion.cp fusion.fn fusion.fns fusion.ky fusion.log fusion.pg fusion.toc fusion.tp fusion.vr

extra-clean: clean
	rm -f fusion.dvi fusion.ps

.PHONY: clean extra-clean
