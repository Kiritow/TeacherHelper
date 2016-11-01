StudentInfo* _FindStudentInfoByID(const char* IDStr)
{
    if(!ls_student) return NULL;
    int i;
    int sz=GetListSize(ls_student);
    for(i=0;i<sz;i++)
    {
        StudentInfo* p=(StudentInfo*)GetMember(ls_student,i);
        if(strcmp(p->ID,IDStr)==0)
        {
            return p;
        }
    }
    return NULL;
}
CourseInfo* _FindCourseInfoByID(const char* IDStr)
{
    if(!ls_course) return NULL;
    int i;
    int sz=GetListSize(ls_course);
    for(i=0;i<sz;i++)
    {
        CourseInfo* p=(CourseInfo*)GetMember(ls_course,i);
        if(strcmp(p->ID,IDStr)==0)
        {
            return p;
        }
    }
    return NULL;
}
ScoreInfo* _FindScoreInfoByStuID_CourseID_Flag(const char* StuID,const char* CourseID,int flag)
{
    if(!ls_score) return NULL;
    int i;
    int sz=GetListSize(ls_score);
    for(i=0;i<sz;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        if(strcmp(p->StudentID,StuID)==0&&strcmp(p->CourseID,CourseID)==0&&p->flag==flag)
        {
            return p;
        }
    }
    return NULL;
}
AchieveInfo* _FindAchieveInfoByStuID(const char* StuID)
{
    if(!ls_achieve) return NULL;
    int i;
    int sz=GetListSize(ls_achieve);
    for(i=0;i<sz;i++)
    {
        AchieveInfo* p=(AchieveInfo*)GetMember(ls_achieve,i);
        if(strcmp(p->StudentID,StuID)==0)
        {
            return p;
        }
    }
    return NULL;
}

