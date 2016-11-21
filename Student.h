int isValidStudentID(const char* incstr)
{
    int len=strlen(incstr);
    const int sample_len=strlen("1508080112");
    if(sample_len!=len)
    {
        return 0;
    }
    for(int i=0;i<len;i++) if(incstr[i]<'0'||incstr[i]>'9') return 0;
    return 1;
}
typedef int class_id;
class_id getStudentClassID(const char* student_id)
{
    int tcls;
    char xbuff[32];
    memset(xbuff,0,32);
    strncpy(xbuff,student_id,8);
    sscanf(xbuff,"%d",&tcls);
    return tcls;
}

#define CLEARBUFF_READIN() do{ClearInput();memset(tmpbuff,0,1024);gets(tmpbuff);}while(0)
#define CLEARBUFF_DO_NONZERO(USG) do{CLEARBUFF_READIN();if(strlen(tmpbuff)>0){USG}}while(0)

void _work_change_score_stuid(const char* OldID,const char* NewID)
{
    printf("正在修改成绩信息...\n");
    int sz=GetListSize(ls_score);
    int cnt=0;
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        if(strcmp(p->StudentID,OldID)==0)
        {
            strncpy(p->StudentID,NewID,CONFIG_STUID_SIZE);
            cnt++;
        }
    }
    printf("%d 条记录已修改.\n",cnt);
}
void _work_change_achieve_stuid(const char* OldID,const char* NewID)
{
    printf("正在修改成就信息...\n");
    int sz=GetListSize(ls_achieve);
    int cnt=0;
    for(int i=0;i<sz;i++)
    {
        AchieveInfo* p=(AchieveInfo*)GetMember(ls_achieve,i);
        if(strcmp(p->StudentID,OldID)==0)
        {
            strncpy(p->StudentID,NewID,CONFIG_STUID_SIZE);
            cnt++;
        }
    }
    printf("%d 条记录已修改.\n",cnt);
}

void GDI_Student_Change_Edit(StudentInfo* p)
{
    int flag_id_changed=0;
    char old_id[1024];
    bzero(old_id,1024);
    char new_id[1024];
    bzero(new_id,1024);

    WARNING("警告: 更改学生ID会影响到所有与该学生相关的记录,请小心修改.\n");
    printf("当前学生ID为: %s 请输入新的学生ID,留空表示不改动.\n",p->ID);
    while(1)
    {
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) break;
        if(strcmp(tmpbuff,p->ID)==0)
        {
            /// Student ID Not Changed.
            flag_id_changed=0;
            printf("学号未改变.\n");
            break;
        }
        if(isValidStudentID(tmpbuff))
        {
            printf("正在检测冲突...\n");
            strcpy(old_id,p->ID);
            strcpy(new_id,tmpbuff);
            StudentInfo* xx=NULL;
            if((xx=_FindStudentInfoByID(new_id))!=NULL)
            {
                printf("检测到学号冲突: %s(%s). 请尝试其他ID.\n",xx->Name,xx->ID);
                continue;
            }

            strncpy(p->ID,new_id,CONFIG_STUID_SIZE);
            printf("修改成功.\n");
            flag_id_changed=1;
            break;
        }
        else
        {
            printf("\t输入学号格式有误,请重新输入.\n");
            ClearInput();
        }
    }
    printf("当前学生姓名为: %s 请输入新的姓名,留空表示不改动.\n",p->Name);
    CLEARBUFF_DO_NONZERO({strncpy(p->Name,tmpbuff,CONFIG_STUNAME_SIZE);});
    printf("当前宿舍号为: %s 请输入新的宿舍号,留空表示不改动\n",p->Dormitory);
    CLEARBUFF_DO_NONZERO({strncpy(p->Dormitory,tmpbuff,CONFIG_STUDOM_SIZE);});
    printf("当前性别为: %d 请输入新的性别(0代表女,1代表男,2代表其他,3跳过此步骤)\n",p->Sexual);
    int tmp;
    while(scanf("%d",&tmp)<1||tmp<0||tmp>3)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    if(tmp!=3) p->Sexual=tmp;
    printf("当前年龄为: %d 请输入新的年龄(周岁),0跳过此步骤\n",p->Age);
    while(scanf("%d",&tmp)<1||tmp<0)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    if(tmp!=0) p->Age=tmp;
    printf("正在保存修改 %s(%s)...\n",p->Name,p->ID);
    printf("保存成功.\n");
    printf("正在更新学生信息缓存...\n");
    _Notify_Cache_Class_OutOfDate_Single(p);
    if(flag_id_changed)
    {
        printf("正在根据学生信息变化更改其他内容，请稍等...\n");
        _work_change_score_stuid(old_id,new_id);
        _work_change_achieve_stuid(old_id,new_id);
    }
}

