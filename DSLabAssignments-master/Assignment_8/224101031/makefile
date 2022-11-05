output: main.o myQueue.o myStack.o myUGraph.o
	gcc main.o myStack.o myQueue.o myUGraph.o -o output
main.o: main.c
	gcc -c main.c
myQueue.o: myQueue.c myQueue.h
	gcc -c myQueue.c
myStack.o: myStack.c myStack.h
	gcc -c myStack.c
myUGrapg.o: myUGraph.c myUGraph.h
	gcc -c myUGraph.c
clean: 
	rm *.o output
