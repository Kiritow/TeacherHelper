#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include <windows.h>
#include <winsock2.h>

/** defines are declared here. */
#include "defines.h"

#include "ikernel.h"

typedef struct
{
    char ID[CONFIG_STUINFO_ID_SIZE];
    char Name[CONFIG_STUINFO_NAME_SIZE];
    char Dormitory[CONFIG_STUINFO_DOM_SIZE];
    int Sexual;
    int Age;
}StudentInfo;

typedef struct
{
    char ID[CONFIG_CLASSINFO_ID_SIZE]; ///课程号
    int Type;///课程类别 0 选修 1 必修
    int Term;///学期 1~8
    char Name[CONFIG_CLASSINFO_NAME_SIZE];
    double Score;///学分 2.5 (?)
}ClassInfo;

typedef struct
{
    char StudentID[CONFIG_SCOINFO_STUID_SIZE];
    char ClassID[CONFIG_SCOINFO_CLASSID_SIZE];
    double Score;
    int flag;
}ScoreInfo;

typedef struct
{
    char StudentID[CONFIG_STUINFO_ID_SIZE];
    char Message[CONFIG_MOINFO_MSG_SIZE];
    int Score;
    int Term;/// 1~8
}MonityInfo;

/** CPPLIB Extends In C
*   Powered By HC TECH - CPPLIB
*/
typedef struct
{
    int year,mon,day;
}STime;

typedef struct
{
    char ID[CONFIG_ACHINFO_STUID_SIZE];
    int type;
    STime time;
    int score;
}AchieveInfo;

/**
***************************************************************
*   CPPLIB - Dynamic Vector (C++)
*   Rewritten in Pure C. (By HC TECH)
*/
#include "dynamicvec.h"

/************* GUI Field************/
#include "gui.h"

/**************Data Field************/
DYHANDLE stuinfo;
DYHANDLE clsinfo;
DYHANDLE scoinfo;
DYHANDLE moinfo;
DYHANDLE achinfo;

/*************Global Settings & Values*************/
int _global_login_status;
int _global_userid;

int _settings_metro_style;

#define PostErrorMsgAndExit(FormatString,Args...) do{cprint(red,black);printf(FormatString,##Args);}while(0);exit(0)
void SelfCheck()
{
    printf("正在启动...\n");
    printf("初始化颜色系统...\n");
    _internal_color_system_init();
    printf("启动内核...\n");
    _internal_kernel_start();
    //Sleep(3000);

    printf("初始化学生系统...\n");
    stuinfo=NULL;
    stuinfo=GetList(sizeof(StudentInfo),1024);
    if(stuinfo==NULL)
    {
        PostErrorMsgAndExit("初始化学生系统失败.\n");
    }
    printf("初始化课程系统...\n");
    clsinfo=NULL;
    clsinfo=GetList(sizeof(ClassInfo),1024);
    if(clsinfo==NULL)
    {
        PostErrorMsgAndExit("初始化课程系统失败.\n");
    }
    scoinfo=NULL;
    scoinfo=GetList(sizeof(ScoreInfo),1024);
    if(scoinfo==NULL)
    {
        PostErrorMsgAndExit("初始化成绩系统失败.\n");
    }
    moinfo=NULL;
    moinfo=GetList(sizeof(MonityInfo),1024);
    if(moinfo==NULL)
    {
        PostErrorMsgAndExit("初始化品行系统失败.\n");
    }
    achinfo=NULL;
    achinfo=GetList(sizeof(AchieveInfo),1024);
    if(achinfo==NULL)
    {
        PostErrorMsgAndExit("初始化成就系统失败.\n");
    }

    WORD _winsock_version=MAKEWORD(2,2);
    WSADATA _winsock_init_data;
    int ret=WSAStartup(_winsock_version,&_winsock_init_data);
    if(ret<0)
    {
        PostErrorMsgAndExit("初始化网络系统失败.\n");
    }

    printf("加载数据...\n");

    printf("正在完成云同步...\n");
    _global_login_status=0;///设置初始状态为未登录(0)

    printf("启动完毕.\n");
}

