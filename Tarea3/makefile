all:	
	g++ -o serial.o serial.cpp -Wall -c -g
	g++ -o funciones.o funciones.cpp -Wall -c -g
	g++ -o raspiRed.o raspiRed.cpp -Wall -c -g
	g++ -Wall -g funciones.o raspiRed.o serial.o -o raspiRed

clean:
	rm *.o raspiRed virtualSocket
redUp:
	g++ -Wall -g virtualSocket.cpp -o virtualSocket
	./virtualSocket ./tmp/p1 ./tmp/p2 &
	./virtualSocket ./tmp/p3 ./tmp/p4 &
	./virtualSocket ./tmp/p5 ./tmp/p6 &
	./virtualSocket ./tmp/p7 ./tmp/p8 &
	./virtualSocket ./tmp/p9 ./tmp/p10 &
	./virtualSocket ./tmp/p11 &
redDown:
	pkill -f socat
redStart:
	./raspiRed Nodo1 ./tmp/p1 > /dev/null &
	./raspiRed Nodo2 ./tmp/p2 ./tmp/p4 > /dev/null &
	./raspiRed Nodo3 ./tmp/p2 ./tmp/p3 ./tmp/p5 > /dev/null &
	./raspiRed Nodo4 ./tmp/p3 ./tmp/p6 > /dev/null &
	./raspiRed Nodo5 ./tmp/p4 ./tmp/p7 > /dev/null &
	./raspiRed Nodo6 ./tmp/p5 ./tmp/p7 ./tmp/p8 ./tmp/p9 > /dev/null &
	./raspiRed Nodo7 ./tmp/p6 ./tmp/p8 ./tmp/p10 > /dev/null &
	./raspiRed Nodo8 ./tmp/p9 ./tmp/p11 ./tmp/p10 > /dev/null &
	./raspiRed Nodo9 ./tmp/p11 > /dev/null &
redStop:
	pkill -f raspiRedk
