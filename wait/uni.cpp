#include <unistd.h>
// microsecond accuarcy
int wait(float fseconds){
    unsigned int microsecond = 1000000;
    usleep(fseconds*microsecond);
    return 0;
}
void cls(){
    for(int i = 0; i < 15; i++){
        cout << endl;
    }
}
