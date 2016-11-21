/** We should use
#include <cpplib/cpplib#gsock>
here, but it is written in C++.(sad)
*/
int _GameHarbor_Login_Real(const char* userid,const char* userpass)
{
    int sfd=_NetIO_NewSock();
    if(sfd<0) return -1;
    int ret=_NetIO_Connect(sfd,DEF_GAMEHARBOR_SERVER_IP,DEF_GAMEHARBOR_SERVER_PORT);
    if(ret<0){_NetIO_CloseSock(sfd);return -2;}
    int clen=strlen(userid);
    int tlen=htonl(clen);
    ret=_NetIO_SendN(sfd,&tlen,sizeof(tlen));
    if(ret<0) {_NetIO_CloseSock(sfd);return -3;}
    ret=_NetIO_SendN(sfd,userid,clen);
    if(ret<0){_NetIO_CloseSock(sfd);return -4;}

    clen=strlen(userpass);
    tlen=htonl(clen);
    ret=_NetIO_SendN(sfd,&tlen,sizeof(tlen));
    if(ret<0){_NetIO_CloseSock(sfd);return -5;}
    ret=_NetIO_SendN(sfd,userpass,clen);
    if(ret<0){_NetIO_CloseSock(sfd);return -6;}

    int tans,cans;
    ret=recv(sfd,(char*)&tans,sizeof(tans),0);
    ret=_NetIO_RecvN(sfd,&tans,sizeof(tans));
    if(ret<0){_NetIO_CloseSock(sfd);return -7;}
    cans=ntohl(tans);
    if(cans<0)
    {
        _NetIO_CloseSock(sfd);
        return -401;/// Not logged in
    }
    _NetIO_CloseSock(sfd);
    return cans;
}

void GDI_GameHarbor_Login_Failed(int ErrorCode)
{
    ClearScreen();
    printf("登录您的GameHarbor账户\n");
    printf("很抱歉,我们无法完成验证. 错误码 %d\n",ErrorCode);
    printf("您可以尝试:\n");
    printf("> 重新登录\n");
    printf("> 查看http://gameharbor.cn获取错误码的更多信息\n");
    printf("---------------------\n");
    printf("键入任何键返回\n");
    ClearInput();
    getch();
}

void GDI_GameHarbor_Login_Process(const char* userid,const char* userpass)
{
    ClearScreen();
    printf("登录您的GameHarbor账户\n");
    printf("请稍等...正在连接%s到GameHarbor(CN)...\n",userid);
    printf("---------------------\n");
    printf("GameHarbor(CN)是HC TECH旗下的一个开放的云服务平台. 注册账户是免费的\n");
    Sleep(2500);
    int ret=_GameHarbor_Login_Real(userid,userpass);
    if(ret<0)
    {
        _global_login_status=0;
        GDI_GameHarbor_Login_Failed(ret);
    }
    else
    {
        /// Login OK (SUCCESS)
        strncpy(_global_cloud_username,userid,1024);
        _global_userid=ret;
        _global_login_status=1;
    }
}

/// This Function is Special ! It Return so that we can know if its father should return.
int GDI_GameHarbor_Login()
{
    ClearScreen();
    printf("登录您的GameHarbor账户\n");
    printf("用户名(留空返回上一层)\n\n密码\n\n");
    printf("-------------------\n");
    printf("没有GameHarbor账户?前往gameharbor.cn注册!\n");
    gotoxy(1,3);
    memset(tmpbuff,0,1024);
    gets(tmpbuff);
    if(strlen(tmpbuff)<1)
    {
        return 0;/// Don't Reload
    }
    gotoxy(1,5);
    char userid[1024];
    char userpass[1024];
    memset(userid,0,1024);
    memset(userpass,0,1024);
    strncpy(userid,tmpbuff,1024);
    memset(tmpbuff,0,1024);
    gets(tmpbuff);
    strncpy(userpass,tmpbuff,1024);

    GDI_GameHarbor_Login_Process(userid,userpass);
    if(_global_login_status)/// logged in
    {
        ClearScreen();
        printf("登录您的GameHarbor账户\n");
        printf("已连接到GameHarbor(CN).\n");
        return 1;/// Need Reload
    }

    return 2;/// Need Reload (Auth Failed)
}


char _N_FileCache[102400];
int _N_LoadFile(const char* File)
{
    FILE* fp=fopen(File,"rb");
    if(fp==NULL) return -1;
    memset(_N_FileCache,0,102400);
    fseek(fp,0L,SEEK_END);
    int fsz=ftell(fp);
    rewind(fp);
    fread(_N_FileCache,1,fsz,fp);
    fclose(fp);
    return fsz;
}

