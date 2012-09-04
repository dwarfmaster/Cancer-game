include ../param.mk
include compil.mk

DEST=../$(PROG)
OBJS=main.o config.o cell.o organ.o body.o menu.o games.o exception.o

GAMESDIR=games/
GAMESOBJS=`./gamesobjs.sh $(GAMESDIR)`

GRAPHICSDIR=graphics/
GRAPHICSOBJS=`./graphicsobjs.sh $(GRAPHICSDIR)`

all : $(DEST)

$(DEST) : $(OBJS)
	cd $(GAMESDIR) && make
	cd $(GAMESDIR) && make store-objs
	cd $(GRAPHICSDIR) && make
	cd $(GRAPHICSDIR) && make store-objs
	$(CC) $(CXXFLAGS)    -o $@	$^ $(GAMESOBJS) $(GRAPHICSOBJS) $(LDFLAGS)

main.o : main.cpp
	$(CC) $(CXXFLAGS) -c -o $@	$<

%.o : %.cpp %.hpp
	$(CC) $(CXXFLAGS) -c -o $@	$<

clean :
	@touch $(DEST) $(OBJS)
	@rm -v $(DEST) $(OBJS)
	cd $(GAMESDIR) && make clean

rec : clean all

.PHONY:rec clean all

