NAME=cdc
DEST=pdf
ifeq ($(DEST),dvi)
	VIEWER=xdvi
else
	VIEWER=zathura
endif

all : $(NAME).$(DEST)

$(NAME).dvi : $(NAME).tex
	latex $<

$(NAME).ps : $(NAME).dvi
	dvips $(NAME).dvi

$(NAME).pdf : $(NAME).tex
	pdflatex $<

view : all
	$(VIEWER) $(NAME).$(DEST) > /dev/null 2>&1

clean :
	@touch $(NAME).$(DEST)
	@rm -v `ls --ignore='*.tex' --ignore=Makefile`

delete :
	@touch $(NAME).$(DEST)
	@rm -v $(NAME).$(DEST)

rec : clean all

re : delete all

.PHONY:all dvi ps pdf view clean all delete re

