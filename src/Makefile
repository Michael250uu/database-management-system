

main: main.o Process.o File.o Analysis.o
	g++ -o main main.o Process.o File.o Analysis.o -g

main.o: main.cc 
	g++ -c -o main.o main.cc 
Process.o: Process.cc 
	g++ -c -o Process.o Process.cc 
File.o: File.cc 
	g++ -c -o File.o File.cc
Analysis.o: Analysis.cc
	g++ -c -o Analysis.o Analysis.cc

clean: 
	rm *.o main 