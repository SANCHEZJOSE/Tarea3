all:	
	g++ -o serial.o serial.cpp -Wall -c -g
	g++ -o empaquetado.o empaquetado.cpp -Wall -c -g
	g++ -o funciones.o funciones.cpp -Wall -c -g
	g++ -o anillo.o anillo.cpp -Wall -c -g
	g++ -Wall -g clock.cpp -lwiringPi -o clock
	g++ -Wall -g funciones.o anillo.o serial.o empaquetado.o -lwiringPi -o anillo

clean:
	rm *.o anillo clock