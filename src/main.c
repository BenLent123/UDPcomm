#include <stdio.h>
#include "yggdrasil.h"
#include "socket.h"
#include "UDP.h"
#include "chat.h"

int main(){

    if(check_status_yggdrasil()<0){
        return -1;
    }

    printf("passed");
    return 0;
}