void GDI_Student_Change_Create()
{
    StudentInfo tmp;
    strncpy(tmp.ID,tmpbuff,CONFIG_STUID_SIZE);
    printf("请输入姓名:\n");
    gets(tmpbuff);
    strncpy(tmp.Name,tmpbuff,CONFIG_STUNAME_SIZE);
    printf("请输入宿舍号\n");
    gets(tmpbuff);
    strncpy(tmp.Dormitory,tmpbuff,CONFIG_STUDOM_SIZE);
    printf("请输入性别(0代表女,1代表男,2代表其他)\n");
    while(scanf("%d",&tmp.Sexual)<1||tmp.Sexual<0||tmp.Sexual>2)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    printf("请输入年龄(周岁)\n");
    while(scanf("%d",&tmp.Age)<1||tmp.Age<1)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    printf("正在保存 %s(%s)...\n",tmp.Name,tmp.ID);
    if(!PushBack(ls_student,&tmp))
    {
        printf("保存失败.内存不足.\n");
    }
    else
    {
        printf("保存成功.\n");
    }

    printf("正在更新学生信息缓存...\n");
    _Notify_Cache_Class_OutOfDate_Single(GetMember(ls_student,GetListSize(ls_student)-1));
}

void GDI_Student_Change()
{
    ClearScreen();
    TITLE("创建/修改学生信息\n");
    while(1)
    {
        ClearInput();
        printf("请输入学生ID,留空返回...\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return ;
        StudentInfo* p=_FindStudentInfoByID(tmpbuff);
        if(p)
        {
            printf("找到学生信息: %s(%s). 开始修改.\n",p->Name,p->ID);
            GDI_Student_Change_Edit(p);
        }
        else
        {
            printf("创建新的学生信息: %s\n",tmpbuff);
            GDI_Student_Change_Create();
        }
    }
}

void GDI_Student_Delete()
{
    ClearScreen();
    TITLE("删除学生信息\n");
    WARNING("警告:对单个学生信息的删除操作将影响到所有与该ID相关的记录\n");
    while(1)
    {
        printf("请输入学生ID,留空返回...\n");
        bzero(tmpbuff,1024);
        gets(tmpbuff);
        if(strlen(tmpbuff)<1) return ;
        StudentInfo* p=_FindStudentInfoByID(tmpbuff);
        if(!p)
        {
            printf("未找到ID=%s的学生信息.忽略删除操作.\n",tmpbuff);
            continue;
        }
        printf("请确认删除:\n");
        WARNING("将要删除: %s(%s). 按下确认键进行确认,其他按键返回\n",p->Name,p->ID);
        ClearInput();
        if(GetUserInputKey()!=KEY_CONFIRM)
        {
            printf("删除操作终止.\n");
            ClearInput();
            continue;
        }

        printf("开始执行删除...\n");

        printf("删除成绩信息...\n");
        int sz=GetListSize(ls_score);
        int i;
        for(i=0;i<sz;i++)
        {
            ScoreInfo* k=(ScoreInfo*)GetMember(ls_score,i);
            if(strcmp(k->StudentID,p->ID)==0)
            {
                printf("删除成绩: %s(%s) %s \n",p->Name,p->ID,k->CourseID);
                EraseSingle(ls_score,i);
                sz--;
                i--;
            }
        }

        printf("删除成就信息...\n");
        sz=GetListSize(ls_achieve);
        for(i=0;i<sz;i++)
        {
            AchieveInfo* k=(AchieveInfo*)GetMember(ls_achieve,i);
            if(strcmp(k->StudentID,p->ID)==0)
            {
                printf("删除成就: %s(%s) ...\n",p->Name,p->ID);
                EraseSingle(ls_achieve,i);
                sz--;
                i--;
            }
        }

        printf("删除学生信息(快速删除)..\n");
        int idx=((char*)p-(char*)(ls_student->pdata))/(ls_student->typesz);
        EraseSingle(ls_student,idx);

        printf("正在更新学生信息缓存...\n");
        _Notify_Cache_Class_OutOfDate();
        printf("删除完成.\n");
    }
}

/// <unfinished> Not recommend
void GDI_Student_List_Unsorted_UseScroll()
{
    THIS_FUNCTION_IS_UNFINISHED;
    int sz=GetListSize(ls_student);
    int realid=0;
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("学生信息列表(未排序)\n");
        for(int i=1;i<=sz;i++)
        {
            StudentInfo* p=(StudentInfo*)GetMember(ls_student,realid+i);
            TAG(realid+i);printf("%s %s %d %s %d\n",p->ID,p->Name,p->Sexual,p->Dormitory,p->Age);
        }
    }
}
void GDI_Student_List_Unsorted_Pure()
{
    ClearScreen();
    TITLE("学生信息列表(未排序)\n");
    int sz=GetListSize(ls_student);
    for(int i=0;i<sz;i++)
    {
        StudentInfo* p=(StudentInfo*)GetMember(ls_student,i);
        printf("%s %s %d %s %d\n",p->ID,p->Name,p->Sexual,p->Dormitory,p->Age);
    }
    ClearInput();
    printf("按任意键继续.\n");
    GetUserInputKey();
}
void GDI_Student_List_Unsorted()
{
    ClearScreen();
    printf("请稍等,正在准备...\n");
    if(_settings_use_scroll)
    {
        GDI_Student_List_Unsorted_UseScroll();
    }
    else
    {
        GDI_Student_List_Unsorted_Pure();
    }
}

