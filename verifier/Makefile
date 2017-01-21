CC = g++-6
LEX = flex
LFLAGS = -8
YACC = bison
YFLAGS = -d
LDFLAGS=-lboost_graph
CFLAGS=-Wall -std=c++11 -g -gdwarf-2 -O2 
SOURCES=Automaton.cpp DBSchema.cpp Artifact.cpp Service.cpp Task.cpp Formula.cpp Verifier.cpp VASSState.cpp SpinVerifier.cpp LivenessProperty.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=has_synthetic
PARSER=has_real

all: $(SOURCES) $(EXECUTABLE) $(PARSER) ltl3ba
	    
$(EXECUTABLE): $(OBJECTS) main.cpp 
	    $(CC) $(LDFLAGS) $(OBJECTS) main.cpp $(CFLAGS) -o $@ -lfl 

$(PARSER):  $(OBJECTS) has.tab.c lex.yy.c
		$(CC) $(LDFLAGS) $(OBJECTS) has.tab.c lex.yy.c $(CFLAGS) -o $@ -lfl

.cpp.o:
	    $(CC) -c $(CFLAGS) $< -o $@

has.tab.c has.tab.h: has.y 
		${YACC} ${YFLAGS} has.y

lex.yy.c: has.l
		${LEX} $(LFLAGS) has.l

ltl3ba:
	    $(MAKE) -C ltl3ba-1.1.3/ 
clean:
	    rm lex* has.tab.* *.o has_real has_synthetic
