    /*************************************************************************
      > File Name: main_cli_fun.c
      > Author:Wsundaysky 
      > Mail:Wsundaysky@outlook.com 
      > Created Time: 2018年03月10日 星期六 17时09分34秒
     ************************************************************************/
    //需要修改的循环的问题  就是那个,输入一次之后还想输入第二次:
    #include "cli.h"
    
    void length(char *buf)
    {
        char *p;
        int flag = 0;
        while(1){
            if (1 == flag){
                printf("please input first letter is lowercase letters or capital\n ");
                fgets(buf,sizeof(buf),stdin);
            }
    
            if (2 <= strlen(buf) && 15 >= strlen(buf)){
                buf[strlen(buf)-1] = '\0';
                p = buf;
                if (('a'<= *p && 'z' >= *p ) || ('A'<= *p && 'Z' >= *p)){
    
                    while( *p++){
                        if(('0' <= *p && '9' >= *p) || (('a'<= *p && 'z' >= *p ) || ('A'<= *p && 'Z' >= *p)))
                            continue;
                        else 
                            flag = 1;
                    }
                    if ('\0' == *(--p))
                        break;
                }
                else
                    flag = 1;
            }
            else 
                flag = 1;
        }
        return ;
    }
    void print_word_information(){
        printf("********************************************\n");
        printf("  **************1.lookup******************  \n");
        printf("  **************2.history*****************  \n");
        printf("  ***************3.quit*******************  \n");
        printf("********************************************\n");
    }
    
    void print_user_opration(){  //打印用户使用说明
        printf("********************************************\n");
        printf("  *************1.regiter******************  \n");
        printf("  *************2.login********************  \n");
        printf("  *************3.quit*********************  \n");
        printf("********************************************\n");
    }
    void deal_with_recv_and_send(int ret){
        if(-1 == ret){
            perror("error");
            exit(-1);
        }
    }
    void deal_with_regist(int connfd,STAT * cmd){
    
        int ret;
        cmd->flag = R; //注册用户
        printf("input user name\n");
        fgets(cmd->name,sizeof(cmd->name),stdin);
        length(cmd->name);
    
        printf("input user passwd\n");
        fgets(cmd->data,sizeof(cmd->data),stdin);
        length(cmd->data);
    
        ret = send(connfd,cmd,sizeof(STAT),0);
        deal_with_recv_and_send(ret);
    
        ret = recv(connfd,cmd,sizeof(STAT),0);
        deal_with_recv_and_send(ret);
    
        printf("%s\n",cmd->data);  //收到服务器发来的信息,
    
        return;
    }
    int deal_with_login(int connfd,STAT *cmd){
        
        int ret;
        cmd->flag = L;
        printf("you are login now\n");
    
        printf("input user name\n");
        fgets(cmd->name,sizeof(cmd->name),stdin);
        length(cmd->name);
    
        
        printf("input user passwd\n");
        fgets(cmd->data,sizeof(cmd->data),stdin);
        length(cmd->data);
        //da yin word 
    
        ret = send(connfd,cmd,sizeof(STAT),0);
        deal_with_recv_and_send(ret);
    
        ret = recv(connfd,cmd,sizeof(STAT),0);
        deal_with_recv_and_send(ret);
    
        if (!strncmp(cmd->data,"OK",3)){
            printf("%s\n",cmd->data);  //收到服务器发来的信息,
            return 1;
        }
    
        printf("%s\n",cmd->data);  //收到服务器发来的信息,
        return 0;
    }
    void  deal_with_query(int connfd,STAT *cmd){  //查找单词
    
        int ret;          //这里出现了问题
        cmd->flag = Q;
        while(1){
            printf("please you look up word\n");
            //scanf("%s",cmd->word);
            fgets(cmd->word,sizeof(cmd->word),stdin);
            length(cmd->word);
            if (!strncmp(cmd->word,"#",2))
                break;   //遇到#就结束.否则继续
            ret = send(connfd,cmd,sizeof(STAT),0);
            deal_with_recv_and_send(ret);
    
            ret = recv(connfd,cmd,sizeof(cmd->data),0);
            deal_with_recv_and_send(ret);
            printf("%s\n",cmd->data);
        }
        
        return ;
    }
    void deal_with_history(int connfd,STAT * cmd){
        int ret;
        int i = 0;
        cmd->flag = H;
        ret = send(connfd,cmd,sizeof(STAT),0); //发送查找单词历史记录
        deal_with_recv_and_send(ret);
        while(1){
            ret = recv(connfd,cmd,sizeof(STAT),0);
            deal_with_recv_and_send(ret);
            if ('\0' == cmd->data[0]){
                printf("\n");
                printf("have no history\n");
                break;
            }
            if(3 == i){
                printf("\n");
                i = 0;
            }
            printf("%s\t\t",cmd->data);
    
            i++;
        }
        return ;
    }
