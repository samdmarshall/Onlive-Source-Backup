all: libfusion.ps fusion.pdf

# common

clean:
	rm -rf libfusion.t2d

extra-clean: clean
	rm -f libfusion.dvi libfusion.ps fusion.pdf

.PHONY: clean extra-clean

# for libfusion

libfusion.ps: libfusion.dvi
	dvips libfusion.dvi

libfusion.dvi: libfusion.texi
	texi2dvi --tidy libfusion.texi

# for fusion.tmpl

%.xml: %.tmpl
	#./docproc doc $< >$@
	cp $< $@

%.pdf : %.xml
	db2pdf -o $(dir $@) $<

