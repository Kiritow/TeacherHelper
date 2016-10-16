#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include <windows.h>
#include <winsock2.h>

//#define DEF_GAMEHARBOR_NEED_DNS
#define DEF_GAMEHARBOR_SERVER_DOMAIN "gameharbor.cn"
#define DEF_GAMEHARBOR_SERVER_IP "123.206.86.73"
#define DEF_GAMEHARBOR_SERVER_PORT 56789

#define CONFIG_STUINFO_ID_SIZE 64
#define CONFIG_STUINFO_NAME_SIZE 128
#define CONFIG_STUINFO_DOM_SIZE 64
#define CONFIG_CLASSINFO_ID_SIZE 64
#define CONFIG_CLASSINFO_NAME_SIZE 128
#define CONFIG_SCOINFO_STUID_SIZE CONFIG_STUINFO_ID_SIZE
#define CONFIG_SCOINFO_CLASSID_SIZE CONFIG_CLASSINFO_ID_SIZE
#define CONFIG_MOINFO_MSG_SIZE 512
#define CONFIG_MOINFO_STUID_SIZE CONFIG_STUINFO_ID_SIZE
#define CONFIG_ACHINFO_STUID_SIZE CONFIG_STUINFO_ID_SIZE

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

int _global_login_status;
int _global_userid;

#define PostErrorMsgAndExit(FormatString,Args...) do{cprint(red,black);printf(FormatString,##Args);}while(0);exit(0)
void SelfCheck()
{
    printf("正在启动...\n");
    printf("初始化颜色系统...\n");
    _internal_color_system_init();
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

    printf("初始化网络系统...\n");


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
    printf("执行退出...\n");
}

int GetAction_WS(int* p,int MinVal,int MaxVal)
{
    int t=GetUserInputKey();
    switch(t)
    {
    case KEY_UP:
        {
            --(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_DOWN:
        {
            ++(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_CONFIRM:
        return 1;
    default:
        return 0;
    }
}
#define TAG(tagID,realID) do{if(realID==tagID)cprint(black,yellow);else cprint(white,black);}while(0)
#define TAGPRINT(tagID,realID,FormatString,Args...) do{TAG(tagID,realID);printf(FormatString,##Args);}while(0)
#define INFOPRINT(tagID,realID,FormatString,Args...) do{if(tagID==realID) printf(FormatString,##Args);}while(0)
#define INFODO(tagID,realID,USG) do{if(tagID==realID) {USG} }while(0)
#define GetAction(realID,minVal,maxVal) GetAction_WS(&realID,minVal,maxVal)
#define ClearInput() fflush(stdin)


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
#include "CloudService.h"

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
        resetcolor();printf("-------------------\n");
        INFOPRINT(1,cid,"对某门功课各分数段成绩进行统计\n");
        INFOPRINT(2,cid,"分学期对学生业务课程平均分按分数段进行统计\n");
        INFOPRINT(3,cid,"统计任意一名同学每门功课的班级排名以及业务课成绩总体排名\n");
        INFOPRINT(4,cid,"以宿舍为单位进行成绩统计分析\n");
        INFOPRINT(5,cid,"以挂科次数为依据分学期对比分析\n");
        INFOPRINT(6,cid,"以业务课班级排名为依据分学期对比分析（前进或退步情况）\n");
    }
}

void GDI_ScolarShip_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("奖学金评定系统\n");
    }
}

void GDI_Export_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("导出-本地硬盘\n");
    }
}

void GDI_CloudService_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("HC Cloud Service 云服务平台\n");
        printf("正在登录到您的HC账户...\n");
        GDI_GameHarbor_Login();/// Jump
        if(_global_login_status==0)
        {
            return;
        }
    }
}

void GDI_Settings_MainPad()
{
    int cid=1;
    while(1)
    {
        ClearScreen();
        printf("设置\n");
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
        TAG(7,cid);printf("导出数据到本地硬盘\n");
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
        INFOPRINT(7,cid,"导出数据在其他设备上查看\n");
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
int main()
{
    SelfCheck();
    GDI_MainPad();
    BeforeExit();
    return 0;
}
