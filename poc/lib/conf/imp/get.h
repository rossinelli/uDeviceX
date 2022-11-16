static bool found(const config_setting_t *s) {return s != NULL;}
static void treat_status(int status, const char *desc, const char *type) {
    if (status == NOTFOUND)
        ERR("Could not find <%s>\n", desc);
    else if (status == WTYPE)
        ERR("<%s> has wrong type: expected %s\n", desc, type);
}

static int lookup_setting(const Config *c, const char *desc, config_setting_t **s) {
    int i;
    for (i = 0; i < NCFG; ++i) {
        *s = config_lookup(c->c + i, desc);
        if ( found(*s) ) return OK;
    }
    return NOTFOUND;
}

static int lookup_int(const Config *c, const char *desc, int *a) {
    config_setting_t *s;
    int status;
    status = lookup_setting(c, desc, /**/ &s);
    if (status != OK) return status;
    if (config_setting_type(s) != CONFIG_TYPE_INT) return WTYPE;
    *a = config_setting_get_int(s);
    set_int(desc, *a, c->r);
    return status;
}

static int lookup_float(const Config *c, const char *desc, float *a) {
    config_setting_t *s;
    int status;
    status = lookup_setting(c, desc, /**/ &s);
    if (status != OK) return status;
    if (config_setting_type(s) != CONFIG_TYPE_FLOAT) return WTYPE;
    *a = config_setting_get_float(s);
    set_float(desc, *a, c->r);
    return status;
}

static int lookup_bool(const Config *c, const char *desc, int *a) {
    config_setting_t *s;
    int status;
    status = lookup_setting(c, desc, /**/ &s);
    if (status != OK) return status;
    if (config_setting_type(s) != CONFIG_TYPE_BOOL) return WTYPE;
    *a = config_setting_get_bool(s);
    set_bool(desc, *a, c->r);
    return status;
}

static int lookup_string(const Config *c, const char *desc, const char **a) {
    config_setting_t *s;
    int status;
    status = lookup_setting(c, desc, /**/ &s);
    if (status != OK) return status;
    if (config_setting_type(s) != CONFIG_TYPE_STRING) return WTYPE;
    *a = config_setting_get_string(s);
    set_string(desc, *a, c->r);
    return status;
}

static int lookup_vint(const Config *c, const char *desc, int maxn, int *n, int a[]) {
    config_setting_t *s, *e;
    int j, status;
    status = lookup_setting(c, desc, /**/ &s);
    if (status != OK) return status;
    if (config_setting_type(s) != CONFIG_TYPE_ARRAY) return WTYPE;
    *n = config_setting_length(s);
    if (*n > maxn) ERR("%s: too many components: %d / %d", desc, *n, maxn);
    for (j = 0; j < *n; ++j) {
        e = config_setting_get_elem(s, j);
        if (config_setting_type(e) != CONFIG_TYPE_INT) return WTYPE;
        a[j] = config_setting_get_int(e);
    }
    set_vint(desc, *n, a, c->r);
    return status;
}

static int lookup_int3(const Config *c, const char *desc, int3 *a) {
    enum {X, Y, Z};
    int status, n = 0, f[3];
    status = lookup_vint(c, desc, 3, &n, f);
    if (status != OK) return status;
    if (n != 3)
        ERR("fail to read `%s`: int3 must have 3 components, found %d", desc, n);
    a->x = f[X];
    a->y = f[Y];
    a->z = f[Z];
    return status;
}

static int lookup_vfloat(const Config *c, const char *desc, int maxn, int *n, float a[]) {
    config_setting_t *s, *e;
    int j, status;
    status = lookup_setting(c, desc, /**/ &s);
    if (status != OK) return status;
    if (config_setting_type(s) != CONFIG_TYPE_ARRAY) return WTYPE;
    *n = config_setting_length(s);

    if (*n > maxn) ERR("%s: too many components: %d / %d", desc, *n, maxn);
    for (j = 0; j < *n; ++j) {
        e = config_setting_get_elem(s, j);
        if (config_setting_type(e) != CONFIG_TYPE_FLOAT) return WTYPE;
        a[j] = config_setting_get_float(e);
    }
    set_vfloat(desc, *n, a, c->r);
    return status;
}

static int lookup_float3(const Config *c, const char *desc, float3 *a) {
    enum {X, Y, Z};
    int n = 0, status;
    float f[3];
    status = lookup_vfloat(c, desc, 3, &n, f);
    if (status != OK) return status;
    if (n != 3)
        ERR("fail to read `%s`: float3 must have 3 components, found %d", desc, n);
    a->x = f[X];
    a->y = f[Y];
    a->z = f[Z];
    return status;
}