void BeforeExit()
{
    ClearScreen();
    printf("准备退出...\n");
    printf("正在完成数据同步，请不要关闭本窗口.\n");
    printf("正在完成本地同步...\n");
    printf("正在完成云同步...\n");
    Sleep(3000);/// Cloud Service is a Time-Consuming Work
    printf("正在等待各子系统相应...\n");

    printf("关闭成就系统...\n");
    FreeList(achinfo);
    printf("关闭品行系统...\n");
    FreeList(moinfo);
    printf("关闭成绩系统...\n");
    FreeList(scoinfo);
    printf("关闭课程系统...\n");
    FreeList(clsinfo);
    printf("关闭学生系统...\n");
    FreeList(stuinfo);

    printf("全部子系统关闭.等待内核返回...\n");
    SendKernelMessage(KM_STOP);

    printf("执行退出...\n");
}




/************** GDI Functions*************/
char tmpbuff[1024];
StudentInfo* _FindStudentInfoByID(const char* IDStr)
{
    if(!stuinfo) return NULL;
    int i;
    int sz=GetListSize(stuinfo);
    for(i=0;i<sz;i++)
    {
        StudentInfo* p=(StudentInfo*)GetMember(stuinfo,i);
        if(strcmp(p->ID,IDStr)==0)
        {
            return p;
        }
    }
    return NULL;
}
ClassInfo* _FindClassInfoByID(const char* IDStr)
{
    if(!stuinfo) return NULL;
    int i;
    int sz=GetListSize(stuinfo);
    for(i=0;i<sz;i++)
    {
        ClassInfo* p=(ClassInfo*)GetMember(stuinfo,i);
        if(strcmp(p->ID,IDStr)==0)
        {
            return p;
        }
    }
    return NULL;
}
ScoreInfo* _FindScoreInfoByStuID_ClassID_Flag(const char* StuID,const char* ClassID,int flag)
{
    if(!scoinfo) return NULL;
    int i;
    int sz=GetListSize(scoinfo);
    for(i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(scoinfo,i);
        if(strcmp(p->StudentID,StuID)==0&&strcmp(p->ClassID,ClassID)==0&&p->flag==flag)
        {
            return p;
        }
    }
    return NULL;
}
MonityInfo* _FindMonityInfoByStuID_Term(const char* StuID,int Term)
{
    if(!moinfo) return NULL;
    int i;
    int sz=GetListSize(moinfo);
    for(i=0;i<sz;i++)
    {
        MonityInfo* p=(MonityInfo*)GetMember(moinfo,i);
        if(strcmp(p->StudentID,StuID)==0&&p->Term==Term)
        {
            return p;
        }
    }
    return NULL;
}

#include "InfoInput.h"
#include "InfoEdit.h"
#include "InfoDel.h"
#include "InfoSearch.h"
#include "InfoView.h"
#include "CloudService.h"
#include "Settings.h"

#include "PassInput.h"

void GDI_InfoInput_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("录入\n");
        TAG(1,cid);printf("学生信息录入\n");
        TAG(2,cid);printf("课程信息录入\n");
        TAG(3,cid);printf("品行表现成绩录入\n");
        TAG(4,cid);printf("课程成绩录入\n");
        TAG(5,cid);printf("奖惩信息录入\n");
        TAG(6,cid);printf("返回...\n");
        resetcolor();printf("-------------------\n");
        INFOPRINT(1,cid,"进行学生信息的录入\n");
        INFOPRINT(2,cid,"进行课程信息的录入\n");
        INFOPRINT(3,cid,"分学期录入品行表现成绩（辅导员，班主任，班级评议）\n");
        INFOPRINT(4,cid,"进行课程成绩的录入\n");
        INFOPRINT(5,cid,"录入学生的各种奖惩信息.\n");
        INFOPRINT(6,cid,"返回到上一层菜单.\n");
        if(GetAction(cid,1,6))
        {
            switch(cid)
            {
            case 1:
                GDI_InfoInput_Student();
                break;
            case 2:
                GDI_InfoInput_Class();
                break;
            case 3:
                GDI_InfoInput_Monity();
                break;
            case 4:
                GDI_InfoInput_Score();
                break;
            case 5:
                GDI_InfoInput_OtherInfo();
                break;
            case 6:
                return;
            }
        }
    }
}


