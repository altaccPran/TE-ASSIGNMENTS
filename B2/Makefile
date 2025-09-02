CXX = g++-11
CXXFLAGS = -pthread
TARGET = main
SOURCES = main.cpp pass1.cpp pass2.cpp tokenizer.cpp symbol_table.cpp literal_table.cpp mnemonic_table.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
