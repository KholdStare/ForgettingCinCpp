DIAS    = $(wildcard *.dia)
PNGS    = ${DIAS:.dia=.png} 
TEXS    = ${DIAS:.dia=.tex} 
PDFS    = ${DIAS:.dia=.pdf} 
	
all : pdf

png : $(PNGS)

%.png : %.dia
	dia -e $@ --size 2046x -t png-libart $<

pdf : $(PDFS)

%.pdf : %.dia
	dia -e $@ -t pdf $<

tex : $(TEXS)

%.tex : %.dia
	dia -e $@ -t tex $<

clean :
	rm -f $(PNGS)
	rm -f $(TEXS)
	rm -f $(PDFS)

