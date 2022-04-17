#include <string.h>
#include "libWad/Wad.h"
#include <string>

using namespace std;

int main()
{
    std::string path = "./wad_files/sample1.wad";
    Wad* doofus = Wad::loadWad(path);

    if (doofus->isContent("/E1M0/01.txt"))
    {
        char buffer[250];
        int bytes = doofus->getContents("/E1M0/01.txt", buffer, 250, 3);
        printf("%s", buffer);
        printf("read %d bytes\n", bytes);
    }

    if (doofus->isDirectory("/E1M0"))
    {
        vector<string> entries;
        int num_entries = doofus->getDirectory("/E1M0", &entries);
        for(auto &it : entries)
            printf("%s\n", it.c_str());

        printf("num of entries is %d\n", num_entries);
    }
    

    delete doofus;

}