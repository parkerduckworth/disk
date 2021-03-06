#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "extern.h"
#include "jsmn.h"
#include "jsmn.c"

static unsigned int set_option(jsmnerr_t ret, jsmntok_t *t, char *config);
static unsigned int jsoneq(const char *json, jsmntok_t *tok, const char *s);
static int eval_json(char *config, jsmntok_t *tok_key, jsmntok_t *tok_val, const char *opt_name);

int set_config(char *c_file)
{
    int i, j, opt;
    char *config;
    jsmn_parser p;
    jsmnerr_t r;
    jsmntok_t *tp;
    jsmntok_t t[128];  /* We expect no more than 128 tokens */

    tp = t;
    config = pull_file(c_file);

    jsmn_init(&p);
    r = jsmn_parse(&p, config, strlen(config), t, sizeof(t)/sizeof(t[0]));
    if (r < 0) {
        printf("\nError log: Failed to parse JSON: %d\n", r);
        return 1;
    }

    // Assume the top-level element is an object
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("\nError log: Settings file must be formatted with correct JSON syntax\n");
        return 1;
    }

    opt = set_option(r, tp, config);
    free(config);

    return opt;
}


/* Fill an array with file contents. */
char* pull_file(char *fname)
{
    FILE *fp;
    char *buf;
    size_t sz;

    if ((fp = fopen(fname, "r")) == NULL) {
        printf("Error: unable to pull file: %s\n", fname);
        exit(1);
    }
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);

    // You are responsible for freeing 'buf' when it is passed to another function
    buf = malloc(sizeof(char) * sz + 1);
    buf[sz] = '\0';

    rewind(fp);
    fread(buf, sz, 1, fp);

    if (fclose(fp) != 0)
        perror("fclose");
    return buf;
}


static unsigned int set_option(jsmnerr_t ret, jsmntok_t *t, char *config)
{
    int i, j, opt;

    /* Option names must remain in order with config.json 
     * TODO: Dynamically generate this list.
     */
    const char *options[] = {"auto open", "pattern match", "case sensitivity", "permission errors", 
                                "search user files", "search system files"};
        
    // Loop over all keys of the root object
    for (i = 1, j = 0; i < ret; i++, j++) {
        if ((opt = eval_json(config, &t[i], &t[i + 1], options[j])) != 0) {

            // Cases must be listed in order with lines in config.json
            switch(j) {
                case 0:
                    option.openf = (opt > 0) ? 1 : 0;
                    break;
                case 1:
                    option.pmatch = (opt > 0) ? 1 : 0;
                    break;
                case 2:
                    option.csens = (opt > 0) ? 1 : 0;
                    break;
                case 3:
                    option.perm = (opt > 0) ? 1 : 0;
                    break;
                case 4:
                    option.home = (opt > 0) ? 1 : 0;
                    break;
                case 5:
                    option.sys = (opt > 0) ? 1 : 0;
                    break;
                default:
                    // Will only occur if for loop fails to iterate through 'j'
                    printf("Error log: set_config() in config.c failed\n");
                    exit(1);
            }
        } else {
            printf("Error log: Option %s not set\n", options[j]);
            return 1;
        }
        i++; // Skip to next key, value pair (&t[i], &t[i+1])
    }
    return 0;
}


static int eval_json(char *config, jsmntok_t *tok_key, jsmntok_t *tok_val, const char *opt_name)
{
    int ret = 0;
    if (jsoneq(config, tok_key, opt_name) == 0) {
            if (!strncmp("true", config + tok_val->start, tok_val->end-tok_val->start))
                ret++;
            else if (!strncmp("false", config + tok_val->start, tok_val->end-tok_val->start))
                ret--;
            else
                printf("Error log: Invalid value for \"%s\" in config.json\n", opt_name);
        }
    return ret;
}


static unsigned int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return 1;
}


/* Allows config file to be accessed from within any directory */
char *build_cfile_path(char *path)
{
    // You must free 'abspath' when passed to another function
    char *abspath = malloc(sizeof(char) * PATH_MAX);
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);

    if (!pwd) {
        return NULL;
    }
    const char *user = pwd->pw_name;

    strcpy(abspath, pwd->pw_dir);
    strcat(abspath, "/");
    strcat(abspath, path);

    return abspath;
}
