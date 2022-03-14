#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <fcntl.h>
#define _GNU_SOURCE

char *command;
char lazyBox_path[4096];

int ln_main();
int stat_main();
int chown_main();
int flag = 0;

int lazyshell_main()
{
    while(1) 
    {
        int len = 0;
        int ret;
        printf("lazybox$");
        fgets(command, 4096, stdin);
        if(command[strlen(command)-1] == '\n')
        {
            command[strlen(command)-1] = '\0';
        }
        //printf("%s\n",command);
        if (strcmp(command,"exit")==0)
        {
            exit(0);
        }
        else if(command[0]=='c' && command[1]=='d')//cd
        {
            command = command + 3;
            chdir(command);
            //system("bash");
        }
        else if((command[0]=='l' && command[1]=='n'))
        {
            ln_main();
        }
        else if((command[0]=='s' && command[1]=='t' && command[2]=='a'&& command[3]=='t'))
        {
            stat_main();
        }
        else if((command[0]=='c' && command[1]=='h' && command[2]=='o' && command[3]=='w' && command[4]=='n') ) 
        {
            chown_main();
        }
        else
        {
            system(command);
        }
    }   
}

int ln_main()
{
    if(command[3]=='-' && command[4]=='s')
    {
        char *ptr;
        int cnt = 0;
        char file[1024];
        command = command + 6;
        ptr = command;
        while(*ptr!=' ')
        {
            file[cnt++] = *ptr++;
        }
        file[cnt] = '\0';
        ptr++;
        //printf("%s   %s\n",file,ptr);
        symlink(file,ptr);
    }
    else
    {
        printf("hard link");
        char *ptr;
        int cnt = 0;
        char file[1024];
        command = command + 3;
        ptr = command;
        while(*ptr!=' ')
        {
            file[cnt++] = *ptr++;
        }
        file[cnt] = '\0';
        ptr++;
        //printf("%s   %s\n",file,ptr);
        link(file,ptr);
    }
}

int stat_main()
{
    struct stat buf;
    char path[1024];
    strcpy(path,"./");
    command = command + 5;
    strcat(path,command);
    //printf("%s\n",path);
    stat(path,&buf);
    printf("file owner:%s\n",getpwuid(buf.st_uid)->pw_name,ctime(&buf.st_atime),ctime(&buf.st_mtime),ctime(&buf.st_ctime));
    printf("atime:%s",ctime(&buf.st_atime));
    printf("mtime:%s",ctime(&buf.st_mtime));
    printf("ctime:%s",ctime(&buf.st_ctime));
}

int chown_main()
{
    char temp[100];
    strcpy(temp,"   ");
    char goal[1024];
    struct timespec file_t, current_t;
    struct stat buf;
    stat("sudo_success", &buf);
    file_t = buf.st_ctim;
    clock_gettime(CLOCK_REALTIME, &current_t);
    //printf("%d\n",current_t.tv_sec-file_t.tv_sec);
    if(flag==0)
    {
        strcpy(goal,"sudo chown ");
        flag = 1;
        printf("你要執行的是特權指令,確定的話,請輸入『yes』,否則按下『enter』終止操作\n");
        scanf("%s",temp);
        if(strcmp("yes",temp)==0)
        {
            strcpy(goal,"sudo chown ");
        }
        else if(strcmp(temp,"\n")==0)
        {
            return 0;
        }
        system("touch sudo_success");
    }
    else if (current_t.tv_sec - file_t.tv_sec > 30) 
    {
        printf("你要執行的是特權指令,確定的話,請輸入『yes』,否則按下『enter』終止操作\n");
        scanf("%s",temp);
        if(strcmp("yes",temp)==0)
        {
            strcpy(goal,"sudo chown ");
        }
        else if(strcmp(temp,"\n")==0)
        {
            return 0;
        }
        system("touch sudo_success");
    }
    else
    {
        system("touch sudo_success");
        strcpy(goal,"sudo chown ");
    }
    int cnt = 0;
    char file[1024];
    char *ptr;
    int id;
    command = command + 6;
    ptr = command;
    while(*ptr!=' ')
    {
        file[cnt++] = *ptr++;
    }
    file[cnt] = '\0';
    //id = atoi(file);
    ptr++;
    strcat(goal,file);
    strcat(goal," ");
    strcat(goal,ptr);
    //printf("%s\n",goal);
    system(goal);
    //strcpy(goal,"chmod +s ");
    //strcat(goal,ptr);
    //printf("%d %s\n",id,ptr);
    //system(goal);
}

int main(int argc,char**argv) 
{
    getcwd(&lazyBox_path, 4096);
    char order[4096];
    //printf("%s\n",lazyBox_path);
    int i;
    command = (char*)malloc(sizeof(char)*4096);
    char *exename = basename(argv[0]);
    if(argc==1)
    {
        lazyshell_main();
    }
    else if(strcmp(exename,"ln")==0)
    {
        //strcat(lazyBox_path,"/ln");
        strcpy(command,exename);
        for(i=1;i<argc;i++)
        {        
            strcat(command," ");
            strcat(command,argv[i]);
            // strcat(lazyBox_path," ");
            // strcat(lazyBox_path,argv[i]);
        }        
        //system(lazyBox_path);
        ln_main();
    }
    else if(strcmp(exename,"chown")==0)
    {
        //strcat(lazyBox_path,"/chown");
        strcpy(command,exename);
        for(i=1;i<argc;i++)
        {        
            strcat(command," ");
            strcat(command,argv[i]);
            // strcat(lazyBox_path," ");
            // strcat(lazyBox_path,argv[i]);
        }
        //system(lazyBox_path);
        chown_main();
    }
    else if(strcmp(exename,"stat")==0)
    {            
        //strcat(lazyBox_path,"/stat");
        strcpy(command,exename);
        for(i=1;i<argc;i++)
        {        
            strcat(command," ");
            strcat(command,argv[i]);
            // strcat(lazyBox_path," ");
            // strcat(lazyBox_path,argv[i]);
        }        
        //system(lazyBox_path);
        stat_main();
    }
    else if(strcmp(exename,"lazyshell")==0)
    {
        // strcat(lazyBox_path,"/lazyshell");
        // strcat(lazyBox_path," ");
        // strcat(lazyBox_path,argv[i]);
        // system(lazyBox_path);
        lazyshell_main();
    }
    return 0;
}