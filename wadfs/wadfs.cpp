#define FUSE_USE_VERSION 26

#include <vector>
#include <string>
#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include "../libWad/Wad.h"

static Wad* file;

static int open_callback(const char *path, struct fuse_file_info *fi) {
    return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    size_t len;
    (void) fi;

    if(!(file->isContent(std::string(path))))
        return -ENOENT;

    std::cout << "ghetto fabulus" << std::endl;

    return file->getContents(path, buf, size, offset);
}

static int getattr_callback(const char *path, struct stat *stbuf) {
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0666;
        stbuf->st_nlink = 2;
    } else if (file->isDirectory(path)) {
        stbuf->st_mode = S_IFDIR | 0666;
        stbuf->st_nlink = 2;
    } else if (file->isContent(path)) {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = file->getSize(path);
    } else
        res = -ENOENT;

    return res;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    if(!(file->isDirectory(std::string(path))))
        return -ENOENT;

    std::vector<std::string> entries;

    int n_contents = file->getDirectory(path, &entries);
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    for(int i = 0; i < n_contents; i++)
        filler(buf, entries[i].c_str(), NULL, 0);

    return 0;
}

static void destroy_callback(void* private_data) {
    delete file;
}

static struct fuse_operations wadfs_operations = {
    .getattr = getattr_callback,
    .open = open_callback,
    .read = read_callback,
    .readdir = readdir_callback,
    .destroy = destroy_callback,
};

int main(int argc, char* argv[]) {
    file = Wad::loadWad(std::string(argv[2]));
    argv[argc-2] = argv[argc-1];
    argv[argc-1] = NULL;
    argc--;
    return fuse_main(argc, argv, &wadfs_operations, NULL);
}