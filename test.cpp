#include <string.h>
#include "libWad/Wad.h"
#include <string>

int main()
{
    std::string path = "./wad_files/DOOM1.WAD";
    Wad* doofus = Wad::loadWad(path);

    delete doofus;

}