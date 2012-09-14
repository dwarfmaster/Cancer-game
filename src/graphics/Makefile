include ../compil.mk

OBJS=button.o anim.o internalContainer.o gui.o checkBox.o icon.o listModel.o

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
	@echo $(OBJS) > /tmp/gobjs

.PHONY:rec clean all objs store-objs


