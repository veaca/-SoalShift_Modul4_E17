#define FUSE_USE_VERSION 28
#define HAVE_SETXATTR

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#include <stdlib.h>
#endif

//static const char *dirpath = "/home/alvin/Desktop";


static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_chmod(const char *path, mode_t mode)
{
	char fpath[1000];
	sprintf (fpath, "%s", path);
	int res;
	 mode = 000;
	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(path);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	int len= strlen (path);
	if (strcmp(&(path[len-4]), ".txt")==0 || strcmp(&(path[len-4]), ".doc")==0 || strcmp(&(path[len-4]), ".pdf")==0){
	close (fd);
	int ganti;
	char awal[1000];
	char akhir[1000];
        sprintf (awal, "%s", path);
	sprintf (akhir, "%s.ditandai", path);
	ganti = rename(awal, akhir);
	if (ganti != 0) printf ("gagal");
	system("zenity --error --text=\"Terjadi kesalahan! File berisi konten berbahaya.\"");
	}
	else res=pread (fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
	char pindah[1000];
	system("mkdir rahasia");
	sprintf (pindah, "%s", path);
	system ("mv &fpath rahasia");
	char modi[1000];
	system("cd rahasia");
	sprintf (modi, "%s", path);
	system ("chmod 000 &fpath");
}

static struct fuse_operations xmp_oper = {
	.read		= xmp_read,
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.mkdir		= xmp_mkdir,
	.chmod		= xmp_chmod,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
