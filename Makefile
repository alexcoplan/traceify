CXXFLAGS = -Wall 

BIN 	= bin/
SOURCE 	= src/
DEPS 	= colour image light viewport world ray vec3
SOURCES = $(addprefix $(SOURCE), $(addsuffix .cpp, $(DEPS)) )
OBJECTS = $(addprefix $(BIN),  $(addsuffix .o, $(DEPS)) )
EXEC 	= traceify
MAIN    = $(SOURCE)$(EXEC).cpp

all: $(SOURCES) $(EXEC)

debug: CXXFLAGS += -g -DDEBUG
debug: $(SOURCES) $(EXEC)

$(OBJECTS) : | $(BIN)

$(EXEC) : $(MAIN) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(MAIN) -o $@

# create the bin directory (if it doesn't exist)
$(BIN) :
	mkdir $(BIN)

$(BIN)%.o : $(SOURCE)%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@


clean :
	rm -f $(BIN)*

