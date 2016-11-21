
void GDI_Scholar_MasterLove()
{
    ClearScreen();
    TITLE("奖学金评定-校长奖学金评定\n");
    _kernel_enter_sudo();
    WARNING("您已进入超级用户模式,在此模式下进行的所有修改将不会被检查.\n");
    while(1)
    {
        printf("请输入学生ID,留空返回...\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) break;
        printf("请稍等,正在查找学生信息...\n");
        StudentInfo* p=_FindStudentInfoByID(tmpbuff);
        if(p==NULL)
        {
            printf("未找到ID=%s的学生.无法评定奖学金.\n",tmpbuff);
            GetUserInputKey();
            continue;
        }
        AchieveInfo tmp;
        tmp.score=0;
        tmp.type=20;
        tmp.score=_GetAchieveScoreByType(tmp.type);
        strncpy(tmp.StudentID,p->ID,CONFIG_STUID_SIZE);
        printf("请输入发奖日期:(格式: 年 月 日)\n");
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
        tmp.time.year=y;
        tmp.time.mon=m;
        tmp.time.day=d;
        printf("请输入您对该学生想说的话:\n");
        CLEARBUFF_READIN();
        snprintf(tmp.Message,CONFIG_ACHMSG_SIZE,"由奖学金系统手动生成. 奖学金级别: 校长奖学金 评语: %s\n",strlen(tmpbuff)<1?"暂无评语":tmpbuff);
        printf("请稍等,正在保存...\n");
        if(PushBack(ls_achieve,&tmp)!=NULL)
        {
            printf("保存成功\n");
        }
        else
        {
            printf("保存失败\n");
        }
    }
    _kernel_left_sudo();
}

void GDI_Scholar_SingleStuff()
{
    while(1)
    {
        ClearScreen();
        TITLE("奖学金评定-单项素质奖学金评定\n");
        printf("请输入学生ID,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;

        printf("请稍等,正在搜索学生信息...\n");
        StudentInfo* p=_FindStudentInfoByID(tmpbuff);
        if(p==NULL)
        {
            printf("未找到ID=%s的学生.无法评定奖学金.\n",tmpbuff);
            GetUserInputKey();
            continue;
        }

        USE_GUI;
        while(1)
        {
            ClearScreen();
            TITLE("评定 %s 的奖学金\n",p->Name);
            TAG(1);printf("思想品德奖\n");
            TAG(2);printf("社会实践奖\n");
            TAG(3);printf("文体优秀奖\n");
            TAG(4);printf("学习进步奖\n");
            TAG(5);printf("科技创新奖\n");
            resetcolor();printf("------------------\n");
            INFOPRINT(1,"200元/年\n");
            INFOPRINT(2,"200元/年\n");
            INFOPRINT(3,"200元/年\n");
            INFOPRINT(4,"200元/年\n");
            INFOPRINT(5,"1000~2000元/年\n");
            if(GetAction(1,5))
            {
                AchieveInfo tmp;
                tmp.score=0;
                tmp.type=20;
                tmp.score=_GetAchieveScoreByType(tmp.type);
                strncpy(tmp.StudentID,p->ID,CONFIG_STUID_SIZE);

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
                tmp.time.year=y;
                tmp.time.mon=m;
                tmp.time.day=d;
                printf("请输入您对该学生想说的话:\n");
                CLEARBUFF_READIN();
                snprintf(tmp.Message,CONFIG_ACHMSG_SIZE,"由奖学金系统手动生成. 奖学金类型: %d 奖学金级别: 单项素质奖学金 评语: %s\n",cid,strlen(tmpbuff)<1?"暂无评语":tmpbuff);
                printf("请稍等,正在保存...\n");
                if(PushBack(ls_achieve,&tmp)!=NULL)
                {
                    printf("保存成功\n");
                }
                else
                {
                    printf("保存失败\n");
                }
                break;
            }
        }
    }
}

void GDI_Scholar_Special()
{
    while(1)
    {
        ClearScreen();
        TITLE("奖学金评定-专项奖学金评定\n");
        printf("请输入学生ID,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;

        printf("请稍等,正在搜索学生信息...\n");
        StudentInfo* p=_FindStudentInfoByID(tmpbuff);
        if(p==NULL)
        {
            printf("未找到ID=%s的学生.无法评定奖学金.\n",tmpbuff);
            GetUserInputKey();
            continue;
        }

        USE_GUI;
        while(1)
        {
            ClearScreen();
            TITLE("评定 %s 的奖学金\n",p->Name);
            TAG(1);printf("优秀运动员奖学金\n");
            TAG(2);printf("定向奖学金\n");
            TAG(3);printf("其他单位或个人出资设立的奖学金\n");

            if(GetAction(1,3))
            {
                AchieveInfo tmp;
                tmp.score=0;
                tmp.type=20;
                tmp.score=_GetAchieveScoreByType(tmp.type);
                strncpy(tmp.StudentID,p->ID,CONFIG_STUID_SIZE);

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
                tmp.time.year=y;
                tmp.time.mon=m;
                tmp.time.day=d;
                printf("请输入您对该学生想说的话:\n");
                CLEARBUFF_READIN();
                snprintf(tmp.Message,CONFIG_ACHMSG_SIZE,"由奖学金系统手动生成. 奖学金类型: %d 奖学金级别: 专项奖学金 评语: %s\n",cid,strlen(tmpbuff)<1?"暂无评语":tmpbuff);
                printf("请稍等,正在保存...\n");
                if(PushBack(ls_achieve,&tmp)!=NULL)
                {
                    printf("保存成功\n");
                }
                else
                {
                    printf("保存失败\n");
                }
                break;
            }
        }
    }
}

void GDI_Scholar_Real_Maunal()
{
    while(1)
    {
        ClearScreen();
        TITLE("奖学金评定-综合奖学金评定\n");
        printf("请输入学生ID,留空返回.\n");
        CLEARBUFF_READIN();
        if(strlen(tmpbuff)<1) return;

        printf("请稍等,正在搜索学生信息...\n");
        StudentInfo* p=_FindStudentInfoByID(tmpbuff);
        if(p==NULL)
        {
            printf("未找到ID=%s的学生.无法评定奖学金.\n",tmpbuff);
            GetUserInputKey();
            continue;
        }

        USE_GUI;
        while(1)
        {
            ClearScreen();
            TITLE("评定 %s 的奖学金\n",p->Name);
            TAG(1);printf("特等奖\n");
            TAG(2);printf("一等奖\n");
            TAG(3);printf("二等奖\n");
            TAG(4);printf("三等奖\n");
            resetcolor();printf("------------------\n");
            INFOPRINT(1,"2000元/年\n");
            INFOPRINT(2,"1200元/年\n");
            INFOPRINT(3,"700元/年\n");
            INFOPRINT(4,"400元/年\n");
            if(GetAction(1,4))
            {
                AchieveInfo tmp;
                tmp.score=0;
                tmp.type=20;
                tmp.score=_GetAchieveScoreByType(tmp.type);
                strncpy(tmp.StudentID,p->ID,CONFIG_STUID_SIZE);

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
                tmp.time.year=y;
                tmp.time.mon=m;
                tmp.time.day=d;
                printf("请输入您对该学生想说的话:\n");
                CLEARBUFF_READIN();
                snprintf(tmp.Message,CONFIG_ACHMSG_SIZE,"由奖学金系统手动生成. 奖学金类型: %d 奖学金级别: 综合奖学金 评语: %s\n",cid,strlen(tmpbuff)<1?"暂无评语":tmpbuff);
                printf("请稍等,正在保存...\n");
                if(PushBack(ls_achieve,&tmp)!=NULL)
                {
                    printf("保存成功\n");
                }
                else
                {
                    printf("保存失败\n");
                }
                break;
            }
        }
    }
}

void GDI_Scholar_Real_Auto()
{
    ClearScreen();
    TITLE("奖学金评定-综合奖学金评定\n");
    printf("正跳转到手动模式...\n");
    GDI_Scholar_Real_Maunal();
}

void GDI_Scholar_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("奖学金自动评定系统\n");
        TAG(1);printf("评选校长奖学金(手动)\n");
        TAG(2);printf("单项素质奖学金评定(手动)\n");
        TAG(3);printf("专项奖学金评定(手动)\n");
        TAG(4);printf("综合奖学金评定(自动)\n");
        TAG(5);printf("综合奖学金评定(手动)\n");
        TAG(6);printf("返回\n");
        resetcolor();printf("----------------------\n");
        INFOPRINT(1,"手动评定“校长奖学金”\n");
        INFOPRINT(2,"手动评定单项素质奖学金\n");
        INFOPRINT(3,"手动评定专项奖学金\n");
        INFOPRINT(4,"自动评定综合奖学金\n");
        INFOPRINT(5,"手动评定综合奖学金\n");
        INFOPRINT(6,"返回上一菜单\n");

        if(GetAction(1,6))
        {
            GUI_LIST
            {
            case 1:
                GDI_Scholar_MasterLove();
                break;
            case 2:
                GDI_Scholar_SingleStuff();
                break;
            case 3:
                GDI_Scholar_Special();
                break;
            case 4:
                GDI_Scholar_Real_Auto();
                break;
            case 5:
                GDI_Scholar_Real_Maunal();
                break;
            case 6:
                return;
            }
        }
    }
}
