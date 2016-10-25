#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include <windows.h>
#include <winsock2.h>

#include "defines.h"
#define bzero(Pointer,Size) memset(Pointer,0,Size)

/// Settings (Dynamic Settings)
int _settings_use_scroll;


int _global_login_status;
char tmpbuff[4096];

typedef struct
{
    char ID[CONFIG_STUID_SIZE];
    char Name[CONFIG_STUNAME_SIZE];
    char Dormitory[CONFIG_STUDOM_SIZE];
    int Sexual;
    int Age;
}StudentInfo;

typedef struct
{
    char ID[CONFIG_COURSEID_SIZE]; ///课程号
    int Type;///课程类别 0 选修 1 必修
    int Term;///学期 1~8
    char Name[CONFIG_COURSENAME_SIZE];
    double Score;///学分 2.5 (?)
}CourseInfo;

typedef struct
{
    char StudentID[CONFIG_STUID_SIZE];
    char CourseID[CONFIG_COURSEID_SIZE];
    double Score;
    int flag;
}ScoreInfo;

typedef struct
{
    int year,mon,day;
}STime;

typedef struct
{
    char StudentID[CONFIG_STUID_SIZE];
    int type;
    STime time;
    int score;
}AchieveInfo;

#include "dynamicvec.h"

DYHANDLE ls_student;
DYHANDLE ls_course;
DYHANDLE ls_score;
DYHANDLE ls_achieve;

#include "gui.h"

#define PostErrorMsgAndExit(FormatString,Args...) \
    do{cprint(red,black);printf(FormatString,##Args);exit(0);}while(0);

/// Forward Declaration.
#include "fwd_decl.h"

#include "startlock.h"

void SelfCheck()
{
    printf("正在启动...\n");
    printf("初始化颜色系统...\n");
    _internal_color_system_init();

    printf("初始化动态内存管理设备...\n");
    _dynamicvec_init();

    printf("检查启动锁状态...\n");
    _start_lock();

    printf("初始化学生系统...\n");
    ls_student=NULL;
    ls_student=GetList(sizeof(StudentInfo),1024);
    if(ls_student==NULL)
    {
        PostErrorMsgAndExit("初始化学生系统失败.\n");
    }

    printf("初始化缓存系统...\n");
    if(_init_cache()<0)
    {
        PostErrorMsgAndExit("初始化缓存系统失败.\n");
    }

    printf("初始化课程系统...\n");
    ls_course=NULL;
    ls_course=GetList(sizeof(CourseInfo),1024);
    if(ls_course==NULL)
    {
        PostErrorMsgAndExit("初始化课程系统失败.\n");
    }
    printf("初始化成绩系统...\n");
    ls_score=NULL;
    ls_score=GetList(sizeof(ScoreInfo),1024);
    if(ls_score==NULL)
    {
        PostErrorMsgAndExit("初始化成绩系统失败.\n");
    }
    printf("初始化成就系统...\n");
    ls_achieve=NULL;
    ls_achieve=GetList(sizeof(AchieveInfo),1024);
    if(ls_achieve==NULL)
    {
        PostErrorMsgAndExit("初始化成就系统失败.\n");
    }
    printf("初始化网络系统...\n");
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
    FreeList(ls_achieve);
    printf("关闭成绩系统...\n");
    FreeList(ls_score);
    printf("关闭课程系统...\n");
    FreeList(ls_course);
    printf("关闭学生系统...\n");
    FreeList(ls_student);

    printf("执行退出...\n");
}

#include "basic_find.h"
#include "Student.h"
#include "ClassCache.h"
#include "Course.h"
#include "Score.h"
#include "Achieve.h"
#include "View.h"

void GDI_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("班主任管家-首页\n");
        TAG(1);printf("学生信息管理\n");
        TAG(2);printf("课程信息管理\n");
        TAG(3);printf("成绩信息管理\n");
        TAG(4);printf("奖惩信息管理\n");
        TAG(5);printf("信息统计\n");
        TAG(6);printf("奖学金评定系统\n");
        TAG(7);printf("搜索...\n");
        TAG(8);printf("本地导入/导出\n");
        TAG(9);printf("云平台\n");
        TAG(10);printf("设置\n");
        TAG(11);printf("退出\n");
        resetcolor();printf("----------------------\n");
        INFOPRINT(1,"学生信息的录入,修改和删除\n");
        INFOPRINT(2,"课程信息的录入,修改和删除\n");
        INFOPRINT(3,"课程成绩的录入,修改和删除\n");
        INFOPRINT(4,"奖惩信息的录入,修改和删除\n");
        INFOPRINT(5,"进行各种统计,并查看和导出\n");
        INFOPRINT(6,"奖学金自动评测系统\n");
        INFOPRINT(7,"进行搜索...\n");
        INFOPRINT(8,"在本地磁盘上导入和导出数据\n");
        INFODO(9,{cprint(yellow,blue);printf("由HC Cloud Service提供");resetcolor();printf(" 同步您的数据到云端,在其他设备继续您的工作.\n");});
        INFOPRINT(10,"调整您的设置.\n");
        INFOPRINT(11,"安全的退出这个系统.未同步的数据将会自动同步.\n");
        if(GetAction(1,11))
        {
            GUI_LIST
            {
            case 1:
                GDI_Student_MainPad();
                break;
            case 2:
                GDI_Class_MainPad();
                break;
            case 3:
                GDI_Score_MainPad();
                break;
            case 4:
                GDI_Achieve_MainPad();
                break;
            case 5:
                GDI_View_MainPad();
                break;
            case 6:
                GDI_Scholar_MainPad();
                break;
            case 7:
                GDI_Search_MainPad();
                break;
            case 8:
                GDI_Export_MainPad();
                break;
            case 9:
                GDI_Cloud_MainPad();
                break;
            case 10:
                GDI_Settings_MainPad();
                break;
            case 11:
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

