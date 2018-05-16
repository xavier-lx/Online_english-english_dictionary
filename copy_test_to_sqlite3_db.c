    /*************************************************************************
    	> File Name: copy_test_to_sqlite2_doc.c
    	> Author:Wsundaysky 
    	> Mail:Wsundaysky@outlook.com 
    	> Created Time: Fri 09 Mar 2018 02:57:54 PM CST
     ************************************************************************/
     //功能:将dict.txt 文件的单词copy到dict.db数据文件中,作为在线词典的词典.
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sqlite3.h>
    
    int main(int argc, char *argv[])
    {
    
        int ret;
        FILE *fp;
        char buf[1024] = "\0";
        char sql[1024] = "\0";
        char *p_s = NULL;
        char *s = NULL;
        sqlite3 *db;
        char *errmsg;
        fp = fopen("dict.txt","r");
        if (NULL == fp ){
            perror("fopen error");
            return -1;
        }
            ret = sqlite3_open("dict.db",&db);
            if (SQLITE_OK != ret){
                fprintf(stderr,"sqlites3_open error:%s\n",sqlite3_errmsg(db));
                return -1;
            }
            sprintf(sql,"create table if not exists words (id integer primary key autoincrement,word text, translations text);");
            ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
            if (SQLITE_OK != ret){
                fprintf(stderr,"create table faile:%s\n",errmsg);
                return -1;
            }
        while(fgets(buf,sizeof(buf),fp)){
            p_s = buf; 
            while(*p_s++ != ' ');
            s = p_s;
            *s = '\0';
            while(*s++ == ' ');
            buf[strlen(buf)-1] = '\0';
            sprintf(sql,"insert into words (word,translations) values(\"%s\",\"%s\");",buf,s);
            ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
            if (SQLITE_OK != ret){
                fprintf(stderr,"insert into failes :%s\n",errmsg);
                return -1;
            }
        }    
        return 0;
    }
