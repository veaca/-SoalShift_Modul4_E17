 #ifdef linux
#define _XOPEN_SOURCE 500
#endif
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statfs.h>

char *direction={"/home/demsy/mas_Adhipur"}; //folder sumber

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,pathcdcd);
    int res;

    res = lstat(i, stbuf);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);
    int res;

    res = readlink(i, buf, size - 1);
    if(res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}


static int xmp_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    DIR *dp;
    struct dirent *de;
    int res = 0;

    dp = opendir(i);
    if(dp == NULL)
        return -errno;

    while((de = readdir(dp)) != NULL) {
        res = filler(h, de->d_name, de->d_type);
        if(res != 0)
            break;
    }

    closedir(dp);
    return res;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int res;

    res = mknod(i, mode, rdev);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int res;

    res = mkdir(i, mode);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_unlink(const char *path)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int res;

    res = unlink(i);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_rmdir(const char *path)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);
    int res;

    res = rmdir(i);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
    int res;

    res = symlink(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    int res;

    res = rename(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_link(const char *from, const char *to)
{
    int res;

    res = link(from, to);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);
    int res;

    res = chmod(i, mode);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int res;

    res = lchown(i, uid, gid);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int res;

    res = truncate(i, size);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_utime(const char *path, struct utimbuf *buf)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int res;

    res = utime(i, buf);
    if(res == -1)
    return -errno;

    return 0;
}


static int xmp_open(const char *path, int flags)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int res;

    res = open(i, flags);
    if(res == -1)
        return -errno;

    close(res);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int fd;
    int res;

    fd = open(i, O_RDONLY);
    if(fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if(res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset)
{
    char i[2000];
    strcpy(i,direction);
    strcat(i,path);

    int fd;
    int res;

    fd = open(i, O_WRONLY);
    if(fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if(res == -1)
        res = -errno;

    close(fd);
    return res;
}


static int xmp_statfs(struct fuse_statfs *fst)
{
    struct statfs st;
    int rv = statfs("/",&st);
    if(!rv) {
     fst->block_size  = st.f_bsize;
     fst->blocks      = st.f_blocks;
     fst->blocks_free = st.f_bavail;
     fst->files       = st.f_files;
     fst->files_free  = st.f_ffree;
     fst->namelen     = st.f_namelen;
    }
    return rv;
}

static int xmp_release(const char *path, int flags)
{
     Just a stub.  This method is optional and can safely be left
       unimplemented

    (void) path;
    (void) flags;
    return 0;
}

static int xmp_fsync(const char *path, int isdatasync)
{
    Just a stub.  This method is optional and can safely be left
       unimplemented

    (void) path;
    (void) isdatasync;
    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readlink = xmp_readlink,
    .getdir = xmp_getdir,
    .mknod = xmp_mknod,
    .mkdir = xmp_mkdir,
    .symlink = xmp_symlink,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .link = xmp_link,
    .chmod = xmp_chmod,
    .chown = xmp_chown,
    .truncate = xmp_truncate,
    .utime = xmp_utime,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
    .statfs = xmp_statfs,
    .release = xmp_release,
    .fsync = xmp_fsync

};

int main(int argc, char *argv[])
{
    fuse_main(argc, argv, &xmp_oper);
    return 0;
}
