#pragma once
#include <string>
#include <vector>

using namespace std;

class Wad
{
    private:
        char magic [4];
        int n_descriptors;
        int offset;
    public:
        Wad();
        static Wad* loadWad(const string &path);
        string getMagic();
        bool isContent(const string &path);
        bool isDirectory(const string &path);
        int getSize(const string &path);
        int getContents(const string &path, char *buffer, int length, int offset = 0);
        int getDirectory(const string &path, vector<string> *directory);

};