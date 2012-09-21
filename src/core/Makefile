include ../compil.mk

OBJS=exception.o config.o sounds.o

all : $(OBJS)

%.o : %.cpp %.hpp
	$(CC) $(CXXFLAGS) -c -o $@	$<

clean :
	@touch $(OBJS)
	@rm -v $(OBJS)

rec : clean all

objs :
	@echo $(OBJS)

store-objs :
	@echo $(OBJS) > objs

.PHONY:rec clean all objs store-objs


