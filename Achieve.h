#define CSRTN(caseNumber,ReturnVal) case caseNumber: return ReturnVal
#define DFRTN(ReturnVal) default: return ReturnVal
int _GetAchieveScoreByType(int Type)
{
    switch(Type)
    {
        CSRTN(1,3);
        CSRTN(2,6);
        CSRTN(3,12);
        CSRTN(4,1);
        CSRTN(5,4);
        CSRTN(6,5);
        CSRTN(7,6);
        CSRTN(8,6);
        CSRTN(9,9);
        CSRTN(10,12);
        CSRTN(11,1);
        CSRTN(12,2);
        CSRTN(13,3);
        CSRTN(14,-1);
        CSRTN(15,-2);
        CSRTN(16,-3);
        CSRTN(17,-4);
        CSRTN(18,-5);
        CSRTN(19,-6);
        CSRTN(20,0);
        DFRTN(0);
    }
}
const char* _GetAchieveNameByType(int Type)
{
    switch(Type)
    {
        CSRTN(1,"校级各种优秀(+3)");
        CSRTN(2,"省级各种优秀(+6)");
        CSRTN(3,"国家级各种优秀(+12)\n");
        CSRTN(4,"省级以上学科竞赛成功参赛奖(+1)");
        CSRTN(5,"省级三等奖(+4)");
        CSRTN(6,"省二等奖(+5)");
        CSRTN(7,"省一等奖(+6)");
        CSRTN(8,"国家级三等奖(+6)");
        CSRTN(9,"国家级二等奖(+9)");
        CSRTN(10,"国家级一等奖(+12)");
        CSRTN(11,"校级三等奖(+1)");
        CSRTN(12,"校级二等奖(+2)");
        CSRTN(13,"校级一等奖(+3)");
        CSRTN(14,"学院通报批评(-1)");
        CSRTN(15,"校级警告(-2)");
        CSRTN(16,"严重警告(-3)");
        CSRTN(17,"记过(-4)");
        CSRTN(18,"记大过(-5)");
        CSRTN(19,"开除学籍留校察看(-6)");
        CSRTN(20,"奖学金(0)");
        DFRTN("未知(?)");
    }
}

