#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define FLAG_REQUIRED 1<<0
#define FLAG_OPTATIVE 1<<1
#define FLAG_POSITIONAL 1<<2
#define TYPE_BOOL 0
#define TYPE_INT 1
#define TYPE_STRING 2
#define TYPE_FLOAT 3

struct st_option {
	const char flagshort;
	const char *flaglong;
	const char *description;
	uint8_t flags;
	uint8_t type;
	void * data;
	const char *def;
};

static char option_err_msg[128];

char * format_option(struct st_option *option)
{
	static char buffer[128];
	char temporal[64];

	if (option->flaglong) {
		snprintf(temporal, 64, "  -%c, --%s", option->flagshort, option->flaglong);
	} else {
		snprintf(temporal, 64, "  -%c", option->flagshort);
	}

	if (option->type == TYPE_INT)
		strcat(temporal, " <integer>");
	else if (option->type == TYPE_FLOAT)
		strcat(temporal, " <float>");
	else if (option->type == TYPE_STRING)
		strcat(temporal, " <string>");

	snprintf(buffer, 128, "%-30s %s", temporal, option->description);

	return buffer;
};

/*
 * Print usage from options list.
 */
void options_usage(struct st_option *options, const char * program, const int configuration)
{
	struct st_option * option = options;
	const char *argumenttype[] = {"bool", "integer", "string", "float"};

	printf("Usage: %s ", program);
	if (configuration == 1) {
		printf("[-");
		for (option = options; option->description != NULL; option++) {
			if (option->flags == FLAG_OPTATIVE && option->type == TYPE_BOOL)
				putc(option->flagshort, stdout);
		}
		printf("] ");
		for (option = options; option->description != NULL; option++) {
			if ((option->flags == FLAG_OPTATIVE || option->def) && option->type != TYPE_BOOL) {
				printf("[-%c <%s>] ", option->flagshort, argumenttype[option->type]);
			}
		}
		for (option = options; option->description != NULL; option++) {
			if (option->flags == FLAG_REQUIRED && !option->def) {
				printf("-%c <%s> ", option->flagshort, argumenttype[option->type]);
			}
		}
	} else {
		printf("[OPTIONS] ");
	}

	for (option = options; option->description != NULL; option++) {
		if (option->flags == (FLAG_POSITIONAL | FLAG_REQUIRED)) {
			printf("<%s> ", option->flaglong);
		}
	}
	for (option = options; option->description != NULL; option++) {
		if (option->flags == (FLAG_POSITIONAL | FLAG_OPTATIVE)) {
			printf("[%s] ", option->flaglong);
		}
	}

	printf("\n\nWhere the available options are:\n\n");
	for (option = options; option->description != NULL; option++) {
		if (!(option->flags & FLAG_POSITIONAL))
			puts(format_option(option));
	}
	printf("\n");
};

/*
 * Parse a given option, after knowing if it was found on the command line, and
 * the possible value that the user provided.
 */
static int option_parse_value(struct st_option *option, const char *value, int found)
{
	if (!found && option->flags & FLAG_REQUIRED && !option->def) {
		sprintf(option_err_msg, "Flag %c is required", option->flagshort);
		return 1;
	}
	if (!found && option->def) {
		value = option->def;
		found = 1;
	}
	if (option->type == TYPE_BOOL) {
		if (found)
			*((int*)option->data) = 1;
		else
			*((int*)option->data) = 0;
	} else if (option->type == TYPE_INT) {
		if (found) {
			if (value) {
				long int number;
				char * ptr;
				ptr = (char *) value;
				number = strtol(value, &ptr, 10);
				if (ptr == value || *ptr != '\0') {
					sprintf(option_err_msg, "Variable for flag %c has to be an integer", option->flagshort);
					return 1;
				}
				*((int*)option->data) = number;
			} else {
				sprintf(option_err_msg, "Flag %c needs an argument", option->flagshort);
				return 1;
			}
		} else
			*((int*)option->data) = 0;
	} else if (option->type == TYPE_FLOAT) {
		if (found) {
			if (value) {
				double number;
				char * ptr;
				ptr = (char *) value;
				number = strtod(value, &ptr);
				if (ptr == value || *ptr != '\0') {
					sprintf(option_err_msg, "Variable for flag %c has to be a floating point number", option->flagshort);
					return 1;
				}
				*((double*)option->data) = number;
			} else {
				sprintf(option_err_msg, "Flag %c needs an argument", option->flagshort);
				return 1;
			}
		} else
			*((double*)option->data) = 0;
	} else if (option->type == TYPE_STRING) {
		if (found) {
			if (value) {
				*((char**)option->data) = (char*)strdup(value);
			} else {
				sprintf(option_err_msg, "Flag %c needs an argument", option->flagshort);
				return 1;
			}
		} else
			*((char**)option->data) = NULL;
	}
	return 0;
};

