all: beamer_presentation.pdf raii.pdf

diagrams:
	$(MAKE) -C diagrams

beamer_presentation.pdf : beamer_presentation.tex diagrams preamble.tex
	pdflatex beamer_presentation.tex

raii.pdf : raii/raii.tex preamble.tex
	pdflatex raii/raii.tex
