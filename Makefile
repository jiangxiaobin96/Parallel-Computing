FC = mpif77
OPT = -O2
LIBPATH = ~/mympi/
MYLIB = mympi
LIBMYMPI = libmympi.a
LIBOBJS = mybegin.o myend.o ring.o datatype.o mpistruct.o diagonal.o rcmatmul.o proc2d.o \
          snglscan.o gemmv.o iteration.o gemmm.o cannon.o
CC = mpicc

mybegin.o: mybegin.c
	$(CC) -c $(OPT) mybegin.c
myend.o: myend.c
	$(CC) -c $(OPT) myend.c
ring.o: ring.c
	$(CC) -c $(OPT) ring.c
datatype.o: datatype.c
	$(CC) -c $(OPT) datatype.c
mpistruct.o: mpistruct.c
	$(CC) -c $(OPT) mpistruct.c
diagonal.o: diagonal.c
	$(CC) -c $(OPT) diagonal.c
rcmatmul.o: rcmatmul.c
	$(CC) -c $(OPT) rcmatmul.c
snglscan.o: snglscan.c
	$(CC) -c $(OPT) snglscan.c
proc2d.o: proc2d.c
	$(CC) -c $(OPT) proc2d.c
gemmv.o: gemmv.c
	$(CC) -c $(OPT) gemmv.c
iteration.o: iteration.c
	$(CC) -c $(OPT) iteration.c
gemmm.o: gemmm.c
	$(CC) -c $(OPT) gemmm.c
cannon.o: cannon.c
	$(CC) -c $(OPT) cannon.c
main.o: main.c
	$(CC) $(OPT) -c main.c

main: main.o
	$(CC) $(OPT) -o main main.o -lm ./libmympi.a

mylib: $(LIBOBJS)
	ar rcs $(LIBMYMPI) $(LIBOBJS)
clean:
	rm -f *.o *.a a.out main
