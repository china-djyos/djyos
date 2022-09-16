#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <misc/misc.h>

#include <arpa/inet.h>


#include <os.h>
#include <shell.h>
#include <netdb.h>
#include <sys/socket.h>
#include "dbug.h"
//base64 encode and decode
extern char * Base64_Encode( char * bindata, char * base64, s32 binlength );
extern s32 Base64_Decode( char * base64, char * bindata );

#undef  ERROR
#undef  OK
#define ERROR -1
#define OK     0
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define SMTP_INPUT_BUFFER_SIZE 512
#define SMTP_INPUT_BIN         360  //at most input 360 bytes to transfer to base64
/* This structure contains the message we should send one transmit */
struct SmtpState
{
    char  *host;    //the pc we used
    char  *server;  //the smtp server
    char  *user;    //the user to login the smtp server
    char  *passwd;  //the user passwd
    char  **to;     //we permit send multi persons .end with NULL if not NULL
    char  **cc;     //we also permit to copy to multi persons .end with NULL if not NULL
    char  **bcc;    //we also permit to send to multi person secrectly
    char  *from;    //only one person could write the mail
    char  *subject; //only subject to need or NULL
    char  **files;  //multi files attachment
    char  *html;    //maybe a html need to send
    char  *msg;     //the message to send,end with NULL
    char  *inl;     //inline,should be a picture
    char  buffer[SMTP_INPUT_BUFFER_SIZE];
};

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const char g_smtpcrnlperiodcrnl[] = "\r\n.\r\n";
static const char g_smtpdata[]           = "DATA\r\n";
static const char g_smtphelo[]           = "EHLO ";
static const char g_smtpmailfrom[]       = "MAIL FROM";
static const char g_smtpquit[]           = "QUIT\r\n";
static const char g_smtprcptto[]         = "RCPT TO";
static const char g_smtpfrom[]           = "From";
static const char g_smtpsubject[]        = "Subject";
static const char g_smtpto[]             = "To";
static const char g_smtpcc[]             = "Cc";
static const char g_smtpbcc[]            = "Bcc";

//socket io
static s32 __SMTP_SockSnd(s32 sockfd,char *buf)    //snd
{
    debug_printf("smtp","SND:%s\r\n",buf);
    s32 len;
    s32 ret;

    len = strlen(buf);
    while(len > 0)
    {
        ret = send(sockfd, buf, strlen(buf), 0);
        if(ret <=0)
        {
            return ERROR;
        }
        buf += ret;
        len -= ret;
    }
    return OK;
}
static inline s32 __SMTP_SockRcv(s32 sockfd,unsigned char *buf,s32 len)  //rcv
{
    s32 ret;

    ret = recv(sockfd,buf,len, 0);
    if (ret <= 0)
    {
        return ERROR;
    }
    debug_printf("smtp","RCV:%s\r\n",(char *)buf);
    return ret;
}
static inline s32 __SMTP_SndBufBase64(s32 sockfd,struct SmtpState *smtp,char *buf, s32 len)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    Base64_Encode(buf,smtp->buffer,len);
    if(OK != __SMTP_SockSnd(sockfd,smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}
//snd a item
static inline s32 __SMTP_SndItem(s32 sockfd,struct SmtpState *smtp,char *type,char *value)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s: %s\r\n", type, value);
    if (__SMTP_SockSnd(sockfd, smtp->buffer) < 0)
    {
        return ERROR;
    }
    return OK;
}
//snd list
static inline s32 __SMTP_SndList(s32 sockfd,struct SmtpState *smtp,char *type,char **list)
{
    char *value;
    if(NULL != list)
    {
        while(*list != NULL)
        {
            value = *list;
            if(OK !=__SMTP_SndItem(sockfd,smtp,type,value))
            {
                return ERROR;
            }
            list++;
        }
    }
    return OK;
}


//receive code
static inline s32 __SMTP_RcvCode(s32 sockfd,struct SmtpState *smtp)
{
    s32 code = -1;
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    if(__SMTP_SockRcv(sockfd,(unsigned char *)smtp->buffer,SMTP_INPUT_BUFFER_SIZE) > 0)
    {
        sscanf(smtp->buffer,"%d",&code);
    }
    return code;
}


