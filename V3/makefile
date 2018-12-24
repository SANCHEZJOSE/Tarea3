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
	./virtualSocket ./tmp/p21 ./tmp/p22 &
	./virtualSocket ./tmp/p23 ./tmp/p24 &
	./virtualSocket ./tmp/p25 ./tmp/p26 &
	./virtualSocket ./tmp/p27 ./tmp/p28 &
	./virtualSocket ./tmp/p29 ./tmp/p30 &
	./virtualSocket ./tmp/p31 ./tmp/p32 &
	./virtualSocket ./tmp/p33 ./tmp/p34 &
	./virtualSocket ./tmp/p35 ./tmp/p36 &
	./virtualSocket ./tmp/p37 ./tmp/p38 &
	./virtualSocket ./tmp/p39 ./tmp/p40 &

redDown:
	pkill -f socat

redStart:
	./raspiRed Kali ./tmp/p1 > /dev/null &
	./raspiRed Raspbian ./tmp/p7 ./tmp/p3 > /dev/null &
	./raspiRed Ubuntu ./tmp/p2 ./tmp/p4 ./tmp/p5 ./tmp/p9 ./tmp > /dev/null &
	./raspiRed Fedora ./tmp/p6 ./tmp/p11 > /dev/null &
	./raspiRed Mint ./tmp/p8 ./tmp/p13 > /dev/null &
	./raspiRed Debian ./tmp/p10 ./tmp/p14 ./tmp/p17 ./tmp/p15 > /dev/null &
	./raspiRed OpenSuse ./tmp/p12 ./tmp/p16 ./tmp/p19 > /dev/null &
	./raspiRed Elementary ./tmp/p21 ./tmp/p23 > /dev/null &
	./raspiRed Puppy ./tmp/p18 ./tmp/p22 ./tmp/p20 ./tmp/p25 > /dev/null &
	./raspiRed Arduino ./tmp/p24 ./tmp/p27 ./tmp/p39 > /dev/null &
	./raspiRed Manjaro ./tmp/p26 ./tmp/p28 ./tmp/p29 ./tmp/p31 > /dev/null &
	./raspiRed Raspberry ./tmp/p30 ./tmp/p33 > /dev/null &
	./raspiRed Camila ./tmp/p40 ./tmp/p37 > /dev/null &
	./raspiRed Jose ./tmp/p32 ./tmp/p35 ./tmp/p38 > /dev/null &
	./raspiRed Nico ./tmp/p36 ./tmp/p34
redStop:
	pkill -f raspiRed
