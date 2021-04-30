#ifndef __MISC_HTTP_H__
#define __MISC_HTTP_H__

enum {
    HTTP_MISC_HEADER = 0,
    HTTP_MISC_BODY,
    HTTP_MISC_END,
    HTTP_MISC_ERROR,
};

typedef struct MiscHttp_StNameValue {
    char *name;
    char *value;
}MiscHttp_StNameValue;


struct MiscHttp_StMiscHttpObj;
typedef int(*FUN_PARSER)(struct MiscHttp_StMiscHttpObj *pHttpObj, char *data, int len);
typedef struct MiscHttp_StMiscHttpObj {
    char *url;
    int is_break;
    int status;
    FUN_PARSER parser;
    void *user;
}MiscHttp_StMiscHttpObj;

int MiscHttpPostContent(MiscHttp_StMiscHttpObj *http_obj, char *url, char *add_header, char *content, int timeout_ms);
int MiscHttpPostForm(MiscHttp_StMiscHttpObj *http_obj, char *url, char *boundary, struct MiscHttp_StNameValue *arr_content, int arr_cnts, int timeout_ms);
int MiscHttpGet(MiscHttp_StMiscHttpObj *http_obj, char *url, int timeout_ms);
int MiscHttpInit(struct MiscHttp_StMiscHttpObj *http_obj, FUN_PARSER my_parser, void *user);
int MiscHttpDeinit(struct MiscHttp_StMiscHttpObj *http_obj);

#endif
