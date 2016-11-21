#define QSORT_ADAPTER(Bool_Expression) !(Bool_Expression)
int _SortScoreInfoByScoreA(const void* a,const void* b)
{
    ScoreInfo** pp=(ScoreInfo**)a;
    ScoreInfo** qq=(ScoreInfo**)b;
    ScoreInfo* p=*pp;
    ScoreInfo* q=*qq;
    if(p->Score==q->Score)
    {
        return 0;
    }
    return QSORT_ADAPTER(p->Score > q->Score) ;
}

char _GetLabelOfScore(double score)
{
    if(score>=90) return 'A';
    if(score>=80) return 'B';
    if(score>=70) return 'C';
    if(score>=60) return 'D';
    return 'E';
}
void GDI_View_SingleCourse()
{
    ClearScreen();
    printf("统计-成绩分段统计\n");
    while(1)
    {
        printf("请输入课程号,留空返回\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) break;
        printf("请稍等,正在准备...\n");
        CourseInfo* t=_FindCourseInfoByID(tmpbuff);
        if(!t)
        {
            printf("未找到ID=%s的课程,无法统计分数.\n",tmpbuff);
            continue;
        }
        printf("正在收集数据,请稍等...\n");
        printf("创建数据连接...\n");
        DYHANDLE tmphandle=GetList(sizeof(void*),1024);
        if(!tmphandle)
        {
            printf("创建数据连接失败. 无法进行统计.\n");
            continue;
        }
        printf("收集成绩信息 (课程名 %s)\n",t->Name);
        int lsz=GetListSize(ls_score);
        int i;
        for(i=0;i<lsz;i++)
        {
            ScoreInfo* p=GetMember(ls_score,i);
            if(strcmp(p->CourseID,t->ID)==0)
            {
                PushBack(tmphandle,&p);
            }
        }
        if(GetListSize(tmphandle)<1)
        {
            FreeList(tmphandle);
            printf("没有找到任何数据,并不生成空的表格.\n");
            printf("按下任意键返回\n");
            ClearInput();
            getch();
            continue;
        }
        printf("收集到%d条数据.处理中...\n",GetListSize(tmphandle));
        qsort(GetRawData(tmphandle),GetListSize(tmphandle),tmphandle->typesz,_SortScoreInfoByScoreA);
        printf("处理完毕.准备输出...\n");
        ClearScreen();
        printf("课程 %s 成绩分段统计表\n",t->ID);
        int tsz=GetListSize(tmphandle);
        for(i=0;i<tsz;i++)
        {
            ScoreInfo** nx=GetMember(tmphandle,i);
            ScoreInfo* x=*nx;
            StudentInfo* y=_FindStudentInfoByID(x->StudentID);
            printf("%s(%s) [%c] %.2f\n",y->Name,y->ID,_GetLabelOfScore(x->Score),x->Score);
        }
        FreeList(tmphandle);
        printf("-----------------\n");
        printf("按下任意键返回\n");
        ClearInput();
        getch();
    }
}

void GDI_View_SingleStudent()
{
    ClearScreen();
    TITLE("统计-个人分学期统计\n");
    while(1)
    {
        USE_RELEASE;
        printf("请输入学生ID,留空返回...\n");
        bzero(tmpbuff,1024);
        gets(tmpbuff);
        if(strlen(tmpbuff)<1) return;
        StudentInfo* t=_FindStudentInfoByID(tmpbuff);
        if(!t)
        {
            printf("未找到学生信息.无法统计.\n");
            continue;
        }
        printf("正在收集数据...\n");
        printf("创建数据缓存...\n");
        DYHANDLE thand=GetList(sizeof(ScoreInfo*),1024);
        if(thand==NULL)
        {
            printf("无法创建数据缓存. 无法统计\n");
            DO_RELEASE;
            continue;
        }
        ADD_RELEASE(thand);
        DYHANDLE tpool=GetList(sizeof(double),16);
        if(tpool==NULL)
        {
            printf("无法创建数据缓存. 无法统计\n");
            DO_RELEASE;
            continue;
        }
        ADD_RELEASE(tpool);
        printf("搜集成绩信息...\n");
        int lsz=GetListSize(ls_score);
        for(int i=0;i<lsz;i++)
        {
            ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
            if(strcmp(p->StudentID,t->ID)==0)
            {
                PushBack(thand,&p);
            }
        }
        printf("处理中...\n");
        lsz=GetListSize(thand);
        for(int i=1;i<=8;i++)
        {
            double total=0;
            int counter=0;
            for(int j=0;j<lsz;j++)
            {
                ScoreInfo** pp=(ScoreInfo**)GetMember(thand,j);
                ScoreInfo* p=*pp;
                CourseInfo* q=_FindCourseInfoByID(p->CourseID);
                if(q->Term==i)
                {
                    total+=p->Score;
                    counter++;
                }
            }
            total/=counter;/// Need This?(20161106)
            PushBack(tpool,&total);
        }
        printf("数据收集完毕,请稍等...\n");
        ClearScreen();
        printf("学生 %s(%s) 分学期成绩统计表\n",t->Name,t->ID);
        printf("学期 科目平均分 分数段\n");
        for(int i=0;i<8;i++)
        {
            printf("%d %.2f %c\n",i+1,*(double*)GetMember(tpool,i),_GetLabelOfScore(*(double*)GetMember(tpool,i)));
        }
        DO_RELEASE;
        printf("-----------------\n");
        printf("按下任意键返回\n");
        ClearInput();getch();
    }
}


typedef struct
{
    int b,s,a;
    int ok;
}rank_pack;
/// Mode: 0: No Second will be calculate; 1: Only Second Will be calculate 2: Both First and Second are calculate
rank_pack getRank_Score(const char* CourseID,double score,int mode)
{
    rank_pack ppk;
    ppk.ok=0;
    if(!ls_score) return ppk;
    int sz=GetListSize(ls_score);
    int before=0;
    int after=0;
    int same=0;
    for(int i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        if(strcmp(p->CourseID,CourseID)==0)
        {
            switch(mode)
            {
            case 0:
                if(p->flag==0)
                {
                    if(p->Score>score)
                    {
                        before++;
                    }
                    else if(p->Score==score)
                    {
                        same++;
                    }
                    else
                    {
                        after++;
                    }
                }
                break;
            case 1:
                if(p->flag==1)
                {
                    if(p->Score>score)
                    {
                        before++;
                    }
                    else if(p->Score==score)
                    {
                        same++;
                    }
                    else
                    {
                        after++;
                    }
                }
            case 2:
                if(p->Score>score)
                {
                    before++;
                }
                else if(p->Score==score)
                {
                    same++;
                }
                else
                {
                    after++;
                }
                break;
            }
        }
    }

    ppk.b=before;
    ppk.s=same;
    ppk.a=after;
    ppk.ok=1;
    return ppk;
}


void GDI_View_StudentInClass()
{
    ClearScreen();
    TITLE("统计-个人分班级统计\n");
    while(1)
    {
        printf("请输入学生ID,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        printf("请稍等,正在匹配学生信息...\n");
        StudentInfo* pStudent=_FindStudentInfoByID(tmpbuff);
        if(pStudent==NULL)
        {
            printf("未找到学生信息,无法完成统计.\n");
            continue;
        }
        printf("获取学生班级...\n");
        class_id clsid=getStudentClassID(pStudent->ID);
        printf("正在获取学生-班级名单缓存\n");
        DYHANDLE hand=getCache_StudentClassList(clsid);
        if(hand==NULL)
        {
            printf("缓存未命中.正在创建缓存...\n");
            hand=makeCache_StudentClassList(clsid);
            if(hand==NULL)
            {
                printf("创建缓存失败.无法完成统计\n");
                continue;
            }
        }
        printf("缓存获取完成.正在计算个人年级排名...\n");
        printf("创建数据缓存...\n");
        //DYHANDLE temp_list=GetList(sizeof(rank_pack),1024);


        //int ans_personal_rank=-1;
        int sz_StuList=GetListSize(hand);
        int sz_ScoList=GetListSize(ls_score);
        int flag_fail=0;
        printf("----------------\n"
               "单科目全年级排行\n"
               "----------------\n");
        for(int i=0;i<sz_ScoList;i++)
        {
            ScoreInfo* kp=(ScoreInfo*)GetMember(ls_score,i);
            if(strcmp(kp->StudentID,pStudent->ID)==0&&kp->flag==0)
            {
                rank_pack pack=getRank_Score(kp->CourseID,kp->Score,0);
                if(pack.ok==0)
                {
                    printf("获取年级排名错误.无法计算排名.\n");
                    flag_fail=1;
                    break;
                }
                printf("科目:%s(%s) 成绩:%.2f\n\t年级排名: %d (前方%d人,同成绩%d人,后方%d人)\n",_FindCourseInfoByID(kp->CourseID)->Name,kp->CourseID,kp->Score,pack.b+1,pack.b,pack.s,pack.a);
                int before=0;
                int after=0;
                int same=0;
                for(int i=0;i<sz_StuList;i++)
                {
                    StudentInfo** spp=(StudentInfo**)GetMember(hand,i);
                    StudentInfo* sp=*spp;
                    ScoreInfo* npp=_FindScoreInfoByStuID_CourseID_Flag(sp->ID,kp->CourseID,0);
                    if(npp!=NULL)
                    {
                        if(npp->Score>kp->Score)
                        {
                            ++before;
                        }
                        else if(npp->Score==kp->Score)
                        {
                            ++same;
                        }
                        else
                        {
                            ++after;
                        }
                    }
                }
                printf("\t班级排名: %d (前方%d人,同成绩%d人,后方%d人)\n",before+1,before,same,after);
            }
        }
        if(flag_fail)
        {
            continue;
        }
    }
}

/*
/// Code Change: The Following Code is Deprecated.
typedef struct
{
    CourseInfo* p;
    DYHANDLE students;
} _T_Mixed_Student_Dom_Course;

void _F_Free_T_Mixed_Student_Dom_Course(DYHANDLE hand)
{
    int sz=GetListSize(hand);
    for(int i=0;i<sz;i++)
    {
        _T_Mixed_Student_Dom_Course* p=(_T_Mixed_Student_Dom_Course*)GetMember(hand,i);
        FreeList(p->students);
    }
}
*/

void GDI_View_SingleDomi()
{
    ClearScreen();
    TITLE("统计-宿舍统计\n");
    while(1)
    {
        printf("请输入宿舍号,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        printf("正在查找%s的宿舍...\n",tmpbuff);
        printf("获取列表...\n");
        DYHANDLE shand=GetList(sizeof(StudentInfo*),16);
        if(shand==NULL)
        {
            printf("无法获取列表.\n");
            continue;
        }
        printf("启动内存管理设备...\n");
        USE_RELEASE;
        ADD_RELEASE(shand);
        int sz_Student=GetListSize(ls_student);
        int flag_fail=0;
        printf("正在填充列表...\n");
        for(int i=0;i<sz_Student;i++)
        {
            StudentInfo* p=(StudentInfo*)GetMember(ls_student,i);
            if(strcmp(p->Dormitory,tmpbuff)==0)
            {
                if(PushBack(shand,&p)==NULL)
                {
                    printf("无法填充列表.\n");
                    flag_fail=1;
                    break;
                }
            }
        }
        if(flag_fail)
        {
            DO_RELEASE;
            continue;
        }
        flag_fail=0;

        int sz_Dom=GetListSize(shand);
        if(sz_Dom<1)
        {
            printf("宿舍人数不足1人,无法统计.\n");
            continue;
        }

        /// We Now Use Methods in GDI_View_SingleStudent.
        flag_fail=0;
        for(int cc=0; cc<sz_Dom; cc++)
        {
            StudentInfo* t=*(StudentInfo**)GetMember(shand,cc);
            DYHANDLE thand=GetList(sizeof(ScoreInfo*),1024);
            if(thand==NULL)
            {
                printf("无法创建数据缓存. 无法统计\n");
                flag_fail=1;
                break;
            }
            ADD_RELEASE(thand);
            DYHANDLE tpool=GetList(sizeof(double),16);
            if(tpool==NULL)
            {
                printf("无法创建数据缓存. 无法统计\n");
                flag_fail=1;
                break;
            }
            ADD_RELEASE(tpool);

            int lsz=GetListSize(ls_score);
            for(int i=0; i<lsz; i++)
            {
                ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
                if(strcmp(p->StudentID,t->ID)==0)
                {
                    PushBack(thand,&p);
                }
            }
            lsz=GetListSize(thand);
            for(int i=1; i<=8; i++)
            {
                double total=0;
                int counter=0;
                for(int j=0; j<lsz; j++)
                {
                    ScoreInfo** pp=(ScoreInfo**)GetMember(thand,j);
                    ScoreInfo* p=*pp;
                    CourseInfo* q=_FindCourseInfoByID(p->CourseID);
                    if(q->Term==i)
                    {
                        total+=p->Score;
                        counter++;
                    }
                }
                PushBack(tpool,&total);
            }
            printf("学生 %s(%s) 分学期成绩统计表\n",t->Name,t->ID);
            printf("学期 科目平均分 分数段\n");
            for(int i=0; i<8; i++)
            {
                printf("%d %.2f %c\n",i+1,*(double*)GetMember(tpool,i),_GetLabelOfScore(*(double*)GetMember(tpool,i)));
            }

            POP_RELEASE();
            POP_RELEASE();
            printf("-----------------\n");
        }

        /*
        /// Code Change: The Following Code is Deprecated.

        printf("正在统计宿舍所选课程...\n");

        //AutoRelease ale=GetAutoReleaser();
        //ale.func=_F_Free_T_Mixed_Student_Dom_Course;

        DYHANDLE dom_list=GetList(sizeof(_T_Mixed_Student_Dom_Course),256);
        if(dom_list==NULL)
        {
            DO_RELEASE;
            printf("创建数据缓存表失败,无法继续.\n");
            continue;
        }
        ADD_RELEASE(dom_list);

        //AddRelease(&ale,dom_list);

        /// WARNING: The Following Code May Cause An Overflow in AutoRelease...

        int sz_Score=GetListSize(ls_score);
        for(int i=0;i<sz_Score;i++)
        {
            ScoreInfo* kp=(ScoreInfo*)GetMember(ls_score,i);
            if(kp->flag==1) continue;
            for(int j=0;j<sz_Dom;j++)
            {
                StudentInfo** spp=(StudentInfo**)GetMember(shand,j);
                StudentInfo* sp=*spp;
                if(strcmp(kp->StudentID,sp->ID)==0)
                {
                    _T_Mixed_Student_Dom_Course x;
                    memset(&x,0,sizeof(x));

                    x.p=_FindCourseInfoByID(kp->CourseID);
                    x.students=GetList(sizeof(StudentInfo*),16);
                    if(x.students==NULL)
                    {
                        printf("创建数据表失败.\n");
                        flag_fail=1;
                        break;
                    }
                    ADD_RELEASE(x.students);

                    if(PushBack(x.students,&sp)==NULL)
                    {
                        printf("无法向数据列表中添加学生信息.\n");
                        flag_fail=1;
                        break;
                    }

                    int sz_Current=GetListSize(dom_list);
                    int flag_exist=0;
                    for(int k=0;k<sz_Current;k++)
                    {
                        _T_Mixed_Student_Dom_Course* p=(_T_Mixed_Student_Dom_Course*)GetMember(dom_list,k);
                        if(p->p==x.p)
                        {
                            if(PushBack(p->students,&sp)==NULL)
                            {
                                printf("无法向数据列表中添加学生信息.\n");
                                flag_fail=1;
                                break;
                            }
                            flag_exist=1;
                            break;
                        }
                    }
                    if(flag_fail) break;
                    if(flag_exist==0&&PushBack(dom_list,&x)==NULL)
                    {
                        printf("无法添加数据.ERROR.\n");
                        flag_fail=1;
                        break;
                    }
                }
            }
            if(flag_fail) break;
        }

        if(flag_fail)
        {
            DO_RELEASE;
            continue;
        }
        flag_fail=0;

        printf("缓存表构建完成.\n");

        printf("正在统计数据...\n");
        int sz_CC=GetListSize(dom_list);
        for(int i=0;i<sz_CC;i++)
        {
            _T_Mixed_Student_Dom_Course* ap=(_T_Mixed_Student_Dom_Course*)GetMember(dom_list,i);
            int sz_K=GetListSize(ap->students);
            for(int j=0;j<sz_K;j++)
            {
                StudentInfo* sp=(StudentInfo*)GetMember(ap->students,j);
                ScoreInfo* np=_FindScoreInfoByStuID_ClassID_Flag(sp->ID,ap->p,0);
                //sz

            }
        }

        */

        DO_RELEASE;
    }
}


void GDI_View_FailTimes()
{
    ClearScreen();
    TITLE("统计-单人挂科统计\n");
    while(1)
    {
        USE_RELEASE;
        printf("请输入学生ID,留空返回...\n");
        bzero(tmpbuff,1024);
        gets(tmpbuff);
        if(strlen(tmpbuff)<1) return;
        StudentInfo* t=_FindStudentInfoByID(tmpbuff);
        if(!t)
        {
            printf("未找到学生信息.无法统计.\n");
            continue;
        }
        printf("正在收集数据...\n");
        printf("创建数据缓存...\n");
        DYHANDLE thand=GetList(sizeof(ScoreInfo*),1024);
        if(thand==NULL)
        {
            printf("无法创建数据缓存. 无法统计\n");
            DO_RELEASE;
            continue;
        }
        ADD_RELEASE(thand);
        DYHANDLE tpool=GetList(sizeof(int),16);
        if(tpool==NULL)
        {
            printf("无法创建数据缓存. 无法统计\n");
            DO_RELEASE;
            continue;
        }
        ADD_RELEASE(tpool);
        printf("搜集成绩信息...\n");
        int lsz=GetListSize(ls_score);
        for(int i=0;i<lsz;i++)
        {
            ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
            if(strcmp(p->StudentID,t->ID)==0&&p->flag==1)
            {
                PushBack(thand,&p);
            }
        }
        printf("处理中...\n");
        lsz=GetListSize(thand);
        for(int i=1;i<=8;i++)
        {
            int counter=0;
            for(int j=0;j<lsz;j++)
            {
                ScoreInfo** pp=(ScoreInfo**)GetMember(thand,j);
                ScoreInfo* p=*pp;
                CourseInfo* q=_FindCourseInfoByID(p->CourseID);
                if(q->Term==i)
                {
                    counter++;
                }
            }
            PushBack(tpool,&counter);
        }
        printf("数据收集完毕,请稍等...\n");
        ClearScreen();
        printf("学生 %s(%s) 分学期挂科统计表\n",t->Name,t->ID);
        printf("学期 挂科次数\n");
        for(int i=0;i<8;i++)
        {
            printf("%d %d\n",i+1,*(int*)GetMember(tpool,i));
        }
        DO_RELEASE;
        printf("-----------------\n");
        printf("按下任意键返回\n");
        ClearInput();GetUserInputKey();
    }
}


void _TGA_Frame_OneManHistory(StudentInfo* t)
{
    USE_RELEASE;
    printf("正在收集数据...\n");
    printf("创建数据缓存...\n");
    DYHANDLE thand=GetList(sizeof(ScoreInfo*),1024);
    if(thand==NULL)
    {
        printf("无法创建数据缓存. 无法统计\n");
        DO_RELEASE;
        return;
    }
    ADD_RELEASE(thand);
    DYHANDLE tpool=GetList(sizeof(double),16);
    if(tpool==NULL)
    {
        printf("无法创建数据缓存. 无法统计\n");
        DO_RELEASE;
        return;
    }
    ADD_RELEASE(tpool);
    printf("搜集成绩信息...\n");
    int lsz=GetListSize(ls_score);
    for(int i=0; i<lsz; i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        if(strcmp(p->StudentID,t->ID)==0)
        {
            PushBack(thand,&p);
        }
    }
    printf("处理中...\n");
    lsz=GetListSize(thand);
    for(int i=1; i<=8; i++)
    {
        double total=0;
        int counter=0;
        for(int j=0; j<lsz; j++)
        {
            ScoreInfo** pp=(ScoreInfo**)GetMember(thand,j);
            ScoreInfo* p=*pp;
            CourseInfo* q=_FindCourseInfoByID(p->CourseID);
            if(q->Term==i)
            {
                total+=p->Score;
                counter++;
            }
        }
        total/=counter;/// Need This?(20161106)
        PushBack(tpool,&total);
    }
    printf("数据收集完毕,请稍等...\n");
    ClearScreen();
    printf("学生 %s(%s) 个人成绩对照表\n",t->Name,t->ID);
    for(int i=1; i<8; i++)
    {
        double _last_term=*(double*)GetMember(tpool,i-1);
        double _this_term=*(double*)GetMember(tpool,i);
        printf("第%d学期 --> 第%d学期 : %s\n",i,i+1,_last_term<_this_term?"上升":"下降");
    }
    DO_RELEASE;
    printf("-----------------\n");
    printf("按下任意键返回\n");
    ClearInput();
    getch();
}

void GDI_View_OneManHistory()
{
    ClearScreen();
    TITLE("统计-个人排名变化分析\n");
    while(1)
    {
        printf("请输入学生ID,留空返回...\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        StudentInfo* t=_FindStudentInfoByID(tmpbuff);
        if(!t)
        {
            printf("未找到学生信息.无法统计.\n");
            continue;
        }

        _TGA_Frame_OneManHistory(t);

    }
}

void GDI_View_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("统计面板\n");
        TAG(1);printf("单科成绩统计\n");
        TAG(2);printf("单人分学期成绩统计\n");
        TAG(3);printf("单人分班级成绩统计\n");
        TAG(4);printf("宿舍成绩统计\n");
        TAG(5);printf("单人分学期挂科数统计\n");
        TAG(6);printf("个人排名变化分析\n");
        TAG(7);printf("返回\n");
        resetcolor();printf("-----------------\n");
        INFOPRINT(1,"对某门功课各分数段成绩进行统计\n");
        INFOPRINT(2,"分学期对学生业务课程平均分按分数段进行统计\n");
        INFOPRINT(3,"统计任意一名同学每门功课的班级排名以及业务课成绩总体排名\n");
        INFOPRINT(4,"以宿舍为单位进行成绩统计分析\n");
        INFOPRINT(5,"以挂科次数为依据分学期对比分析\n");
        INFOPRINT(6,"以业务课班级排名为依据分学期对比分析（前进或退步情况）\n");
        INFOPRINT(7,"返回上一菜单\n");

        if(GetAction(1,7))
        {
            GUI_LIST
            {
            case 1:
                GDI_View_SingleCourse();
                break;
            case 2:
                GDI_View_SingleStudent();
                break;
            case 3:
                GDI_View_StudentInClass();
                break;
            case 4:
                GDI_View_SingleDomi();
                break;
            case 5:
                GDI_View_FailTimes();
                break;
            case 6:
                GDI_View_OneManHistory();
                break;
            case 7:
                return;
            }
        }
    }
}
