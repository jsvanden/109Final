# CS109 quarter project
# simple makefile, creates the executable "sri_phase2”
SOURCES		= KnowledgeBase.cpp RuleBase.cpp SRI.cpp Utility.cpp
HEADERS		= KnowledgeBase.hpp RuleBase.hpp SRI.hpp Utility.hpp
OBJECTS		= KnowledgeBase.o RuleBase.o SRI.o Utility.o

EXEBIN		= sri
MAINSOURCE	= main.cpp
MAINOBJECT	= main.o

TESTEXEBIN	= sri_test
TESTHEADER	= Catch.hpp
TESTSOURCE	= TestSRI.cpp
TESTOBJECT	= TestSRI.o

FLAGS		= -std=gnu++11
WARNING_FLAGS	= -std=gnu++11 -Wall



##########################

all : $(EXEBIN)

$(EXEBIN) : $(MAINOBJECT) $(OBJECTS) $(HEADERS)
	g++ -o $(EXEBIN) $(MAINOBJECT) $(OBJECTS)

$(MAINOBJECT) : $(MAINSOURCE)
	g++ -c $(WARNING_FLAGS) $(MAINSOURCE)

$(OBJECTS) : $(SOURCES) $(HEADERS)
	g++ -c $(WARNING_FLAGS) $(SOURCES)

run :
	make all
	./$(EXEBIN)

##########################

test : $(TESTOBJECT) $(OBJECTS) $(TESTHEADER) $(HEADERS)
	g++ -o $(TESTEXEBIN) $(TESTOBJECT) $(OBJECTS)

$(TESTOBJECT) : $(TESTSOURCE) $(TESTHEADER)
	g++ -c $(WARNING_FLAGS) $(TESTSOURCE)

testrun :
	make test
	./$(TESTEXEBIN)

##########################

clean :
	rm -f $(EXEBIN) $(TESTEXEBIN) $(OBJECTS) $(MAINOBJECT) $(TESTOBJECT)

clean_files :
	rm -f *.sri

##########################

check :
	valgrind --leak-check=full $(EXEBIN)
