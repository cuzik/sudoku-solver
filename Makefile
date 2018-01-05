CC=g++
override CFLAGS+=-DIL_STD -c -Wall -g --std=c++11
override CFLAGSL+= -lallegro -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro_image -lallegro_dialog -lallegro_acodec -lallegro_audio -pthread -O3 -lm

override LDFLAGS+=

SOURCES=							\
  main.cpp						\
  board.cpp

OBJECTS=$(SOURCES:.cpp=.o)

#@echo "Objetos Compilados com sucesso"

EXECUTABLE= hocus_pocus

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(CFLAGSL)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(CFLAGSL)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
