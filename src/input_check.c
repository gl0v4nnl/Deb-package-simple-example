#include <stdio.h>

int check_input(int argc) {
    int ret = argc;

    switch (ret) {
        case 1:
            printf("[%s] The poop app. Tap TAB to make app dumb\npoop [random string]\n", __FUNCTION__);
            ret = 0;
            break;
        case 2:
            break;
        default:
            printf("[%s] WTF?!\npoop [random string]\n", __FUNCTION__);
            ret = -1;
            break;
    }

    return ret;
}