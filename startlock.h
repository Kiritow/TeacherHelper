/// This Function will be called in initialization progress.
void _start_lock()
{
    printf("--------------------\n"
           "程序密码锁\n"
           "作者: Kiritow\n"
           "版本:v0.1x (x表示这个版本是开源的)\n"
           "--------------------\n"
           );
    printf("NOTICE: The Latest Version is v0.6a.\n"
           "        Please Upgrade To Get Better Security.\n");
    char _pass_real[1024];
    bzero(_pass_real,1024);
    printf("Reading Configure File...\n");
    FILE* fp=fopen("LocalPass.config","r");
    if(fp==NULL)
    {
        printf("No Configure File Found.\n");
        return;
    }
    printf("Preparing...\n");
    fgets(_pass_real,1024,fp);
    fclose(fp);
    printf("请输入启动密码:\n");
    bzero(tmpbuff,1024);
    gets(tmpbuff);
    printf("正在验证...\n");
    if(strcmp(tmpbuff,_pass_real)==0)
    {
        printf("验证成功.\n");
    }
    else
    {
        printf("验证失败.\n");
        PostErrorMsgAndExit("启动密码验证错误.\n");
    }
}
