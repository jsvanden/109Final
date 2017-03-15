COMMON_SRC		= Utility.cpp
COMMON_HDR		= Utility.hpp
COMMON_OBJ		= Utility.o

CLIENT_EXE		= Client
CLIENT_SRC		= ClientSocket.cpp Client.cpp
CLIENT_HDR		= ClientSocket.hpp
CLIENT_OBJ		= ClientSocket.o Client.o
C_MAIN_SRC		= ClientMain.cpp
C_MAIN_OBJ		= ClientMain.o

SERVER_EXE		= Server
SERVER_SRC		= Server.cpp SRI.cpp KnowledgeBase.cpp RuleBase.cpp
SERVER_HDR		= Server.hpp SRI.hpp KnowledgeBase.hpp RuleBase.hpp
SERVER_OBJ		= Server.o SRI.o KnowledgeBase.o RuleBase.o
S_MAIN_SRC		= ServerMain.cpp
S_MAIN_OBJ		= ServerMain.o

TEST_EXE		= TestAll
TEST_SRC		= TestAll.cpp
TEST_HDR		= Catch.hpp
T_MAIN_OBJ		= TestAll.o

LINKING_FLAGS	= -pthread -lpthread
COMPILE_FLAGS	= -std=gnu++11 -Wall

####################################################################################
# Build Commands
####################################################################################

all :
	make client
	make server

client : $(C_MAIN_OBJ) $(CLIENT_OBJ) $(COMMON_OBJ) $(CLIENT_HDR) $(COMMON_HDR)
	g++ $(LINKING_FLAGS) -o $(CLIENT_EXE) $(C_MAIN_OBJ) $(CLIENT_OBJ) $(COMMON_OBJ)

server : $(S_MAIN_OBJ) $(SERVER_OBJ) $(COMMON_OBJ) $(SERVER_HDR) $(COMMON_HDR)
	g++ $(LINKING_FLAGS) -o $(SERVER_EXE) $(S_MAIN_OBJ) $(SERVER_OBJ) $(COMMON_OBJ)

test : $(T_MAIN_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ) $(COMMON_OBJ) $(CLIENT_HDR) $(COMMON_HDR) $(SERVER_HDR)
	g++ $(LINKING_FLAGS) -o $(TEST_EXE) $(T_MAIN_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ) $(COMMON_OBJ)

####################################################################################
# Clean Commands
####################################################################################

clean :
	rm -f $(CLIENT_EXE) $(SERVER_EXE) $(TEST_EXE)
	rm -f *.o

clean_files :
	rm -f *.sri

####################################################################################
# Helper Commands
####################################################################################

$(COMMON_OBJ) : $(COMMON_SRC) $(COMMON_HDR)
	g++ -c $(COMPILE_FLAGS) $(COMMON_SRC)

$(CLIENT_OBJ) : $(CLIENT_SRC) $(CLIENT_HDR)
	g++ -c $(COMPILE_FLAGS) $(CLIENT_SRC)

$(SERVER_OBJ) : $(SERVER_SRC) $(SERVER_HDR)
	g++ -c $(COMPILE_FLAGS) $(SERVER_SRC)

$(C_MAIN_OBJ) : $(C_MAIN_SRC)
	g++ -c $(COMPILE_FLAGS) $(C_MAIN_SRC)

$(S_MAIN_OBJ) : $(S_MAIN_SRC)
	g++ -c $(COMPILE_FLAGS) $(S_MAIN_SRC)

$(T_MAIN_OBJ) : $(TEST_SRC) $(TEST_HDR)
	g++ -c $(COMPILE_FLAGS) $(TEST_SRC)	