#include <windows.h>
// millisecond accuracy
int wait(float fseconds){
    int milliseconds = fseconds*1000;
    Sleep(milliseconds);
    return 0;
}
void cls(){
    system("CLS");
}