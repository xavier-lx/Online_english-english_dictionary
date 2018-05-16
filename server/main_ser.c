    /*************************************************************************
    	> File Name: main_ser.c
    	> Author: 
    	> Mail: 
    	> Created Time: 2018年03月09日 星期五 20时11分26秒
     ************************************************************************/
    
    #include "ser.h"
    
    int main(int argc, char *argv[]){
        
        int sockfd,connfd;
        int ret;
        struct sockaddr_in seraddr,cliaddr;
        socklen_t addrlen = sizeof(cliaddr);
        sqlite3 *db;
        if (sqlite3_open("dict.db",&db) != SQLITE_OK){
            fprintf(stdin,"error:%s\n",sqlite3_errmsg(db));
            return -1;
        }
        
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(-1 == sockfd){
            perror("socket error");
            return -1;
        }
    
        memset(&seraddr,0,sizeof(seraddr));
        seraddr.sin_family = AF_INET;
        seraddr.sin_port = htons(8888);
        seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        ret = bind(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
        if (-1 == sockfd){
            perror("bind error");
            return -1;
        }
        ret = listen(sockfd,5);
        if (-1 == sockfd){
            perror("listen error");
            return -1;
        }
        
        printf("open server\n");
        while(1){
            pthread_t tid;
            connfd = accept(sockfd,(struct sockaddr*)&cliaddr,&addrlen);
            if (-1 == connfd){
                perror("error error");
                return -1;
            }
            STAT cmd;
            cmd.connfd = connfd;
            cmd.db = db;
            printf("successfully\n");
            pthread_create(&tid,NULL,function,(void *)&cmd);
            pthread_join(tid,NULL);
        }
        return 0;
    }
