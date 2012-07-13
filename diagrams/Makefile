DIAS    = $(wildcard *.dia)
PNGS    = ${DIAS:.dia=.png} 
	

all : $(PNGS)

%.png : %.dia
	dia -e $@ --size 1024x -t png $<
