#include <unistd.h>
// microsecond accuarcy
int wait(float fseconds){
    unsigned int microsecond = 1000000;
    usleep(fseconds*microsecond);
}

