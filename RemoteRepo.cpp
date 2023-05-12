#include "RemoteRepo.h"
#include "Repository.h"

void RemoteRepo::readFromConfigFile() {
    if (fs::exists(Repository::CONFIG) == false) {
        return;
    }

    readObject(Repository::CONFIG, *this);
}

void RemoteRepo::writeToConfigFile() {
    writeObject(Repository::CONFIG, *this);
}
