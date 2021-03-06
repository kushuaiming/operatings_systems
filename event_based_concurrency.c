#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // open and set up a bunch of sockets (not shown)
  // main loop
  while (1) {
    // initiallize the fd_set to all zero
    fd_set readFDs;
    FD_ZERO(&readFDs);

    // now set the bits for the descriptors
    // this server is interested in
    // (for simplicity, all of them from min to max)
    int fd;
    for (fd = minFD; fd < maxFD; fd++) {
      FD_SET(fd, &readFDs);
    }

    // do the select
    int rc = select(maxFD + 1, &readFDs, NULL, NULL, NULL);

    // check which actually have data using FD_ISSET()
    int df;
    for (fd = minFD; fd < maxFD; fd++)
      if (FD_ISSET(fd, &readFDs)) processFD(fd);
  }
}