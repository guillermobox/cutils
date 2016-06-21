#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include "options.h"

#define IS_POSITIONAL(opt)     (opt->flagshort == '-')
#define IT_RECEIVES_INPUT(opt) (opt->type & (TYPE_INT | TYPE_STRING | TYPE_FLOAT))
#define IT_HAS_DEFAULT(opt)    (opt->def != NULL)

#define REMOVE(arraylist, i, len) \
do {\
	int _i;\
	for (_i = i + 1; _i < len + 1; _i++) {\
		arraylist[_i - 1] = arraylist[_i];\
	}\
	len--;\
} while(0);

char option_err_msg[128];

static void update_parse_error(struct st_option *option, unsigned int err, const char * value)
{
	static const char *types[] = { "bool", "int", "string", "float"};

	switch (err) {
		case ERR_UNKNOWN:
			snprintf(option_err_msg, 128, "Unknown argument in command line: %s", value);
			return;
		case ERR_EXTRA_POSITIONAL:
			snprintf(option_err_msg, 128, "Unexpected positional argument in command line: %s", value);
			return;
	}
	if (IS_POSITIONAL(option)) {
		switch (err) {
			case ERR_REQUIRED:
				snprintf(option_err_msg, 128, "Positional argument required: %s", option->flaglong);
				break;
			case ERR_CONVERSION:
				snprintf(option_err_msg, 128, "Error parsing the positional argument %s: %s is not a %s", option->flaglong, value, types[option->type]);
				break;
		}
	} else {
		switch (err) {
			case ERR_PARAMETER:
				snprintf(option_err_msg, 128, "The flag -%c requires a parameter", option->flagshort);
				break;
			case ERR_CONVERSION:
				snprintf(option_err_msg, 128, "Error parsing the parameter for the flag -%c: %s is not a %s", option->flagshort, value, types[option->type]);
				break;
			case ERR_LIMITS:
				snprintf(option_err_msg, 128, "The parameter for the flag -%c is out of bounds: %s", option->flagshort, value);
				break;
		}
	}
};

char * format_option(struct st_option *option)
{
	static char buffer[128];
	char flagname[32];
	char defaultvalue[32];

	if (IT_HAS_DEFAULT(option)) {
		snprintf(defaultvalue, 32, " [default %s]", option->def);
	} else {
		defaultvalue[0] = 0;
	}

	if (IS_POSITIONAL(option)) {
		snprintf(buffer, 128, "  %-20s %s%s", option->flaglong, option->description, defaultvalue);
		return buffer;
	}

	if (option->flaglong) {
		snprintf(flagname, 32, "  -%c, --%s", option->flagshort, option->flaglong);
	} else {
		snprintf(flagname, 32, "  -%c", option->flagshort);
	}

	if (IT_RECEIVES_INPUT(option)) {
		if (option->placeholder) {
			snprintf(flagname + strlen(flagname), 32 - strlen(flagname), " <%s>", option->placeholder);
		} else {
			if (option->type == TYPE_INT)
				strcat(flagname, " <integer>");
			else if (option->type == TYPE_FLOAT)
				strcat(flagname, " <float>");
			else if (option->type == TYPE_STRING)
				strcat(flagname, " <string>");
		}
	}

	snprintf(buffer, 128, "%-32s %s%s", flagname, option->description, defaultvalue);
	return buffer;
};

void options_usage(struct st_option *options, const char * program)
{
	struct st_option * option = options;
	int there_are_flags = 0, there_are_positionals = 0;

	for (option = options; option->description != NULL; option++) {
		if (IS_POSITIONAL(option))
			there_are_positionals = 1;
		else
			there_are_flags = 1;
		if (there_are_positionals && there_are_flags) break;
	}

	if (there_are_flags)
		printf("Usage: %s [OPTIONS] ", program);
	else
		printf("Usage: %s ", program);

	if (there_are_positionals)
		for (option = options; option->description != NULL; option++) {
			if (!IS_POSITIONAL(option))
				continue;
			if (IT_HAS_DEFAULT(option))
				printf("[%s] ", option->flaglong);
			else
				printf("<%s> ", option->flaglong);
		}

	printf("\n\n");

	if (there_are_positionals) {
		printf("Where\n\n");
		for (option = options; option->description != NULL; option++) {
			if (!IS_POSITIONAL(option))
				continue;
			puts(format_option(option));
		}
		printf("\n");
	}

	if (there_are_flags) {
		printf("The available options are:\n\n");
		for (option = options; option->description != NULL; option++) {
			if (IS_POSITIONAL(option))
				continue;
			puts(format_option(option));
		}
		printf("\n");
	}
};

