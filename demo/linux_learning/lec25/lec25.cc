#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    struct stat sb;
    off_t len;
    char *addr;
    fd = open("example.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    len = sb.st_size;
    addr = (char *)mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    printf("%s", addr);
    if (munmap(addr, len) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return 0;
}