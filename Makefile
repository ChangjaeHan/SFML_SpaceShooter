myGame.exe : main.o myBullet.o meteor.o
	g++ -std=c++11 -g main.o myBullet.o meteor.o -o myGame.exe -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system


main.o : main.cpp myBullet.h
	g++ -std=c++11 -c main.cpp

myBullet.o : myBullet.cpp myBullet.h
	g++ -std=c++11 -c myBullet.cpp

meteor.o : meteor.cpp meteor.h
	g++ -std=c++11 -c meteor.cpp

clean :
	rm -rf *.o myGame.exe
