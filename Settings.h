void _Load_Settings()
{
    ///Load Witness
    FILE* fp=fopen("_auto_witness","r");
    if(fp!=NULL)
    {
        _global_flag_is_first_use=0;
        fclose(fp);
    }
    else
    {
        _global_flag_is_first_use=1;
        fp=fopen("_auto_witness","w");
        fclose(fp);
    }
    _settings_use_fast_sort=1;
    _settings_use_scroll=0;
    _settings_use_log=0;
}
void GDI_Settings_SortMode()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("选择排序模式\n");
        TAG(1);printf("启用快速排序\n");
        TAG(2);printf("关闭快速排序\n");
        TAG(3);printf("返回\n");
        resetcolor();printf("-----------------\n");
        INFOPRINT(1,"启动快速排序,排序速度显著加快,但对于有缓存的数据会引起缓存的失效与更新.\n");
        INFOPRINT(2,"关闭快速排序,排序速度降低.\n");
        INFOPRINT(3,"不修改设置,返回上一菜单\n");

        if(GetAction(1,3))
        {
            GUI_LIST
            {
                case 3:return;
                default:
                {
                    _settings_use_fast_sort=2-cid;
                }
            }
            break;
        }
    }
}
void GDI_Settings_ScrollMode()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("选择滚动模式\n");
        TAG(1);printf("启用滚动浏览\n");
        TAG(2);printf("关闭滚动浏览\n");
        TAG(3);printf("返回\n");
        resetcolor();printf("-----------------\n");
        INFOPRINT(1,"启动滚动浏览,使用方向键操作\n");
        INFOPRINT(2,"关闭滚动浏览.\n");
        INFOPRINT(3,"不修改设置,返回上一菜单\n");

        if(GetAction(1,3))
        {
            GUI_LIST
            {
                case 3:return;
                default:
                {
                    _settings_use_scroll=0;/// OFF
                }
            }
            break;
        }
    }
}

void GDI_Settings_Log()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("设置-日志\n");
        TAG(1);printf("开启日志功能\n");
        TAG(2);printf("关闭日志功能\n");
        TAG(3);printf("返回\n");
        resetcolor();printf("-----------------\n");
        INFOPRINT(1,"启动日志记录操作\n");
        INFOPRINT(2,"关闭日志记录操作.\n");
        INFOPRINT(3,"不修改设置,返回上一菜单\n");

        if(GetAction(1,3))
        {
            GUI_LIST
            {
                case 3:return;
                default:
                {
                    _settings_use_log=0;/// OFF
                }
            }
            break;
        }
    }
}

void GDI_Settings_Terminal()
{
    ClearScreen();
    TITLE("系统控制台-口令\n");
    printf("请输入调试入口口令\n");
    CLEARBUFF_READIN();
    printf("口令错误.\n");
    printf("按任意键继续\n");
    GetUserInputKey();
}

void _SaveCode(const char* code)
{
    FILE* fp=fopen("LocalPass.config","w");
    fprintf(fp,"%s",code);
    fclose(fp);
}

void GDI_Settings_StartLock()
{
    ClearScreen();
    TITLE("启动锁参数设置\n");
    printf("请输入启动密码,留空关闭启动锁\n");
    CLEARBUFF_READIN();
    _SaveCode(tmpbuff);
    printf("保存完成.\n");
    GetUserInputKey();
}

void GDI_Settings_MainPad()
{
    USE_GUI;
    while(1)
    {
        ClearScreen();
        TITLE("设置\n");
        TAG(1);printf("排序模式设置\n");
        TAG(2);printf("滚动模式设置\n");
        TAG(3);printf("日志系统设置\n");
        TAG(4);printf("启动锁设置\n");
        TAG(5);printf("系统控制台\n");
        TAG(6);printf("返回\n");
        resetcolor();printf("---------------------\n");
        INFOPRINT(1,"设置排序的模式\n");
        INFOPRINT(2,"设置界面滚动显示的模式\n");
        INFOPRINT(3,"设置日志系统相关参数\n");
        INFOPRINT(4,"设置启动锁相关参数\n");
        INFOPRINT(5,"进入核心系统控制台,调试用\n");
        INFOPRINT(6,"返回上一菜单\n");
        if(GetAction(1,6))
        {
            GUI_LIST
            {
            case 1:
                GDI_Settings_SortMode();
                break;
            case 2:
                GDI_Settings_ScrollMode();
                break;
            case 3:
                GDI_Settings_Log();
                break;
            case 4:
                GDI_Settings_StartLock();
                break;
            case 5:
                GDI_Settings_Terminal();
                break;
            case 6:
                return;
            }
        }
    }
}