void GDI_Achieve_Add()
{
    ClearScreen();
    printf("奖惩信息录入\n");
    while(1)
    {
        ClearInput();
        printf("请输入学号,留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        printf("请稍等,正在匹配学生信息...\n");
        StudentInfo* p=_FindStudentInfoByID(tmpbuff);
        if(!p)
        {
            printf("未找到学号为%s的学生.匹配失败.\n",tmpbuff);
            continue;
        }
        printf("学生: %s(%s)\n",p->Name,p->ID);
        int cid=1;
        while(1)
        {
            ClearScreen();
            printf("奖惩信息 创建\n");
            printf("目标: %s(%s)\n",p->Name,p->ID);
            resetcolor();printf(">>>荣誉积分<<<\n");
            TAGPRINT(1,"校级各种优秀(+3)\n");
            TAGPRINT(2,"省级各种优秀(+6)\n");
            TAGPRINT(3,"国家级各种优秀(+12)\n");
            resetcolor();printf(">>学科竞赛<<<\n");
            TAGPRINT(4,"省级以上学科竞赛成功参赛奖(+1)\n");
            TAGPRINT(5,"省级三等奖(+4)\n");
            TAGPRINT(6,"省二等奖(+5)\n");
            TAGPRINT(7,"省一等奖(+6)\n");
            TAGPRINT(8,"国家级三等奖(+6)\n");
            TAGPRINT(9,"国家级二等奖(+9)\n");
            TAGPRINT(10,"国家级一等奖(+12)\n");
            TAGPRINT(11,"校级三等奖(+1)\n");
            TAGPRINT(12,"校级二等奖(+2)\n");
            TAGPRINT(13,"校级一等奖(+3)\n");
            resetcolor();printf(">>>惩罚<<<\n");
            TAGPRINT(14,"学院通报批评(-1)\n");
            TAGPRINT(15,"校级警告(-2)\n");
            TAGPRINT(16,"严重警告(-3)\n");
            TAGPRINT(17,"记过(-4)\n");
            TAGPRINT(18,"记大过(-5)\n");
            TAGPRINT(19,"开除学籍留校察看(-6)\n");
            resetcolor();printf("------------------\n");
            if(GetAction(1,19))
            {
                printf("请输入日期(格式: 年 月 日)\n");
                int y,m,d;
                while(scanf("%d %d %d",&y,&m,&d)<3||y<0||m<1||m>12||d<1||
                      ( ((y%4==0&&y%100!=0)||(y%400==0))&&(m==2)&&(d>29) ) ||
                      ( (!((y%4==0&&y%100!=0)||(y%400==0)))&&(m==2)&&(d>28) ) ||
                      ( (m==4||m==6||m==9||m==11)&&(d>30) ) ||
                      ( d>31 )
                      )
                {
                    printf("输入有误.请重新输入\n");
                    ClearInput();
                }
                AchieveInfo tmp;
                strncpy(tmp.StudentID,p->ID,CONFIG_STUID_SIZE);
                tmp.type=cid;
                tmp.score=_GetAchieveScoreByType(tmp.type);
                tmp.time.year=y;
                tmp.time.mon=m;
                tmp.time.day=d;
                printf("请输入评价:\n");
                CLEARBUFF_READIN();
                strncpy(tmp.Message,tmpbuff,CONFIG_ACHMSG_SIZE);
                printf("正在保存...\n");
                if(!PushBack(ls_achieve,&tmp))
                {
                    printf("保存成功\n");
                }
                else
                {
                    printf("保存失败\n");
                }
                printf("按任意键以继续...\n");
                GetUserInputKey();
                break;
            }
        }
    }
}

/// NOTICE: Caller Needs to Call FreeList to free memory.
DYHANDLE getAchievesInfoByStuID(const char* StuID,int* pret)
{
    if(!ls_achieve) return NULL;
    DYHANDLE h=GetList(sizeof(AchieveInfo),1024);
    if(h==NULL)
    {
        if(pret) *pret=-1;
        return NULL;
    }
    int i;
    int sz=GetListSize(ls_achieve);
    for(i=0;i<sz;i++)
    {
        AchieveInfo* p=(AchieveInfo*)GetMember(ls_achieve,i);
        if(strcmp(p->StudentID,StuID)==0)
        {
            if(PushBack(h,&p)==NULL)
            {
                FreeList(h);
                if(pret) *pret=-2;
                return NULL;
            }
        }
    }
    if(pret) *pret=0;
    return h;
}

void GDI_Achieve_Delete()
{
    while(1)
    {
        ClearScreen();
        TITLE("奖惩信息删除\n");
        while(1)
        {
            ClearInput();
            printf("请输入学号,留空返回\n");
            CLEARBUFF_READIN();
            if(strlen(tmpbuff)<1) return;
            printf("请稍等,正在匹配学生信息...\n");
            StudentInfo* p=_FindStudentInfoByID(tmpbuff);
            if(!p)
            {
                printf("未找到学号为%s的学生.匹配失败.\n",tmpbuff);
                continue;
            }
            printf("学生: %s(%s)\n",p->Name,p->ID);
            int ret;
            DYHANDLE h=getAchievesInfoByStuID(p->ID,&ret);
            if(ret<0&&h==NULL)
            {
                printf("无法获取奖惩信息列表.\n");
                continue;
            }
            printf("奖惩信息获取成功\n");
            int sz=GetListSize(h);
            while(1)
            {
                ClearScreen();
                TITLE("奖惩信息-删除: %s(%s)\n",p->Name,p->ID);
                USE_GUI;
                int i;
                for(i=1;i<=sz;i++)
                {
                    AchieveInfo* k=*(AchieveInfo**)GetMember(h,i);
                    TAG(i);printf("%d-%d-%d %s\n",k->time.year,k->time.mon,k->time.day,_GetAchieveNameByType(k->type));
                }
                TAG(sz+1);printf("返回上一菜单\n");
                if(GetAction(1,sz+1))
                {
                    if(cid==sz+1)
                    {
                        break;
                    }
                    ClearScreen();
                    AchieveInfo* k=(AchieveInfo*)GetMember(h,i);
                    TITLE("奖惩信息-确认删除\n");
                    if(CONFIRM("确认删除: %d-%d-%d %s\n ",k->time.year,k->time.mon,k->time.day,_GetAchieveNameByType(k->type)))
                    {
                        EraseSingle(ls_achieve,k-((AchieveInfo*)GetRawData(ls_achieve)));
                        printf("删除成功.\n");
                    }
                }
            }
            FreeList(h);
        }
    }
}

int _SortAchieveInfoByStuID(const void* a,const void* b)
{
    AchieveInfo* p=(AchieveInfo*)a;
    AchieveInfo* q=(AchieveInfo*)b;
    return strcmp(p->StudentID,q->StudentID);
}

void GDI_Achieve_List_SortByStu()
{
    ClearScreen();
    TITLE("奖惩信息-列举-学生学号排序-请稍等\n");
    printf("请稍等,正在排序...\n");
    qsort(GetRawData(ls_achieve),GetListSize(ls_achieve),ls_achieve->typesz,_SortAchieveInfoByStuID);
    printf("排序已完成.\n");
    ClearScreen();
    TITLE("奖惩信息-列举-学生学号排序\n");
    int sz=GetListSize(ls_achieve);
    for(int i=0;i<sz;i++)
    {
        AchieveInfo* p=(AchieveInfo*)GetMember(ls_achieve,i);
        printf("%s %s %d %04d/%02d/%02d\n  %s\n",p->StudentID,_GetAchieveNameByType(p->type),p->score,p->time.year,p->time.mon,p->time.day,p->Message);
    }
    printf("------------------------\n");
    printf("按任意键返回.\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_Achieve_List_Unsort()
{
    ClearScreen();
    TITLE("奖惩信息-列举\n");
    int sz=GetListSize(ls_achieve);
    for(int i=0;i<sz;i++)
    {
        AchieveInfo* p=(AchieveInfo*)GetMember(ls_achieve,i);
        printf("%s %s %d %04d/%02d/%02d\n  %s\n",p->StudentID,_GetAchieveNameByType(p->type),p->score,p->time.year,p->time.mon,p->time.day,p->Message);
    }
    printf("------------------------\n");
    printf("按任意键返回.\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_Achieve_List()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("奖惩信息-列举\n");
        TAG(1);printf("以学号为顺序列出奖惩信息\n");
        TAG(2);printf("无序输出奖惩信息\n");
        TAG(3);printf("返回...\n");
        resetcolor();printf("-----------------------------\n");
        INFOPRINT(1,"以学号为顺序列出奖惩信息,默认是升序.\n");
        INFOPRINT(2,"无序输出.若最近经历过排序,那么输出看起来会像排过序一样.\n");
        INFOPRINT(3,"返回上一菜单.\n");
        if(GetAction(1,3))
        {
            GUI_LIST
            {
            case 1:
                GDI_Achieve_List_SortByStu();
                break;
            case 2:
                GDI_Achieve_List_Unsort();
                break;
            case 3:
                return;
            }
        }
    }
}

void GDI_Achieve_Search()
{
    ClearScreen();
    TITLE("奖惩信息-搜索\n");
    while(1)
    {
        printf("请输入学生ID,留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        printf("请稍等,正在查找 %s..\n",tmpbuff);
        DYHANDLE h=getAchievesInfoByStuID(tmpbuff,NULL);
        if(h==NULL)
        {
            printf("查找失败.\n");
            continue;
        }
        int sz=GetListSize(h);
        for(int i=0;i<sz;i++)
        {
            AchieveInfo* p=*(AchieveInfo**)GetMember(h,i);
            printf("%s %s %d %04d/%02d/%02d\n  %s\n",p->StudentID,_GetAchieveNameByType(p->type),p->score,p->time.year,p->time.mon,p->time.day,p->Message);
        }
        FreeList(h);
        printf("------%d条记录------\n",sz);
        printf("按任意键返回.\n");
        ClearInput();
        GetUserInputKey();
    }
}
void GDI_Achieve_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("奖惩信息管理\n");
        TAG(1);printf("新建奖惩信息\n");
        TAG(2);printf("删除奖惩信息\n");
        TAG(3);printf("列举奖惩信息\n");
        TAG(4);printf("搜索奖惩信息\n");
        TAG(5);printf("返回\n");
        resetcolor();printf("----------------------\n");
        INFOPRINT(1,"新建一条奖励或处罚信息,并附加评议\n");
        INFOPRINT(2,"删除一条奖励或处罚信息\n");
        INFOPRINT(3,"列举奖励或处罚信息\n");
        INFOPRINT(4,"搜索奖励或处罚信息\n");
        INFOPRINT(5,"返回上一菜单.\n");
        if(GetAction(1,5))
        {
            GUI_LIST
            {
            case 1:
                GDI_Achieve_Add();
                break;
            case 2:
                GDI_Achieve_Delete();
                break;
            case 3:
                GDI_Achieve_List();
                break;
            case 4:
                GDI_Achieve_Search();
                break;
            case 5:
                return;
            }
        }
    }
}
