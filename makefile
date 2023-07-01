CFLAGS = clang++ -std=c++20 `pkg-config --libs --cflags sdl2` `pkg-config --libs --cflags sdl2_image` `pkg-config --libs --cflags sdl2_ttf`

windy: main.o functions.o
	$(CFLAGS) main.o functions.o -o windy 

main.o: main.cpp
	$(CFLAGS) -c main.cpp

functions.o: functions.cpp
	$(CFLAGS) -c functions.cpp

clean:
	rm *.o 
