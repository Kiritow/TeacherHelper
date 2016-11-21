int isValidCourseID(const char* incstr)
{
    int len=strlen(incstr);
    const int sample_len=strlen("B08010100");
    if(sample_len!=len)
    {
        return 0;
    }
    if(incstr[0]<'A'||incstr[0]>'Z') return 0;
    for(int i=1;i<len;i++) if(incstr[i]<'0'||incstr[i]>'9') return 0;
    return 1;
}

void GDI_Course_Change_Create()
{
    CourseInfo tmp;
    strncpy(tmp.ID,tmpbuff,CONFIG_COURSEID_SIZE);
    printf("请输入课程名称\n");
    gets(tmpbuff);
    strncpy(tmp.Name,tmpbuff,CONFIG_COURSENAME_SIZE);
    printf("请输入课程类别 (0:选修 1:必修)\n");
    while(scanf("%d",&tmp.Type)<1||tmp.Type<0||tmp.Type>1)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    printf("请输入课程所在学期(1-8)\n");
    while(scanf("%d",&tmp.Term)<1||tmp.Term<1||tmp.Term>8)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    printf("请输入课程学分\n");
    while(scanf("%lf",&tmp.Score)<1||tmp.Score<0)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }

    /// Call _SaveClassInfo_real
    printf("正在保存...\n");
    if(!PushBack(ls_course,&tmp))
    {
        printf("保存失败.内存不足.\n");
    }
    else
    {
        printf("保存成功.\n");
    }
}

void _work_change_score_courseid(const char* OldID,const char* NewID)
{
    printf("正在修改成绩信息...\n");
    int sz=GetListSize(ls_score);
    int cnt=0;
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        if(strcmp(p->CourseID,OldID)==0)
        {
            strncpy(p->CourseID,NewID,CONFIG_STUID_SIZE);
            cnt++;
        }
    }
    printf("%d 条记录已修改.\n",cnt);
}