//snd a command and check the return code:return the code if success or -1 faild
static inline s32 __SMTP_SndItemAndCheck(s32 sockfd,struct SmtpState *smtp,char *cmd,char *para,s32 codecmp)
{
    s32 ret = ERROR;
    s32 code;
    if(OK ==__SMTP_SndItem(sockfd,smtp,cmd,para))
    {
        code = __SMTP_RcvCode(sockfd,smtp);
        if(code == codecmp)
        {
            ret = OK;
        }
    }
    return ret;
}
static inline s32 __SMTP_SndListAndCheck(s32 sockfd,struct SmtpState *smtp,char *cmd,char **para,s32 codecmp)
{
    s32 ret = ERROR;
    s32 code;
    char *value;

    if(NULL == para)
    {
        ret = OK;
    }
    else
    {
        while(*para != NULL)
        {
            value = *para;
            if(OK ==__SMTP_SndItem(sockfd,smtp,cmd,value))
            {
                code = __SMTP_RcvCode(sockfd,smtp);
                if(code != codecmp)
                {
                    return ERROR;
                }
            }
            else
            {
                return ERROR;
            }
            para++;
        }
        ret = OK;
    }
    return ret;
}

static inline s32 __SMTP_SndStrAndcheck(s32 sockfd,struct SmtpState *smtp,char *str,s32 codecmp)
{
    s32 ret = ERROR;
    s32 code;
    if(OK ==__SMTP_SockSnd(sockfd,str))
    {
        code = __SMTP_RcvCode(sockfd,smtp);
        if(code == codecmp)
        {
            ret = OK;
        }
    }
    return ret;
}

static inline s32 __SMTP_SendHeader(s32 sockfd,struct SmtpState *smtp)
{
    char *value;
    char *type;
    char **strstr;
    //now send the mail from
    value = smtp->from;
    type = (char*)g_smtpfrom;
    if(OK !=__SMTP_SndItem(sockfd,smtp,type,value))
    {
        return ERROR;
    }
    //header-to
    strstr = smtp->to;
    type = (char*)g_smtpto;
    if(OK != __SMTP_SndList(sockfd,smtp,type,strstr))
    {
        return ERROR;
    }
    //header --cc
    strstr = smtp->cc;
    type = (char*)g_smtpcc;
    if(OK != __SMTP_SndList(sockfd,smtp,type,strstr))
    {
        return ERROR;
    }
    //header --bcc
    strstr = smtp->bcc;
    type = (char*)g_smtpbcc;
    if(OK != __SMTP_SndList(sockfd,smtp,type,strstr))
    {
        return ERROR;
    }
    //header-subject
    value = smtp->subject;
    type = (char*)g_smtpsubject;
    if(OK !=__SMTP_SndItem(sockfd,smtp,type,value))
    {
        return ERROR;
    }
    //header-mime veriosn
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s\r\n", "MIME-Version: 1.0");
    if (send(sockfd, smtp->buffer, strlen(smtp->buffer), 0) < 0)
    {
        return ERROR;
    }
    value = "1.0";
    type = "MIME-Version";
    if(OK !=__SMTP_SndItem(sockfd,smtp,type,value))
    {
        return ERROR;
    }
    return OK;
}
static inline s32 __SMTP_SndClassHeader(s32 sockfd,struct SmtpState *smtp, char *type, char *boundary)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "Content-Type: %s; boundary=\"%s\"\r\n",type,boundary);
    if(OK != __SMTP_SockSnd(sockfd, smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}
static inline s32 __SMTP_SndSessionText(s32 sockfd,struct SmtpState *smtp, char *type, char *charset, char *encode)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "Content-Type: %s; charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n",\
                type,charset,encode);
    if(OK != __SMTP_SockSnd(sockfd, smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}

static inline s32 __SMTP_SndBoundary(s32 sockfd,struct SmtpState *smtp, char *bs, char *be)
{
    if(NULL != bs)
    {
        memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
        snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "\r\n--%s\r\n",bs);
        if(OK != __SMTP_SockSnd(sockfd, smtp->buffer))
        {
            return ERROR;
        }
    }
    if(NULL != be)
    {
        memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
        snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "\r\n\r\n--%s--\r\n",be);
        if(OK != __SMTP_SockSnd(sockfd, smtp->buffer))
        {
            return ERROR;
        }
    }
    return OK;
}
//send a session header
static inline s32 __SMTP_SndBodyText(s32 sockfd,struct SmtpState *smtp,char *type,\
                  char *text, char *charset, char *boundary)
{
    char *src;
    u32 len = 0;
    s32 sessionlen;
    if(NULL == text)
    {
        return OK;
    }
    if(OK != __SMTP_SndBoundary(sockfd,smtp,boundary,NULL))
    {
        return ERROR;
    }
    if(OK != __SMTP_SndSessionText(sockfd,smtp,type,charset,"base64"))
    {
        return ERROR;
    }
    src = text;
    len = strlen(src);
    while(len > 0)
    {
        if(len > SMTP_INPUT_BIN)
        {
            sessionlen = SMTP_INPUT_BIN;
        }
        else
        {
            sessionlen = len;
        }
        if(OK != __SMTP_SndBufBase64(sockfd,smtp,src,sessionlen))
        {
            return ERROR;
        }
        len -= sessionlen;
        src += sessionlen;
    }
    //now snd the text to the server
    if(OK != __SMTP_SockSnd(sockfd,"\r\n\r\n")) //for the text end
    {
        return ERROR;
    }

    return OK;
}

