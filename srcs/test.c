//#include "csapp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[])
{
 int fd1, fd2, fd3;
 char *fname = argv[1];
 char *test;

 test = "lalalalalalalalalalalalalalalalalalal";
 fd1 = open(fname, O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR);
 write(fd1, "pqrs", 4);
 fd3 = open(fname, O_WRONLY, S_IRUSR|S_IWUSR);
 write(fd3, test, strlen(test));
 fd2 = dup(fd1); /* Returns new descriptor mapped to same file */
 dup2(fd2, fd1);
 write(fd2, "wxyz", 4);
 write(fd3, "ef", 2);
 close(fd1);
 close(fd2);
 close(fd3);
 return 0;
}
