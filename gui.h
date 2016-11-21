int _last_keyval;
int _GetUserInputKey_Real()
{
    return _last_keyval=getch();///defined in conio.h
}

#define KEY_UNDEF 0
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_CONFIRM 5
#define KEY_ESC 6
#define KEY_HOME 7
#define KEY_END 8

/** Update! Now you can change key in settings
*   Update! Key: Esc, Home, End added.
*/
int _last_isSpecial;

int GetUserInputKey()
{
    int a=_GetUserInputKey_Real();
    if(a==224||a==0)
    {
        _last_isSpecial=1;
        int b=_GetUserInputKey_Real();
        switch(b)
        {
            case 72:return KEY_UP;
            case 80:return KEY_DOWN;
            case 75:return KEY_LEFT;
            case 77:return KEY_RIGHT;
            case 71:return KEY_HOME;
            case 79:return KEY_END;
            default:return KEY_UNDEF;
        }
    }
    else
    {
        _last_isSpecial=0;
        switch(a)
        {
            case ' ':case '\r':case '\n':case '5':
            return KEY_CONFIRM;
            case 'w':case 'W':case '8':
            return KEY_UP;
            case 's':case 'S':case '2':
            return KEY_DOWN;
            case 'a':case 'A':case '4':
            return KEY_LEFT;
            case 'd':case 'D':case '6':
            return KEY_RIGHT;
            case 27:
            return KEY_ESC;

            default:
            return KEY_UNDEF;
        }
    }
}



void KeyCheckLoop()
{
    while(1)
    {
        int c=GetUserInputKey();
        switch(c)
        {
        case KEY_UNDEF:
            printf("Undefined.\n");
            break;
        case KEY_UP:
            printf("Up.\n");
            break;
        case KEY_DOWN:
            printf("Down.\n");
            break;
        case KEY_LEFT:
            printf("Left.\n");
            break;
        case KEY_RIGHT:
            printf("Right.\n");
            break;
        case KEY_CONFIRM:
            printf("Confirm.\n");
            break;
        case KEY_ESC:
            printf("Escape.\n");
            break;
        case KEY_HOME:
            printf("Home.\n");
            break;
        case KEY_END:
            printf("End.\n");
            break;
        }
    }
}

/**************Color System**************/
/** This Color System is part of CPPLIB#GCOLOR project.
*   Rewritten in pure C.
*   HC TECH 2016
*/
CONSOLE_CURSOR_INFO _CPPLIB_GCOLOR_OINFO;
HANDLE _CPPLIB_GCOLOR_WIN_HOUT;
WORD _CPPLIB_GCOLOR_ORIGINAL_COLOR;
COORD _CPPLIB_GCOLOR_COORD;

void _internal_color_system_init()
{
    _CPPLIB_GCOLOR_WIN_HOUT=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO tInfo;
    GetConsoleScreenBufferInfo(_CPPLIB_GCOLOR_WIN_HOUT,&tInfo);
    _CPPLIB_GCOLOR_ORIGINAL_COLOR=tInfo.wAttributes;
    GetConsoleCursorInfo(_CPPLIB_GCOLOR_WIN_HOUT,&_CPPLIB_GCOLOR_OINFO);
}
#define hout _CPPLIB_GCOLOR_WIN_HOUT
#define oldcolor _CPPLIB_GCOLOR_ORIGINAL_COLOR
#define oinfo _CPPLIB_GCOLOR_OINFO
#define internal_cursor_info _CPPLIB_GCOLOR_COORD

