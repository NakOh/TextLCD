#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/mman.h>
#include <errno.h>

struct strcommand_varible {
    char rows;
    char nfonts;
    char display_enable;
    char cursor_enable;
    char nblink;
    char set_screen;
    char set_rightshit;
    char increase;
    char nshift;
    char pos;
    char command;
    char strlength;
    char buf[16];
};

static struct strcommand_varible strcommand;
static int initialized = 0;

#define TEXTLCD_BASE		 0xbc
#define TEXTLCD_COMMAND_SET		_IOW(TEXTLCD_BASE,0,int)
#define TEXTLCD_FUNCTION_SET		_IOW(TEXTLCD_BASE,1,int)
#define TEXTLCD_DISPLAY_CONTROL	_IOW(TEXTLCD_BASE,2,int)
#define TEXTLCD_CURSOR_SHIFT		_IOW(TEXTLCD_BASE,3,int)
#define TEXTLCD_ENTRY_MODE_SET	_IOW(TEXTLCD_BASE,4,int)
#define TEXTLCD_RETURN_HOME     _IOW(TEXTLCD_BASE,5,int)
#define TEXTLCD_CLEAR           _IOW(TEXTLCD_BASE,6,int)
#define TEXTLCD_DD_ADDRESS      _IOW(TEXTLCD_BASE,7,int)
#define TEXTLCD_WRITE_BYTE      _IOW(TEXTLCD_BASE,8,int)

void initialize()
{
    if(!initialized)
    {
        strcommand.rows = 0;
        strcommand.nfonts = 0;
        strcommand.display_enable = 1;
        strcommand.cursor_enable = 0;
        strcommand.nblink = 0;
        strcommand.set_screen = 0;
        strcommand.set_rightshit = 1;
        strcommand.increase = 1;
        strcommand.nshift = 0;
        strcommand.pos = 10;
        strcommand.command = 1;
        strcommand.strlength = 16;
        initialized = 1;
    }
}
int TextLCDIoctl(int cmd, char *buf)
{
    int fd,ret,i;


    fd = open("/dev/textlcd",O_WRONLY | O_NDELAY);
    if(fd < 0) return -errno;

    if(cmd == TEXTLCD_WRITE_BYTE) {
        ioctl(fd,TEXTLCD_DD_ADDRESS,&strcommand,32);
        for(i=0;i<strlen(buf);i++)
        {
            strcommand.buf[0] = buf[i];
            ret = ioctl(fd, cmd, &strcommand, 32);
        }
    } else {
        ret = ioctl(fd, cmd, &strcommand, 32);
    }

    close(fd);

    return ret;
}

jint
Java_myapplication_com_textlcd_TextLCDActivity_TextLCDOut( JNIEnv* env, jobject thiz, jstring data0, jstring data1 )
{
    jboolean iscopy;
    char *buf0, *buf1;
    int fd,ret;
    fd = open("/dev/textlcd",O_WRONLY | O_NDELAY);
    if(fd < 0) return -errno;
    initialize();
    buf0 = (char *)(*env)->GetStringUTFChars(env,data0,&iscopy);
    buf1 = (char *)(*env)->GetStringUTFChars(env,data1,&iscopy);
    strcommand.pos = 0;
    ioctl(fd,TEXTLCD_DD_ADDRESS,&strcommand,32);
    ret = write(fd,buf0,strlen(buf0));
    strcommand.pos = 40;
    ioctl(fd,TEXTLCD_DD_ADDRESS,&strcommand,32);
    ret = write(fd,buf1,strlen(buf1));
    close(fd);
    return ret;
}

jint
Java_myapplication_com_textlcd_TextLCDActivity_IOCtlWriteByte( JNIEnv* env, jobject thiz,  jstring data )
{
    jboolean iscopy;
    char *buf;
    int i,ret;
    buf = (char *)(*env)->GetStringUTFChars(env,data,&iscopy);
    initialize();
    ret = TextLCDIoctl(TEXTLCD_WRITE_BYTE,buf);
    return ret;
}

jint
Java_myapplication_com_textlcd_TextLCDActivity_IOCtlPos( JNIEnv* env, jobject thiz,  jint pos )
{
    initialize();
    strcommand.pos = pos;
    return TextLCDIoctl(TEXTLCD_DD_ADDRESS,NULL);
}

jint
Java_myapplication_com_textlcd_TextLCDActivity_IOCtlClear( JNIEnv* env, jobject thiz )
{
    initialize();
    return TextLCDIoctl(TEXTLCD_CLEAR,NULL);
}

jint
Java_myapplication_com_textlcd_TextLCDActivity_IOCtlReturnHome( JNIEnv* env, jobject thiz)
{
    initialize();
    return TextLCDIoctl(TEXTLCD_RETURN_HOME,NULL);
}

jint
Java_myapplication_com_textlcd_TextLCDActivity_IOCtlDisplay( JNIEnv* env, jobject thiz, jboolean bOn)
{
    initialize();
    if(bOn) {
        strcommand.display_enable =  0x01;
    } else {
        strcommand.display_enable =  0x00;
    }
    return TextLCDIoctl(TEXTLCD_DISPLAY_CONTROL,NULL);
}
jint
Java_myapplication_com_textlcd_TextLCDActivity_IOCtlCursor( JNIEnv* env, jobject thiz , jboolean bOn)
{
    initialize();
    if(bOn) {
        strcommand.cursor_enable = 0x01;
    } else  {
        strcommand.cursor_enable = 0x00;
    }
    return TextLCDIoctl(TEXTLCD_DISPLAY_CONTROL,NULL);
}

jint
Java_myapplication_com_textlcd_TextLCDActivity_IOCtlBlink( JNIEnv* env, jobject thiz, jboolean bOn )
{
    initialize();
    if(bOn) {
        strcommand.nblink = 0x01;
    } else {
        strcommand.nblink = 0x00;
    }
    return TextLCDIoctl(TEXTLCD_DISPLAY_CONTROL,NULL);
}
