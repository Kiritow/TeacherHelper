typedef struct
{
    class_id ClassID;
    DYHANDLE students;
}_StudentClassInfo;

DYHANDLE _Cache_Class;

typedef struct
{
    class_id ClassID;
    DYHANDLE scores;
}_ScoreClassInfo;

DYHANDLE _Cache_Score;

int _init_cache()
{
    _Cache_Class=GetList(sizeof(_StudentClassInfo),1024);
    if(_Cache_Class==NULL) return -1;

    _Cache_Score=GetList(sizeof(_ScoreClassInfo),1024);
    if(_Cache_Score==NULL) return -2;

    return 0;
}

int _Log_Output(const char* fmt,...)
{
    va_list vlist;
    va_start(vlist,fmt);

    if(_settings_use_log)
    {
        if(_log_fp)
        {
            vfprintf(_log_fp,fmt,vlist);
            va_end(vlist);
            return 0;
        }
        else
        {
            va_end(vlist);
            return -2;/// File Not Opened.
        }
    }
    else
    {
        va_end(vlist);
        return -1;/// Log Plug-in not enabled.
    }
}

void _Notify_Cache_Class_OutOfDate()
{

}

void _Notify_Cache_Class_OutOfDate_Single(StudentInfo* p)
{

}

void _Notify_Cache_Score_OutOfDate()
{

}

void _Notify_Cache_Score_OutOfDate_Single(ScoreInfo* p)
{

}

DYHANDLE getCache_StudentClassList(class_id ClassID)
{
    if(_Cache_Class==NULL)
    {
        /// Not Cached.
        return NULL;
    }
    int sz_ClassListSize=GetListSize(_Cache_Class);
    for(int i=0;i<sz_ClassListSize;i++)
    {
        _StudentClassInfo* p=(_StudentClassInfo*)GetMember(_Cache_Class,i);
        if(p->ClassID==ClassID)
        {
            return p->students;
        }
    }
    return NULL;/// Not Found
}

DYHANDLE makeCache_StudentClassList(class_id ClassID)
{
    if(_Cache_Class==NULL)
    {
        /// Not Cached
        return NULL;
    }
    DYHANDLE hand=GetList(sizeof(StudentInfo*),64);
    if(hand==NULL)
    {
        /// Out of Memory
        return NULL;
    }
    /// Make Cache From Student Info, whose classID is ClassID.
    int sz_Student=GetListSize(ls_student);
    for(int i=0;i<sz_Student;i++)
    {
        StudentInfo* p=(StudentInfo*)GetMember(ls_student,i);
        if(getStudentClassID(p->ID)==ClassID)
        {
            if(PushBack(hand,&p)==NULL)
            {
                FreeList(hand);
                return NULL;/// Cannot Push Back (Out of Memory)
            }
        }
    }
    _StudentClassInfo tmp;
    tmp.ClassID=ClassID;
    tmp.students=hand;
    if(PushBack(_Cache_Class,&tmp)==NULL)
    {
        FreeList(hand);
        return NULL;/// Cannot Push Back (Out of Memory)
    }

    return hand;
}

DYHANDLE getCache_ScoreClassList(class_id ClassID)
{
    if(_Cache_Score==NULL)
    {
        /// Not Cached
        return NULL;
    }
    int sz_ClassListSize=GetListSize(_Cache_Score);
    for(int i=0;i<sz_ClassListSize;i++)
    {
        _ScoreClassInfo* p=(_ScoreClassInfo*)GetMember(_Cache_Score,i);
        if(p->ClassID==ClassID)
        {
            return p->scores;
        }
    }
    return NULL;/// Not Found
}

DYHANDLE makeCache_ScoreClassList(class_id ClassID)
{
    if(_Cache_Score==NULL)
    {
        /// Not Cached
        return NULL;
    }
    DYHANDLE hand=GetList(sizeof(ScoreInfo*),1024);
    if(hand==NULL)
    {
        /// Out of Memory
        return NULL;
    }
    /// Make Cache From Score Info, whose StudentID -> classID is ClassID.
    int sz_Student=GetListSize(ls_score);
    for(int i=0;i<sz_Student;i++)
    {
        ScoreInfo* p=(ScoreInfo*)GetMember(ls_score,i);
        if(getStudentClassID(p->StudentID)==ClassID)
        {
            if(PushBack(hand,&p)==NULL)
            {
                FreeList(hand);
                return NULL;/// Cannot Push Back (Out of Memory)
            }
        }
    }
    _ScoreClassInfo tmp;
    tmp.ClassID=ClassID;
    tmp.scores=hand;
    if(PushBack(_Cache_Score,&tmp)==NULL)
    {
        FreeList(hand);
        return NULL;/// Cannot Push Back (Out of Memory)
    }

    return hand;
}
