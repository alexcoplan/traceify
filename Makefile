CXXFLAGS = -Wall 

BIN 	= bin/
SOURCE 	= src/
DEPS 	= colour image light viewport ray vec3 geometry world material
SOURCES = $(addprefix $(SOURCE), $(addsuffix .cpp, $(DEPS)) )
OBJECTS = $(addprefix $(BIN),  $(addsuffix .o, $(DEPS)) )
EXEC 	= traceify
MAIN    = $(SOURCE)$(EXEC).cpp

all: $(SOURCES) $(EXEC)

debug: CXXFLAGS += -g -DDEBUG
debug: $(SOURCES) $(EXEC)

# these two rules are specific to an
# OS X build environment
render: all
	./$(EXEC)
	killall ToyViewer
	open render.ppm

rerender: all
	mv render.ppm old_render.ppm
	./$(EXEC)
	killall ToyViewer
	open render.ppm old_render.ppm


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

