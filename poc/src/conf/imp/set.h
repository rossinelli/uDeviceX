static config_setting_t* subsetting(const char *desc, int type, config_setting_t *group) {
    config_setting_t *sub;
    sub = config_setting_lookup(group, desc);
    if (NULL == sub)
        sub = config_setting_add(group, desc, type);
    return sub;
}

static config_setting_t* get_subgroup_setting(int n, const char *desc[], config_t *c) {
    config_setting_t *group;
    group = config_root_setting(c);
    
    for (int i = 0; i < n; ++i)
        group = subsetting(desc[i], CONFIG_TYPE_GROUP, group);
    return group;    
}

static bool is_end(char c) {return c == '\0';}
static bool is_sep(char c) {return c == '.' || c == '/';}

static void cpy(const char *in, char *out) {
    while ( !is_end(*in) && !is_sep(*in)) {
        *out = *in;
        ++in; ++out;
    }
    *out = '\0';
}

static void split_str(const char *in, int *n, CBuf *out) {
    int i = 0;
    cpy(in, out->c[i++]);

    while ( !is_end(*in) ) {
        if ( is_sep(*in) ) cpy(++in, out->c[i++]);
        if (i > MAX_LEVEL) ERR("Too many levels in desc <%s> : found %d/%d\n", in, i, MAX_LEVEL);
        ++in;
    }
    *n = i;
}

static void to_ptr_array(const CBuf *buf, const char *ptr[]) {
    int i;
    for (i = 0; i < MAX_LEVEL; ++i) ptr[i] = buf->c[i];
}

static void set_int(const char *name, int a, config_t *c) {
    int n;
    CBuf buf;
    const char *desc[MAX_LEVEL];
    config_setting_t *group, *setting;
    int status;
    
    UC(split_str(name, &n, &buf));
    to_ptr_array(&buf, desc);
        
    group = get_subgroup_setting(n-1, desc, /**/ c);
    setting = subsetting(desc[n-1], CONFIG_TYPE_INT, /**/ group);
    
    status = config_setting_set_int(setting, a);
    if (CONFIG_TRUE != status)
        ERR("could not set <%s>", desc[n-1]);
}

static void set_vint(const char *name, int nelem, const int a[], config_t *c) {
    enum {APPEND = -1};
    int n;
    CBuf buf;
    const char *desc[MAX_LEVEL];
    config_setting_t *group, *setting, *status;
    int i;
        
    UC(split_str(name, &n, &buf));
    to_ptr_array(&buf, desc);
        
    group = get_subgroup_setting(n-1, desc, /**/ c);
    setting = subsetting(desc[n-1], CONFIG_TYPE_ARRAY, /**/ group);

    for (i = 0; i < nelem; ++i) {
        status = config_setting_set_int_elem(setting, APPEND, a[i]);
        if (NULL == status)
            ERR("could not set element %d/%d of <%s>", i, nelem, desc[n-1]);
    }
}

static void set_int3(const char *desc, int3 a, config_t *c) {
    const int a3[] = {a.x, a.y, a.z};
    UC(set_vint(desc, 3, a3, c));
}

static void set_float(const char *name, float a, config_t *c) {
    int n;
    CBuf buf;
    const char *desc[MAX_LEVEL];
    config_setting_t *group, *setting;
    int status;
    
    UC(split_str(name, &n, &buf));
    to_ptr_array(&buf, desc);
        
    group = get_subgroup_setting(n-1, desc, /**/ c);
    setting = subsetting(desc[n-1], CONFIG_TYPE_FLOAT, /**/ group);
    
    status = config_setting_set_float(setting, a);
    if (CONFIG_TRUE != status)
        ERR("could not set <%s>", desc[n-1]);
}