static inline s32 __SMTP_SndSessionFile(s32 sockfd,struct SmtpState *smtp, char *type, char *typename,\
                                  char *encode, char *position, char *filename)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "Content-Type: %s; name=\"%s\"\r\nContent-Transfer-Encoding: %s\r\nContent-Disposition: %s; filename=\"%s\"\r\n\r\n",\
                type,typename,encode,position,filename);
    if(OK != __SMTP_SockSnd(sockfd, smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}
//send a file
static inline s32 __SMTP_SndBodyFile(s32 sockfd,struct SmtpState *smtp,char *type,\
         char *filename, char *position, char *boundary)
{
    s32 len = 0;
    s32 sessionlen;
    if(NULL == filename)
    {
        return OK;
    }
    if(OK != __SMTP_SndBoundary(sockfd,smtp,boundary,NULL))
    {
        return ERROR;
    }
    if(OK != __SMTP_SndSessionFile(sockfd,smtp,type,filename,"base64",position,filename))
    {
        return ERROR;
    }

    //we should open the file first then read page by page
    s32 fd;
    char buf[SMTP_INPUT_BIN];
    fd = open(filename,O_RDONLY);
    if(fd < 0)
    {
        return ERROR;
    }
    while((len = read(fd,buf,SMTP_INPUT_BIN))> 0)
    {
        sessionlen = len;
        if(OK != __SMTP_SndBufBase64(sockfd,smtp,buf,sessionlen))
        {
            close(fd);
            return ERROR;
        }
    }
    close(fd);
    //now snd the text to the server
    return OK;
}

//send filelist
static inline s32 __SMTP_SndBodyFileList(s32 sockfd,struct SmtpState *smtp,char *type,\
         char **filelist, char *position, char *boundary)
{
    char *filename;

    if(filelist == NULL)
    {
        return OK;
    }
    while((filename = *filelist++) != NULL)
    {
        if(OK != __SMTP_SndBodyFile(sockfd,smtp,type,filename,position,boundary))
        {
            return ERROR;
        }
    }
    return OK;
}


//200   (nonstandard success response, see rfc876)
//211   System status, or system help reply
//214   Help message
//220   <domain> Service ready
//221   <domain> Service closing transmission channel
//250   Requested mail action okay, completed
//251   User not local; will forward to <forward-path>
//354   Start mail input; end with <CRLF>.<CRLF>
//421   <domain> Service not available, closing transmission channel
//450   Requested mail action not taken: mailbox unavailable
//451   Requested action aborted: local error in processing
//452   Requested action not taken: insufficient system storage
//500   Syntax error, command unrecognised
//501   Syntax error in parameters or arguments
//502   Command not implemented
//503   Bad sequence of commands
//504   Command parameter not implemented
//521   <domain> does not accept mail (see rfc1846)
//530   Access denied (???a Sendmailism)
//550   Requested action not taken: mailbox unavailable
//551   User not local; please try <forward-path>
//552   Requested mail action aborted: exceeded storage allocation
//553   Requested action not taken: mailbox name not allowed
//554   Transaction failed
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static inline s32 __SMTP_SendMessage(s32 sockfd, struct SmtpState *smtp)
{
    s32 code;
    char strarray[128];
    char *type,*value,**list;
    //when connect, we will get the server information here
    code = __SMTP_RcvCode(sockfd,smtp);
    if(code != 220)
    {
        debug_printf("smtp","server not ready:%d\n\r",code);
        return ERROR;
    }
    //then we will say hello to the server.ehlo means AUTH use the base64 in the user and passwd
    //the server should also say hello to us
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s%s\r\n", g_smtphelo, smtp->host?smtp->host:"NULL");
    if(OK != __SMTP_SndStrAndcheck(sockfd,smtp,smtp->buffer,250))
    {
        debug_printf("smtp","email action failed:%d\n\r",code);
        return ERROR;
    }
    //then we will send the auth login here
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s\r\n","auth login");
    if(OK != __SMTP_SndStrAndcheck(sockfd,smtp,smtp->buffer,334))
    {
        debug_printf("smtp","No login:%d\n\r",code);
        return ERROR;
    }

    //then send our username in the base64
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    memset(strarray,0,64);
    Base64_Encode(smtp->user,strarray,strlen(smtp->user));
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s\r\n", (char *)strarray);
    if(OK != __SMTP_SndStrAndcheck(sockfd,smtp,smtp->buffer,334))
    {
        debug_printf("smtp","No login:%d\n\r",code);
        return ERROR;
    }

    //then send our passwd in the base64
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    memset(strarray,0,64);
    Base64_Encode(smtp->passwd,strarray,strlen(smtp->passwd));
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s\r\n", (char *)strarray);
    if(OK != __SMTP_SndStrAndcheck(sockfd,smtp,smtp->buffer,235))
    {
        debug_printf("smtp","login failed:%d\n\r",code);
        return ERROR;
    }
    //now send the mail from
    value = smtp->from;
    type = (char*)g_smtpmailfrom;
    if(OK != __SMTP_SndItemAndCheck(sockfd,smtp,type,value,250))
    {
        debug_printf("smtp","snd from command error\n\r");
        return ERROR;
    }
    //now send the mail to
    list = smtp->to;
    type = (char*)g_smtprcptto;
    if(OK != __SMTP_SndListAndCheck(sockfd,smtp,type,list,250))
    {
        debug_printf("smtp","snd to command error\n\r");
        return ERROR;
    }
    //now send the cc
    list = smtp->cc;
    type = (char*)g_smtprcptto;
    if(OK != __SMTP_SndListAndCheck(sockfd,smtp,type,list,250))
    {
        debug_printf("smtp","snd to command error\n\r");
        return ERROR;
    }
    //now send the bcc
    list = smtp->bcc;
    type = (char*)g_smtprcptto;
    if(OK != __SMTP_SndListAndCheck(sockfd,smtp,type,list,250))
    {
        debug_printf("smtp","snd to command error\n\r");
        return ERROR;
    }
    //now send the data
    if(OK != __SMTP_SndStrAndcheck(sockfd,smtp,(char*)g_smtpdata,354))
    {
        debug_printf("smtp","snd data command error\n\r");
        return ERROR;
    }
    if(OK != __SMTP_SendHeader(sockfd,smtp))
    {
        return ERROR;
    }

    char *mixedb="mixed boundary";
    char *relatedb="related boundary";
    char *alternativeb = "alternative boundary";
    //mixed header--boundary
    if(OK != __SMTP_SndClassHeader(sockfd,smtp,"multipart/mixed",mixedb))
    {
        return ERROR;
    }
    //send the related header
    if(OK != __SMTP_SndBoundary(sockfd,smtp,mixedb,NULL))
    {
        return ERROR;
    }
    if(OK != __SMTP_SndClassHeader(sockfd,smtp,"multipart/related",relatedb))
    {
        return ERROR;
    }
    //send the alternative header
    if(OK != __SMTP_SndBoundary(sockfd,smtp,relatedb,NULL))
    {
        return ERROR;
    }
    if(OK != __SMTP_SndClassHeader(sockfd,smtp,"multipart/alternative",alternativeb))
    {
        return ERROR;
    }
    //alternative part:text
    if(OK != __SMTP_SndBodyText(sockfd,smtp,"text/plain",smtp->msg,"utf-8",alternativeb))
    {
        return ERROR;
    }
    //alternative part:html
    if(OK != __SMTP_SndBodyText(sockfd,smtp,"text/html",smtp->html,"utf-8",alternativeb))
    {
        return ERROR;
    }
    //for the alternative end
    if(OK != __SMTP_SndBoundary(sockfd,smtp,NULL,alternativeb))
    {
        return ERROR;
    }

    //then the relative new part which means the image in the body or image back ground
    //if any added here;
    if(OK != __SMTP_SndBodyFile(sockfd,smtp,"image/gif",smtp->inl,"inline",relatedb))
    {
        return ERROR;
    }
    //realtive end
    if(OK != __SMTP_SndBoundary(sockfd,smtp,NULL,relatedb))
    {
        return ERROR;
    }
    //then the mixed new part, if any attachment here,please add it here
    if(OK != __SMTP_SndBodyFileList(sockfd,smtp,"application/octet-stream",smtp->files,"attachment",mixedb))
    {
        return ERROR;
    }
    //mixed end
    if(OK != __SMTP_SndBoundary(sockfd,smtp,NULL,mixedb))
    {
        return ERROR;
    }

    //body--file attachment
    //body --end
    if(OK != __SMTP_SndStrAndcheck(sockfd,smtp,(char*)g_smtpcrnlperiodcrnl,250))
    {
        debug_printf("smtp","accept message error\n\r");
        return ERROR;
    }
    //send the quit
    if(OK != __SMTP_SndStrAndcheck(sockfd,smtp,(char*)g_smtpquit,221))
    {
        debug_printf("smtp","quit error\n\r");
        return ERROR;
    }
    return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/* Specificy an SMTP server and hostname.
 *
 */
bool_t SMTP_Configure( void *handle, char *host, char *server,\
         char *user, char *passwd, char **to, char **cc,\
         char  **bcc, char *from, char *subject, char **files,\
         char *msg, char *html, char *inl)
{
    bool_t result = false;
    struct SmtpState *smtp = ( struct SmtpState *)handle;
    if((NULL != handle)&&(NULL != server)&&(NULL != user)&&(NULL != passwd))
    {
        smtp->host = host;
        smtp->server=server;
        smtp->user = user;
        smtp->passwd = passwd;
        smtp->to =to;
        smtp->cc = cc;
        smtp->bcc = bcc;
        smtp->from = from;
        smtp->subject = subject;
        smtp->files = files;
        smtp->msg = msg;
        smtp->html = html;
        smtp->inl = inl;
        result = true;
    }
    return result;
}

/* Send an e-mail.
 */

s32 smtp_send(void *handle)
{
    struct SmtpState *smtp = (struct SmtpState *)handle;
    struct sockaddr_in server;
    in_addr_t  serverip;
    s32 sockfd;
    s32 ret=-1;
    struct addrinfo *addr = NULL;

    getaddrinfo(smtp->server,NULL,NULL,&addr);
    if(NULL == addr)
    {
        debug_printf("smtp","%s:connot resolve the server:%s\n\r",__FUNCTION__,smtp->server);
        goto EXIT_SERVERIP;
    }
    serverip = ((struct sockaddr_in *)addr->ai_addr)->sin_addr.s_addr;
    freeaddrinfo(addr);
    server.sin_family = AF_INET;
    memcpy(&server.sin_addr.s_addr, &serverip, sizeof(in_addr_t));
    server.sin_port = htons(25);
    debug_printf("smtp","%s:resolve the server:%s:%s\n\r",__FUNCTION__,smtp->server,inet_ntoa(server.sin_addr));

    //create a client socket to connect the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        debug_printf("smtp","%s:create the socket error\n\r",__FUNCTION__);
        goto EXIT_SOCKET;
    }
    debug_printf("smtp","%s:create the socket success\n\r",__FUNCTION__);

    /* Connect to server.*/
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0)
    {
        debug_printf("smtp","%s:conect the server failed\n\r",__FUNCTION__);
        goto EXIT_CONNECT;
    }
        debug_printf("smtp","%s:conect the server success\n\r",__FUNCTION__);

    //then send the email
    ret = __SMTP_SendMessage(sockfd, smtp);
    //close the socket and exit
    closesocket(sockfd);
    return ret;

EXIT_CONNECT:
    closesocket(sockfd);
EXIT_SOCKET:
EXIT_SERVERIP:
    return ret;
}

