OBJ = obj/
SRC = src/
MAIN = Pool
GAME_PATH = $(SRC)Game/
GAME = Game
TABLE_PATH = $(SRC)Table/
TABLE = Table
BALL_PATH = $(SRC)Ball/
BALL = Ball
CUE_PATH = $(SRC)Cue/
CUE = Cue
SCORE_PATH = $(SRC)Score/
SCORE = Score
VECTOR_PATH = $(SRC)Vector_aux/
VECTOR = vector_operations
KEY = -std=c++11 -lm -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all : clean $(MAIN)
$(MAIN) : $(OBJ)$(MAIN).o $(OBJ)$(GAME).o $(OBJ)$(TABLE).o $(OBJ)$(BALL).o\
 $(OBJ)$(CUE).o $(OBJ)$(SCORE).o $(OBJ)$(VECTOR).o
	g++ -o $(MAIN) $(OBJ)$(MAIN).o $(OBJ)$(GAME).o $(OBJ)$(TABLE).o $(OBJ)$(BALL).o\
 $(OBJ)$(CUE).o $(OBJ)$(SCORE).o $(OBJ)$(VECTOR).o $(KEY)
$(OBJ)$(MAIN).o : $(SRC)$(MAIN).cpp
	g++ -o $(OBJ)$(MAIN).o -c $(SRC)$(MAIN).cpp $(KEY)
$(OBJ)$(GAME).o : $(GAME_PATH)$(GAME).cpp
	g++ -o $(OBJ)$(GAME).o -c $(GAME_PATH)$(GAME).cpp
$(OBJ)$(TABLE).o : $(TABLE_PATH)$(TABLE).cpp
	g++ -o $(OBJ)$(TABLE).o -c $(TABLE_PATH)$(TABLE).cpp
$(OBJ)$(BALL).o : $(BALL_PATH)$(BALL).cpp
	g++ -o $(OBJ)$(BALL).o -c $(BALL_PATH)$(BALL).cpp
$(OBJ)$(CUE).o : $(CUE_PATH)$(CUE).cpp
	g++ -o $(OBJ)$(CUE).o -c $(CUE_PATH)$(CUE).cpp
$(OBJ)$(SCORE).o : $(SCORE_PATH)$(SCORE).cpp
	g++ -o $(OBJ)$(SCORE).o -c $(SCORE_PATH)$(SCORE).cpp
$(OBJ)$(VECTOR).o : $(VECTOR_PATH)$(VECTOR).cpp
	g++ -o $(OBJ)$(VECTOR).o -c $(VECTOR_PATH)$(VECTOR).cpp
clean :
	rm -f $(OBJ)*.o *~ $(MAIN)