void GDI_Course_Change_Edit(CourseInfo* p)
{
    int flag_course_id_changed=0;
    char old_id[1024];
    bzero(old_id,1024);
    char new_id[1024];
    bzero(new_id,1024);

    WARNING("警告: 更改课程ID会影响到所有与该课程相关的记录,请小心修改.\n");
    printf("当前课程ID为: %s 请输入新的课程ID,留空表示不改动.\n",p->ID);
    while(1)
    {
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) break;
        if(strcmp(tmpbuff,p->ID)==0)
        {
            /// Student ID Not Changed.
            flag_course_id_changed=0;
            printf("课程号未改变.\n");
            break;
        }
        if(isValidCourseID(tmpbuff))
        {
            printf("正在检测冲突...\n");
            strcpy(old_id,p->ID);
            strcpy(new_id,tmpbuff);
            CourseInfo* xx=NULL;
            if((xx=_FindCourseInfoByID(new_id))!=NULL)
            {
                printf("检测到课程号冲突: %s(%s). 请尝试其他ID.\n",xx->Name,xx->ID);
                continue;
            }

            strncpy(p->ID,new_id,CONFIG_COURSEID_SIZE);
            flag_course_id_changed=1;
            printf("修改成功.\n");
            break;
        }
        else
        {
            printf("\t输入课程号格式有误,请重新输入.\n");
            ClearInput();
        }
    }

    printf("当前课程名为: %s 请输入课程名称,留空表示不改动\n",p->Name);
    CLEARBUFF_DO_NONZERO({strncpy(p->Name,tmpbuff,CONFIG_COURSENAME_SIZE);});

    printf("当前课程类别为: %d 请输入课程类别 (0:选修 1:必修 2:跳过此步骤)\n",p->Type);
    int tmp;
    while(scanf("%d",&tmp)<1||tmp<0||tmp>2)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    if(tmp!=2) p->Type=tmp;

    printf("当前课程所在学期为: %d 请输入课程所在学期(1-8),0跳过此步骤\n",p->Term);
    while(scanf("%d",&tmp)<1||tmp<0||tmp>8)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    if(tmp) p->Term=tmp;

    printf("当前课程学分: %.2f 请输入课程学分,输入负数跳过此步骤\n",p->Score);
    double tmpf;
    while(scanf("%lf",&tmpf)<1)
    {
        printf("\t输入有误.请重新输入\n");
        ClearInput();
    }
    if(tmp>=0) p->Score=tmpf;

    /// Call _SaveClassInfo_real
    printf("正在保存...\n");
    printf("保存成功.\n");
    if(flag_course_id_changed)
    {
        printf("正在根据课程信息变化更改其他内容，请稍等...\n");
        _work_change_score_courseid(old_id,new_id);
    }
}
int GDI_Course_Change_BeforeEdit_Confirm()
{
    ClearInput();
    printf("按下确定键确定，其他按键返回.\n");
    int key=GetUserInputKey();
    ClearInput();
    if(key!=KEY_CONFIRM)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void GDI_Course_Change()
{
    ClearScreen();
    TITLE("创建/修改课程信息\n");
    while(1)
    {
        ClearInput();
        printf("请输入课程ID,留空返回...\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return ;
        printf("请稍等...\n");
        CourseInfo* p=_FindCourseInfoByID(tmpbuff);
        if(p)
        {
            printf("找到课程信息: %s(%s). 开始修改么?.\n",p->Name,p->ID);
            if(GDI_Course_Change_BeforeEdit_Confirm()==0)
            {
                /// Not Confirmed.
                printf("修改取消.\n");
                continue;
            }
            GDI_Course_Change_Edit(p);
        }
        else
        {
            printf("创建新的课程信息: %s\n",tmpbuff);
            GDI_Course_Change_Create();
        }
    }
}
int GDI_Course_Delete_Before_Confirm()
{
    ClearInput();
    printf("按下确定键确定，其他按键返回.\n");
    int key=GetUserInputKey();
    ClearInput();
    if(key!=KEY_CONFIRM)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void _work_delete_score_courseid(const char* CourseID)
{
    printf("正在删除成绩信息...\n");
    int sz=GetListSize(ls_score);
    int cnt=0;
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* k=(ScoreInfo*)GetMember(ls_score,i);
        if(strcmp(k->CourseID,CourseID)==0)
        {
            printf("删除成绩: %s %s \n",k->StudentID,CourseID);
            EraseSingle(ls_score,i);
            sz--;
            i--;
            ++cnt;
        }
    }
    printf("%d 条记录已删除.\n",cnt);
}
void GDI_Course_Delete()
{
    ClearScreen();
    TITLE("删除 课程信息\n");
    while(1)
    {
        WARNING("警告: 对单个课程ID的删除会影响到所有与该课程相关的记录.\n");
        printf("请输入课程ID,留空返回...\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        printf("请稍等...\n");
        CourseInfo* p=_FindCourseInfoByID(tmpbuff);
        if(p==NULL)
        {
            printf("没有找到课程信息,请检查后重新输入.\n");
            continue;
        }
        printf("找到课程信息: %s(%s). 开始删除么?.\n",p->Name,p->ID);
        if(GDI_Course_Delete_Before_Confirm()==0)
        {
            /// Not Confirmed.
            printf("删除取消.\n");
            continue;
        }

        _work_delete_score_courseid(p->ID);
        printf("操作完成.\n");
    }
}


/// <unfinished> Not Recommend
void GDI_Course_List_Unsorted_UseScroll()
{
    THIS_FUNCTION_IS_UNFINISHED;
}

void GDI_Course_List_Unsorted_Pure()
{
    ClearScreen();
    TITLE("课程信息列表(未排序)\n");
    int sz=GetListSize(ls_course);
    printf("课程ID 课程名称 课程学分 所在学期 类型\n");
    for(int i=0;i<sz;i++)
    {
        CourseInfo* p=(CourseInfo*)GetMember(ls_course,i);
        printf("%s %s %.2f %d %d\n",p->ID,p->Name,p->Score,p->Term,p->Type);
    }
    ClearInput();
    printf("按任意键继续.\n");
    GetUserInputKey();
}

void GDI_Course_List_Unsorted()
{
    ClearScreen();
    printf("请稍等,正在准备数据...\n");
    if(_settings_use_scroll)
    {
        GDI_Course_List_Unsorted_UseScroll();
    }
    else
    {
        GDI_Course_List_Unsorted_Pure();
    }
}


int _SortCourseByID(const void* a,const void* b)
{
    CourseInfo* p=(CourseInfo*)a;
    CourseInfo* q=(CourseInfo*)b;
    return strcmp(p->ID,q->ID);
}

void GDI_Course_List_Sorted()
{
    ClearScreen();
    printf("请稍等,正在准备...\n");
    printf("正在启动排序...\n");
    qsort(GetRawData(ls_course),GetListSize(ls_course),ls_course->typesz,_SortCourseByID);
    int sz=GetListSize(ls_course);
    for(int i=0;i<sz;i++)
    {
        CourseInfo* p=(CourseInfo*)GetMember(ls_course,i);
        printf("%s %s %.2f %d %d\n",p->ID,p->Name,p->Score,p->Term,p->Type);
    }
    ClearInput();
    printf("---------------\n按任意键继续.\n");
    GetUserInputKey();
}

void GDI_Course_List()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("查看课程信息\n");
        TAG(1);printf("列出全部课程信息(无序)\n");
        TAG(2);printf("列出全部课程信息(按课程号排序)\n");
        TAG(3);printf("返回\n");
        resetcolor();printf("------------------------\n");
        INFOPRINT(1,"直接列出课程信息.\n");
        INFOPRINT(2,"按照学号顺序输出课程信息,可能会比较耗时.\n");
        INFOPRINT(3,"返回上一菜单.\n");
        if(GetAction(1,3))
        {
            GUI_LIST
            {
            case 1:
                GDI_Course_List_Unsorted();
                break;
            case 2:
                GDI_Course_List_Sorted();
                break;
            case 3:
                return;
            }
        }
    }
}
void GDI_Course_Search()
{
    ClearScreen();
    TITLE("搜索课程信息\n");
    while(1)
    {
        printf("请输入搜索关键字,搜索范围是: 课程号,课程名称. 留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        printf("请稍等,正在搜索%s\n",tmpbuff);
        int sz=GetListSize(ls_course);
        int counter=0;
        for(int i=0;i<sz;i++)
        {
            CourseInfo* p=(CourseInfo*)GetMember(ls_course,i);
            if(strstr(p->ID,tmpbuff)||strstr(p->Name,tmpbuff))
            {
                printf("%s %s %.2f %d %d\n",p->Name,p->ID,p->Score,p->Term,p->Type);
                ++counter;
            }
        }
        printf("-----找到%d条结果-----\n",counter);
    }
}
void GDI_Course_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("课程信息管理\n");
        TAG(1);printf("创建/修改 课程信息.\n");
        TAG(2);printf("删除 课程信息\n");
        TAG(3);printf("查看 课程信息\n");
        TAG(4);printf("搜索 课程信息...\n");
        TAG(5);printf("返回\n");
        resetcolor();printf("---------------------\n");
        INFOPRINT(1,"创建新的课程信息或修改已有课程信息.\n");
        INFOPRINT(2,"删除对应的课程信息\n");
        INFOPRINT(3,"按照顺序列举课程信息\n");
        INFOPRINT(4,"对已有课程信息进行搜索\n");
        INFOPRINT(5,"返回上一菜单.\n");
        if(GetAction(1,5))
        {
            GUI_LIST
            {
            case 1:
                GDI_Course_Change();
                break;
            case 2:
                GDI_Course_Delete();
                break;
            case 3:
                GDI_Course_List();
                break;
            case 4:
                GDI_Course_Search();
                break;
            case 5:
                return;
            }
        }
    }
}
