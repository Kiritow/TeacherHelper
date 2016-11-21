void GDI_Score_AddSingle()
{
    ClearScreen();
    TITLE("创建成绩信息\n");
    while(1)
    {
        printf("请输入学生学号,留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;

        printf("请稍等,正在完成学生信息...\n");
        StudentInfo* tp;
        if((tp=_FindStudentInfoByID(tmpbuff))==NULL)
        {
            printf("未找到学生信息,无法录入成绩.\n");
            continue;
        }
        printf("学生: %s (%s)\n",tp->Name,tp->ID);

        ScoreInfo x;
        bzero(&x,sizeof(x));
        strncpy(x.StudentID,tmpbuff,CONFIG_STUID_SIZE);
        printf("请输入课程号\n");
        CLEARBUFF_READIN();
        CourseInfo* tq;
        if((tq=_FindCourseInfoByID(tmpbuff))==NULL)
        {
            printf("未找到课程信息,无法录入成绩.\n");
            continue;
        }
        printf("课程: %s (%s)\n",tq->Name,tq->ID);
        strncpy(x.CourseID,tmpbuff,CONFIG_COURSEID_SIZE);

        printf("请输入成绩来源(0: 第一次考试获得 1:重修获得)\n");
        while(scanf("%d",&x.flag)<1||x.flag<0||x.flag>1)
        {
            printf("\t输入有误.请重新输入\n");
            ClearInput();
        }

        printf("请稍等,正在准备...\n");
        ScoreInfo* r=NULL;
        if((r=_FindScoreInfoByStuID_CourseID_Flag(x.StudentID,x.CourseID,x.flag)))
        {
            printf("冲突：发现了一条记录: %s %s [%d] -> %.2f\n不能继续录入.\n",r->StudentID,r->CourseID,r->flag,r->Score);
            continue;
        }

        printf("请输入成绩:(0~100)\n");
        while(scanf("%lf",&x.Score)<1||x.Score<0||x.Score>100)
        {
            printf("\t输入有误.请重新输入\n");
            ClearInput();
        }

        printf("正在保存...\n");
        if(!PushBack(ls_score,&x))
        {
            printf("保存失败.内存不足.\n");
        }
        else
        {
            printf("保存成功.\n");
        }
    }
}

void GDI_Score_AddByStu()
{
    ClearScreen();
    TITLE("添加单人成绩\n");
    while(1)
    {
        printf("请输入学生ID,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) break;

        printf("请稍等,正在完成学生信息...\n");
        StudentInfo* tp;
        if((tp=_FindStudentInfoByID(tmpbuff))==NULL)
        {
            printf("未找到学生信息,无法录入成绩.\n");
            continue;
        }
        printf("学生: %s (%s)\n",tp->Name,tp->ID);

        while(1)
        {
            printf("请输入课程号,留空返回\n");
            CLEARBUFF_READIN();
            if(strlen(tmpbuff)<1) break;

            CourseInfo* tq;
            if((tq=_FindCourseInfoByID(tmpbuff))==NULL)
            {
                printf("未找到课程信息,无法录入成绩.\n");
                continue;
            }
            printf("课程: %s (%s)\n",tq->Name,tq->ID);

            ScoreInfo x;
            strncpy(x.StudentID,tp->ID,CONFIG_STUID_SIZE);
            strncpy(x.CourseID,tq->ID,CONFIG_COURSEID_SIZE);

            printf("请输入成绩来源(0: 第一次考试获得 1:重修获得)\n");
            while(scanf("%d",&x.flag)<1||x.flag<0||x.flag>1)
            {
                printf("\t输入有误.请重新输入\n");
                ClearInput();
            }

            printf("请稍等,正在准备...\n");
            ScoreInfo* r=NULL;
            if((r=_FindScoreInfoByStuID_CourseID_Flag(x.StudentID,x.CourseID,x.flag)))
            {
                printf("冲突：发现了一条记录: %s %s [%d] -> %.2f\n不能继续录入.\n",r->StudentID,r->CourseID,r->flag,r->Score);
                continue;
            }

            printf("请输入成绩:(0~100)\n");
            while(scanf("%lf",&x.Score)<1||x.Score<0||x.Score>100)
            {
                printf("\t输入有误.请重新输入\n");
                ClearInput();
            }

            printf("正在保存...\n");
            if(!PushBack(ls_score,&x))
            {
                printf("保存失败.内存不足.\n");
            }
            else
            {
                printf("保存成功.\n");
            }
        }
    }
}

void GDI_Score_AddByCourse()
{
    ClearScreen();
    TITLE("添加单科成绩\n");
    while(1)
    {
        printf("请输入课程号,留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) break;

        CourseInfo* tq;
        if((tq=_FindCourseInfoByID(tmpbuff))==NULL)
        {
            printf("未找到课程信息,无法录入成绩.\n");
            continue;
        }
        printf("课程: %s (%s)\n",tq->Name,tq->ID);

        while(1)
        {
            printf("请输入学生ID,留空返回.\n");
            CLEARBUFF_READIN();
            if(strlen(tmpbuff)<1) break;

            printf("请稍等,正在完成学生信息...\n");
            StudentInfo* tp;
            if((tp=_FindStudentInfoByID(tmpbuff))==NULL)
            {
                printf("未找到学生信息,无法录入成绩.\n");
                continue;
            }
            printf("学生: %s (%s)\n",tp->Name,tp->ID);

            ScoreInfo x;
            strncpy(x.StudentID,tp->ID,CONFIG_STUID_SIZE);
            strncpy(x.CourseID,tq->ID,CONFIG_COURSEID_SIZE);

            printf("请输入成绩来源(0: 第一次考试获得 1:重修获得)\n");
            while(scanf("%d",&x.flag)<1||x.flag<0||x.flag>1)
            {
                printf("\t输入有误.请重新输入\n");
                ClearInput();
            }

            printf("请稍等,正在准备...\n");
            ScoreInfo* r=NULL;
            if((r=_FindScoreInfoByStuID_CourseID_Flag(x.StudentID,x.CourseID,x.flag)))
            {
                printf("冲突：发现了一条记录: %s %s [%d] -> %.2f\n不能继续录入.\n",r->StudentID,r->CourseID,r->flag,r->Score);
                continue;
            }

            printf("请输入成绩:(0~100)\n");
            while(scanf("%lf",&x.Score)<1||x.Score<0||x.Score>100)
            {
                printf("\t输入有误.请重新输入\n");
                ClearInput();
            }

            printf("正在保存...\n");
            if(!PushBack(ls_score,&x))
            {
                printf("保存失败.内存不足.\n");
            }
            else
            {
                printf("保存成功.\n");
            }
        }
    }
}

void GDI_Score_Edit()
{
    ClearScreen();
    TITLE("编辑/修改成绩信息\n");
    while(1)
    {
        ClearInput();
        printf("请输入学号,留空返回...\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        ScoreInfo tmp;
        printf("请稍等...正在匹配学生信息...\n");
        StudentInfo* p=NULL;
        if(!(p=_FindStudentInfoByID(tmpbuff)))
        {
            printf("未找到学生信息(ID=%s).无法完成成绩录入.\n",tmpbuff);
            continue;
        }
        printf("匹配学生信息: %s(%s)\n",p->Name,p->ID);
        strncpy(tmp.StudentID,p->ID,CONFIG_STUID_SIZE);

        printf("请输入课程号\n");
        gets(tmpbuff);
        printf("请稍等...正在匹配课程信息...\n");
        CourseInfo* q=NULL;
        if(!(q=_FindCourseInfoByID(tmpbuff)))
        {
            printf("未找到课程信息(ID=%s).无法完成成绩录入.\n",tmpbuff);
            continue;
        }
        printf("匹配课程信息：%s(%s)\n",q->Name,q->ID);
        strncpy(tmp.CourseID,q->ID,CONFIG_COURSEID_SIZE);

        printf("请输入成绩来源(0: 第一次考试获得 1:重修获得)\n");
        while(scanf("%d",&tmp.flag)<1||tmp.flag<0||tmp.flag>1)
        {
            printf("\t输入有误.请重新输入\n");
            ClearInput();
        }

        printf("请稍等,正在准备...\n");
        ScoreInfo* r=NULL;
        if(!(r=_FindScoreInfoByStuID_CourseID_Flag(tmp.StudentID,tmp.CourseID,tmp.flag)))
        {
            printf("未发现相关记录.无法修改成绩.\n");
            continue;
        }

        printf("原有成绩为: %.2f 请输入成绩(0~100)\n",r->Score);
        while(scanf("%lf",&(r->Score))<1||r->Score<0||r->Score>100)
        {
            printf("\t输入有误.请重新输入\n");
            ClearInput();
        }
        printf("正在保存...\n");
        printf("保存成功.\n");
    }
}

void GDI_Score_DeleteSingle()
{
    ClearScreen();
    TITLE("删除成绩信息\n");
    while(1)
    {
        printf("请输入学生学号,留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;

        printf("请稍等,正在完成学生信息...\n");
        StudentInfo* tp;
        if((tp=_FindStudentInfoByID(tmpbuff))==NULL)
        {
            printf("未找到学生信息,无法录入成绩.\n");
            continue;
        }
        printf("学生: %s (%s)\n",tp->Name,tp->ID);

        ScoreInfo x;
        bzero(&x,sizeof(x));
        strncpy(x.StudentID,tmpbuff,CONFIG_STUID_SIZE);
        printf("请输入课程号\n");
        CLEARBUFF_READIN();
        CourseInfo* tq;
        if((tq=_FindCourseInfoByID(tmpbuff))==NULL)
        {
            printf("未找到课程信息,无法录入成绩.\n");
            continue;
        }
        printf("课程: %s (%s)\n",tq->Name,tq->ID);
        strncpy(x.CourseID,tmpbuff,CONFIG_COURSEID_SIZE);

        printf("请输入成绩来源(0: 第一次考试获得 1:重修获得)\n");
        while(scanf("%d",&x.flag)<1||x.flag<0||x.flag>1)
        {
            printf("\t输入有误.请重新输入\n");
            ClearInput();
        }

        printf("请稍等,正在准备...\n");
        ScoreInfo* r=NULL;
        if((r=_FindScoreInfoByStuID_CourseID_Flag(x.StudentID,x.CourseID,x.flag)))
        {
            printf("删除: %s %s [%d] -> %.2f\n删除成功\n",r->StudentID,r->CourseID,r->flag,r->Score);
            EraseSingle(ls_score,r-(ScoreInfo*)(ls_score->pdata));
        }
        else
        {
            printf("未找到记录.\n");
        }
    }
}
void GDI_Score_DeleteByStu()
{
    ClearScreen();
    TITLE("删除成绩信息-删除单人全部成绩信息\n");
    while(1)
    {
        printf("请输入学生学号,留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;

        printf("请稍等,正在完成学生信息...\n");
        StudentInfo* tp;
        if((tp=_FindStudentInfoByID(tmpbuff))==NULL)
        {
            printf("未找到学生信息,无法删除成绩信息.\n");
            continue;
        }
        printf("学生: %s (%s)\n",tp->Name,tp->ID);

        if(CONFIRM("确认要删除该学生的所有成绩信息么?\n"))
        {
            printf("操作取消.\n");
            continue;
        }

        printf("正在删除...\n");
        int cnt=0;
        int sz=GetListSize(ls_score);
        for(int i=0;i<sz;i++)
        {
            ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
            if(strcmp(p->StudentID,tp->ID)==0)
            {
                EraseSingle(ls_score,i);
                sz--;
                i--;
                ++cnt;
            }
        }
        printf("删除完成.共删除%d条记录.\n",cnt);
    }
}
void GDI_Score_DeleteByCourse()
{
    ClearScreen();
    TITLE("删除成绩信息-删除单科全部成绩信息\n");
    while(1)
    {
        printf("请输入课程号,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;

        CourseInfo* tq;
        if((tq=_FindCourseInfoByID(tmpbuff))==NULL)
        {
            printf("未找到课程信息,无法录入成绩.\n");
            continue;
        }
        printf("课程: %s (%s)\n",tq->Name,tq->ID);

        if(CONFIRM("确认要删除该课程的所有成绩信息么?\n"))
        {
            printf("操作取消.\n");
            continue;
        }

        printf("正在删除...\n");
        int cnt=0;
        int sz=GetListSize(ls_score);
        for(int i=0;i<sz;i++)
        {
            ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
            if(strcmp(p->CourseID,tq->ID)==0)
            {
                EraseSingle(ls_score,i);
                sz--;
                i--;
                ++cnt;
            }
        }
        printf("删除完成.共删除%d条记录.\n",cnt);
    }
}

int _SortScoreInfoByStuID(const void* a,const void* b)
{
    ScoreInfo* p=(ScoreInfo*)a;
    ScoreInfo* q=(ScoreInfo*)b;
    return strcmp(p->StudentID,q->StudentID);
}

void GDI_Score_List_SortByStu_FastSort()
{
    ClearScreen();
    TITLE("成绩信息-列举-学号升序\n");
    printf("请稍等,正在准备数据...\n");
    qsort(GetRawData(ls_score),GetListSize(ls_score),ls_score->typesz,_SortScoreInfoByStuID);
    printf("排序完成,正在准备列举...\n");
    ClearScreen();
    TITLE("成绩信息-列举-学号升序\n");
    int sz=GetListSize(ls_score);
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        printf("%s %s %.2f %s\n",p->StudentID,p->CourseID,p->Score,p->flag?"重修获得":"考试成绩");
    }
    printf("------------------------\n");
    printf("按任意键返回.\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_Score_List_SortByStu_SlowSort()
{
    WARNING("提示: 此处将直接跳转前往FastSort\n");
    GDI_Score_List_SortByStu_FastSort();
}

void GDI_Score_List_SortByStu()
{
    ClearScreen();
    TITLE("成绩信息-列举-学号升序\n");
    printf("正在查看排序设置参数...\n");
    if(_settings_use_fast_sort)
    {
        GDI_Score_List_SortByStu_FastSort();
    }
    else
    {
        GDI_Score_List_SortByStu_SlowSort();
    }
}



int _SortScoreInfoByCourseID(const void* a,const void* b)
{
    ScoreInfo* p=(ScoreInfo*)a;
    ScoreInfo* q=(ScoreInfo*)b;
    return strcmp(p->CourseID,q->CourseID);
}

void GDI_Score_List_SortByCourseID_FastSort()
{
    ClearScreen();
    TITLE("成绩信息-列举-课程号升序\n");
    printf("请稍等,正在准备数据...\n");
    qsort(GetRawData(ls_score),GetListSize(ls_score),ls_score->typesz,_SortScoreInfoByCourseID);
    printf("排序完成,正在准备列举...\n");
    ClearScreen();
    TITLE("成绩信息-列举-课程号升序\n");
    int sz=GetListSize(ls_score);
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        printf("%s %s %.2f %s\n",p->StudentID,p->CourseID,p->Score,p->flag?"重修获得":"考试成绩");
    }
    printf("------------------------\n");
    printf("按任意键返回.\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_Score_List_SortByCourseID_SlowSort()
{
    WARNING("提示: 此处将直接跳转前往FastSort\n");
    GDI_Score_List_SortByCourseID_FastSort();
}

void GDI_Score_List_SortByCourseID()
{
    ClearScreen();
    TITLE("成绩信息-列举-课程号升序\n");
    printf("正在查看排序设置参数...\n");
    if(_settings_use_fast_sort)
    {
        GDI_Score_List_SortByCourseID_FastSort();
    }
    else
    {
        GDI_Score_List_SortByCourseID_SlowSort();
    }
}




void GDI_Score_List_SortByScore_FastSort()
{
    ClearScreen();
    TITLE("成绩信息-列举-成绩降序\n");
    printf("请稍等,正在准备数据...\n");
    qsort(GetRawData(ls_score),GetListSize(ls_score),ls_score->typesz,_SortScoreInfoByScoreA);
    printf("排序完成,正在准备列举...\n");
    ClearScreen();
    TITLE("成绩信息-列举-课程号升序\n");
    int sz=GetListSize(ls_score);
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        printf("%s %s %.2f %s\n",p->StudentID,p->CourseID,p->Score,p->flag?"重修获得":"考试成绩");
    }
    printf("------------------------\n");
    printf("按任意键返回.\n");
    ClearInput();
    GetUserInputKey();
}

void GDI_Score_List_SortByScore_SlowSort()
{
    WARNING("提示: 此处将直接跳转前往FastSort\n");
    GDI_Score_List_SortByScore_FastSort();
}

void GDI_Score_List_SortByScore()
{
    ClearScreen();
    TITLE("成绩信息-列举-成绩降序\n");
    printf("正在查看排序设置参数...\n");
    if(_settings_use_fast_sort)
    {
        GDI_Score_List_SortByScore_FastSort();
    }
    else
    {
        GDI_Score_List_SortByScore_SlowSort();
    }
}

void GDI_Score_List_Unsort()
{
    ClearScreen();
    TITLE("成绩信息-列举-无序输出\n");
    WARNING("注意: 如果之前曾经执行过排序,那么数据的顺序将会与上一次排序后的顺序十分相近.\n");
    int sz=GetListSize(ls_score);
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        printf("%s %s %.2f %s\n",p->StudentID,p->CourseID,p->Score,p->flag?"重修获得":"考试成绩");
    }
    printf("------------------------\n");
    printf("按任意键返回.\n");
    ClearInput();
    GetUserInputKey();
}
void GDI_Score_List()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("成绩信息-列举\n");
        TAG(1);printf("以学号为顺序列出所有成绩信息\n");
        TAG(2);printf("以课程号为顺序列出所有成绩信息\n");
        TAG(3);printf("以成绩高低为顺序列出所有成绩信息\n");
        TAG(4);printf("无序输出\n");
        TAG(5);printf("返回\n");
        resetcolor();printf("---------------------------------\n");
        INFOPRINT(1,"以学号升序输出所有成绩信息\n");
        INFOPRINT(2,"以课程号升序输出所有成绩信息\n");
        INFOPRINT(3,"以成绩降序输出所有成绩信息\n");
        INFOPRINT(4,"不排序,直接输出\n");
        INFOPRINT(5,"返回上一菜单\n");
        if(GetAction(1,5))
        {
            GUI_LIST
            {
            case 1:
                GDI_Score_List_SortByStu();
                break;
            case 2:
                GDI_Score_List_SortByCourseID();
                break;
            case 3:
                GDI_Score_List_SortByScore();
                break;
            case 4:
                GDI_Score_List_Unsort();
                break;
            case 5:
                return;
            }
        }
    }
}

void GDI_Score_Search()
{
    ClearScreen();
    TITLE("成绩信息-搜索\n");
    while(1)
    {
        printf("请输入学生ID,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        ScoreInfo tmp;
        strncpy(tmp.StudentID,tmpbuff,CONFIG_STUID_SIZE);
        printf("请输入课程ID:\n");
        CLEARBUFF_READIN();
        strncpy(tmp.CourseID,tmpbuff,CONFIG_COURSEID_SIZE);

        printf("请稍等,正在检索...\n");
        ScoreInfo* p=_FindScoreInfoByStuID_CourseID_Flag(tmp.StudentID,tmp.CourseID,0);
        ScoreInfo* q=_FindScoreInfoByStuID_CourseID_Flag(tmp.StudentID,tmp.CourseID,1);
        if(p==NULL&&q==NULL)
        {
            printf("未找到学生%s的%s课程成绩信息.\n",tmp.StudentID,tmp.CourseID);
            continue;
        }
        if(p==NULL)
        {
            printf("找到1条成绩信息\n");
            printf("%s %s %.2f %s\n",q->StudentID,q->CourseID,q->Score,q->flag?"重修获得":"考试成绩");
        }
        else if(q==NULL)
        {
            printf("找到1条成绩信息\n");
            printf("%s %s %.2f %s\n",p->StudentID,p->CourseID,p->Score,p->flag?"重修获得":"考试成绩");
        }
        else
        {
            printf("找到2条成绩信息\n");
            printf("%s %s %.2f %s\n",p->StudentID,p->CourseID,p->Score,p->flag?"重修获得":"考试成绩");
            printf("%s %s %.2f %s\n",q->StudentID,q->CourseID,q->Score,q->flag?"重修获得":"考试成绩");
        }
    }
}

void GDI_Score_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("成绩信息管理\n");
        TAG(1);printf("添加新成绩记录\n");
        TAG(2);printf("添加一个人的成绩记录\n");
        TAG(3);printf("添加一个科目的成绩记录\n");
        TAG(4);printf("修改成绩记录\n");
        TAG(5);printf("删除成绩记录\n");
        TAG(6);printf("删除一个人的成绩记录\n");
        TAG(7);printf("删除一个科目的成绩记录.\n");
        TAG(8);printf("列出所有成绩记录\n");
        TAG(9);printf("搜索成绩记录\n");
        TAG(10);printf("返回\n");
        resetcolor();
        printf("----------------------------\n");
        INFOPRINT(1,"提供完整的课程信息,建立新的成绩信息\n");
        INFOPRINT(2,"以人为主,输入多门课程的成绩\n");
        INFOPRINT(3,"以课程为主,输入多个人的课程的成绩.\n");
        INFOPRINT(4,"提供一些信息,修改某条成绩信息\n");
        INFOPRINT(5,"删除成绩信息.删除前需要确认\n");
        INFOPRINT(6,"一次性的删除一个人的所有成绩信息.\n");
        INFOPRINT(7,"一次性的删除一个科目的所有成绩信息\n");
        INFOPRINT(8,"列出所有成绩信息\n");
        INFOPRINT(9,"在已储存的数据中进行搜索\n");
        INFOPRINT(10,"返回上一个页面\n");
        if(GetAction(1,10))
        {
            GUI_LIST
            {
            case 1:
                GDI_Score_AddSingle();
                break;
            case 2:
                GDI_Score_AddByStu();
                break;
            case 3:
                GDI_Score_AddByCourse();
                break;
            case 4:
                GDI_Score_Edit();
                break;
            case 5:
                GDI_Score_DeleteSingle();
                break;
            case 6:
                GDI_Score_DeleteByStu();
                break;
            case 7:
                GDI_Score_DeleteByCourse();
                break;
            case 8:
                GDI_Score_List();
                break;
            case 9:
                GDI_Score_Search();
                break;
            case 10:
                return;
            }
        }
    }
}
