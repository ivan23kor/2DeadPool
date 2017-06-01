MAIN = Pool
GAME = Game
TABLE = Table
BALL = Ball
BILLIARD = Billiard
SCORE = Score
VECTOR = vector_operations
KEY = -std=c++11 -lm -lsfml-graphics -lsfml-window -lsfml-system

all : clean $(MAIN)
$(MAIN) : $(MAIN).o $(GAME).o $(TABLE).o $(BALL).o $(BILLIARD).o $(SCORE).o $(VECTOR).o
	g++ -o $(MAIN) $(MAIN).o $(GAME).o $(TABLE).o $(BALL).o $(BILLIARD).o $(SCORE).o $(VECTOR).o $(KEY)
$(MAIN).o : $(MAIN).cpp
	g++ -c $(MAIN).cpp $(KEY)
$(GAME).o : $(GAME).cpp
	g++ -c $(GAME).cpp
$(TABLE).o : $(TABLE).cpp
	g++ -c $(TABLE).cpp
$(BALL).o : $(BALL).cpp
	g++ -c $(BALL).cpp
$(BILLIARD).o : $(BILLIARD).cpp
	g++ -c $(BILLIARD).cpp
$(SCORE).o : $(SCORE).cpp
	g++ -c $(SCORE).cpp
$(VECTOR).o : $(VECTOR).cpp
	g++ -c $(VECTOR).cpp
clean :
	rm -f *.o *~ $(MAIN)