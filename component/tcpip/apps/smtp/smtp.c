
#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>


#include <os.h>
#include <shell.h>
#include <netdb.h>
#include <sys/socket.h>
#include "dbug.h"
//base64 encode and decode
extern char * base64_encode( const unsigned char * bindata, char * base64, int binlength );
extern int base64_decode( const char * base64, unsigned char * bindata );

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
    const char  *host;    //the pc we used
    const char  *server;  //the smtp server
    const char  *user;    //the user to login the smtp server
    const char  *passwd;  //the user passwd
    const char  **to;     //we permit send multi persons .end with NULL if not NULL
    const char  **cc;     //we also permit to copy to multi persons .end with NULL if not NULL
    const char  **bcc;    //we also permit to send to multi person secrectly
    const char  *from;    //only one person could write the mail
    const char  *subject; //only subject to need or NULL
    const char  **files;  //multi files attachment
    const char  *html;    //maybe a html need to send
    const char  *msg;     //the message to send,end with NULL
    const char  *inl;     //inline,should be a picture
    char         buffer[SMTP_INPUT_BUFFER_SIZE];
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
static int socksnd(int sockfd,const char *buf)    //snd
{
    debug_printf("smtp","SND:%s",buf);
    int len;
    int ret;

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
static inline int sockrcv(int sockfd,unsigned char *buf,int len)  //rcv
{
    int ret;

    ret = recv(sockfd,buf,len, 0);
    if (ret <= 0)
    {
        return ERROR;
    }
    debug_printf("smtp","RCV:%s",(char *)buf);
    return ret;
}
static inline int sndbufbase64(int sockfd,struct SmtpState *smtp,const unsigned char *buf, int len)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    base64_encode(buf,smtp->buffer,len);
    if(OK != socksnd(sockfd,smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}
//snd a item
static inline int snditem(int sockfd,struct SmtpState *smtp,const char *type,const char *value)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s: %s\r\n", type, value);
    if (socksnd(sockfd, smtp->buffer) < 0)
    {
        return ERROR;
    }
    return OK;
}
//snd list
static inline int sndlist(int sockfd,struct SmtpState *smtp,const char *type,const char **list)
{
    const char *value;
    if(NULL != list)
    {
        while(*list != NULL)
        {
            value = *list;
            if(OK !=snditem(sockfd,smtp,type,value))
            {
                return ERROR;
            }
            list++;
        }
    }
    return OK;
}


//receive code
static inline int rcvcode(int sockfd,struct SmtpState *smtp)
{
    int code = -1;
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    if(sockrcv(sockfd,(unsigned char *)smtp->buffer,SMTP_INPUT_BUFFER_SIZE) > 0)
    {
        sscanf(smtp->buffer,"%d",&code);
    }
    return code;
}


//snd a command and check the return code:return the code if success or -1 faild
static inline int snditemandcheck(int sockfd,struct SmtpState *smtp,const char *cmd,const char *para,int codecmp)
{
    int ret = ERROR;
    int code;
    if(OK ==snditem(sockfd,smtp,cmd,para))
    {
        code = rcvcode(sockfd,smtp);
        if(code == codecmp)
        {
            ret = OK;
        }
    }
    return ret;
}
static inline int sndlistandcheck(int sockfd,struct SmtpState *smtp,const char *cmd,const char **para,int codecmp)
{
    int ret = ERROR;
    int code;
    const char *value;

    if(NULL == para)
    {
        ret = OK;
    }
    else
    {
        while(*para != NULL)
        {
            value = *para;
            if(OK ==snditem(sockfd,smtp,cmd,value))
            {
                code = rcvcode(sockfd,smtp);
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

static inline int sndstrandcheck(int sockfd,struct SmtpState *smtp,const char *str,int codecmp)
{
    int ret = ERROR;
    int code;
    if(OK ==socksnd(sockfd,str))
    {
        code = rcvcode(sockfd,smtp);
        if(code == codecmp)
        {
            ret = OK;
        }
    }
    return ret;
}

static inline int sendheader(int sockfd,struct SmtpState *smtp)
{
    const char *value;
    const char *type;
    const char **strstr;
    //now send the mail from
    value = smtp->from;
    type = g_smtpfrom;
    if(OK !=snditem(sockfd,smtp,type,value))
    {
        return ERROR;
    }
    //header-to
    strstr = smtp->to;
    type = g_smtpto;
    if(OK != sndlist(sockfd,smtp,type,strstr))
    {
        return ERROR;
    }
    //header --cc
    strstr = smtp->cc;
    type = g_smtpcc;
    if(OK != sndlist(sockfd,smtp,type,strstr))
    {
        return ERROR;
    }
    //header --bcc
    strstr = smtp->bcc;
    type = g_smtpbcc;
    if(OK != sndlist(sockfd,smtp,type,strstr))
    {
        return ERROR;
    }
    //header-subject
    value = smtp->subject;
    type = g_smtpsubject;
    if(OK !=snditem(sockfd,smtp,type,value))
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
    if(OK !=snditem(sockfd,smtp,type,value))
    {
        return ERROR;
    }
    return OK;
}
static inline int sndclassheader(int sockfd,struct SmtpState *smtp,const char *type,const char *boundary)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "Content-Type: %s; boundary=\"%s\"\r\n",type,boundary);
    if(OK != socksnd(sockfd, smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}
static inline int sndsessiontext(int sockfd,struct SmtpState *smtp,const char *type,const char *charset,const char *encode)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "Content-Type: %s; charset=\"%s\"\r\nContent-Transfer-Encoding: %s\r\n\r\n",\
                type,charset,encode);
    if(OK != socksnd(sockfd, smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}

static inline int sndboundary(int sockfd,struct SmtpState *smtp,const char *bs,const char *be)
{
    if(NULL != bs)
    {
        memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
        snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "\r\n--%s\r\n",bs);
        if(OK != socksnd(sockfd, smtp->buffer))
        {
            return ERROR;
        }
    }
    if(NULL != be)
    {
        memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
        snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "\r\n\r\n--%s--\r\n",be);
        if(OK != socksnd(sockfd, smtp->buffer))
        {
            return ERROR;
        }
    }
    return OK;
}
//send a session header
static inline int sndbodytext(int sockfd,struct SmtpState *smtp,char *type,\
                  const char *text,const char *charset,const char *boundary)
{
    const char *src;
    int len = 0;
    int sessionlen;
    if(NULL == text)
    {
        return OK;
    }
    if(OK != sndboundary(sockfd,smtp,boundary,NULL))
    {
        return ERROR;
    }
    if(OK != sndsessiontext(sockfd,smtp,type,charset,"base64"))
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
        if(OK != sndbufbase64(sockfd,smtp,src,sessionlen))
        {
            return ERROR;
        }
        len -= sessionlen;
        src += sessionlen;
    }
    //now snd the text to the server
    if(OK != socksnd(sockfd,"\r\n\r\n")) //for the text end
    {
        return ERROR;
    }

    return OK;
}

static inline int sndsessionfile(int sockfd,struct SmtpState *smtp,const char *type,const char *typename,\
                                 const char *encode,const char *position,const char *filename)
{
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "Content-Type: %s; name=\"%s\"\r\nContent-Transfer-Encoding: %s\r\nContent-Disposition: %s; filename=\"%s\"\r\n\r\n",\
                type,typename,encode,position,filename);
    if(OK != socksnd(sockfd, smtp->buffer))
    {
        return ERROR;
    }
    return OK;
}
//send a file
static inline int sndbodyfile(int sockfd,struct SmtpState *smtp,char *type,\
        const char *filename,const char *position,const char *boundary)
{
    int len = 0;
    int sessionlen;
    if(NULL == filename)
    {
        return OK;
    }
    if(OK != sndboundary(sockfd,smtp,boundary,NULL))
    {
        return ERROR;
    }
    if(OK != sndsessionfile(sockfd,smtp,type,filename,"base64",position,filename))
    {
        return ERROR;
    }

    //we should open the file first then read page by page
    int fd;
    unsigned char buf[SMTP_INPUT_BIN];
    fd = open(filename,O_RDONLY);
    if(fd < 0)
    {
        return ERROR;
    }
    while((len = read(fd,buf,SMTP_INPUT_BIN))> 0)
    {
        sessionlen = len;
        if(OK != sndbufbase64(sockfd,smtp,buf,sessionlen))
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
static inline int sndbodyfilelist(int sockfd,struct SmtpState *smtp,char *type,\
        const char **filelist,const char *position,const char *boundary)
{
    const char *filename;

    if(filelist == NULL)
    {
        return OK;
    }
    while((filename = *filelist++) != NULL)
    {
        if(OK != sndbodyfile(sockfd,smtp,type,filename,position,boundary))
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
static inline int smtp_send_message(int sockfd, struct SmtpState *smtp)
{
    int code;
    char strarray[128];
    const char *type,*value,**list;
    //when connect, we will get the server information here
    code = rcvcode(sockfd,smtp);
    if(code != 220)
    {
        debug_printf("smtp","server not ready:%d\n\r",code);
        return ERROR;
    }
    //then we will say hello to the server.ehlo means AUTH use the base64 in the user and passwd
    //the server should also say hello to us
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s%s\r\n", g_smtphelo, smtp->host?smtp->host:"NULL");
    if(OK != sndstrandcheck(sockfd,smtp,smtp->buffer,250))
    {
        debug_printf("smtp","email action failed:%d\n\r",code);
        return ERROR;
    }
    //then we will send the auth login here
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s\r\n","auth login");
    if(OK != sndstrandcheck(sockfd,smtp,smtp->buffer,334))
    {
        debug_printf("smtp","No login:%d\n\r",code);
        return ERROR;
    }

    //then send our username in the base64
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    memset(strarray,0,64);
    base64_encode((const unsigned char *)smtp->user,(char *)strarray,strlen(smtp->user));
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s\r\n", (char *)strarray);
    if(OK != sndstrandcheck(sockfd,smtp,smtp->buffer,334))
    {
        debug_printf("smtp","No login:%d\n\r",code);
        return ERROR;
    }

    //then send our passwd in the base64
    memset(smtp->buffer,0,SMTP_INPUT_BUFFER_SIZE);
    memset(strarray,0,64);
    base64_encode((const unsigned char *)smtp->passwd,(char *)strarray,strlen(smtp->passwd));
    snprintf(smtp->buffer, SMTP_INPUT_BUFFER_SIZE, "%s\r\n", (char *)strarray);
    if(OK != sndstrandcheck(sockfd,smtp,smtp->buffer,235))
    {
        debug_printf("smtp","login failed:%d\n\r",code);
        return ERROR;
    }
    //now send the mail from
    value = smtp->from;
    type = g_smtpmailfrom;
    if(OK != snditemandcheck(sockfd,smtp,type,value,250))
    {
        debug_printf("smtp","snd from command error\n\r");
        return ERROR;
    }
    //now send the mail to
    list = smtp->to;
    type = g_smtprcptto;
    if(OK != sndlistandcheck(sockfd,smtp,type,list,250))
    {
        debug_printf("smtp","snd to command error\n\r");
        return ERROR;
    }
    //now send the cc
    list = smtp->cc;
    type = g_smtprcptto;
    if(OK != sndlistandcheck(sockfd,smtp,type,list,250))
    {
        debug_printf("smtp","snd to command error\n\r");
        return ERROR;
    }
    //now send the bcc
    list = smtp->bcc;
    type = g_smtprcptto;
    if(OK != sndlistandcheck(sockfd,smtp,type,list,250))
    {
        debug_printf("smtp","snd to command error\n\r");
        return ERROR;
    }
    //now send the data
    if(OK != sndstrandcheck(sockfd,smtp,g_smtpdata,354))
    {
        debug_printf("smtp","snd data command error\n\r");
        return ERROR;
    }
    if(OK != sendheader(sockfd,smtp))
    {
        return ERROR;
    }

    const char *mixedb="mixed boundary";
    const char *relatedb="related boundary";
    const char *alternativeb = "alternative boundary";
    //mixed header--boundary
    if(OK != sndclassheader(sockfd,smtp,"multipart/mixed",mixedb))
    {
        return ERROR;
    }
    //send the related header
    if(OK != sndboundary(sockfd,smtp,mixedb,NULL))
    {
        return ERROR;
    }
    if(OK != sndclassheader(sockfd,smtp,"multipart/related",relatedb))
    {
        return ERROR;
    }
    //send the alternative header
    if(OK != sndboundary(sockfd,smtp,relatedb,NULL))
    {
        return ERROR;
    }
    if(OK != sndclassheader(sockfd,smtp,"multipart/alternative",alternativeb))
    {
        return ERROR;
    }
    //alternative part:text
    if(OK != sndbodytext(sockfd,smtp,"text/plain",smtp->msg,"utf-8",alternativeb))
    {
        return ERROR;
    }
    //alternative part:html
    if(OK != sndbodytext(sockfd,smtp,"text/html",smtp->html,"utf-8",alternativeb))
    {
        return ERROR;
    }
    //for the alternative end
    if(OK != sndboundary(sockfd,smtp,NULL,alternativeb))
    {
        return ERROR;
    }

    //then the relative new part which means the image in the body or image back ground
    //if any added here;
    if(OK != sndbodyfile(sockfd,smtp,"image/gif",smtp->inl,"inline",relatedb))
    {
        return ERROR;
    }
    //realtive end
    if(OK != sndboundary(sockfd,smtp,NULL,relatedb))
    {
        return ERROR;
    }
    //then the mixed new part, if any attachment here,please add it here
    if(OK != sndbodyfilelist(sockfd,smtp,"application/octet-stream",smtp->files,"attachment",mixedb))
    {
        return ERROR;
    }
    //mixed end
    if(OK != sndboundary(sockfd,smtp,NULL,mixedb))
    {
        return ERROR;
    }

    //body--file attachment
    //body --end
    if(OK != sndstrandcheck(sockfd,smtp,g_smtpcrnlperiodcrnl,250))
    {
        debug_printf("smtp","accept message error\n\r");
        return ERROR;
    }
    //send the quit
    if(OK != sndstrandcheck(sockfd,smtp,g_smtpquit,221))
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
bool_t smtp_configure( void *handle,  const char *host,const char *server,\
        const char *user,const char *passwd,const char **to,const char **cc,\
        const char  **bcc,const char *from,const char *subject,const char **files,\
        const char *msg,const char *html,const char *inl)
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

int smtp_send(void *handle)
{
    struct SmtpState *smtp = (struct SmtpState *)handle;
    struct sockaddr_in server;
    in_addr_t  serverip;
    int sockfd;
    int ret=-1;
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
    ret = smtp_send_message(sockfd, smtp);
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
    int cur;
    const char *list[CN_LIST_LIMIT];
}tagListCtrl;

//-h host -s server -u user -p passwd -to to -from from -topic topic -m msg
//bool_t sendemail(char *param)
ADD_TO_SHELL_HELP(smtp,"usage:smtp [-h host] [-s server] [-u user] [-p passwd] [-to to] [-from from] [-topic topic] [-m msg] [-file file] [-in inline]");
ADD_TO_IN_SHELL bool_t smtp(char *param)
{
    const char *host ="DJYOS_BOARD";
    const char *server = NULL;
    const char *user= NULL;
    const char *passwd= NULL;
    const char *from= NULL;
    const char *subject="DJYOS SMTP TEST";
    const char *msg="HELLO,THIS IS FROM DJYOS SMTP:www.djyos.com\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline\r\nline";
    const char *html="<html><body>"\
        "<p><img src=\"http://www.djyos.com/wp-content/themes/wpressmonster/images/logo.png\"></p><br/>"\
        "<h1>hello,djyos users.</h1>"\
        "<h4>婵″倹鐏夋担鐘哄厴閺�璺哄煂鏉╂瑥鐨濇穱鈽呯礉闁絿婀￠弰顖氥亰婵傛垝绨￠敍灞芥礈娑撻缚绻栫亸浣蜂繆閺夈儴鍤滄禍宸噅yos閻ㄥ墕mtp</h4>"\
        "<h4>閻楄锟窖冾洤娑撳绱伴弨顖涘瘮閸欐垿锟戒礁顦挎禍鐚寸礉閺�顖涘瘮閹跺嫰锟戒礁顦挎禍鐚寸礉娑旂喐鏁幐浣哥槕闁礁顦挎禍鐚寸礉瑜版挾鍔ф稊鐔告暜閹镐礁顦挎稉顏堟娴犺绱濇潻妯绘暜閹镐弓tml,鏉╂瑥鐨濇穱鈥虫皑閺勭棢tml閺嶇厧绱￠敍锟�</h4>"\
        "<h4>鐠囷附鍎忛崣鍌濓拷鍍簃tp.c閺傚洣娆㈤敍灞剧垽缁屾椽妫块敍姘劀鐢憡鏋冮張顒佹1KB瀹革箑褰搁敍娑樺絺闁線妾禒璺烘躬2KB瀹革箑褰搁敍宀冾嚞閸斺�崇箑妫板嫮鏆�鐡掑啿顧勭粚娲？</h4>"\
        "<h4>娴ｈ法鏁ら弬瑙勭《閿涙俺鐨熼悽鈺痬tp_open閼惧嘲褰囬崣銉︾労閿涘奔濞囬悽鈺痬tp_configure闁板秶鐤嗛幍锟界憰浣稿絺闁礁鍞寸�圭櫢绱濈拫鍐暏smtp_send閸欐垿锟戒線鍋栨禒璁圭礉鐠嬪啰鏁mtp_close閸忔娊妫撮崣銉︾労</h4>"\
        "<h4>閸欘垯浜掓担璺ㄦ暏鐎瑰啯娼甸崣鎴︼拷浣风鐏忎線鍋栨禒鑸垫降閹躲儱鎲¤ぐ鎾冲閻ㄥ嫯绻嶇悰宀�濮搁幀渚婄礉鏉╂瑥顕禍搴㈠閺堢儤娼电拠瀛樻Ц娑擃亙绗夐柨娆戞畱娑撶粯鍓�</h4>"\
        "<h1>濞嗐垼绻嬫担璺ㄦ暏</h1>"\
        "</body></html>";
    const char *inl = NULL;
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
    listto->list[0] = "zhangqf@sznari.com";

    int argc = 16;
    char *argv[16];
    string2arg(&argc,argv,param);
    int i =0;
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
    if(smtp_configure(handle,host,server,user,passwd,listto->list,listcc->list,\
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
ADD_TO_SHELL_HELP(enbase64file,"enbase64file srcfile dstfile");
ADD_TO_IN_SHELL  bool_t enbase64file(char *param)
{
    const char *srcfile;
    const char *dstfile;
    int srcfd;
    int dstfd;
    unsigned char srcbuf[SMTP_INPUT_BIN];
    unsigned char dstbuf[SMTP_INPUT_BUFFER_SIZE];
    int argc =2;
    const char *argv[2];
    int srclen = 0,dstlen = 0,writelen = 0;
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
    if((srcfd <0)||(dstfile < 0))
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
        base64_encode(srcbuf,dstbuf,srclen);
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

static struct shell_debug  gSmtpClientDebug[] =
{
    {
        "enbase64file",
        enbase64file,
        "enbase64file srcfile dstfile",
        NULL
    },
    {
        "smtp",
        smtp,
        "usage:smtp [-h host] [-s server] [-u user] [-p passwd] [-to to] [-from from] [-topic topic] [-m msg] [-file file] [-in inline]",
        NULL
    },
};
#define CN_SmtpClientDebug_NUM  ((sizeof(gSmtpClientDebug))/(sizeof(struct shell_debug)))
//static struct ShellCmdRsc gSmtpClientDebugCmdRsc[CN_SmtpClientDebug_NUM];
// =============================================================================
// 闁跨喐鏋婚幏鐑芥晸閺夊府缍囬幏绌塪d the SmtpClient debug to the system
// 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹风ara
// 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归崐锟�  闁跨喐鏋婚幏绌瀝ue闁跨喓鍗抽惂鍛婂  false婢堕亶鏁撻弶鎵虫閹凤拷
// =============================================================================
bool_t ServiceSmtpClient()
{
    shell_debug_add(gSmtpClientDebug, CN_SmtpClientDebug_NUM);
    return true;
}