static int lookup_vstring(const Config *c, const char *desc, int maxn, int *n, const char **ss) {
    config_setting_t *s, *e;
    int j, status;
    status = lookup_setting(c, desc, /**/ &s);
    if (status != OK) return status;
    if (config_setting_type(s) != CONFIG_TYPE_ARRAY) return WTYPE;
    *n = config_setting_length(s);

    if (*n > maxn) ERR("%s: too many components: %d / %d", desc, *n, maxn);
    for (j = 0; j < *n; ++j) {
        e = config_setting_get_elem(s, j);
        if (config_setting_type(e) != CONFIG_TYPE_STRING) return WTYPE;
        ss[j] = config_setting_get_string(e);
    }
    set_vstring(desc, *n, ss, c->r);
    return status;
}

void conf_lookup_int(const Config *c, const char *desc, int *a) {
    int status = lookup_int(c, desc, a);
    UC(treat_status(status, desc, "int"));
}

void conf_lookup_float(const Config *c, const char *desc, float *a) {
    int status = lookup_float(c, desc, a);
    UC(treat_status(status, desc, "float"));
}

void conf_lookup_bool(const Config *c, const char *desc, int *a) {
    int status = lookup_bool(c, desc, a);
    UC(treat_status(status, desc, "bool"));
}

void conf_lookup_string(const Config *c, const char *desc, const char **a) {
    int status = lookup_string(c, desc, a);
    UC(treat_status(status, desc, "string"));
}

void conf_lookup_vint(const Config *c, const char *desc, int maxn, int *n, int a[]) {
    int status = lookup_vint(c, desc, maxn, n, a);
    UC(treat_status(status, desc, "array of int"));
}

void conf_lookup_int3(const Config *c, const char *desc, int3 *a) {
    int status = lookup_int3(c, desc, a);
    UC(treat_status(status, desc, "array of int"));
}

void conf_lookup_vfloat(const Config *c, const char *desc, int maxn, int *n, float a[]) {
    int status = lookup_vfloat(c, desc, maxn, n, a);
    UC(treat_status(status, desc, "array of float"));
}

void conf_lookup_float3(const Config *c, const char *desc, float3 *a) {
    int status = lookup_float3(c, desc, a);
    UC(treat_status(status, desc, "array of float"));
}

void conf_lookup_vstring(const Config *c, const char *desc, int maxn, int *n, const char **ss) {
    int status = lookup_vstring(c, desc, maxn, n, ss);
    UC(treat_status(status, desc, "array of string"));
}


static void get_desc(const char *ns, const char *d, char *desc) {
    sprintf(desc, "%s.%s", ns, d);
}

void conf_lookup_int_ns(const Config *c, const char *ns, const char *d, int *a) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_int(c, desc, a));
}

void conf_lookup_float_ns(const Config *c, const char *ns, const char *d, float *a) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_float(c, desc, a));
}

void conf_lookup_bool_ns(const Config *c, const char *ns, const char *d, int *a) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_bool(c, desc, a));
}

void conf_lookup_string_ns(const Config *c, const char *ns, const char *d, const char **a) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_string(c, desc, a));
}

void conf_lookup_vint_ns(const Config *c, const char *ns, const char *d, int maxn, int *n, int a[]) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_vint(c, desc, maxn, n, a));
}

void conf_lookup_int3_ns(const Config *c, const char *ns, const char *d, int3 *a) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_int3(c, desc, a));
}

void conf_lookup_vfloat_ns(const Config *c, const char *ns, const char *d, int maxn, int *n, float a[]) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_vfloat(c, desc, maxn, n, a));
}

void conf_lookup_float3_ns(const Config *c, const char *ns, const char *d, float3 *a) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_float3(c, desc, a));
}

void conf_lookup_vstring_ns(const Config *c, const char *ns, const char *d, int maxn, int *n, const char **ss) {
    char desc[FILENAME_MAX];
    get_desc(ns, d, desc);
    UC(conf_lookup_vstring(c, desc, maxn, n, ss));
}



bool conf_opt_int(const Config *c, const char *desc, int *a) {
    return OK == lookup_int(c, desc, a);
}

bool conf_opt_float(const Config *c, const char *desc, float *a)  {
    return OK == lookup_float(c, desc, a);
}

bool conf_opt_bool(const Config *c, const char *desc, int *a)  {
    return OK == lookup_bool(c, desc, a);
}

bool conf_opt_string(const Config *c, const char *desc, const char **a)  {
    return OK == lookup_string(c, desc, a);
}

bool conf_opt_vint(const Config *c, const char *desc, int maxn, int *n, int a[]) {
    return OK == lookup_vint(c, desc, maxn, n, a);
}

bool conf_opt_int3(const Config *c, const char *desc, int3 *a) {
    return OK == lookup_int3(c, desc, a);
}

bool conf_opt_vfloat(const Config *c, const char *desc, int maxn, int *n, float a[]) {
    return OK == lookup_vfloat(c, desc, maxn, n, a);
}

bool conf_opt_float3(const Config *c, const char *desc, float3 *a) {
    return OK == lookup_float3(c, desc, a);
}

bool conf_opt_vstring(const Config *c, const char *desc, int maxn, int *n, const char **ss) {
    return OK == lookup_vstring(c, desc, maxn, n, ss);
}
