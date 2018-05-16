    /*************************************************************************
    > File Name: main_ser.c
    > Author: Wsundaysky
    > Mail:   Wsundaysky@outlook.com
    > Created Time: 2018年03月09日 星期五 20时11分26秒
    ************************************************************************/
    //客户端主程序:
    #include "cli.h"
    
    int main(int argc, char *argv[]){
    
        int sockfd;
        int ret;
        int n;
        int num;
        struct sockaddr_in seraddr;
        socklen_t addrlen = sizeof(seraddr);
    
        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(-1 == sockfd){
            perror("socket error");
            return -1;
        }
    
        memset(&seraddr,0,sizeof(seraddr));
        seraddr.sin_family = AF_INET;
        seraddr.sin_port = htons(8888);
        seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        ret = connect(sockfd,(struct sockaddr*)&seraddr,addrlen);
        deal_with_recv_and_send(ret);
    
        STAT *cmd = (STAT *)malloc(sizeof(STAT));
        if (NULL == cmd){
            perror("error");
            return 0;
        }
        ret = send(sockfd,cmd,sizeof(STAT),0);
        deal_with_recv_and_send(ret);
        
        while(1){
            print_user_opration();//打印用户使用说明
            printf("plesae your choose\n");
            scanf("%d",&n);
            if (n > 4 && n < 0){
                printf("please watch Usage\n");
                continue;
            }
            
                switch(n){
                    case 1: deal_with_regist(sockfd,cmd);break;
    
                    case 2: num = deal_with_login(sockfd,cmd);break;
    
                    case 3: close(sockfd);exit(0);
                }
            if (1 == num)
                break;
        }
        while(1){
            print_word_information();
    
            printf("please input your choose\n");
            scanf("%d",&n);
            
            if (n > 4 && n < 0){
                printf("please watch Usage\n");
                continue;
            }
            switch(n){
                case 1:  deal_with_query(sockfd,cmd);break;
                case 2:  deal_with_history(sockfd,cmd);break;
                case 3:  close(sockfd);exit(0);
            }
        }
        return 0;
    }
    
    
    
    
    
    
    
    
    
    
    
    
