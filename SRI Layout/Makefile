# CS109 quarter project
# simple makefile, creates the executable "sri_test"
SOURCES		= *.cpp
HEADERS		= *.hpp
EXEBIN		= sri_test
FLAGS		= -std=gnu++11
WARNING_FLAGS	= -std=gnu++11 -Wall

all : $(EXEBIN)

$(EXEBIN) : $(HEADERS) $(SOURCES)
	g++ $(WARNING_FLAGS) -o $(EXEBIN) $(SOURCES)

clean :
	rm -f $(EXEBIN)

clean_files :
	rm -f *.sri

check :
	valgrind --leak-check=full $(EXEBIN)
