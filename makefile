CXX = g++
CXXFLAGS = -O3 -Wall -pthread
TARGET = varredor

all:
	$(CXX) $(CXXFLAGS) main.cpp executar_trabalho.cpp calcular_passos.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)