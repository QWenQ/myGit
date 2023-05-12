# three targets:
# 	default: compile the program
#	check  : compile the program, if needed, and then for each file, F.in, in directory testing, use F.in as input and compare
#			 the output to the contents of the file names F.out.
#   clean  : remove all files produced by the make command.

# target ... : prerequisities ...
# 	recipe
# 	...


# objects = main.o Blob.o Commit.o RemoteRepo.o \
			Repository.o StagingArea.o Tree.o Utils.o

# objects := $(wildcard *.o)
objects := $(patsubst %.cpp, %.o, $(wildcard *.cpp))


gitlet : $(objects)
	g++ -o gitlet -g $(objects) -lboost_serialization -Wall

# main.o : Utils.h Repository.h
#	g++ -c main.cpp 

# Blob.o : Blob.h Repository.h
#	g++ -c Blob.cpp 

# Commit.o : Commit.h Repository.h
#	g++ -c Commit.cpp 

# RemoteRepo.o : RemoteRepo.h Repository.h
#	g++ -c RemoteRepo.cpp

# Repository.o : Repository.h
#	g++ -c Repository.cpp

# StagingArea.o : StagingArea.h Repository.h
#	g++ -c StagingArea.cpp 

# Tree.o : Tree.h Repository.h
#	g++ -c Tree.cpp

# Utils.o : Utils.h
#	g++ -c Utils.cpp

.PHONY : clean
clean : 
	-rm -rf gitlet *.o