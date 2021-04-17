//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cpu_peri.h>
#include <misc/md5.h>
#include "component_config_wifi.h"


/** Swap the bytes in an uint16_t: much like lwip_htons() for little-endian */
#ifndef SWAP_BYTES_IN_WORD
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
#endif /* SWAP_BYTES_IN_WORD */

#ifndef FOLD_U32T
#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))
#endif


uint16_t wlan_standard_chksum(const void *dataptr, int len)
{
  const uint8_t *pb = (const uint8_t *)dataptr;
  const uint16_t *ps;
  uint16_t t = 0;
  uint32_t sum = 0;
  int odd = ((uint32_t)pb & 1);

  /* Get aligned to uint16_t */
  if (odd && len > 0) {
    ((uint8_t *)&t)[1] = *pb++;
    len--;
  }

  /* Add the bulk of the data */
  ps = (const uint16_t *)(const void *)pb;
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* Consume left-over byte, if any */
  if (len > 0) {
    ((uint8_t *)&t)[0] = *(const uint8_t *)ps;
  }

  /* Add end bytes */
  sum += t;

  /* Fold 32-bit sum to 16 bits
     calling this twice is probably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  /* Swap if alignment was odd */
  if (odd) {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return (uint16_t)sum;
}

//int wlan_fast_connect_info_read(wlan_fast_connect_t *data_info)
//{
//    int result = 0;
//    uint32_t crc1, crc2;
//    uint32_t data[(sizeof(struct wlan_fast_connect) + 4 - 1) / 4 + 1 ];
//
//    if(data_info==NULL)
//        return -1;
//
//    memset(data, 0, sizeof(struct wlan_fast_connect));
//
//    File_GetNameValueFs(CFG_FAST_DATA_FILE_NAME,data,sizeof(data));
//    crc1 = data[(sizeof(struct wlan_fast_connect) + 4 - 1 ) / 4];
//    crc2 = wlan_standard_chksum(data, sizeof(struct wlan_fast_connect));
//    if ((data[0] != ~0x0)   && (crc1 == crc2)) // 0xFFFFFFFF
//    {
//        memcpy(data_info, data, sizeof(struct wlan_fast_connect));
//        result = 0;
//    }
//    else
//    {
//        printf("fast_connect ap info crc failed\r\n");
//        result = -1;
//    }
//    if( (data_info->channel < 1) || (data_info->channel > 13) )
//    {
//        printf("[%s] channel out of range! \n", __FUNCTION__);
//        result = -2;
//    }
//    return result;
//}

void hex_dump(const char *desc, const void *addr, const int len);

/*
* ssid: 需要连接ssid
* passwd: 需要连接ssid对应的密码，如果passwd,就只匹配ssid, 并使用记录中ssid的对应的密码
* out_info: 如果查找成功，返回快连信息
* 返回值： 返回0，没快连信息； 返回1,获取快连信息成功
*/
#define BLOCK_SIZE 4*1024

#if (CN_BEKEN_SDK_V3 == 1) //chenws: 主要是SDK3.0更换快连方式，这里配合处理
#include "role_launch.h"
int wlan_fast_info_match(char *ssid, char *passwd, RL_BSSID_INFO_PTR out_info)
{
    int ret = 0;
    RL_BSSID_INFO_T *p = 0;
    FILE *fd = NULL;

    if(ssid==0 /* || passwd==0 */) return -1;

    u8 *new_buf = (u8*)malloc(BLOCK_SIZE);
    if(new_buf==0) return -1;


    fd = fopen(CFG_FAST_DATA_FILE_NAME,"r+");
    if(fd)
    {
        ret = fread(new_buf, 1, BLOCK_SIZE, fd);
        if(fclose(fd) == -1)
            printf("close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
    }
    if(ret <= 0) {
        goto FUN_RET;
    }
    int cnt = ret/sizeof(RL_BSSID_INFO_T);
    printf("info: %s, cnt=%d, ret=%d, size_item=%d!\r\n", __FUNCTION__, cnt, ret, sizeof(RL_BSSID_INFO_T));
    p = (RL_BSSID_INFO_T*)&new_buf[ret];//指向存放下个数据的开始位置
    int idx = 0; //逆向查找
    while(cnt > 0) {
        --idx;
        if (strcmp(p[idx].ssid, ssid) == 0 ){
            if (passwd) {
                if(strcmp(p[idx].pwd, passwd) != 0){//密码不匹配
                    cnt--;
                    continue;
                }
            }
            if (p[idx].ssid[0] != 0xFF &&
                p[idx].channel >= 1 &&
                p[idx].channel <= 13 &&
                out_info) {
                printf("info: %s, matched ok, Do WiFi Quick Connnecting ... \r\n", __FUNCTION__);
                memcpy(out_info, &p[idx], sizeof(RL_BSSID_INFO_T));
                ret = 1;
                break;
            }
        }
        cnt--;
    }
    if(cnt<=0) {
        printf("info: %s, matched failed, Do Normal WiFi Connecting ... \r\n", __FUNCTION__);
        ret = 0;
    }

FUN_RET:
    if(new_buf) free(new_buf);
    return ret;
}

int CompFastInfo(RL_BSSID_INFO_PTR src, RL_BSSID_INFO_PTR  dest)
{
    if (src == 0 || dest==0) return -1;
    if (strcmp(src->ssid, dest->ssid) != 0) {
        //printf("info: %s, src->ssid=%s, dest->ssid=%s!\r\n", __FUNCTION__, src->ssid, dest->ssid);
        return -1;
    }
    if (memcmp(src->bssid, dest->bssid, sizeof(src->bssid)) != 0){
        //hex_dump("warnning src bssid", src->bssid, sizeof(src->bssid));
        //hex_dump("warnning dest bssid", dest->bssid, sizeof(dest->bssid));
        printf("info: %s, src bssid != dest bssid!\r\n", __FUNCTION__);
        return -2;
    }
    if (src->channel != dest->channel) {
        printf("warning: src->channel=%d, dest->channel=%d!\r\n", src->channel, dest->channel);
        return -3;
    }
    if (src->security != dest->security) {
        printf("warning: src->security=%d, dest->security=%d!\r\n", src->security, dest->security);
        return -3;
    }
    if (memcmp(src->psk, dest->psk, sizeof(src->psk)) != 0){
        //hex_dump("warnning src psk", src->psk, sizeof(src->psk));
        //hex_dump("warnning dest psk", dest->psk, sizeof(dest->psk));
        printf("info: %s, src psk != dest psk!\r\n", __FUNCTION__);
        return -5;
    }
    return 0;
}

int wlan_fast_connect_info_write(RL_BSSID_INFO_PTR data_info)
{
    int ret = 0;
    RL_BSSID_INFO_T *p = 0;
    struct stat file_state;
    FILE *fd = NULL;
    if(data_info == NULL)
      return -1;

    u8 *new_buf = (u8*)malloc(BLOCK_SIZE);
    if(new_buf==0) return -1;

    memset(new_buf, 0, BLOCK_SIZE);

    fd = fopen(CFG_FAST_DATA_FILE_NAME,"r+");
    if(fd)
    {
        ret = fread(new_buf, 1, BLOCK_SIZE, fd);
        if(fclose(fd) == -1)
            printf("close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
    }
    if(ret < 0) {
        goto FUN_RET;
    }

    int cnt = ret/sizeof(RL_BSSID_INFO_T);
    p = (RL_BSSID_INFO_T*)&new_buf[ret];//指向存放下个数据的开始位置
    int idx = 0; //逆向查找
    while(cnt > 0) {
        idx--;
        if (CompFastInfo(&p[idx], data_info)==0){
            printf("info: %s, mathed item, index=%d, Do Not Writed New, Do Nothing!!!\r\n", __FUNCTION__, cnt-1);
            break;
        }
        cnt--;
    }
    if(cnt<=0) {//找不到，重新写入
        printf("info: %s, New Item Will Be Writed!\r\n", __FUNCTION__);
        memset(&file_state, 0, sizeof(struct stat));
        stat(CFG_FAST_DATA_FILE_NAME,&file_state);
        fd = fopen(CFG_FAST_DATA_FILE_NAME,"a+");
        if(fd)
        {
#if CFG_MODULE_ENABLE_EASY_FILE_SYSTEM
            if((file_state.st_size + sizeof(RL_BSSID_INFO_T)) > CFG_EFS_FILE_SIZE_LIMIT)
            {
                if(fclose(fd) == -1)
                {
                    printf("befor remove close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
                }
                else
                {
                    fd = NULL;
                    if(remove(CFG_FAST_DATA_FILE_NAME) == -1)
                        printf("remove file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
                    else
                    {
                        fd = fopen(CFG_FAST_DATA_FILE_NAME,"a+");
                        if(fd == NULL)
                            printf("after remove file, open file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
                    }
                }
            }
#endif
            if(fd)
            {
                fseek(fd, 0, SEEK_END);
                if(fwrite(data_info, 1, sizeof(RL_BSSID_INFO_T), fd) != sizeof(RL_BSSID_INFO_T))
                    printf("write file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
            }
            if(fd)
            {
                if(fclose(fd) == -1)
                    printf("close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
            }
        }
        else
            printf("open file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);


        ret = 1;
    }

FUN_RET:
    if(new_buf) free(new_buf);
    return ret;
}

#else
int wlan_fast_info_match(char *ssid, char *passwd, wlan_fast_connect_t *out_info)
{
    int ret = 0;
    uint32_t crc = 0;
    struct wlan_fast_connect *p = 0;
    FILE *fd = NULL;
    unsigned char md5_tmp[16];
    MD5_CTX ctx;

    if(ssid==0 /* || passwd==0 */) return -1;

    u8 *new_buf = (u8*)malloc(BLOCK_SIZE);
    if(new_buf==0) return -1;

    memset(md5_tmp, 0, sizeof(md5_tmp));
    memset(new_buf, 0, BLOCK_SIZE);

    fd = fopen(CFG_FAST_DATA_FILE_NAME,"r+");
    if(fd)
    {
//        fseek(fd, -(BLOCK_SIZE), SEEK_END);
        ret = fread(new_buf, 1, BLOCK_SIZE, fd);
        if(fclose(fd) == -1)
            printf("close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
    }

//    ret = File_GetNameValueFs(CFG_FAST_DATA_FILE_NAME, new_buf, BLOCK_SIZE);
    if(ret <= 0) {
        goto FUN_RET;
    }
    int cnt = ret/sizeof(struct wlan_fast_connect);
    printf("info: %s, cnt=%d, ret=%d, size_item=%d!\r\n", __FUNCTION__, cnt, ret, sizeof(struct wlan_fast_connect));
    p = (struct wlan_fast_connect*)new_buf;
    while(cnt > 0) {
        printf("info: %s, p[%d].ssid=%s, old_crc=0x%08x!\r\n", __FUNCTION__, cnt-1, p[cnt-1].ssid, p[cnt-1].crc);
        if (strcmp(p[cnt-1].ssid, ssid) == 0 ){
            if (passwd) {
                MD5_Init(&ctx);
                MD5_Update(&ctx, passwd, strlen(passwd));
                MD5_Final(md5_tmp, &ctx);
                if(memcmp(p[cnt-1].md5_passphrase, md5_tmp, sizeof(md5_tmp)) != 0){//密码不匹配
                    cnt--;
                    continue;
                }
            }
            //hex_dump("wlan_fast_info_match: md5", md5_tmp, sizeof(md5_tmp));

            //if(memcmp(p[cnt-1].md5_passphrase, md5_tmp, sizeof(md5_tmp)) == 0){
            printf("info: %s, md5_passphrase matched ok!!!\r\n", __FUNCTION__);
            crc = wlan_standard_chksum(&p[cnt-1], sizeof(wlan_fast_connect_t)-4);
            printf("info: %s, index: %d, crc validated, (crc)(%08x %s %08x)!\r\n",
                    __FUNCTION__, cnt-1, crc, (crc==p[cnt-1].crc?"=":"!="), p[cnt-1].crc);
            if (p[cnt-1].ssid[0] != 0xFF &&
                p[cnt-1].channel >= 1 &&
                p[cnt-1].channel <= 13 &&
                p[cnt-1].crc == crc &&
                out_info) {
                printf("info: %s, matched ok, Do WiFi Quick Connnecting ... \r\n", __FUNCTION__);
                memcpy(out_info, &p[cnt-1], sizeof(wlan_fast_connect_t));
                ret = 1;
                break;
            }
            //}
        }
        cnt--;
    }
    if(cnt<=0) {
        printf("info: %s, matched failed, Do Normal WiFi Connecting ... \r\n", __FUNCTION__);
        ret = 0;
    }

FUN_RET:
    if(new_buf) free(new_buf);
    return ret;
}



int CompFastInfo(wlan_fast_connect_t *src, wlan_fast_connect_t *dest)
{
    if (src == 0 || dest==0) return -1;
    if (strcmp(src->ssid, dest->ssid) != 0) {
        //printf("info: %s, src->ssid=%s, dest->ssid=%s!\r\n", __FUNCTION__, src->ssid, dest->ssid);
        return -1;
    }
    if (memcmp(src->bssid, dest->bssid, sizeof(src->bssid)) != 0){
        //hex_dump("warnning src bssid", src->bssid, sizeof(src->bssid));
        //hex_dump("warnning dest bssid", dest->bssid, sizeof(dest->bssid));
        printf("info: %s, src bssid != dest bssid!\r\n", __FUNCTION__);
        return -2;
    }
    if (src->channel != dest->channel) {
        printf("warning: src->channel=%d, dest->channel=%d!\r\n", src->channel, dest->channel);
        return -3;
    }
    if (src->security != dest->security) {
        printf("warning: src->security=%d, dest->security=%d!\r\n", src->security, dest->security);
        return -3;
    }
    if (memcmp(src->md5_passphrase, dest->md5_passphrase, sizeof(src->md5_passphrase)) != 0){
        //hex_dump("warnning src md5_passphrase", src->md5_passphrase, sizeof(src->md5_passphrase));
        //hex_dump("warnning dest md5_passphrase", dest->md5_passphrase, sizeof(dest->md5_passphrase));
        printf("info: %s, src md5_passphrase != dest md5_passphrase!\r\n", __FUNCTION__);
        return -4;
    }
    if (memcmp(src->psk, dest->psk, sizeof(src->psk)) != 0){
        //hex_dump("warnning src psk", src->psk, sizeof(src->psk));
        //hex_dump("warnning dest psk", dest->psk, sizeof(dest->psk));
        printf("info: %s, src psk != dest psk!\r\n", __FUNCTION__);
        return -5;
    }
    if (src->crc != dest->crc) {
        printf("warning: src->crc=%d, dest->crc=%d!\r\n", src->crc, dest->crc);
        return -6;
    }
    return 0;
}

int wlan_fast_connect_info_write(wlan_fast_connect_t *data_info)
{
    int ret = 0;
    struct wlan_fast_connect *p = 0;
    struct stat file_state;
    FILE *fd = NULL;
    if(data_info == NULL)
      return -1;

    u8 *new_buf = (u8*)malloc(BLOCK_SIZE);
    if(new_buf==0) return -1;

    memset(new_buf, 0, BLOCK_SIZE);


    fd = fopen(CFG_FAST_DATA_FILE_NAME,"r+");
    if(fd)
    {
//        fseek(fd, -(BLOCK_SIZE), SEEK_END);
        ret = fread(new_buf, 1, BLOCK_SIZE, fd);
        if(fclose(fd) == -1)
            printf("close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
    }
//    ret = File_GetNameValueFs(CFG_FAST_DATA_FILE_NAME, new_buf, BLOCK_SIZE);
    if(ret < 0) {
        goto FUN_RET;
    }

    int cnt = ret/sizeof(struct wlan_fast_connect);
    //printf("info: %s, cnt=%d, ret=%d, size_item=%d!\r\n", __FUNCTION__, cnt, ret, sizeof(struct wlan_fast_connect));
    p = (struct wlan_fast_connect*)new_buf;
    data_info->crc = wlan_standard_chksum(data_info, sizeof(struct wlan_fast_connect)-4);
    //printf("info: %s, data_info->crc=0x%08x!\r\n", __FUNCTION__, data_info->crc);
    //hex_dump("wlan_fast_connect_info_write: md5", data_info->md5_passphrase, sizeof(data_info->md5_passphrase));
    while(cnt > 0) {
        //printf("info: %s, p[%d].ssid=%s, old_crc=0x%08x!\r\n", __FUNCTION__, cnt-1, p[cnt-1].ssid, p[cnt-1].crc);
        if (CompFastInfo(&p[cnt-1], data_info)==0){
            printf("info: %s, mathed item, index=%d, Do Not Writed New, Do Nothing!!!\r\n", __FUNCTION__, cnt-1);
            break;
        }
        cnt--;
    }
    if(cnt<=0) {//找不到，重新写入
        printf("info: %s, New Item Will Be Writed!\r\n", __FUNCTION__);
//        File_SetNameValueFs(CFG_FAST_DATA_FILE_NAME, data_info, sizeof(struct wlan_fast_connect));

        memset(&file_state, 0, sizeof(struct stat));
        stat(CFG_FAST_DATA_FILE_NAME,&file_state);
        fd = fopen(CFG_FAST_DATA_FILE_NAME,"a+");
        if(fd)
        {
#if CFG_MODULE_ENABLE_EASY_FILE_SYSTEM
            if((file_state.st_size + sizeof(struct wlan_fast_connect)) > CFG_EFS_FILE_SIZE_LIMIT)
            {
                if(fclose(fd) == -1)
                {
                    printf("befor remove close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
                }
                else
                {
                    fd = NULL;
                    if(remove(CFG_FAST_DATA_FILE_NAME) == -1)
                        printf("remove file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
                    else
                    {
                        fd = fopen(CFG_FAST_DATA_FILE_NAME,"a+");
                        if(fd == NULL)
                            printf("after remove file, open file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
                    }
                }
            }
#endif
            if(fd)
            {
                fseek(fd, 0, SEEK_END);
                if(fwrite(data_info, 1, sizeof(struct wlan_fast_connect), fd) != sizeof(struct wlan_fast_connect))
                    printf("write file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
            }
            if(fd)
            {
                if(fclose(fd) == -1)
                    printf("close file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);
            }
        }
        else
            printf("open file \" %s \" fail\r\n",CFG_FAST_DATA_FILE_NAME);


        ret = 1;
    }

FUN_RET:
    if(new_buf) free(new_buf);
    return ret;
}

#endif


