#include <string.h>
#include "libWad/Wad.h"
#include <string>

int main()
{
    std::string path = "./wad_files/sample1.wad";
    Wad* doofus = Wad::loadWad(path);

    delete doofus;

}