#include <stdio.h>
#include <string.h>

#define TYPE_1 "user_1"
#define TYPE_2 "user_2"
  
int check_login(const char* login_to_check) {
    int ret = 0;
	if(login_to_check != NULL) {     
		if ((strcmp(login_to_check, TYPE_1) == 0) || (strcmp(login_to_check, TYPE_2) == 0)){  
			printf("%s\n", login_to_check);
		} else {
			printf("[%s]: Логин не определен\n", __FUNCTION__);
            ret = -2;
		}  
	} else {
		printf("[%s] Пусто!\n", __FUNCTION__);
        ret = -3;
	}

    return ret;  
} 