/*
 * Parse the command line using the options structure. After parsing,
 * set the value for the assignated value of each option. Return
 * error if the command line arguments are not compliant.
 */
int options_parse(struct st_option *options, int argc, char *argv[])
{
	struct st_option * option = options;
	const char * value = NULL;
	int found, argi, argj, err;

	for (option = options; option->description != NULL; option++) {
		if (option->flags & FLAG_POSITIONAL) continue;
		char buf[3], longbuf[32];
		snprintf(buf, 3, "-%c", option->flagshort);
		snprintf(longbuf, 32, "--%s", option->flaglong);
		found = 0;

		for (argi = 1; argi < argc; argi++) {
			if (strncmp(argv[argi], buf, 3) == 0 || strncmp(argv[argi], longbuf, 32) == 0) {
				for (argj = argi + 1; argj < argc + 1; argj++)
					argv[argj - 1] = argv[argj];
				argc--;
				found = 1;
				value = argv[argi];
				if (option->type != TYPE_BOOL) {
					for (argj = argi + 1; argj < argc + 1; argj++)
						argv[argj - 1] = argv[argj];
					argc--;
				}
				break;
			}
		}

		if (option->data) {
			err = option_parse_value(option, value, found);
			if (err) return 1;
		};

	}

	for (argi = 1; argi < argc; argi++) {
		if (argv[argi][0] == '-') {
			sprintf(option_err_msg, "Unknown or repeated option: %s", argv[argi]);
			return 1;
		}
	};

	argi = 1;
	for (option = options; option->description != NULL; option++) {
		if (!(option->flags & FLAG_POSITIONAL)) continue;
		value = argv[argi];
		err = option_parse_value(option, value, value != NULL);
		if (err) return 1;
		argi++;
	};

	return 0;
};

int main(int argc, char *argv[])
{
	unsigned int verbosity, help;
	unsigned int n, m = 0;
	const char *type, *rowfilename, *filename;
	double tol;

	struct st_option myoptions[] = {
		{'v',"verbosity","Set verbosity to print more information",FLAG_OPTATIVE                  ,TYPE_BOOL  ,&verbosity  ,NULL}     ,
		{'h',"help"     ,"Show this usage information"            ,FLAG_OPTATIVE                  ,TYPE_BOOL  ,&help       ,NULL}     ,
		{'n',NULL       ,"Number of rows for the matrix"          ,FLAG_OPTATIVE                  ,TYPE_INT   ,&n          ,NULL}     ,
		{'m',NULL       ,"Number of columns for the matrix"       ,FLAG_OPTATIVE                  ,TYPE_INT   ,&m          ,NULL}     ,
		{'a',"algorithm","Type of algorithm to run"               ,FLAG_OPTATIVE                  ,TYPE_STRING,&type       ,"general"},
		{'t',NULL       ,"Tolerance of the solver"                ,FLAG_REQUIRED                  ,TYPE_FLOAT ,&tol        ,"1e-6"}   ,
		{'-',"matrix"   ,"Matrix file"                            ,FLAG_REQUIRED | FLAG_POSITIONAL,TYPE_STRING,&filename   ,NULL}     ,
		{'-',"row"      ,"Independent row file"                   ,FLAG_REQUIRED | FLAG_POSITIONAL,TYPE_STRING,&rowfilename,NULL}     ,
		{}  ,
	};

	if (options_parse(myoptions, argc, argv)) {
		printf("\033[31mError parsing options: %s\033[0m\n\n", option_err_msg);
		options_usage(myoptions, "main", 0);
		return 1;
	};

	if (help) {
		options_usage(myoptions, "main", 0);
		return 0;
	}

	if (verbosity)
		printf("Verbosity is turned on!\n");

	printf("COMMAND LINE PARAMETERS:\n");
	printf("%14s <- %d\n", "n", n);
	printf("%14s <- %d\n", "m", m);
	printf("%14s <- %s\n", "type", (type != NULL) ? type:"NULL");
	printf("%14s <- %e\n", "tolerance", tol);
	printf("%14s <- %s\n", "matrix", filename);
	printf("%14s <- %s\n", "row", rowfilename);

};
