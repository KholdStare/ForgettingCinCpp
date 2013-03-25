all: beamer_presentation.pdf raii.pdf

diagrams:
	$(MAKE) -C diagrams

beamer_presentation.pdf : beamer_presentation.tex diagrams
	pdflatex beamer_presentation.tex

raii.pdf : raii/raii.tex
	pdflatex raii/raii.tex
