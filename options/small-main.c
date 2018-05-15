#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "options.h"

int main(int argc, char *argv[])
{
	unsigned long int x, y;
	int help;

	struct st_option options[] = {
		FLAG_BOL('h', "help", "Show this usage information", &help, NULL),
		PARAM_INT("numerator", "Value of the numerator to compute", &x, "10"),
		PARAM_INT("denominator", "Value of the denominator to compute", &y, "5"),
		OPTIONS_END
	};

	if (options_parse(options, argc, argv)) {
		options_usage(options, "small-main");
		printf("Error parsing options: %s\n", option_err_msg);
		exit(EXIT_FAILURE);
	}

	if (help) {
		options_usage(options, "small-main");
		exit(EXIT_FAILURE);
	}

	printf("%u/%u = %lf\n", x, y, (double)x/(double)y);
	exit(EXIT_SUCCESS);
}
