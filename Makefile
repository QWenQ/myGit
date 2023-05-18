CC = g++
CFLAGS = -g -Wall -c
LDFLAGS = -g -Wall
BOOSTFLAGS = -lboost_serialization


all: PartA

PartA: main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o
	$(CC) $(LDFLAGS) -o gitlet main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o $(BOOSTFLAGS)

gitlet: main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o
	$(CC) $(LDFLAGS) -o gitlet main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o $(BOOSTFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< $(BOOSTFLAGS)

clean:
	rm -rf *.o gitlet

