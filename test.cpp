#include <string.h>
#include "libWad/Wad.h"
#include <string>

int main()
{
    std::string path = "./wad_files/sample1.wad";
    Wad* doofus = Wad::loadWad(path);

    delete doofus;


    char poop[9] = "OS_START";

    printf("%s\n", poop);
    printf("%s\n", strtok(poop, "_"));
    printf("%s\n", poop);
    for (size_t i = 0; i < 9; i++)
    {
        if(poop[i] == '\0')
            printf("\\0");
        else
            printf("%c", poop[i]);
    }



}