int _SortStudentByID(const void* a,const void* b)
{
    StudentInfo* pa=(StudentInfo*)a;
    StudentInfo* pb=(StudentInfo*)b;
    return strcmp(pa->ID,pb->ID);
}
void GDI_Student_List_Sorted_FastSort()
{
    printf("启动快速排序...\n");
    qsort(GetRawData(ls_student),GetListSize(ls_student),ls_student->typesz,_SortStudentByID);
    printf("正在更新学生信息缓存...\n");
    _Notify_Cache_Class_OutOfDate();
    printf("排序完成\n");
    ClearScreen();
    TITLE("学生信息列表(已排序)\n");
    int sz=GetListSize(ls_student);
    for(int i=0;i<sz;i++)
    {
        StudentInfo* p=(StudentInfo*)GetMember(ls_student,i);
        printf("%s %s %d %s %s\n",p->ID,p->Name,p->Sexual,p->Dormitory,p->Age?"男":"女");
    }
    ClearInput();
    printf("---------------\n按任意键继续.\n");
    getch();
}
void GDI_Student_List_Sorted_SlowSort()
{
    printf("创建数据缓存区...\n");
    printf("启动内存管理设备...\n");
    USE_RELEASE;
    int sz=GetListSize(ls_student);
    DYHANDLE hand=GetList(sizeof(StudentInfo),sz<16?16:sz);
    if(hand==NULL)
    {
        printf("无法创建数据缓存区. 无法继续.\n");
        ClearInput();printf("按任意键继续.\n");getch();
        return;
    }
    printf("添加到内存管理设备...\n");
    ADD_RELEASE(hand);
    printf("复制数据...\n");
    WARNING("内存操作警告: 复制数据操作可能导致错误.\n");
    printf("正在复制数据...\n");
    memcpy(hand->pdata,ls_student->pdata,sizeof(StudentInfo)*sz);
    hand->used=ls_student->used;
    printf("正在启动快速排序...\n");
    qsort(GetRawData(hand),GetListSize(hand),hand->typesz,_SortStudentByID);
    printf("排序完成\n");
    ClearScreen();
    TITLE("学生信息列表(已排序)\n");
    for(int i=0;i<sz;i++)
    {
        StudentInfo* p=(StudentInfo*)GetMember(hand,i);
        printf("%s %s %d %s %s\n",p->ID,p->Name,p->Sexual,p->Dormitory,p->Age?"男":"女");
    }
    DO_RELEASE;
    ClearInput();printf("---------------\n按任意键继续.\n");getch();
}
void GDI_Student_List_Sorted()
{
    ClearScreen();
    printf("请稍等,正在准备...\n");
    printf("正在启动排序...\n");
    printf("正在检测排序参数...\n");
    if(_settings_use_fast_sort)
    {
        GDI_Student_List_Sorted_FastSort();
    }
    else
    {
        GDI_Student_List_Sorted_SlowSort();
    }
}
void GDI_Student_List()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("查看学生信息\n");
        TAG(1);printf("列出全部学生信息(无序)\n");
        TAG(2);printf("列出全部学生信息(按学号排序)\n");
        TAG(3);printf("返回\n");
        resetcolor();printf("------------------------\n");
        INFOPRINT(1,"直接列出学生信息.\n");
        INFOPRINT(2,"按照学号顺序输出学生信息,可能会比较耗时.\n");
        INFOPRINT(3,"返回上一菜单.\n");
        if(GetAction(1,3))
        {
            GUI_LIST
            {
            case 1:
                GDI_Student_List_Unsorted();
                break;
            case 2:
                GDI_Student_List_Sorted();
                break;
            case 3:
                return;
            }
        }
    }
}
void GDI_Student_Search()
{
    ClearScreen();
    TITLE("学生信息搜索\n");
    while(1)
    {
        printf("请输入搜索关键字,搜索范围是: 学号,姓名,宿舍号. 留空返回\n");
        bzero(tmpbuff,1024);
        gets(tmpbuff);
        if(strlen(tmpbuff)<1) return;
        printf("请稍等,正在搜索 %s\n",tmpbuff);
        int sz=GetListSize(ls_student);
        int counter=0;
        for(int i=0;i<sz;i++)
        {
            StudentInfo* p=(StudentInfo*)GetMember(ls_student,i);
            if(strstr(p->Dormitory,tmpbuff)!=NULL || strstr(p->ID,tmpbuff)!=NULL || strstr(p->Name,tmpbuff)!=NULL)
            {
                printf("%s %s %d %s %d\n",p->ID,p->Name,p->Sexual,p->Dormitory,p->Age);
                ++counter;
            }
        }
        printf("-----找到%d条结果-----\n",counter);
    }
}
void GDI_Student_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("学生信息管理\n");
        TAG(1);printf("创建/修改 学生信息.\n");
        TAG(2);printf("删除 学生信息\n");
        TAG(3);printf("查看 学生信息\n");
        TAG(4);printf("搜索 学生信息...\n");
        TAG(5);printf("返回\n");
        resetcolor();printf("---------------------\n");
        INFOPRINT(1,"创建新的学生信息或修改已有学生信息.\n");
        INFOPRINT(2,"删除对应的学生信息\n");
        INFOPRINT(3,"按照顺序列举学生信息\n");
        INFOPRINT(4,"对已有学生信息进行搜索\n");
        INFOPRINT(5,"返回上一菜单.\n");
        if(GetAction(1,5))
        {
            GUI_LIST
            {
            case 1:
                GDI_Student_Change();
                break;
            case 2:
                GDI_Student_Delete();
                break;
            case 3:
                GDI_Student_List();
                break;
            case 4:
                GDI_Student_Search();
                break;
            case 5:
                return;
            }
        }
    }
}
