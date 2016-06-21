#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdint.h>

/*
 * This structure represents the possible arguments in the form of flags and
 * positional parameters. Use the macros to define entries of this structure.
 */
struct st_option {
	const char  flagshort;
	const char *flaglong;
	const char *description;
	const char *placeholder;
	uint8_t type;
	void * data;
	const char *def;
};

#define TYPE_BOOL 0
#define TYPE_INT 1
#define TYPE_STRING 2
#define TYPE_FLOAT 3

/*
 * Use this macros to define the elements of the options structure
 */
#define FLAG_BOOL(flag, longflag, description, variable, default)                { flag, longflag, description, NULL, TYPE_BOOL, variable, default}
#define FLAG_INT(flag, longflag, description, placeholder, variable, default)    { flag, longflag, description, placeholder, TYPE_INT, variable, default}
#define FLAG_FLOAT(flag, longflag, description, placeholder, variable, default)  { flag, longflag, description, placeholder, TYPE_FLOAT, variable, default}
#define FLAG_STRING(flag, longflag, description, placeholder, variable, default) { flag, longflag, description, placeholder, TYPE_STRING, variable, default}
#define PARAM_INT(longflag, description, variable, default)                      { '-',  longflag, description, NULL, TYPE_INT, variable, default}
#define PARAM_STRING(longflag, description, variable, default)                   { '-',  longflag, description, NULL, TYPE_STRING, variable, default}
#define OPTIONS_END {}

/*
 * Print a well formatted usage using the options structure. Also provide
 * the name of the program to be shown in the listing.
 */
void options_usage(struct st_option *options, const char * program);

/*
 * Parse the command line using the options structure. After parsing,
 * set the value for the assignated value of each option. Return
 * error if the command line arguments are not compliant with the
 * structure options.
 */
int options_parse(struct st_option *options, int argc, char *argv[]);

/*
 * The previous function 'options_parse' can return any of this errors'
 */
enum {ERR_CONVERSION = 1, ERR_PARAMETER, ERR_REQUIRED, ERR_UNKNOWN, ERR_EXTRA_POSITIONAL, ERR_LIMITS};

/*
 * This variable holds the last error obtained when parsing the
 * structure of options with the command line. Print it after options_parse
 * returns != 0, and before calling it again.
 */
extern char option_err_msg[128];

#endif
