include ../param.mk
include compil.mk

DEST=../$(PROG)
OBJS=main.o organ.o body.o menu.o games.o configEditor.o attacker.o cell.o mutedcell.o sanecell.o mediator.o

GAMESDIR=games/
GAMESOBJS=`./gamesobjs.sh $(GAMESDIR)`

GRAPHICSDIR=graphics/
GRAPHICSOBJS=`./graphicsobjs.sh $(GRAPHICSDIR)`

COREDIR=core/
COREOBJS=`./coreobjs.sh $(COREDIR)`

POTNAME=cancer
POTFILE=$(POTNAME).pot
PODIR=i18n/
POMK=$(PODIR)/pot.mk
POINSTDIR=/usr/share/locale/

all : $(DEST)

$(DEST) : $(OBJS)
	cd $(GAMESDIR) && make
	cd $(GAMESDIR) && make store-objs
	cd $(GRAPHICSDIR) && make
	cd $(GRAPHICSDIR) && make store-objs
	cd $(COREDIR) && make
	cd $(COREDIR) && make store-objs
	$(CC) $(CXXFLAGS)    -o $@	$^ $(GAMESOBJS) $(GRAPHICSOBJS) $(COREOBJS) $(LDFLAGS)

main.o : main.cpp
	$(CC) $(CXXFLAGS) -c -o $@	$<

%.o : %.cpp %.hpp
	$(CC) $(CXXFLAGS) -c -o $@	$<

clean :
	@touch $(DEST) $(OBJS)
	@rm -v $(DEST) $(OBJS)
	cd $(GAMESDIR) && make clean
	cd $(GRAPHICSDIR) && make clean
	cd $(COREDIR) && make clean

rec : clean all

po-init :
	mkdir -p $(PODIR)

pot-gen :
	@touch $(PODIR)/$(POTFILE)
	@rm -r $(PODIR)/$(POTFILE)
	xgettext -c -k_i -d cancer-game -s -o i18n/cancer.pot *.cpp $(GAMESDIR)/*.{c,h}pp $(GRAPHICSDIR)/*.{c,h}pp $(COREDIR)/*.{c,h}pp
	@touch $(POMK)
	echo -e 'NAME=$(POTNAME)' > $(POMK)
	echo -e 'DIR=$(POINSTDIR)' >> $(POMK)

.PHONY:rec clean all po-init pot-gen

