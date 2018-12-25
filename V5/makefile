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
	./virtualSocket ./tmp/p11 ./tmp/p12 &
	./virtualSocket ./tmp/p13 ./tmp/p14 &
	./virtualSocket ./tmp/p15 ./tmp/p16 &
	./virtualSocket ./tmp/p17 ./tmp/p18 &
	./virtualSocket ./tmp/p19 ./tmp/p20 &
redDown:
	pkill -f socat

redStart:
	./raspiRed Kali ./tmp/p1 > /dev/null &
	./raspiRed Raspbian ./tmp/p7 ./tmp/p3 > /dev/null &
	./raspiRed Ubuntu ./tmp/p2 ./tmp/p4 ./tmp/p5 ./tmp/p9 > /dev/null &
	./raspiRed Fedora ./tmp/p6 ./tmp/p11 > /dev/null &
	./raspiRed Mint ./tmp/p8 ./tmp/p13 > /dev/null &
	./raspiRed Debian ./tmp/p10 ./tmp/p14 ./tmp/p17 ./tmp/p15 > /dev/null &
	./raspiRed OpenSuse ./tmp/p12 ./tmp/p16 ./tmp/p19 > /dev/null &
	./raspiRed Puppy ./tmp/p18 ./tmp/p20
redTest:
	./raspiRed Raspbian ./tmp/p7 ./tmp/p3 > /dev/null &
	./raspiRed Ubuntu ./tmp/p2 ./tmp/p4 ./tmp/p5 ./tmp/p9 > /dev/null &
	./raspiRed Fedora ./tmp/p6 ./tmp/p11 > /dev/null &
	./raspiRed Mint ./tmp/p8 ./tmp/p13 > /dev/null &
	./raspiRed Debian ./tmp/p10 ./tmp/p14 ./tmp/p17 ./tmp/p15 > /dev/null &
	./raspiRed OpenSuse ./tmp/p12 ./tmp/p16 ./tmp/p19 > /dev/null &
redNodo1:
	./raspiRed Kali ./tmp/p1
redNodo2:
	./raspiRed Puppy ./tmp/p18 ./tmp/p20
redStop:
	pkill -f raspiRed