void GDI_InfoEdit_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("修改\n");
        TAG(1,cid);printf("学生信息修改\n");
        TAG(2,cid);printf("课程信息修改\n");
        TAG(3,cid);printf("品行表现成绩修改\n");
        TAG(4,cid);printf("课程成绩修改\n");
        TAG(5,cid);printf("返回...\n");
        resetcolor();printf("-------------------\n");
        INFOPRINT(1,cid,"进行学生信息的修改\n");
        INFOPRINT(2,cid,"进行课程信息的修改\n");
        INFOPRINT(3,cid,"分学期修改品行表现成绩（辅导员，班主任，班级评议）\n");
        INFOPRINT(4,cid,"进行课程成绩的修改\n");
        INFOPRINT(5,cid,"返回到上一层菜单.\n");
        if(GetAction(cid,1,5))
        {
            switch(cid)
            {
            case 1:
                GDI_InfoEdit_Student();
                break;
            case 2:
                GDI_InfoEdit_Class();
                break;
            case 3:
                GDI_InfoEdit_Monity();
                break;
            case 4:
                GDI_InfoEdit_Score();
                break;
            case 5:
                return;
            }
        }
    }
}

void GDI_InfoDel_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("删除\n");
        cprint(yellow,black);printf("警告: 对单个数据的删除操作可能会影响多项条目\n");resetcolor();
        TAG(1,cid);printf("学生信息删除\n");
        TAG(2,cid);printf("课程信息删除\n");
        TAG(3,cid);printf("品行表现成绩删除\n");
        TAG(4,cid);printf("课程成绩删除\n");
        TAG(5,cid);printf("奖惩信息删除\n");
        TAG(6,cid);printf("返回...\n");
        resetcolor();printf("-------------------\n");
        INFOPRINT(1,cid,"进行学生信息的删除\n");
        INFOPRINT(2,cid,"进行课程信息的删除\n");
        INFOPRINT(3,cid,"删除品行表现成绩（辅导员，班主任，班级评议）\n");
        INFOPRINT(4,cid,"进行课程成绩的删除\n");
        INFOPRINT(5,cid,"删除学生的各种奖惩信息.\n");
        INFOPRINT(6,cid,"返回到上一层菜单.\n");
        if(GetAction(cid,1,6))
        {
            switch(cid)
            {
            case 1:
                GDI_InfoDel_Student();
                break;
            case 2:
                GDI_InfoDel_Class();
                break;
            case 3:
                GDI_InfoDel_Monity();
                break;
            case 4:
                GDI_InfoDel_Score();
                break;
            case 5:
                GDI_InfoDel_OtherInfo();
                break;
            case 6:
                return;
            }
        }
    }
}

void GDI_InfoSearch_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("搜索...\n");
        TAG(1,cid);printf("学生信息搜索\n");
        TAG(2,cid);printf("课程信息搜索\n");
        TAG(3,cid);printf("返回...\n");
        resetcolor();printf("---------------\n");
        INFOPRINT(1,cid,"[实验性功能]: 根据学生部分信息进行搜索\n");
        INFOPRINT(2,cid,"[实验性功能]: 根据课程部分信息进行搜索\n");
        INFOPRINT(3,cid,"返回上一菜单\n");
        if(GetAction(cid,1,3))
        {
            switch(cid)
            {
            case 1:
                GDI_InfoSearch_Student();
                break;
            case 2:
                GDI_InfoSearch_Class();
                break;
            case 3:
                return;
            }
        }
    }
}

void GDI_InfoView_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("统计面板\n");
        TAG(1,cid);printf("成绩分段统计\n");
        TAG(2,cid);printf("分学期平均分分数段统计\n");
        TAG(3,cid);printf("单人成绩统计\n");
        TAG(4,cid);printf("宿舍成绩统计\n");
        TAG(5,cid);printf("挂科成绩统计\n");
        TAG(6,cid);printf("按照学习统计...\n");
        TAG(7,cid);printf("返回\n");
        resetcolor();printf("-------------------\n");
        INFOPRINT(1,cid,"对某门功课各分数段成绩进行统计\n");
        INFOPRINT(2,cid,"分学期对学生业务课程平均分按分数段进行统计\n");
        INFOPRINT(3,cid,"统计任意一名同学每门功课的班级排名以及业务课成绩总体排名\n");
        INFOPRINT(4,cid,"以宿舍为单位进行成绩统计分析\n");
        INFOPRINT(5,cid,"以挂科次数为依据分学期对比分析\n");
        INFOPRINT(6,cid,"以业务课班级排名为依据分学期对比分析（前进或退步情况）\n");
        INFOPRINT(7,cid,"返回到上一菜单\n");
        if(GetAction(cid,1,7))
        {
            switch(cid)
            {
            case 1:
                GDI_InfoView_ScoreSegmentView();
                break;
            case 2:
                GDI_InfoView_ScoreTermView();
                break;
            case 3:
                GDI_InfoView_RankInOneClass();
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                return;
            }
        }
    }
}

