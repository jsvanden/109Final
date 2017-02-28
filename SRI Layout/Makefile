# CS109 quarter project
# simple makefile, creates the executable "sri_test"
SOURCES		= KnowledgeBase.cpp RuleBase.cpp SRI.cpp Utility.cpp main.cpp
HEADERS		= KnowledgeBase.hpp RuleBase.hpp SRI.hpp Utility.hpp
OBJECTS		= KnowledgeBase.o RuleBase.o SRI.o Utility.o main.o
EXEBIN		= sri_test
FLAGS		= -std=gnu++11
WARNING_FLAGS	= -std=gnu++11 -Wall

all : $(EXEBIN)

$(EXEBIN) : $(OBJECTS) $(HEADERS)
	g++ -o $(EXEBIN) $(OBJECTS)

$(OBJECTS) : $(SOURCES) $(HEADERS)
	g++ -c $(WARNING_FLAGS) $(SOURCES)

clean :
	rm -f $(EXEBIN) $(OBJECTS)

clean_files :
	rm -f *.sri

check :
	valgrind --leak-check=full $(EXEBIN)
