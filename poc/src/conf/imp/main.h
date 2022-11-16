static void set_write_options(config_t *c) {
    config_set_option(c, CONFIG_OPTION_SEMICOLON_SEPARATORS,            CONFIG_FALSE);
    config_set_option(c, CONFIG_OPTION_COLON_ASSIGNMENT_FOR_GROUPS,     CONFIG_FALSE);
    config_set_option(c, CONFIG_OPTION_COLON_ASSIGNMENT_FOR_NON_GROUPS, CONFIG_FALSE);
    config_set_option(c, CONFIG_OPTION_OPEN_BRACE_ON_SEPARATE_LINE,     CONFIG_FALSE);    
    config_set_option(c, CONFIG_OPTION_ALLOW_SCIENTIFIC_NOTATION,       CONFIG_TRUE );
}

void conf_ini(/**/ Config **pq) {
    Config *q;
    EMALLOC(1, &q);
    for (int i = 0; i < NCFG; ++i) {
        config_init(q->c + i);
        set_write_options(q->c + i);
    }
    
    EMALLOC(1, &q->r);
    config_init(q->r);
    set_write_options(q->r);
    *pq = q;
}

void conf_fin(/**/ Config *q) {
    int i;
    for (i = 0; i < NCFG; ++i)
        config_destroy(&q->c[i]);
    config_destroy(q->r);
    EFREE(q->r);
    EFREE(q);
}

static void concatenate(int n, char **ss, /**/ char *a) {
    char *s;
    a[0] = '\0';

    for(int i = 0; i < n; ++i) {
        s = ss[i];
        strcat(a, s);
        strcat(a, " ");
    }
}

static void read_file(const char *fname, /**/ config_t *c) {
    msg_print("read '%s'", fname);
    if (!config_read_file(c, fname))
        ERR( "%s:%d - %s\n", config_error_file(c),
             config_error_line(c), config_error_text(c));
}

static void read_args(int argc, char **argv, /**/ config_t *c) {
   enum {MAX_CHAR = 100000};
   char *args;

   EMALLOC(MAX_CHAR, &args);

   concatenate(argc, argv, /**/ args);
   if (!config_read_string(c, args)) {
       msg_print("read args: %s", args);
       ERR("%d - %s\n",
           config_error_line(c), config_error_text(c));
   }

   EFREE(args);
}

static void shift(int *c, char ***v) {
    (*c) --;
    (*v) ++;
}

static int get_opt_file(int *argc, char ***argv, /**/ char fname[]) {
    char *lastpnt, *a;
    int differ;

    if (*argc) {
        a = (*argv)[0];
        lastpnt = strrchr(a, '.');

        if (lastpnt != NULL) {
            differ = strcmp(lastpnt, ".cfg");
            if (differ) return 0;
            strcpy(fname, a);
            shift(argc, argv);
            return 1;
        }
    }
    return 0;
}

static void set_include_dir(const char *path, Config *cfg) {
    for (int i = 0; i < NCFG; ++i)
        config_set_include_dir(&cfg->c[i], path);
}

void conf_read(int argc, char **argv, /**/ Config *cfg) {
    char *home, defname[1024] = {0}, optname[1024], definclude[1024] = {0};
    home = getenv("HOME");

    // default include dir
    strcpy(definclude, home);
    strcat(definclude, "/.udx/");
    set_include_dir(definclude, cfg);
    
    // default parameters file
    strcpy(defname, home);
    strcat(defname, "/.udx/default.cfg");

    UC(read_file(defname, /**/ &cfg->c[DEF]));

    if (get_opt_file(&argc, &argv, /**/ optname)) {
        UC(read_file(optname, /**/ &cfg->c[OPT]));
    }

    if (argc)
        UC(read_args(argc, argv, /**/ &cfg->c[ARG]));
}

void conf_write_exe(const Config *cfg, FILE *stream) {
    config_write(&cfg->c[EXE], stream);
}

void conf_write_history(const Config *cfg, FILE *stream) {
    config_write(cfg->r, stream);
}
