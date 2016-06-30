#include <stdio.h>
#include <stdlib.h>
#include "options.h"

int main(int argc, char *argv[])
{
	/* define the variables that you want to work with */
	unsigned int verbosity, help;
	unsigned long int n, m;
	char *algorithm, *filename, *method;
	double tolerance;

	/* create a structure with the flags and posicional parameters */
	struct st_option myoptions[] = {
		FLAG_BOL('v', "verbose", "Set verbosity to print more information", &verbosity, NULL),
		FLAG_BOL('h', "help", "Show this usage information", &help, NULL),
		FLAG_INT('n', "rows", "Number of rows for the matrix", "ROWS", &n, NULL),
		FLAG_INT('m', "cols", "Number of columns for the matrix", "COLS", &m, NULL),
		FLAG_STR('a', "algorithm", "Type of algorithm to run: general, symmetric", "TYPE", &algorithm, "general"),
		FLAG_FLT('t', NULL, "Tolerance of the solevr", NULL, &tolerance, "1e-3"),
		PARAM_STRING("matrix", "File to write the matrix into", &filename, NULL),
		PARAM_STRING("method", "Simulation method to use", &method, "cgstab"),
		OPTIONS_END
	};

	/* parse the command line and write the values in the variables */
	if (options_parse(myoptions, argc, argv)) {
		printf("*** Error parsing options: %s ***\n\n", option_err_msg);
		options_usage(myoptions, "main");
		return 1;
	};

	/* if the user wants to see the usage, show them */
	if (help) {
		options_usage(myoptions, "main");
		return 0;
	}

	printf("\n  The program has a set of valid command line parameters, this ones:\n\n");
	printf("%14s  %s\n", "PARAMETER", "VALUE");
	printf("%14s  %d\n", "verbosity", verbosity);
	printf("%14s  %ld\n", "n", n);
	printf("%14s  %ld\n", "m", m);
	printf("%14s  %.2f\n", "tolerance", tolerance);
	printf("%14s  %s\n", "algorithm", algorithm);
	printf("%14s  %s\n", "matrix", filename);
	printf("%14s  %s\n", "method", method);
	printf("\n");

	return (EXIT_SUCCESS);
}