static int parse_int(struct st_option *option, const char *value, int found)
{
	unsigned long int number = 0;
	if (found) {
		char * ptr = (char *) value;;
		number = strtol(value, &ptr, 10);
		if ((number == LONG_MIN || number == LONG_MAX) && errno == ERANGE)
			return ERR_LIMITS;
		if (ptr == value || *ptr != '\0')
			return ERR_CONVERSION;
	}
	*((unsigned long int*)option->data) = number;
	return 0;
};

static int parse_float(struct st_option *option, const char *value, int found)
{
	double number = 0;
	if (found) {
		char * ptr;
		ptr = (char *) value;
		number = strtod(value, &ptr);
		if (errno == ERANGE)
			return ERR_LIMITS;
		if (ptr == value || *ptr != '\0')
			return ERR_CONVERSION;
	}
	*((double*)option->data) = number;
	return 0;
};

static int parse_string(struct st_option *option, const char *value, int found)
{
	char * buffer = NULL;
	if (found) {
		buffer = malloc(strlen(value) + 1);
		strcpy(buffer, value);
	}
	*((char**)option->data) = buffer;
	return 0;
};

static int parse_bool(struct st_option *option, const char *value, int found)
{
	if (found)
		*((int*) option->data) = 1;
	else
		*((int*) option->data) = 0;
	return 0;
};

static int parse_value(struct st_option *option, const char *value, int found)
{
	if (IT_HAS_DEFAULT(option) && !found) {
		value = option->def;
		found = 1;
	}

	if (IS_POSITIONAL(option) && !found)
		return ERR_REQUIRED;

	if (IT_RECEIVES_INPUT(option) && found && !value)
		return ERR_PARAMETER;

	switch (option->type) {
		case TYPE_BOOL:
			return parse_bool(option, value, found);
		case TYPE_INT:
			return parse_int(option, value, found);
		case TYPE_FLOAT:
			return parse_float(option, value, found);
		case TYPE_STRING:
			return parse_string(option, value, found);
	}

	return 0;
};

int options_parse(struct st_option *options, int argc, char *argv[])
{
	struct st_option * option = options;
	const char * value = NULL;
	int found, argi, err;

	/*
	 * the first sweep will locate all the flags and their
	 * correspondent values
	 */
	for (option = options; option->description != NULL; option++) {
		char shortbuf[3], longbuf[32];

		if (IS_POSITIONAL(option)) continue;

		snprintf(shortbuf, 3, "-%c", option->flagshort);
		snprintf(longbuf, 32, "--%s", option->flaglong);
		found = 0;
		value = NULL;

		for (argi = 1; argi < argc; argi++) {
			if (strncmp(argv[argi], shortbuf, 3) == 0 || strncmp(argv[argi], longbuf, 32) == 0) {
				REMOVE(argv, argi, argc);
				found = 1;
				if (IT_RECEIVES_INPUT(option)) {
					value = argv[argi];
					REMOVE(argv, argi, argc);
				}
				break;
			}
		}

		err = parse_value(option, value, found);
		if (err) {
			update_parse_error(option, err, value);
			return err;
		}
	}

	/*
	 * any remaining flag, it is not known, hence an error
	 */
	for (argi = 1; argi < argc; argi++) {
		if (argv[argi][0] == '-') {
			update_parse_error(NULL, ERR_UNKNOWN, argv[argi]);
			return ERR_UNKNOWN;
		}
	}

	/*
	 * the remaining are positional arguments
	 */
	argi = 1;
	for (option = options; option->description != NULL; option++) {
		if (!IS_POSITIONAL(option)) continue;
		value = argv[argi];

		err = parse_value(option, value, value != NULL);
		if (err) {
			update_parse_error(option, err, value);
			return err;
		}
		argi++;
	}

	/*
	 * any remaining positional argument is not expected
	 */
	if (argi != argc) {
		update_parse_error(NULL, ERR_EXTRA_POSITIONAL, argv[argi]);
		return ERR_EXTRA_POSITIONAL;
	}

	return 0;
};
