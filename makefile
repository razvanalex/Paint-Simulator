build: paint_simulator

paint_simulator: paint_simulator.c
	gcc paint_simulator.c -o paint_simulator -Wall

run:
	./paint_simulator

clean:
	rm -f paint_simulator
