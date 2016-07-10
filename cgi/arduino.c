#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <parson.h>

#define CONTENT_LENGTH "CONTENT_LENGTH"
#define BUF_SIZE 256
#define REQUEST_METHOD "REQUEST_METHOD"
#define REQUEST_METHOD_GET "GET"
#define REQUEST_METHOD_POST "POST"
#define REQUEST_METHOD_PUT "PUT"
#define REQUEST_METHOD_DELETE "DELETE"
#define LOG_FILE_PATH "/home/kame/cgi/log/arduino.log"
#define DAT_FILE_PATH "/home/kame/cgi/data/arduino.dat"
#define SESSIONID "HTTP_SESSION_ID"
#include "secret.h"

int error(int);
//int main( int , char ** );
void putlog(char *);
void putdat(const char *);
void printdat(void);

FILE *fpLog = NULL;
FILE *fpDat = NULL;

//int main( int argc , char **argv )
int main(void)
{
	if ((fpLog = fopen(LOG_FILE_PATH, "a")) == NULL) {
		putlog("log file open error");
		error(5);
	}
//	putlog("main start");

	char* request_method = NULL;
	if (getenv(REQUEST_METHOD)!=NULL)
	{
		request_method = getenv(REQUEST_METHOD);
	}
	else
	{
		putlog("request method is null");
		error(8);
	}

	if (strcmp(request_method, REQUEST_METHOD_GET) != 0)
	{
	//Authentication
	if (getenv(SESSIONID)!=NULL)
		{
			char* sessionid = getenv(SESSIONID);
			if (strcmp(sessionid, SYMMETRICID)==0)
			{
				putlog("valid session id");
			}
			else
			{
	//			putlog("invalid session id");
				error(6);
			}
		}
		else
		{
	//		putlog("session id is null");
			error(3);
		}
	}

	//==================PROCESS FOR GET======================
	if(strcmp(request_method, REQUEST_METHOD_GET) == 0)
	{
//		putlog("GET is called");

  printf( "Content-type: text/html\n\n" );

	printdat();

	}
	//==================PROCESS FOR POST======================
	else if(strcmp(request_method, REQUEST_METHOD_POST) == 0)
	{
		putlog("POST is called");

	char *pchBuf = NULL;
	int inLen = 0;

  printf( "Content-type: application/json\n\n" );


	
	if (getenv(CONTENT_LENGTH)!=NULL)
	{
		inLen = atoi(getenv(CONTENT_LENGTH));
	}
	else
	{
		putlog("content length is null");
		error(4);
	}
	{
		char s[256]; sprintf(s, "inLen=%d", inLen);
		putlog(s);
	}
	if (inLen==0){
		putlog("content length is 0");
		error(1);
	}

	putlog("trace -1");
	pchBuf = (char *)calloc(inLen+1, sizeof(char));
	if (!pchBuf){
		error(2);
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

  JSON_Value *root_value = json_parse_string(pchBuf);
//  JSON_Array *tweets = json_value_get_array(root_value);
//  for (i = 0; i < json_array_get_count(tweets); i++) {
//    JSON_Object *tweet = json_array_get_object(tweets, i);
//    printf("%s: %s\n",
//      json_object_dotget_string(tweet, "user.screen_name"),
//      json_object_dotget_string(tweet, "text"));
//  }
  JSON_Object *json_object = json_value_get_object(root_value);
	const char* pchValue = NULL;
	pchValue = json_object_dotget_string(json_object, "key");
	pchValue = json_object_dotget_string(json_object, "test");

	if(pchValue != NULL)
	{
		putdat(pchValue);
	}

	JSON_Value *new_root_value = json_value_init_object();
	JSON_Object *new_json_object = json_value_get_object(new_root_value);
	json_object_set_string(new_json_object, "hakuna", "matata");

//	printf(json_serialize_to_string(new_root_value));

//	printf("{\"result\":\"%s\"}", pchValue);

  json_value_free(root_value);
  json_value_free(new_root_value);

#if 0
	pchBufTemp = pchBuf;
	for(i=0; i<inLen; i++)
	{
		//printf("%c", *pchBufTemp);
		putchar(*pchBufTemp);
		pchBufTemp++;
	}
#else
//	printf("%s", pchBuf);
#endif
	free(pchBuf);
	putlog("trace 1");

	putlog("trace end");

	}
	//==================PROCESS FOR PUT======================
	else if(strcmp(request_method, REQUEST_METHOD_PUT) == 0)
	{
		//TBD
		putlog("PUT is called");
	}
	//==================PROCESS FOR DELETE======================
	else if(strcmp(request_method, REQUEST_METHOD_DELETE) == 0)
	{
		//TBD
		putlog("DELETE is called");
	}





        //printf( "Content-type: text/html\n\n" );
//        printf( "\n\n" );
//        printf( "Hello, world.\n" );
	fclose(fpLog);
	return;
}

int error(int inErrorCode){
  printf( "{\"error\":\"%d\"}\n", inErrorCode );
	fclose(fpLog);
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
	sprintf(s, "[%4d/%2d/%2d %2d:%2d:%2d %d] %s",
		local->tm_year + 1900,
		local->tm_mon + 1,
    local->tm_mday,
    local->tm_hour,
    local->tm_min,
    local->tm_sec,
    local->tm_isdst, message);
	fprintf(fpLog, "%s\n", s);
	return;
}

void putdat(const char* message)
{
  time_t timer;
  struct tm *local;
  struct tm *utc;

  /* 現在時刻を取得 */
  timer = time(NULL);
  local = localtime(&timer); /* 地方時に変換 */

	char s[256];
	if ((fpDat = fopen(DAT_FILE_PATH, "a")) == NULL) {
		putlog("dat file open error");
		error(7);
	}
#define DATAFORMAT "%d/%d/%d %d:%d:%d\t%s"
	sprintf(s, DATAFORMAT,
		local->tm_year + 1900,
		local->tm_mon + 1,
    local->tm_mday,
    local->tm_hour,
    local->tm_min,
    local->tm_sec, message);
	fprintf(fpDat, "%s\n", s);
	fclose(fpDat);
	return;
}

void printdat(void)
{
	char s[256];
	if ((fpDat = fopen(DAT_FILE_PATH, "r")) == NULL) {
		putlog("dat file open error");
		error(9);
	}
//	while (fscanf(fpDat, "%s", s) != EOF)
	while (fgets(s, 256, fpDat) != NULL)
	{
		printf("%s<br>", s);
	}
	fclose(fpDat);
	return;
}

