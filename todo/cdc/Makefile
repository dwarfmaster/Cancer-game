NAME=cdc
DEST=pdf
ifeq ($(DEST),dvi)
	VIEWER=xdvi
else
	VIEWER=evince
endif

all : $(DEST) $(DEST)

dvi : $(NAME).tex
	latex $<

ps : dvi
	dvips $(NAME).dvi

pdf : $(NAME).tex
	pdflatex $<

view : all
	$(VIEWER) $(NAME).$(DEST)

clean :
	@touch $(NAME).$(DEST)
	@rm -v `ls --ignore='*.tex' --ignore=Makefile`

rec : clean all

.PHONY:all dvi ps pdf view clean all

