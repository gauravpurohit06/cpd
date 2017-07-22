#include "common_helper.h"

gboolean get_boolean(const char *g)
{
    if (!g)
        return FALSE;

    if (g_str_equal(g, "true"))
        return TRUE;

    return FALSE;
}

char *get_string_copy(const char *str)
{
    if (str == NULL)
        return NULL;
    char *s = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(s, str);
    return s;
}

void unpack_string_array(GVariant *variant, int num_val, char ***val)
{
    GVariantIter *iter;

    if (!num_val)
    {
        printf("No supported values found.\n");
        *val = NULL;
        return;
    }
    char **array = (char **)malloc(sizeof(char *) * num_val);

    g_variant_get(variant, "a(s)", &iter);

    int i = 0;
    char *str;
    for (i = 0; i < num_val; i++)
    {
        g_variant_iter_loop(iter, "(s)", &str);
        array[i] = get_string_copy(str);
        printf(" %s\n", str);
    }
    *val = array;
}

void unpack_option_array(GVariant *var, int num_options, Option **options)
{
    Option *opt = (Option *)(malloc(sizeof(Option) * num_options));
    int i, j;
    char *str;
    GVariantIter *iter;
    GVariantIter *array_iter;
    char *name, *default_val;
    int num_sup;
    g_variant_get(var, "a(ssia(s))", &iter);
    for (i = 0; i < num_options; i++)
    {
        //printf("i = %d\n", i);

        g_variant_iter_loop(iter, "(ssia(s))", &name, &default_val,
                            &num_sup, &array_iter);
        opt[i].option_name = get_string_copy(name);
        opt[i].default_value = get_string_copy(default_val);
        opt[i].num_supported = num_sup;
        opt[i].supported_values = new_cstring_array(num_sup);
        for (j = 0; j < num_sup; j++)
        {
            g_variant_iter_loop(array_iter, "(s)", &str);
            opt[i].supported_values[j] = get_string_copy(str); //mem
        }
        print_option(&opt[i]);
    }

    *options = opt;
}

GVariant *pack_string_array(int num_val, char **val)
{
    GVariantBuilder *builder;
    GVariant *values;
    builder = g_variant_builder_new(G_VARIANT_TYPE("a(s)"));
    for (int i = 0; i < num_val; i++)
    {
        // g_message("%s", val[i]);
        g_variant_builder_add(builder, "(s)", val[i]);
    }

    if (num_val == 0)
        g_variant_builder_add(builder, "(s)", "NA");

    values = g_variant_new("a(s)", builder);
    return values;
}

GVariant *pack_option(const Option *opt)
{
    GVariant **t = g_new(GVariant *, 4);
    t[0] = g_variant_new_string(opt->option_name);
    t[1] = g_variant_new_string(opt->default_value); //("s", get_string_copy(opt->default_value));
    t[2] = g_variant_new_int32(opt->num_supported);
    t[3] = pack_string_array(opt->num_supported, opt->supported_values);
    GVariant *tuple_variant = g_variant_new_tuple(t, 4);
    g_free(t);
    return tuple_variant;
}

char *get_absolute_path(char *file_path)
{
    if (!file_path)
        return NULL;

    if (file_path[0] == '/')
        return file_path;

    if (file_path[0] == '~')
    {
        struct passwd *passwdEnt = getpwuid(getuid());
        char *home = passwdEnt->pw_dir;
        char fp[1024], suffix[512];
        strcpy(suffix, file_path + 2);
        sprintf(fp, "%s/%s", home, suffix);
        return get_string_copy(fp);
    }
    char fp[1024], cwd[512];
    getcwd(cwd, sizeof(cwd));
    sprintf(fp, "%s/%s", cwd, file_path);
    printf("%s\n", fp);
    return get_string_copy(fp);
}
void unpack_job_array(GVariant *var, int num_jobs, Job *jobs)
{
    int i;
    char *str;
    GVariantIter *iter;
    g_variant_get(var, "a(isssssi)", &iter);
    int jobid, size;
    char *title, *printer, *user, *state, *submit_time;
    for (i = 0; i < num_jobs ; i++)
    {
        g_variant_iter_loop(iter, "(isssssi)", &jobid, &title, &printer , &user, &state, &submit_time , &size);
        jobs[i].job_id = jobid;
        jobs[i].title = get_string_copy(title);
        jobs[i].printer = get_string_copy(printer);
        jobs[i].user = get_string_copy(user);
        jobs[i].state = get_string_copy(state);
        jobs[i].submitted_at = get_string_copy(submit_time);
        jobs[i].size = size;

        printf("Printer %s ; state %s \n",printer, state);
    }
}
/**************Option************************************/
void print_option(const Option *opt)
{
    g_message("%s", opt->option_name);
    int i;
    for (i = 0; i < opt->num_supported; i++)
    {
        printf(" %s\n", opt->supported_values[i]);
    }
    printf("****DEFAULT: %s\n", opt->default_value);
}