void *smtp_open(void)
{
    /* Allocate the handle */

    struct SmtpState *smtp = (struct SmtpState *)net_malloc(sizeof(struct SmtpState));
    if (smtp)
    {
        /* Initialize the handle */
        memset(smtp, 0, sizeof(struct SmtpState));
    }

    return (void*)smtp;
}
void smtp_close(void *handle)
{
    struct SmtpState *smtp = (struct SmtpState *)handle;
    if (smtp)
    {
        net_free(smtp);
    }
}


#define CN_LIST_LIMIT 32
typedef struct
{
    s32 cur;
    char *list[CN_LIST_LIMIT];
}tagListCtrl;

//-h host -s server -u user -p passwd -to to -from from -topic topic -m msg
//bool_t sendemail(char *param)
bool_t smtp(char *param)
{
    char *host ="DJYOS_BOARD";
    char *server = NULL;
    char *user= NULL;
    char *passwd= NULL;
    char *from= NULL;
    char *subject="DJYOS SMTP TEST";
    char *msg="HELLO,THIS IS FROM DJYOS SMTP:www.djyos.com\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline";
    char *html="<html><body>"\
        "<p><img src=\"http://www.djyos.com/wp-content/themes/wpressmonster/images/logo.png\"></p><br/>"\
        "<h1>hello,djyos users.</h1>"\
        "<h4>如果你能收到这封信，那真是太好了，因为这封信来自于djyos的smtp</h4>"\
        "<h4>特性如下：支持发送多人，支持抄送多人，也支持密送多人，当然也支持多个附件，还支持html,这封信就是html格式！</h4>"\
        "<h4>详情参考smtp.c文件，栈空间：正常文本时1KB左右；发送附件在2KB左右，请务必预留足够空间</h4>"\
        "<h4>使用方法：调用smtp_open获取句柄，使用smtp_configure配置所要发送内容，调用smtp_send发送邮件，调用smtp_close关闭句柄</h4>"\
        "<h4>可以使用它来发送一封邮件来报告当前的运行状态，这对于拷机来说是个不错的主意</h4>"\
        "<h1>欢迎使用</h1>"\
        "</body></html>";
    char *inl = NULL;
    tagListCtrl *listto;
    tagListCtrl *listcc;
    tagListCtrl *listbcc;
    tagListCtrl *listfile;
    tagListCtrl *list;

    listto = net_malloc(sizeof(tagListCtrl));
    listcc = net_malloc(sizeof(tagListCtrl));
    listbcc = net_malloc(sizeof(tagListCtrl));
    listfile = net_malloc(sizeof(tagListCtrl));
    if((NULL == listto)||(NULL == listcc)||(NULL == listbcc)||(NULL == listfile))
    {
        net_free(listto);
        net_free(listcc);
        net_free(listbcc);
        net_free(listfile);
        return true;
    }
    memset(listto,0,sizeof(tagListCtrl));
    memset(listcc,0,sizeof(tagListCtrl));
    memset(listbcc,0,sizeof(tagListCtrl));
    memset(listfile,0,sizeof(tagListCtrl));

    //set the default
    listto->list[0] = "djyos@djyos.com";

    s32 argc = 16;
    char *argv[16];
    string2arg(&argc,argv,param);
    s32 i =0;
    while(i <argc)
    {
        if(0 ==strcmp(argv[i],"-h"))
        {
            host = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-s"))
        {
            server = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-from"))
        {
            from = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-to"))
        {
            list = listto;
            if(list->cur < (CN_LIST_LIMIT-1))
            {
                list->list[list->cur] = argv[i+1];
                list->cur++;
            }
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-cc"))
        {
            list = listcc;
            if(list->cur < (CN_LIST_LIMIT-1))
            {
                list->list[list->cur] = argv[i+1];
                list->cur++;
            }
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-bcc"))
        {
            list = listbcc;
            if(list->cur < (CN_LIST_LIMIT-1))
            {
                list->list[list->cur] = argv[i+1];
                list->cur++;
            }
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-subject"))
        {
            subject = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-m"))
        {
            msg = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-file"))
        {
            list = listfile;
            if(list->cur < (CN_LIST_LIMIT-1))
            {
                list->list[list->cur] = argv[i+1];
                list->cur++;
            }
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-u"))
        {
            user = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-p"))
        {
            passwd = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-html"))
        {
            html = argv[i+1];
            i +=2;
        }
        else if(0 == strcmp(argv[i],"-inl"))
        {
            inl = argv[i+1];
            i +=2;
        }
        else
        {
            i++;
        }
    }
    //we should build a pmstat here
    void *handle = smtp_open();
    if(SMTP_Configure(handle,host,server,user,passwd,listto->list,listcc->list,\
       listbcc->list,from,subject,listfile->list,msg,html,inl))
    {
        debug_printf("smtp","From:%s To:%s Subject:%s User:%s Passwd:%s\n\r",from,listto->list[0],subject,user,passwd);
        smtp_send(handle);
    }
    else
    {
        debug_printf("smtp","Configure Err\n\r");
    }
    smtp_close(handle);
    net_free(listto);
    net_free(listcc);
    net_free(listbcc);
    net_free(listfile);
    return true;
}

//static bool_t base64encodeshell(char *param)
bool_t SMTP_EnBase64File(char *param)
{
    char *srcfile;
    char *dstfile;
    s32 srcfd;
    s32 dstfd;
    char srcbuf[SMTP_INPUT_BIN];
    char dstbuf[SMTP_INPUT_BUFFER_SIZE];
    s32 argc =2;
    char *argv[2];
    s32 srclen = 0,dstlen = 0,writelen = 0;
    string2arg(&argc,argv,param);
    if(argc != 2)
    {
        debug_printf("smtp","para error\n\r");
        return true;
    }
    srcfile = argv[0];
    dstfile = argv[1];

    srcfd = open(srcfile,O_RDONLY);
    dstfd = open(dstfile,O_CREAT|O_RDWR);
    if((srcfd <0)||(dstfd < 0))
    {
        debug_printf("smtp","open file error\n\r");
        close(srcfd);
        close(dstfd);
        return true;
    }

    memset(srcbuf,0,SMTP_INPUT_BIN);
    memset(dstbuf,0,SMTP_INPUT_BUFFER_SIZE);
    while((srclen = read(srcfd,srcbuf,SMTP_INPUT_BIN))> 0)
    {
        Base64_Encode(srcbuf,dstbuf,srclen);
        dstlen = strlen((char *)dstbuf);
        writelen = write(dstfd,dstbuf,dstlen);
        if(writelen != dstlen)
        {
            debug_printf("smtp","write error\n\r");
        }
            debug_printf("smtp","srclen:%04d dstlen:%04d writelen:%04d\n\r",srclen,dstlen,writelen);

        memset(srcbuf,0,SMTP_INPUT_BIN);
        memset(dstbuf,0,SMTP_INPUT_BUFFER_SIZE);
    }
    close(srcfd);
    close(dstfd);
    return true;

}


// =============================================================================
// 功能：add the SmtpClient debug to the system
// 参数：para
// 返回值  ：true成功  false失败。
// =============================================================================
bool_t SMTP_ServiceSMTP_Client()
{
    return true;
}

