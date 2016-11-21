int _Do_Save(int show)
{
    if(show)
    {
        printf("保存学生信息...\n");
    }
    FILE* fp=_DiskIO_OpenWriteFile("_autosave_student.bin",1);
    if(fp==NULL)
    {
        if(show) printf("保存学生信息失败: 无法打开文件.\n");
        return -1;
    }
    _DiskIO_VecToFile(fp,ls_student);
    _DiskIO_CloseFile(fp);

    if(show) printf("保存课程信息...\n");
    fp=_DiskIO_OpenWriteFile("_autosave_course.bin",1);
    if(fp==NULL)
    {
        if(show) printf("保存课程信息失败: 无法打开文件.\n");
        return -2;
    }
    _DiskIO_VecToFile(fp,ls_course);
    _DiskIO_CloseFile(fp);

    if(show) printf("保存成绩信息...\n");
    fp=_DiskIO_OpenWriteFile("_autosave_score.bin",1);
    if(fp==NULL)
    {
        if(show) printf("保存成绩信息失败: 无法打开文件.\n");
        return -3;
    }
    _DiskIO_VecToFile(fp,ls_score);
    _DiskIO_CloseFile(fp);

    if(show) printf("保存奖惩信息...\n");
    fp=_DiskIO_OpenWriteFile("_autosave_achieve.bin",1);
    if(fp==NULL)
    {
        if(show) printf("保存成就信息失败: 无法打开文件.\n");
        return -4;
    }
    _DiskIO_VecToFile(fp,ls_achieve);
    _DiskIO_CloseFile(fp);

    if(show) printf("保存完成.\n");
    return 0;
}

int _Do_Load(int show)
{
    if(show)
    {
        printf("载入学生信息...\n");
    }
    FILE* fp=_DiskIO_OpenReadFile("_autosave_student.bin");
    if(fp!=NULL)
    {
        _DiskIO_LoadVec(fp,ls_student);
        _DiskIO_CloseFile(fp);
    }

    if(show) printf("载入课程信息...\n");
    fp=_DiskIO_OpenReadFile("_autosave_course.bin");
    if(fp!=NULL)
    {
        _DiskIO_LoadVec(fp,ls_course);
        _DiskIO_CloseFile(fp);
    }

    if(show) printf("载入成绩信息...\n");
    fp=_DiskIO_OpenReadFile("_autosave_score.bin");
    if(fp!=NULL)
    {
        _DiskIO_LoadVec(fp,ls_score);
        _DiskIO_CloseFile(fp);
    }


    if(show) printf("载入奖惩信息...\n");
    fp=_DiskIO_OpenReadFile("_autosave_achieve.bin");
    if(fp==NULL)
    {
        _DiskIO_LoadVec(fp,ls_achieve);
        _DiskIO_CloseFile(fp);
    }

    if(show) printf("载入完成.\n");
    return 0;
}

void GDI_Export_SaveNow()
{
    ClearScreen();
    TITLE("保存数据\n");
    printf("正在启动数据保存...\n");
    if(_Do_Save(1)<0)
    {
        printf("保存错误.\n");
        GetUserInputKey();
        return;
    }
    printf("保存完毕.\n");
}

void GDI_Export_ExportTo()
{
    ClearScreen();
    TITLE("导出数据文件\n");
    printf("请输入文件名\n");
    CLEARBUFF_READIN();
    if(strstr(tmpbuff,".bin")==NULL) strcat(tmpbuff,".bin");
    printf("正在保存...\n");

    FILE* fp=_DiskIO_OpenWriteFile(tmpbuff,1);
    if(fp==NULL)
    {
        printf("保存错误: 无法写入%s\n",tmpbuff);
        GetUserInputKey();
        return;
    }
    _DiskIO_VecToFile(fp,ls_student);
    _DiskIO_VecToFile(fp,ls_course);
    _DiskIO_VecToFile(fp,ls_score);
    _DiskIO_VecToFile(fp,ls_achieve);

    _DiskIO_CloseFile(fp);
    printf("保存成功.\n");
    GetUserInputKey();
}

void GDI_Export_Import()
{
    ClearScreen();
    TITLE("导入数据文件\n");
    WARNING("导入数据文件可能导致当前数据丢失.\n");


    printf("请输入文件名\n");
    CLEARBUFF_READIN();
    if(strstr(tmpbuff,".bin")==NULL) strcat(tmpbuff,".bin");
    printf("正在导入...\n");

    FILE* fp=_DiskIO_OpenReadFile(tmpbuff);
    if(fp==NULL)
    {
        printf("保存错误: 无法打开%s\n",tmpbuff);
        GetUserInputKey();
        return;
    }
    ClearList(ls_student);
    ClearList(ls_course);
    ClearList(ls_score);
    ClearList(ls_achieve);

    _DiskIO_LoadVec(fp,ls_student);
    _DiskIO_LoadVec(fp,ls_course);
    _DiskIO_LoadVec(fp,ls_score);
    _DiskIO_LoadVec(fp,ls_achieve);

    _DiskIO_CloseFile(fp);
    printf("保存成功.\n");
    GetUserInputKey();

}

void GDI_Export_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("本地导入/导出\n");
        TAG(1);printf("立即保存\n");
        TAG(2);printf("导出到...\n");
        TAG(3);printf("导入...\n");
        TAG(4);printf("返回\n");
        resetcolor();printf("--------------\n");
        INFOPRINT(1,"立即保存数据到磁盘中.\n");
        INFOPRINT(2,"导出数据到指定磁盘文件,该文件可在其他客户端上导入\n");
        INFOPRINT(3,"从磁盘文件导入数据到系统中.\n");
        INFOPRINT(4,"返回到上一菜单.\n");
        if(GetAction(1,4))
        {
            GUI_LIST
            {
            case 1:
                GDI_Export_SaveNow();
                break;
            case 2:
                GDI_Export_ExportTo();
                break;
            case 3:
                GDI_Export_Import();
                break;
            case 4:
                return;
            }
        }
    }
}
