#include "ucm_parse_toml.h"

CONFIG_TABLE *parse_toml_file(const char *_config_file_)
{
    FILE *fp;
    char buf[MAX_LEN];
    char errbuf[MAX_LEN];
    int s_start;
    int s_len;
    toml_table_t *config_table_parse;
    CONFIG_TABLE *config_table;
    char *n;
    int t;

    /* initialization */
    t = 0;
    errno = 0;
    config_table = (CONFIG_TABLE *)malloc(sizeof(CONFIG_TABLE));

    /* parse toml file */
    if (!EXIST(_config_file_))
    {
        errno = ENOENT;
        FATAL("%s don't exist!", _config_file_);
    }

    if (!READ(_config_file_))
    {
        FATAL("unable to read %s!", _config_file_);
    }

    fp = fopen(_config_file_, "r");
    if (!fp)
    { 
        fclose(fp);
        FATAL("connot open %s!", _config_file_);
    }

    config_table_parse = toml_parse_file(fp, errbuf, sizeof(errbuf));
    if (!config_table_parse)
    {
        toml_free(config_table_parse);
        fclose(fp);
        FATAL("connot parse %s! [%s]", _config_file_, errbuf);
    }

    config_table->num = toml_table_ntab(config_table_parse);
    config_table->toml_table = config_table_parse;

    /* extract name list */
    config_table->name = (char **)malloc(sizeof(char *) * config_table->num);
    if (!config_table->name)
    {
        FATAL("don't allocate memory!");
    }

    rewind(fp);
    while (!feof(fp))
    {
        n = fgets(buf, sizeof(buf), fp);
        if (!n)
        {
            FATAL("file(%s) read exception!", _config_file_);
        }

        if (!strchr(buf, '[') && !strchr(buf, ']'))
        {
            continue;
        }

        s_len = strchr(buf, ']') - strchr(buf, '[') - 1;
        s_start = strchr(buf, '[') - buf + 1;
        config_table->name[t] = (char *)malloc(sizeof(char) * (s_len + 1));
        memcpy(config_table->name[t], buf + s_start, s_len);
        
        if (t < config_table->num)
        {
            ++t;
        }
    }

    /* end */
    fclose(fp);

    return config_table;
}

CONFIG_LIST *dump_config_list(const char *_config_file_)
{
    CONFIG_LIST *config_list;
    CONFIG_LIST *pt;
    CONFIG_TABLE *_parse_toml_;
    toml_table_t *_raw_config;
    toml_table_t *config;
    toml_datum_t config_info;
    int num;

    /* initialization */
    errno = 0;
    _parse_toml_ = parse_toml_file(_config_file_);
    num = _parse_toml_->num;
    config_list = (CONFIG_LIST *)malloc(sizeof(CONFIG_LIST));
    pt = config_list;
    _raw_config = _parse_toml_->toml_table;

    /* dump configure infomation */
    for (int i = 0; i < num; i++)
    {
        pt->name = _parse_toml_->name[i];
        config = toml_table_in(_raw_config, pt->name);
        if (!config)
        {
            WARN("don't find %s from %s!", pt->name, _config_file_);
            continue;
        }

        config_info = toml_string_in(config, SRC);
        if (!config_info.ok)
        {
            WARN("%s missing %s!", pt->name, SRC);
            pt->src = MISSING;
        } else {
            pt->src = config_info.u.s;
        }

        config_info = toml_string_in(config, DEST);
        if (!config_info.ok)
        {
            WARN("%s missing %s!", pt->name,DEST);
            pt->dest = MISSING;
        } else {
            pt->dest = config_info.u.s;
        }

        config_info = toml_bool_in(config, FORCE);
        if (!config_info.ok)
        {
            pt->force = false;
        } else {
            pt->force = config_info.u.b;
        }

        if (!strcmp(pt->src, MISSING) && strcmp(pt->dest, MISSING))
        {
            pt->enable = 1;
        } else {
            pt->enable = 0;
        }

        /* move pt to next list */
        if (i == (num - 1))
        {
            pt->next = NULL;
            break;
        } else {
            pt->next = (CONFIG_LIST *)malloc(sizeof(CONFIG_LIST));
            pt = pt->next;
        }
    }

    /* end */
    toml_free(_parse_toml_->toml_table);

    return config_list;
}