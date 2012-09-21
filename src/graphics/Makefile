include ../compil.mk

OBJS=button.o anim.o deleterContainer.o gui.o checkBox.o icon.o listModel.o selectListener.o list.o radioButton.o slider.o focusListener.o textField.o textBox.o fileexplorer.o dropDown.o tabbedArea.o theme.o

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


