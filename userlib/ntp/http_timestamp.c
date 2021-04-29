#include "mongoose.h"
#include "cJSON.h"
#include "misc_http/misc_http.h"
int do_timestamp(struct MiscHttp_StMiscHttpObj *pHttpObj, char *data, int len)
{
    int ret = -1;
    cJSON* cjson = 0;
    char str_timestamp[100] = { 0 };
    if (pHttpObj == 0 || data == 0 || len <= 0) return -1;

    cjson = cJSON_Parse(data);
    if (cjson && pHttpObj->user)
    {
        cJSON* sub = cJSON_GetObjectItem(cjson, "data");
        if (sub)
        {
            sub = cJSON_GetObjectItem(sub, "t");
            if (sub->valuestring) {
                memset(str_timestamp, 0, sizeof(str_timestamp));
                memcpy(str_timestamp, sub->valuestring, 10);
                *(unsigned int*)pHttpObj->user = atol(str_timestamp);
                ret = 0;
            }
            //printf("info: timestamp = %d !\r\n", *(unsigned int*)pHttpObj->user);
        }
    }
    if (cjson) {
        cJSON_Delete(cjson);
    }
    return ret;
}

int timestamp_http(unsigned int *timestamp, int timeout_ms)
{
    int ret = 0;
    MiscHttp_StMiscHttpObj http_obj;
//    misc_http_init(&http_obj, do_timestamp, timestamp
    MiscHttpInit(&http_obj, do_timestamp, timestamp);
//  ret = misc_http_get(&http_obj, "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp", timeout_ms);
    ret = MiscHttpGet(&http_obj, "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp", timeout_ms);
    if (ret >= 0) {
        printf("info: timestamp_http get successfully!\r\n");
    }
    else {
        printf("info: timestamp_http get failed!\r\n");
    }
//  misc_http_deinit(&http_obj);
    MiscHttpDeinit(&http_obj);
    return ret;
}
