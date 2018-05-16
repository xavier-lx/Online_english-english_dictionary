    /*************************************************************************
    	> File Name: cli.h
    	> Author:Wsundaysky 
    	> Mail:Wsundaysky@outlook.com 
    	> Created Time: 2018年03月11日 星期日 10时17分56秒
     ************************************************************************/
    
    #ifndef _CLI_H
    #define _CLI_H
    
    #include<stdio.h>
    #include<stdlib.h>
    #include<unistd.h>
    #include<sys/socket.h>
    #include<string.h>
    #include<arpa/inet.h>
    #include<netinet/in.h>
    #include<sys/types.h>
    #include<fcntl.h>
    #include<sqlite3.h>
    
    #define  R  1   //  user register
    #define  L  2   //  user login
    #define  Q  3   //  query word
    #define  H  4   //  history record
    
    typedef struct{
        int flag;    //1表示继续
        //2表示进行下一个操作
        //3表示退出
        sqlite3 *db;
        char name[20];
        char data[256];
        char word[20];
        int connfd;
    }STAT;
    
    
    void length(char *buf);//检测输入的是数字和密码,但是长度1-15,第一个开头是小写字母或则大些字母.
    
    void deal_with_recv_and_send(int ret);
    
    void print_word_information();//打印word操作说明 
    void print_user_opration();//打印用户使用说明
     //用来判断recv和send函数接受情况
    void deal_with_regist(int connfd,STAT * cmd);
    
    //注册函数,如果用户名重复就重新注册
    int deal_with_login(int connfd,STAT *cmd);
    
    void  deal_with_query(int connfd,STAT *cmd);//查找单词
    
    //查看用户的历史情况.
    void deal_with_history(int connfd,STAT * cmd);
    #endif
    
