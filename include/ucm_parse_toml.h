#ifndef UCM_PARSE_TOML_H
#define UCM_PARSE_TOML_H

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "toml.h"
#include "ucm_log.h"

#define MAX_LEN 256

#define FATAL(fmt, msgs...) _FATAL_((errno), __FILE__, fmt, ##msgs)
#define WARN(fmt,msgs...) _WARN_(fmt, ##msgs)

#define EXIST(x) _BOOL_(access(x,F_OK))
#define READ(x) _BOOL_(access(x,R_OK))

#define _BOOL_(x) (x == 0 ? true : false)

/* Configuration files need to contain information */
#define SRC "src"
#define DEST "dest"
#define FORCE "force"
#define MISSING "missing"

typedef struct CONFIG_TABLE CONFIG_TABLE;
typedef struct CONFIG_LIST CONFIG_LIST;


struct CONFIG_TABLE {
    int num;                    /* the number of configure */
    char **name;                /* the name list of configure */
    toml_table_t *toml_table;   /* the table of configure */
};

struct CONFIG_LIST
{
    char *name;     /* the name of configure in this table */

    char *src;   /* the source path list of configure in this table */
    char *dest;   /* the taget path list of configure in this table*/

    int force;     /* force symlinker flag */
    int enable;    /* enable flag */
    int exist;     /* target existence flag */

    CONFIG_LIST *next;
};

/// @brief Analyze configuration files
/// @param _config_file_ User's configure file
/// @return CONFIG_TABLE pointer
extern CONFIG_TABLE *parse_toml_file(const char *_config_file_);

/// @brief Extract the configuration into a chained table
/// @param _config_file_ User's configure file
/// @return Pointer to CONFIG_LIST type containing configuration information
extern CONFIG_LIST *dump_config_list(const char *_config_file_);

#endif