int _Do_Upload()
{
    printf("正在打包...\n");
    FILE* fp=_DiskIO_OpenWriteFile("_auto_cloud.bin",1);
    if(fp==NULL)
    {
        printf("保存错误: 无法写入%s\n",tmpbuff);
        GetUserInputKey();
        return -1;
    }
    _DiskIO_VecToFile(fp,ls_student);
    _DiskIO_VecToFile(fp,ls_course);
    _DiskIO_VecToFile(fp,ls_score);
    _DiskIO_VecToFile(fp,ls_achieve);

    _DiskIO_CloseFile(fp);

    int nsz=_N_LoadFile("_auto_cloud.bin");
    if(nsz<0)
    {
        printf("块读取出错.\n");
        return -1;
    }

    printf("正在建立连接...\n");
    int sfd=_NetIO_NewSock();
    int ret=_NetIO_Connect(sfd,DEF_GAMEHARBOR_SERVER_IP,DEF_GAMEHARBOR_SERVER_PORT+1);
    if(ret<0)
    {
        printf("无法建立通讯连接.\n");
        _NetIO_CloseSock(sfd);
        return -2;
    }

    printf("正在发送认证信息...\n");
    int tmp=_global_userid;
    tmp=htonl(tmp);
    ret=_NetIO_SendN(sfd,&tmp,sizeof(tmp));
    if(ret<0)
    {
        printf("发送认证信息出现错误.\n");_NetIO_CloseSock(sfd);
        return -3;
    }
    ret=_NetIO_RecvN(sfd,&tmp,sizeof(tmp));
    tmp=ntohl(tmp);
    if(ret<0||tmp!=1)/// 1: OK Others: Not OK
    {
        printf("认证失败.\n");_NetIO_CloseSock(sfd);
        return -4;
    }

    tmp=htonl(nsz);
    ret=_NetIO_SendN(sfd,&tmp,sizeof(tmp));

    ret=_NetIO_SendN(sfd,_N_FileCache,nsz);
    if(ret<0)
    {
        printf("发送数据包出错.\n");_NetIO_CloseSock(sfd);
        return -5;
    }

    ret=_NetIO_RecvN(sfd,&tmp,sizeof(tmp));
    tmp=ntohl(tmp);
    if(ret<0||tmp!=1)
    {
        printf("数据包确认状态出错.\n");_NetIO_CloseSock(sfd);
        return -6;
    }

    printf("同步成功.\n");_NetIO_CloseSock(sfd);
    return 0;

}

void GDI_CloudService_SyncOnCloud_SaveToCloud()
{
    ClearScreen();
    TITLE("云平台服务-同步到云\n");
    printf("请稍等,正在同步...\n");
    if(_Do_Upload()<0)
    {
        printf("上传过程中遇到问题,请重试.\n");
    }
    else
    {
        printf("上传成功.\n");
    }
    ClearInput();
    GetUserInputKey();
}

void GDI_CloudService_SyncOnCloud_DownloadFromCloud()
{
    ClearScreen();
    TITLE("云平台服务-从云同步\n");
    printf("请稍等,正在同步...\n");
    WARNING("目前暂不支持从云端下载!\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_CloudService_SyncOnCloud()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("云平台服务-数据同步\n");
        TAG(1);printf("同步到云\n");
        TAG(2);printf("从云同步\n");
        TAG(3);printf("返回\n");
        resetcolor();printf("----------------\n");
        INFOPRINT(1,"将数据同步到云上. 此过程可能会消耗较多的数据流量.\n");
        INFOPRINT(2,"从云上下载数据到本地. 此过程可能会消耗较多的数据流量\n");
        INFOPRINT(3,"返回上一菜单\n");
        if(GetAction(1,3))
        {
            GUI_LIST
            {
            case 1:
                GDI_CloudService_SyncOnCloud_SaveToCloud();
                break;
            case 2:
                GDI_CloudService_SyncOnCloud_DownloadFromCloud();
                break;
            case 3:
                return;
            }
        }
    }
}

