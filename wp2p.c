#include <stdio.h>
 
#include <curl/curl.h>

#include<string.h>
 
#include<stdlib.h>
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/*
告诉到大家一个好消息，一直以来我都想用C语言开发一个命令行的下载工具
在百度的过程中发现一个liburl网络传输库，对我开发wp2p的工具很有帮助
于是百度了相关的资料，大都数资料都转载的，连续半个月都一无所获，后来
查看参照了liburl官网例子
struct FtpFile ftpfile={
"curl.tar.gz", /* name to store the file as if successful */ 
NULL
  };开发，但我发现当我下载某个文件的时候，我必须把要下载的文件名放到哦这里
它才能正确的下载我要的文件，一次偶然的机会才完成了这个工具。
声明：本程序所有的代码均来源于网络，我所做的只是把们整合在一起而已，在整个程序中
struct FtpFile ftpfile={Get_File_Name(argv[1])};这行代码关键代码，程序
遵循开源的精神，自由发布，自由修改。
如有需要请联系：maichenzhen@sina.com;
*/
FILE*fp;
struct FtpFile{
char*filename;
FILE*stream;
};
static size_t WriteData(void *buffer, size_t size, size_t nmemb,void *stream)
{

 struct FtpFile *out=(struct FtpFile *)stream;    
  if(out && !out->stream) {
    /* open file for writing */ 
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */ 
  }
  return fwrite(buffer, size, nmemb, out->stream);
}
char *Get_File_Name(char *url)
{
FILE*fp; 
char* save_name, *pos;
int name_len;
name_len = strlen(url);
pos = url + name_len;
while(*pos != '/' && pos != url)
pos --;
if(pos == url)
{
save_name = url+1;
return save_name;
}
name_len = name_len-(pos-url);
save_name = (char*) malloc(name_len+1);
memcpy(save_name,pos+1,name_len);
return save_name;
} 
int main(int argc,char *argv[])
{
  CURL *curl;
  CURLcode res;
  struct FtpFile ftpfile={Get_File_Name(argv[1])}; //获取文件名赋值给ftpfile;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);  //用于接受用户传进来的url地址
    curl_easy_setopt(curl, CURLOPT_HEADER, 1); 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);   //功能回调函数用于数据传输
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&ftpfile);       //设置指向我们的struct的指针传递给回调
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); //我们激活SSL，我们需要控制和数据
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);    //打印信息
    res = curl_easy_perform(curl);    //执行下载
    curl_easy_cleanup(curl);
    if(CURLE_OK!=res)      //判断是否下载成功
{
fprintf(stderr,"curl ltold us %d\n",res);
if(ftpfile.stream)
fclose(ftpfile.stream);
}
}
  curl_global_cleanup();
 
  return 0;
}