/// Temporary defines. Used to avoid death-loop and unused warnings.
#define _DEF_GDI_NOT_IMPLIED_(cid) do{cid=cid;printf("这个功能还没有实现呐~\n");ClearInput();getch();return;}while(0)

void GDI_ScolarShip_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("奖学金评定系统\n");
        _DEF_GDI_NOT_IMPLIED_(cid);
    }
}

void GDI_Export_ImportFromDisk()
{
    ClearScreen();
    printf("导出到磁盘\n");
    cprint(yellow,black);printf("以下数据将被导出:\n学生信息,课程信息,成绩信息,品行表现,奖惩信息.\n");
    resetcolor();printf("请输入导出文件的名称,默认为ExportData.bin\n");
    memset(tmpbuff,0,1024);
    gets(tmpbuff);
    if(strlen(tmpbuff)<1)
    {
        strcpy(tmpbuff,"ExportData.bin");
    }
    else
    {
        if(strstr(tmpbuff,".bin")==NULL)
        {
            strcat(tmpbuff,".bin");
        }
    }
    FILE* fp=fopen(tmpbuff,"wb");
    if(fp==NULL)
    {
        printf("错误!不能导出到%s\n",tmpbuff);
        printf("按任意键返回...\n");ClearInput();getch();
        return;
    }
    /// Export: StudentInfo (stuinfo)
    int sz=GetListSize(stuinfo);
    fwrite(&sz,sizeof(sz),1,fp);
    for(int i=0;i<sz;i++)
    {
        StudentInfo* p=GetMember(stuinfo,i);
        fwrite(p,sizeof(StudentInfo),1,fp);
    }
    /// Export: ClassInfo (clsinfo)
    sz=GetListSize(clsinfo);
    fwrite(&sz,sizeof(sz),1,fp);
    for(int i=0;i<sz;i++)
    {
        ClassInfo* p=GetMember(clsinfo,i);
        fwrite(p,sizeof(ClassInfo),1,fp);
    }
    /// Export: ScoreInfo (scoinfo)
    sz=GetListSize(scoinfo);
    fwrite(&sz,sizeof(sz),1,fp);
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=GetMember(scoinfo,i);
        fwrite(p,sizeof(ScoreInfo),1,fp);
    }
    /// Export: MonityInfo (moinfo)
    sz=GetListSize(moinfo);
    fwrite(&sz,sizeof(sz),1,fp);
    for(int i=0;i<sz;i++)
    {
        MonityInfo* p=GetMember(moinfo,i);
        fwrite(p,sizeof(MonityInfo),1,fp);
    }
    /// Export: AchieveInfo (achinfo)
    sz=GetListSize(achinfo);
    fwrite(&sz,sizeof(sz),1,fp);
    for(int i=0;i<sz;i++)
    {
        AchieveInfo* p=GetMember(achinfo,i);
        fwrite(p,sizeof(AchieveInfo),1,fp);
    }
    /// Done.
    fclose(fp);

    printf("导出成功!\n");
    ClearInput();getch();
}
void GDI_Export_ExportToDisk()
{

}
void GDI_Export_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("本地导入/导出\n");
        TAG(1,cid);printf("从本地存储导入...\n");
        TAG(2,cid);printf("导出到本地存储...\n");
        TAG(3,cid);printf("返回\n");
        resetcolor();printf("-------------------\n");
        INFOPRINT(1,cid,"从外部存储器导入数据\n");
        INFOPRINT(2,cid,"导出数据到外部存储器\n");
        INFOPRINT(3,cid,"返回上一菜单.\n");
        if(GetAction(cid,1,3))
        {
            switch(cid)
            {
            case 1:
                GDI_Export_ImportFromDisk();
                break;
            case 2:
                GDI_Export_ExportToDisk();
                break;
            case 3:
                return;
            }
        }
    }
}

