APP = Toy.exe
SOURCES = main.cpp lexer.cpp parser.cpp helper.cpp
OBJECTS = main.o lexer.o parser.o helper.o
CPPFLAGS = 
LINKER = 

$(APP): $(OBJECTS)
	g++ -o $(APP) $(OBJECTS) $(CPPFLAGS) $(LINKER)

%.o: %.cpp
	g++ -c $< $(CPPFLAGS)