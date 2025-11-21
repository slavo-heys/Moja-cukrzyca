CC := g++
CFLAGS := -std=c++17 -O2 -Wall -Wextra
SRC := start.cpp
OUT := start

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT) report.txt report.pdf

.PHONY: all run clean

# Generate XLSX report from CSV
xlsx:
	python3 export_xlsx.py

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
SRC = start.cpp
TARGET = start

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) report.txt report.pdf
