#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stack>
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
    root = new node(nullptr, nullptr);
}

void Wad::operator delete(void* w)
{
    Wad* wad = (Wad*) w;
    wad->deletePrivate();
    free(wad);
}

Wad* Wad::loadWad(const string &path){
    
    Wad* loader = new Wad;

    //open up file descriptor and read header
    int fd;
    char buffer [8];
    fd = open(path.c_str(), O_RDONLY);

    if (fd == -1)
        return nullptr;

    read(fd, loader->magic, 4);

    read(fd, buffer, 4);
    loader->n_descriptors = *(int*) buffer;

    memset(buffer, 0, 8);
    read(fd, buffer, 4);
    loader->offset = *(int*) buffer;

    cout << "magic is " << loader->magic << endl;
    cout << "number of descriptors is " << loader->n_descriptors << endl;
    cout << "offset is " << loader->offset << endl;

    lseek(fd, loader->offset, SEEK_SET);
    memset(buffer, 0,8);

    //read file descriptors
    loader->lumps = new Lump[loader->n_descriptors];
    for(int i = 0; i < loader->n_descriptors; i++)
    {
        //offset
        read(fd, buffer, 4);
        loader->lumps[i].offset = *(int*) buffer;
        memset(buffer, 0, 4);

        //length
        read(fd, buffer, 4);
        loader->lumps[i].length = *(int*) buffer;
        memset(buffer, 0, 4);

        //name
        read(fd, buffer, 8);
        strcpy(loader->lumps[i].name, buffer);
        loader->lumps[i].name[8] = '\0';
        memset(buffer, 0, 8);

        cout << loader->lumps[i].name << " | offset: " << loader->lumps[i].offset << " length: " << loader->lumps[i].length << endl;
    }

    //create dir tree and load data into lumps
    node* current_node = loader->root;
    stack<char*> ns_stack;

    for(int i = 0; i < loader->n_descriptors; i++)
    {
        Lump* lump = &loader->lumps[i];

        //Markers
        if (lump->length == 0)
        {
            //E#M# marker -> next 10 lumps are going to be in this directory
            if (lump->name[0] == 'E' && lump->name[2] == 'M' && strlen(lump->name) == 4)
            {
                printf("Map Marker: %s\n", lump->name);
                //adding marker as child to current node
                node* map_marker = new node(lump, current_node);
                current_node->children.push_back(map_marker);

                //switching to map marker
                current_node = map_marker;

                //adding the 10 map elements to the children vector
                for(int j = 0; j < 10; j++)
                {
                    current_node->children.push_back(new node(&loader->lumps[++i], current_node));

                    //load data
                    loader->lumps[i].data = new char[loader->lumps[i].length];
                    lseek(fd, loader->lumps[i].offset, SEEK_SET);
                    read(fd, loader->lumps[i].data, loader->lumps[i].length);

                    cout << loader->lumps[i].name << endl;
                }
                    
                //going back up to marker parent
                current_node = current_node->parent;
            }
            
            else if(strstr(lump->name, "_START") != nullptr)
            {
                printf("Namespace starter: %s\n", lump->name);
                //obtains namespace name
                char*  name_space = new char[9];
                strcpy(name_space, lump->name);
                strtok(name_space, "_");
                strcat(name_space, "_END");

                //add namespace to stack
                ns_stack.push(name_space);

                //create namespace node
                node* ns_node = new node(lump, current_node);
                current_node = ns_node;
            }

            else if(strcmp(lump->name, ns_stack.top()) == 0)
            {
                printf("Namespace ender: %s\n", lump->name);
                current_node = current_node->parent;
                ns_stack.pop();
            }
            
        }

        //data
        else
        {
            current_node->children.push_back(new node(lump, current_node));

            //load data
            lump->data = new char[lump->length];
            lseek(fd, lump->offset, SEEK_SET);
            read(fd, lump->data, loader->lumps[i].length);

            cout << lump->name << endl;
        }



    }

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

void Wad::deletePrivate()
{
    for(int i = 0; i < n_descriptors; i++)
    {
        delete[] lumps[i].data;
    }
    delete[] lumps;
}