void gotoxy(int x,int y)
{
    COORD coord;
    coord.X=x-1;
    coord.Y=y-1;
    SetConsoleCursorPosition(hout,coord);
};
enum
{
    black=0,red=1,green=2,yellow=3,blue=4,purple=5,lightblue=6,deepgreen=6,white=7
};
void resetcolor()
{
    SetConsoleTextAttribute(hout,oldcolor);
}
void cprint(int FrontColor,int BackColor)
{
    int iFront;
    int iBack;
    switch(FrontColor)
    {
    case black:
        iFront=0;break;
    case blue:
        iFront=FOREGROUND_BLUE;break;
    case lightblue:
        iFront=FOREGROUND_GREEN | FOREGROUND_BLUE;break;
    case green:
        iFront=FOREGROUND_GREEN;break;
    case purple:
        iFront=FOREGROUND_RED |FOREGROUND_BLUE;break;
    case red:
        iFront=FOREGROUND_RED;break;
    case white:
        iFront=FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; break;
    case yellow:
        iFront=FOREGROUND_RED | FOREGROUND_GREEN;break;
    }
    switch(BackColor)
    {
    case black:
        iBack=0;break;
    case blue:
        iBack=BACKGROUND_BLUE;break;
    case lightblue:
        iBack=BACKGROUND_GREEN | BACKGROUND_BLUE;break;
    case green:
        iFront=BACKGROUND_GREEN;break;
    case purple:
        iBack=BACKGROUND_RED | BACKGROUND_BLUE;break;
    case red:
        iBack=BACKGROUND_RED;break;
    case white:
        iBack=BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; break;
    case yellow:
        iBack=BACKGROUND_RED | BACKGROUND_GREEN;break;
    }
    if(iFront!=0) iFront=iFront | FOREGROUND_INTENSITY;
    if(iBack!=0) iBack=iBack| BACKGROUND_INTENSITY;
    SetConsoleTextAttribute(hout,iFront | iBack );
}
void ClearScreen()
{
    resetcolor();
    system("cls");
}

int GetAction_WS(int* p,int MinVal,int MaxVal)
{
    int t=GetUserInputKey();
    switch(t)
    {
    case KEY_UP:
        {
            --(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_DOWN:
        {
            ++(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_CONFIRM:
        return 1;
    default:
        return 0;
    }
}

int GetAction_WS_E(int* p,int MinVal,int MaxVal,int EscapeVal)
{
    int t=GetUserInputKey();
    switch(t)
    {
    case KEY_UP:
        {
            --(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_DOWN:
        {
            ++(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_ESC:
        {
            *p=EscapeVal;
            return 0;
        }
    case KEY_CONFIRM:
        return 1;
    default:
        return 0;
    }
}
typedef int (*IACTION)(int isControl,int Key,int* cid);
int GetAction_WS_E_X(int* p,int MinVal,int MaxVal,int EscapeVal,IACTION FunctionPointer)
{
    int t=GetUserInputKey();
    switch(t)
    {
    case KEY_UP:
        {
            --(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_DOWN:
        {
            ++(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_ESC:
        {
            *p=EscapeVal;
            return 0;
        }
    case KEY_CONFIRM:
        return 1;
    default:
        if(FunctionPointer)
        {
            return FunctionPointer(_last_isSpecial,t,p);
        }
        else
        {
            return 0;
        }
    }
}


#define USE_GUI int cid=1;
#define GUI_LIST switch(cid)
#define TITLE(FormatString,Args...) do{cprint(black,lightblue);printf(FormatString,##Args);resetcolor();}while(0)
#define WARNING(FormatString,Args...) do{cprint(yellow,black);printf(FormatString,##Args);resetcolor();}while(0)
#define CONFIRM _ABOX_Confirm
#define TAG(tagID) do{if(cid==tagID)cprint(black,yellow);else cprint(white,black);}while(0)
#define TAGPRINT(tagID,FormatString,Args...) do{TAG(tagID);printf(FormatString,##Args);}while(0)
#define INFOPRINT(tagID,FormatString,Args...) do{if(tagID==cid) printf(FormatString,##Args);}while(0)
#define INFODO(tagID,USG) do{if(tagID==cid) {USG} }while(0)
//#define GetAction(cid,minVal,maxVal) GetAction_WS(&cid,minVal,maxVal)
//#define GetAction(cid,minVal,maxVal) GetAction_WS_E(&cid,minVal,maxVal,maxVal)
#define GetAction(minVal,maxVal) GetAction_WS_E_X(&cid,minVal,maxVal,maxVal,NULL)
#define GetActionX(minVal,maxVal,FunctionPointer) GetAction_WS_E_X(&cid,minVal,maxVal,maxVal,FunctionPointer)
#define GetActionEX(minVal,maxVal,escVal,FunctionPointer) GetAction_WS_E_X(&cid,minVal,maxVal,escVal,FunctionPointer)
#define ClearInput() fflush(stdin)


int _ABOX_Confirm(const char* fmt,...)
{
    cprint(yellow,black);
    va_list vlist;
    va_start(vlist,fmt);
    vprintf(fmt,vlist);
    va_end(vlist);
    ClearInput();
    return (GetUserInputKey()==KEY_CONFIRM);
}
