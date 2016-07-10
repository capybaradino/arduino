#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CONTENT_LENGTH "CONTENT_LENGTH"
#define BUF_SIZE 256
#define REQUEST_METHOD "REQUEST_METHOD"
#define DAT_FILE_PATH "/home/kame/cgi/data/arduino.dat"
#define LOG_FILE_PATH "/home/kame/cgi/log/arduino.log"

int error(int);
//int main( int , char ** );
void putlog(char *);

FILE *fp = NULL;

//int main( int argc , char **argv )
int main(void)
{
	char *pchBuf = NULL;
	int inLen = 0;

  printf( "Content-type: application/json\n\n" );

	if ((fp = fopen(LOG_FILE_PATH, "w")) == NULL) {
		error(3);
	}
	putlog("main start");
	
	if (getenv(CONTENT_LENGTH)!=NULL)
	{
		inLen = atoi(getenv(CONTENT_LENGTH));
	}
	{
		char s[256]; sprintf(s, "inLen=%d", inLen);
		putlog(s);
	}
	if (inLen==0){
		error(1);
		return;
	}

	putlog("trace -1");
	pchBuf = (char *)calloc(inLen+1, sizeof(char));
	if (!pchBuf){
		error(2);
		return;
	}

	char *pchBufTemp = pchBuf;
	int i=0;
	char c;
	for(i=0; i<inLen; i++)
	{
		c = getchar();
//		putchar(c);
//		putlog(&c);
		*pchBufTemp = c;
//		putchar(*pchBufTemp);
		pchBufTemp++;
		if((c == EOF) || (c == 0x00) || (c == '\n'))break;
	}
	putlog("trace 0");

#if 0
	pchBufTemp = pchBuf;
	for(i=0; i<inLen; i++)
	{
		//printf("%c", *pchBufTemp);
		putchar(*pchBufTemp);
		pchBufTemp++;
	}
#else
	printf("%s", pchBuf);
#endif
	putlog("trace 1");

	free(pchBuf);



        //printf( "Content-type: text/html\n\n" );
//        printf( "\n\n" );
//        printf( "Hello, world.\n" );
	putlog("trace end");
}

int error(int inErrorCode){
  printf( "{\"error\":\"%d\"}\n", inErrorCode );
	fclose(fp);
	exit(EXIT_FAILURE);
}

void putlog(char* message)
{
  time_t timer;
  struct tm *local;
  struct tm *utc;

  /* 現在時刻を取得 */
  timer = time(NULL);
  local = localtime(&timer); /* 地方時に変換 */

	char s[256];
	sprintf(s, "[%4d/%2d/%2d/%2d:%2d:%2d %d] %s",
		local->tm_year + 1900,
		local->tm_mon + 1,
    local->tm_mday,
    local->tm_hour,
    local->tm_min,
    local->tm_sec,
    local->tm_isdst, message);
	fprintf(fp, "%s\n", s);
	return;
}

