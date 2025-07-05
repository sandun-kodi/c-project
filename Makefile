CC = gcc
LDFLAGS = -lm

# Clock build
CLOCK_TARGET = clock.exe
CLOCK_SRC = main.c canvas.c
CLOCK_PGM = clock.pgm

# Cube build
CUBE_TARGET = cube.exe
CUBE_SRC = test_math.c canvas.c math3d.c
CUBE_GIF = cube_rotation.gif

all: $(CLOCK_TARGET) $(CUBE_TARGET)
	./$(CLOCK_TARGET)
	-display -geometry +0+0 $(CLOCK_PGM)
	./$(CUBE_TARGET)
	-convert -delay 15 -loop 0 frames/cube_*.pgm $(CUBE_GIF)
	-explorer.exe $(CUBE_GIF)

$(CLOCK_TARGET): $(CLOCK_SRC) canvas.h
	$(CC) $(CLOCK_SRC) $(LDFLAGS) -o $(CLOCK_TARGET)

$(CUBE_TARGET): $(CUBE_SRC) canvas.h math3d.h
	$(CC) $(CUBE_SRC) $(LDFLAGS) -o $(CUBE_TARGET)

clean:
	rm -f $(CLOCK_TARGET) $(CUBE_TARGET) $(CLOCK_PGM) $(CUBE_PGM)
