    /*************************************************************************
    	> File Name: ser.h
    	> Author:Wsundaysky 
    	> Mail:Wsundaysky@outlook.com 
    	> Created Time: 2018年03月11日 星期日 10时34分31秒
     ************************************************************************/
    
    #ifndef _SER_H
    #define _SER_H
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <string.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include <sqlite3.h>
    #include <time.h>
    
    #define  R  1   //  user register
    #define  L  2   //  user login
    #define  Q  3   //  query word
    #define  H  4   //  history record
    typedef struct{
        int flag;
        sqlite3 *db;
        char name[20];
        char data[256];
        char word[20];
        int connfd;
        
    }STAT;
    
    
    //处理 recv和send 发送失败
    extern void deal_with_recv_and_send(int ret);
    //处理sqlite3函数操作失败
    extern void deal_with_sqlite3_option(int ret,char *errmsg);
      //注册
     extern void regist(int connfd,STAT *cmd, sqlite3 *db);
    //登录
    extern int login(int connfd,STAT *cmd,sqlite3 *db);
    //获取时间
    extern void get_date(char *data);
    //查询单词
    extern int query(int connfd,STAT *cmd,sqlite3 *db);
    //历史记录
    extern int  history(int connfd,STAT *cmd,sqlite3 *db);
    
    //线程函数
     extern void *function(void *arg); //查询功能函数
    #endif
