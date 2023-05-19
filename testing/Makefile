CC = g++
CFLAGS = -g -Wall -c
LDFLAGS = -g -Wall
BOOSTFLAGS = -lboost_serialization

vpath %.cpp ../
vpath %.h ../
vpath %.o ../


all: PartA

PartA: main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o
	$(CC) $(LDFLAGS) -o gitlet main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o $(BOOSTFLAGS)

gitlet: main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o
	$(CC) $(LDFLAGS) -o gitlet main.o Blob.o Commit.o RemoteRepo.o Repository.o StagingArea.o Tree.o Utils.o $(BOOSTFLAGS)
	# rm -rf *.o
%.o: %.cpp
	$(CC) $(CFLAGS) $< $(BOOSTFLAGS)

clean:
	rm -rf *.o gitlet