void GDI_CloudService_ChatOnline()
{
    ClearScreen();
    TITLE("班主任在线-大厅\n");
    printf("该功能即将上线,敬请期待.\n");
    printf("---按任意键继续---\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_CloudService_Update()
{
    /// NOTICE: We should make UPDATE an online plugin. But .... We have no time to fix it...
    ClearScreen();
    TITLE("版本信息\n");
    printf("班主任管家系统 V1.3x\n"
           "内核版本: V20161024x\n"
           "版本支持商: HC TECH\n");
    printf("------------------------\n");
    printf("正在查询最新版本...\n");
    Sleep(1500);
    printf("------------------------\n");
    printf("最新版本:\n"
           "班主任管家系统 V1.3x(开源版)\n"
           "内核版本: V20161024x(开源版)\n"
           "版本支持商: HC TECH\n");
    printf("------------------------\n");
    printf("当前版本为最新版本,不需要升级.\n");
    printf("---按任意键继续---\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_CloudService_Report()
{
    ClearScreen();
    TITLE("反馈信息\n");
    printf("请在此键入您的反馈信息:(留空返回)\n");
    CLEARBUFF_READIN();
    if(strlen(tmpbuff)<1) return;
    char UNAME[1024];
    memset(UNAME,0,1024);
    printf("请在此键入您的联系方式:(可不填)\n");
    gets(UNAME);
    ClearScreen();
    TITLE("感谢您的反馈\n");
    printf("请稍等,我们正在将您的反馈发送...\n");
    int s=_NetIO_NewSock();
    int ret=_NetIO_Connect(s,DEF_GAMEHARBOR_SERVER_IP,DEF_GAMEHARBOR_SERVER_PORT+2);
    if(ret<0)
    {
        printf("无法连接到反馈信息服务器.\n");
        printf("---按任意键继续---\n");
        ClearInput();
        GetUserInputKey();
        return;
    }
    int tmp=_global_userid;
    tmp=htonl(tmp);
    ret=_NetIO_SendN(s,&tmp,sizeof(tmp));
    tmp=strlen(tmpbuff);
    tmp=htonl(tmp);
    ret=_NetIO_SendN(s,&tmp,sizeof(tmp));///len
    ret=_NetIO_SendN(s,tmpbuff,strlen(tmpbuff));///text
    tmp=strlen(UNAME);
    tmp=htonl(tmp);
    ret=_NetIO_SendN(s,&tmp,sizeof(tmp));///len
    ret=_NetIO_SendN(s,UNAME,strlen(UNAME));///text

    _NetIO_CloseSock(s);
    printf("反馈成功.\n");
    printf("---按任意键继续---\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_CloudService_LogOut()
{
    ClearScreen();
    TITLE("云平台服务中心-登出\n");
    printf("正在登出...\n");
    memset(_global_cloud_username,0,1024);
    _global_login_status=0;
    _global_userid=-1;
    Sleep(500);
    ClearScreen();
    TITLE("云平台服务中心-登出\n");
    printf("登出成功！\n");
}

void GDI_CloudService_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("云平台服务中心\n");
        printf("你好,%s\n",_global_cloud_username);
        TAG(1);printf("数据同步服务\n");
        TAG(2);printf("班主任在线交流服务\n");
        TAG(3);printf("版本更新\n");
        TAG(4);printf("在线反馈\n");
        TAG(5);printf("退出登录\n");
        TAG(6);printf("返回...\n");
        resetcolor();printf("--------------------\n");
        INFOPRINT(1,"在云上工作,上传/下载您的工作数据以便在所有设备上查看\n");
        INFODO(2,{ cprint(yellow,blue);printf("由HC Chat Online提供");resetcolor();printf(" 与其他班主任在线交流\n");} );
        INFOPRINT(3,"跟进软件更新动态,随时保持新版本!\n");
        INFOPRINT(4,"与开发者面对面,报告在使用中遇到的问题.\n");
        INFOPRINT(5,"安全的退出本次登录\n");
        INFOPRINT(6,"返回上一菜单,登录状态会保持.\n");
        if(GetAction(1,6))
        {
            GUI_LIST
            {
            case 1:
                GDI_CloudService_SyncOnCloud();
                break;
            case 2:
                GDI_CloudService_ChatOnline();
                break;
            case 3:
                GDI_CloudService_Update();
                break;
            case 4:
                GDI_CloudService_Report();
                break;
            case 5:
                GDI_CloudService_LogOut();
                return;
            case 6:
                return;
            }
        }
    }
}

void GDI_Cloud_MainPad()
{
    ClearScreen();
    TITLE("正在跳转...\n");
    printf("请稍等,正在检测登录情况...\n");
    if(_global_login_status)
    {
        GDI_CloudService_MainPad();
    }
    else
    {
        while(1)
        {
            int ret=GDI_GameHarbor_Login();
            if(ret==1)
            {
                GDI_CloudService_MainPad();
                break;
            }
            else if(ret==2)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
}
