    /*************************************************************************
      > File Name: main_ser_fun.c
      > Author:Wsundaysky 
      > Mail:Wsundaysky@outlook.com 
      > Created Time: 2018年03月10日 星期六 10时43分48秒
     ************************************************************************/
    
    #include "ser.h"
    
    void deal_with_recv_and_send(int ret){
        if(-1 == ret){
            perror("error");
            exit(-1);
        }
    }
    //int deal_with_sqlite3_option(int ret,char *errmsg){
    //    if(SQLITE_OK != ret){
    //        printf("error:%s\n",errmsg);
    //        sqlite3_free(errmsg);
    //        return 1;   
    //    }
    //    return 0;
    //}
    void regist(int connfd,STAT *cmd, sqlite3 *db){
        char sql[256] = "\0";
        int ret = 0;
        char *errmsg = NULL;
        int nrow = 0,ncolumn = 0;
        char **result = NULL;
    
        memset(sql,0,sizeof(sql));
        sprintf(sql,"select * from user_infomations where name=\"%s\";",cmd->name);
    
        if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
            fprintf(stderr,"error:%s\n",errmsg);
            sqlite3_free(errmsg);
            return;
            ///////////////////
        }
        if (nrow){  //用户存在
            memset(cmd->data,0,sizeof(cmd->data));
            sprintf(cmd->data,"user %s is always exists",cmd->data);
    
            ret = send(connfd,cmd,sizeof(STAT),0);
            deal_with_recv_and_send(ret);
            sqlite3_free_table(result);
            return;
        }
        else{ //用户不存在就添加
            memset(cmd->data,0,sizeof(cmd->data));
            sprintf(sql,"insert into user_infomations(name,passwd,stat) values(\"%s\",\"%s\",'N');",cmd->name,cmd->data);
            ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
            if (SQLITE_OK != ret){
                fprintf(stderr,"error %s\n",errmsg);
                sqlite3_free(errmsg);  //这里也是开辟一个空间.需要释放
                return;
            }
            memset(cmd->data,0,sizeof(cmd->data));
            strcpy(cmd->data,"Resigter is OK");
    
            ret = send(connfd,cmd,sizeof(STAT),0);
            deal_with_recv_and_send(ret);
            return;
        } 
    }
    int login(int connfd,STAT *cmd,sqlite3 *db){
        char sql[256];
        char *errmsg;
        char **result;
        int nrow=0 ,ncolumn = 0;
        int ret;
        //匹配用户输入账户
    
        memset(sql,0,sizeof(sql));
        sprintf(sql,"select *  from user_infomations where name='%s' and passwd='%s';",cmd->name,cmd->data);
    
        ret = sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg);
        if(SQLITE_OK  != ret){
            fprintf(stderr,"error sqlite3 :%s\n",errmsg);
            sqlite3_free(errmsg);
            return 0;
        }
        if (!nrow){  //not exists
            strcpy(cmd->data,"your name and your passwd are not exists");
            ret = send(connfd,cmd,sizeof(STAT),0);
            deal_with_recv_and_send(ret);
            return 0;
        }
        else{  //exists
            strcpy(cmd->data,"OK");
            memset(sql,0,sizeof(sql));
            sprintf(sql,"update user_infomations set stat='Y' where name=\"%s\";",cmd->name);
            ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
            //   deal_with_sqlite3_option(ret,errmsg);
            if(SQLITE_OK != ret){
                fprintf(stderr,"error:%s\n",errmsg);
                sqlite3_free(errmsg);
                return 0;   
            }
    
            ret = send(connfd,cmd,sizeof(STAT),0);
            deal_with_recv_and_send(ret);
            sqlite3_free_table(result);
            printf("端口号:%d\n",connfd);
            return 1;
        }
    }
    
    void get_date(char *data){
        time_t t;
        struct tm *tp;
        time(&t);
        tp = localtime(&t);
        strftime(data,64,"%Y-%m-%d %H:%M:%S",tp);
        return;
    }
    int query(int connfd,STAT *cmd,sqlite3 *db){
    
        char sql[256] = "\0";
        char **result;
        int nrow,ncolumn;
        char *errmsg = NULL;
        char date[64] = "\0";
        int ret;
    
        printf("now is querrying \n");
    
        sprintf(sql,"select *  from words where word=\"%s\" ;",cmd->word);
        ret = sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg);
        //deal_with_sqlite3_option(ret,errmsg);
        if(SQLITE_OK != ret){
            fprintf(stderr,"error:%s\n",errmsg);
            sqlite3_free(errmsg);
            return 1;   
        }
        if (nrow){ //查询到了
            sprintf(cmd->data,"%s",result[5]);
            sqlite3_free_table(result);
            memset(sql,0,sizeof(sql));
            get_date(date);
            sprintf(sql,"insert into  history values(\"%s\",\"%s\",\"%s\");" ,cmd->name,cmd->word,date);
        }
        else{  //没有查到单词
            sprintf(cmd->data,"have no word in dictory");
        }
    
        ret = send(connfd,cmd,sizeof(STAT),0);
        deal_with_recv_and_send(ret);
    
        return 0;
    }
    int history(int connfd,STAT * cmd,sqlite3 *db){
        char sql[256] = "\0";
        char *errmsg = NULL;
        char **result = NULL;
        int nrow,ncolumn,i,j = 0;
        int ret;
        printf("you are watching history\n");
        sprintf(sql,"select * from history where name=\"%s\";",cmd->name);
        if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
            printf("%s\n",errmsg);
            sqlite3_free(errmsg);
        }
        memset(cmd->data,0,sizeof(cmd->data));
        if(nrow){
            i = (nrow+1)*ncolumn;
            while(j != i){
                strcpy(cmd->data,result[j]);
                ret = send(connfd,cmd,sizeof(STAT),0);
                deal_with_recv_and_send(ret);
                j++;
            }
            cmd->data[0] = '\0';
            ret = send(connfd,cmd,sizeof(STAT),0);
            deal_with_recv_and_send(ret);
            sqlite3_free_table(result);
        }
        return 0;
    }
    
    void *function(void * arg){
        //功能:登陆,注册,退出, 
        STAT *cmd = (STAT *)arg;
        int connfd = cmd->connfd;
        int ret = 0;
        sqlite3 *db = cmd->db;
        while((ret = recv(connfd,cmd,sizeof(STAT),0)) ){
            if (-1 == ret){
                perror("error");
                break;
            }
            switch(cmd->flag){
                case R: regist(connfd,cmd,db); break;
                case L: login(connfd,cmd,db);  break;
                case Q: query(connfd,cmd,db);  break;
                case H: history(connfd,cmd,db);break;
            }
        }
        close(connfd);
        printf("client is quit\n");
    }
