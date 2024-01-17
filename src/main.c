#include <stdio.h>

#include "input_check.h"
#include "login.h"
  
int main(int argc, char **argv) {
	int ret = check_input(argc);

	if (0 < ret)
		ret = check_login(argv[1]);

	return ret;
} 