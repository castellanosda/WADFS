#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "Wad.h"

using namespace std;

Wad::Wad()
{
    strcpy(magic, "goo");
    n_descriptors = 0;
    offset = 0;
}

Wad* Wad::loadWad(const string &path){
    
    Wad* loader = new Wad;

    //open up file descriptor and read header
    int fd;
    char buff [100];
    fd = open(path.c_str(), O_RDONLY);

    if (fd == -1)
        return nullptr;

    read(fd, loader->magic, 4);

    read(fd, buff, 4);
    loader->n_descriptors = *(int*) buff;

    memset(buff, 0, 4);
    read(fd, buff, 4);
    loader->offset = *(int*) buff;

    cout << "magic is " << loader->magic << endl;
    cout << "number of descriptors is " << loader->n_descriptors << endl;
    cout << "offset is " << loader->offset << endl;

    lseek(fd, offset);

    close(fd);

    return loader;
}

string Wad::getMagic(){
    return this->magic;
}

bool Wad::isContent(const string &path){
    return false;
}

bool Wad::isDirectory(const string &path){
    return false;
}

int Wad::getSize(const string &path){
    return 0;
}

int Wad::getContents(const string &path, char *buffer, int length, int offset /*= 0*/){
    return 0;
}

int Wad::getDirectory(const string &path, vector<string> *directory){
    return 0;
}