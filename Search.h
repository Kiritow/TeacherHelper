void GDI_Search_MainPad()
{
    while(1)
    {
        ClearScreen();
        TITLE("搜索\n");
        printf("请输入搜索关键字,留空返回\n搜索范围为: 学生信息(学号,姓名,宿舍号) 课程信息(课程号,课程名称) 成绩信息(学号,课程号) 奖惩信息(学号)\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;
        printf("---学生信息搜索结果---\n");
        int sz=GetListSize(ls_student);
        for(int i=0;i<sz;i++)
        {
            StudentInfo* p=(StudentInfo*)GetMember(ls_student,i);
            if(strstr(p->Dormitory,tmpbuff)!=NULL || strstr(p->ID,tmpbuff)!=NULL || strstr(p->Name,tmpbuff)!=NULL)
            {
                printf("%s %s %d %s %d\n",p->ID,p->Name,p->Sexual,p->Dormitory,p->Age);
            }
        }
        printf("---课程信息搜索结果---\n");
        sz=GetListSize(ls_course);
        for(int i=0;i<sz;i++)
        {
            CourseInfo* p=(CourseInfo*)GetMember(ls_course,i);
            if(strstr(p->ID,tmpbuff)||strstr(p->Name,tmpbuff))
            {
                printf("%s %s %.2f %d %d\n",p->Name,p->ID,p->Score,p->Term,p->Type);
            }
        }
        printf("---成绩信息搜索结果---\n");
        sz=GetListSize(ls_score);
        for(int i=0;i<sz;i++)
        {
            ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
            if(strstr(p->StudentID,tmpbuff)||strstr(p->CourseID,tmpbuff))
            {
                printf("%s %s %.2lf %s\n",p->StudentID,p->CourseID,p->Score,p->flag?"重修获得":" ");
            }
        }
        printf("---奖惩信息搜索结果---\n");
        DYHANDLE h=getAchievesInfoByStuID(tmpbuff,NULL);
        if(h==NULL)
        {
            printf("查找失败.\n");
        }
        else
        {
            int sz=GetListSize(h);
            for(int i=0;i<sz;i++)
            {
                AchieveInfo* p=*(AchieveInfo**)GetMember(h,i);
                printf("%s %s %d %04d/%02d/%02d\n  %s\n",p->StudentID,_GetAchieveNameByType(p->type),p->score,p->time.year,p->time.mon,p->time.day,p->Message);
            }
            FreeList(h);
        }

        printf("------------------------\n");
        printf("按任意键返回.\n");
        ClearInput();
        GetUserInputKey();
    }
}