void GDI_CloudService_MainPad()
{
    // Avoid WARNING by Compiler of unused variables
    //int cid=1;
    while(1)
    {
        ClearScreen();
        printf("HC Cloud Service 云服务平台\n");
        printf("正在登录到您的HC账户...\n");
        GDI_GameHarbor_Login();/// Jump to GameHarbor Account
        if(_global_login_status==0)
        {
            return;
        }
        GDI_CloudService_Internal_MainPad();
    }
}


void GDI_Settings_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("设置\n");
        TAG(1,cid);printf("菜单显示风格设置\n");
        TAG(2,cid);printf("扁平化设置\n");
        TAG(3,cid);printf("返回\n");
        resetcolor();printf("----------------\n");
        INFOPRINT(1,cid,"修改菜单的显示风格,定制专属风格\n");
        INFOPRINT(2,cid,"设置菜单是否显示为扁平化的风格\n");
        INFOPRINT(3,cid,"返回上一菜单\n");
        if(GetAction(cid,1,3))
        {
            switch(cid)
            {
            case 1:
                GDI_Settings_Style();
                break;
            case 2:
                GDI_Settings_Metro();
                break;
            case 3:
                return;
            }
        }
    }
}

void GDI_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("班主任管家软件 - 首页\n");
        TAG(1,cid);printf("信息录入\n");
        TAG(2,cid);printf("信息修改\n");
        TAG(3,cid);printf("信息删除\n");
        TAG(4,cid);printf("信息查找\n");
        TAG(5,cid);printf("信息统计\n");
        TAG(6,cid);printf("评定奖学金\n");
        TAG(7,cid);printf("本地导入/导出\n");
        TAG(8,cid);printf("云平台\n");
        TAG(9,cid);printf("设置\n");
        TAG(10,cid);printf("退出\n");
        resetcolor();printf("----------------------\n");
        INFOPRINT(1,cid,"学生信息,课程信息,课程成绩等内容的录入\n");
        INFOPRINT(2,cid,"学生信息,课程信息,课程成绩等内容的修改\n");
        INFOPRINT(3,cid,"学生信息,课程信息,课程成绩等内容的删除\n");
        INFOPRINT(4,cid,"进行搜索...\n");
        INFOPRINT(5,cid,"查看各种统计信息\n");
        INFOPRINT(6,cid,"奖学金自动评测系统\n");
        INFOPRINT(7,cid,"在本地磁盘上导入和导出数据\n");
        INFODO(8,cid,{cprint(yellow,blue);printf("由HC Cloud Service提供");resetcolor();printf(" 同步您的数据到云端,在其他设备继续您的工作.\n");});
        INFOPRINT(9,cid,"调整您的设置.\n");
        INFOPRINT(10,cid,"安全的退出这个系统.未同步的数据将会自动同步.\n");
        if(GetAction(cid,1,10))
        {
            switch(cid)
            {
            case 1:
                GDI_InfoInput_MainPad();
                break;
            case 2:
                GDI_InfoEdit_MainPad();
                break;
            case 3:
                GDI_InfoDel_MainPad();
                break;
            case 4:
                GDI_InfoSearch_MainPad();
                break;
            case 5:
                GDI_InfoView_MainPad();
                break;
            case 6:
                GDI_ScolarShip_MainPad();
                break;
            case 7:
                GDI_Export_MainPad();
                break;
            case 8:
                GDI_CloudService_MainPad();
                break;
            case 9:
                GDI_Settings_MainPad();
                break;
            case 10:
                return;
            }
        }
    }
}
/*
void SafeCheck()
{
    printf("请输入密码:\n");
    int ret=password_input(tmpbuff,'\r',1,6);
    printf("RET: %d PASS: %s\n",ret,tmpbuff);
}
*/
int main()
{
    //SafeCheck();
    SelfCheck();
    GDI_MainPad();
    BeforeExit();
    return 0;
}
