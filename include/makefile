EXE = pool
BALL = ball
TABLE = table
BILLIARD = billiard
SCORE = score
VECTOR = vector_operations
KEY = -std=c++11 -lm -lsfml-graphics -lsfml-window -lsfml-system

all : clean $(EXE)
$(EXE) : $(EXE).o $(BALL).o $(TABLE).o $(BILLIARD).o $(SCORE).o $(VECTOR).o
	g++ -o $(EXE) $(EXE).o $(BALL).o $(TABLE).o $(BILLIARD).o $(SCORE).o $(VECTOR).o $(KEY)
$(EXE).o : $(EXE).cpp
	g++ -c $(EXE).cpp $(KEY)
$(BALL).o : $(BALL).cpp
	g++ -c $(BALL).cpp
$(TABLE).o : $(TABLE).cpp
	g++ -c $(TABLE).cpp
$(BILLIARD).o : $(BILLIARD).cpp
	g++ -c $(BILLIARD).cpp
$(SCORE).o : $(SCORE).cpp
	g++ -c $(SCORE).cpp
$(VECTOR).o : $(VECTOR).cpp
	g++ -c $(VECTOR).cpp
clean :
	rm -f *.o *~ $(EXE)