static void set_vfloat(const char *name, int nelem, const float a[], config_t *c) {
    enum {APPEND = -1};
    int n;
    CBuf buf;
    const char *desc[MAX_LEVEL];
    config_setting_t *group, *setting, *status;
    int i;

    UC(split_str(name, &n, &buf));
    to_ptr_array(&buf, desc);
        
    group = get_subgroup_setting(n-1, desc, /**/ c);
    setting = subsetting(desc[n-1], CONFIG_TYPE_ARRAY, /**/ group);

    for (i = 0; i < nelem; ++i) {
        status = config_setting_set_float_elem(setting, APPEND, a[i]);
        if (NULL == status)
            ERR("could not set element %d/%d of <%s>", i, nelem, desc[n-1]);
    }
}

static void set_float3(const char *name, float3 a, config_t *c) {
    const float a3[] = {a.x, a.y, a.z};
    UC(set_vfloat(name, 3, a3, c));
}

static void set_bool(const char *name, int a, config_t *c) {
    int n;
    CBuf buf;
    const char *desc[MAX_LEVEL];
    config_setting_t *group, *setting;
    int status;
    UC(split_str(name, &n, &buf));
    to_ptr_array(&buf, desc);
    
    group = get_subgroup_setting(n-1, desc, /**/ c);
    setting = subsetting(desc[n-1], CONFIG_TYPE_BOOL, /**/ group);
    
    status = config_setting_set_bool(setting, a);
    if (CONFIG_TRUE != status)
        ERR("could not set <%s>", desc[n-1]);
}

static void set_string(const char *name, const char *a, config_t *c) {
    int n;
    CBuf buf;
    const char *desc[MAX_LEVEL];
    config_setting_t *group, *setting;
    int status;
    UC(split_str(name, &n, &buf));
    to_ptr_array(&buf, desc);
    
    group = get_subgroup_setting(n-1, desc, /**/ c);
    setting = subsetting(desc[n-1], CONFIG_TYPE_STRING, /**/ group);
    
    status = config_setting_set_string(setting, a);
    if (CONFIG_TRUE != status)
        ERR("could not set <%s>", desc[n-1]);
}

static void set_vstring(const char *name, int nelem, const char *ss[], config_t *c) {
    enum {APPEND = -1};
    int n;
    CBuf buf;
    const char *desc[MAX_LEVEL];
    config_setting_t *group, *setting, *status;
    int i;

    UC(split_str(name, &n, &buf));
    to_ptr_array(&buf, desc);
        
    group = get_subgroup_setting(n-1, desc, /**/ c);
    setting = subsetting(desc[n-1], CONFIG_TYPE_ARRAY, /**/ group);

    for (i = 0; i < nelem; ++i) {
        status = config_setting_set_string_elem(setting, APPEND, ss[i]);
        if (NULL == status)
            ERR("could not set element %d/%d of <%s>", i, nelem, desc[n-1]);
    }
}


void conf_set_int(const char *desc, int a, Config *cfg) {
    UC(set_int(desc, a, &cfg->c[EXE]));
}

void conf_set_vint(const char *desc, int nelem, const int a[], Config *cfg) {
    UC(set_vint(desc, nelem, a, &cfg->c[EXE]));
}

void conf_set_int3(const char *desc, int3 a, Config *cfg) {
    UC(set_int3(desc, a, &cfg->c[EXE]));
}

void conf_set_float(const char *desc, float a, Config *cfg) {
    UC(set_float(desc, a, &cfg->c[EXE]));
}

void conf_set_vfloat(const char *desc, int nelem, const float a[], Config *cfg) {
    UC(set_vfloat(desc, nelem, a, &cfg->c[EXE]));
}

void conf_set_float3(const char *desc, float3 a, Config *cfg) {
    UC(set_float3(desc, a, &cfg->c[EXE]));
}

void conf_set_bool(const char *desc, int a, Config *cfg) {
    UC(set_bool(desc, a, &cfg->c[EXE]));
}

void conf_set_string(const char *desc, const char *a, Config *cfg) {
    UC(set_string(desc, a, &cfg->c[EXE]));
}

