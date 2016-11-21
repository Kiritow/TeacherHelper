/**
***************************************************************
*   CPPLIB - Dynamic Vector (C++)
*   Rewritten in Pure C. (By HC TECH)
*/
typedef struct
{
    void* pdata;
    int typesz;
    int maxsz;
    int used;
}DynamicVec;

typedef DynamicVec* DYHANDLE;

DYHANDLE GetList(int TypeSize,int MaxSize)
{
    DYHANDLE h=(DYHANDLE)malloc(sizeof(DynamicVec));
    if(!h)return NULL;
    h->typesz=TypeSize;
    h->maxsz=MaxSize;
    h->used=0;
    h->pdata=malloc((h->typesz)*(h->maxsz));
    if(!(h->pdata))
    {
        free(h);
        return NULL;
    }
    memset(h->pdata,0,(h->typesz)*(h->maxsz));
    return h;
}
void ClearList(DYHANDLE handle)
{
    if(!handle||!handle->pdata) return;
    handle->used=0;
    memset(handle->pdata,0,handle->typesz*handle->maxsz);
}
void FreeList(DYHANDLE Handle)
{
    if(Handle->pdata) free(Handle->pdata);
    Handle->maxsz=0;
    Handle->used=0;
    free(Handle);
}
void* GetRawData(DYHANDLE Handle)
{
    return Handle->pdata;
}
void* GetMember(DYHANDLE Handle,int index)
{
    if(!Handle||index<0||index>=Handle->used)
    {
        return NULL;
    }
    return (char*)(Handle->pdata)+(Handle->typesz)*index;/// Use CHAR as 1 bit
}
int GetListSize(DYHANDLE Handle)
{
    if(!Handle) return -1;
    return Handle->used;
}
int GetListMaxSize(DYHANDLE Handle)
{
    if(!Handle) return -1;
    return Handle->maxsz;
}
DYHANDLE TryMoreList(DYHANDLE Handle)
{
    void* pnewdata=malloc((Handle->typesz)*((Handle->maxsz)*2));
    if(!pnewdata)
    {
        return NULL;
    }
    memcpy(pnewdata,Handle->pdata,(Handle->typesz)*(Handle->used));
    free(Handle->pdata);
    Handle->pdata=pnewdata;
    Handle->maxsz*=2;
    return Handle;
}
void* SetMember(DYHANDLE Handle,int index,const void* pMem)
{
    if(!Handle||index<0||!pMem)
    {
        return NULL;
    }
    if(index>=(Handle->maxsz))
    {
        if(TryMoreList(Handle))
        {
            return SetMember(Handle,index,pMem);
        }
        else
        {
            return NULL;
        }
    }
    if(index>=Handle->used)
    {
        Handle->used=index+1;
    }
    memcpy((char*)(Handle->pdata)+(Handle->typesz)*index,pMem,Handle->typesz);
    return Handle;
}
void* PushBack(DYHANDLE Handle,const void* pMem)
{
    if(!Handle||!pMem)
    {
        return NULL;
    }
    if(Handle->used==Handle->maxsz)
    {
        if(!TryMoreList(Handle))
        {
            return NULL;
        }
    }
    SetMember(Handle,Handle->used,pMem);
    return Handle;
}

void* EraseSingle(DYHANDLE Handle,int index)
{
    if(!Handle||index>=Handle->used||index<0) return NULL;
    int i;
    for(i=index;i<((Handle->used)-1);i++)
    {
        SetMember(Handle,i,GetMember(Handle,i+1));
    }
    Handle->used=(Handle->used)-1;
    return Handle;
}

void _dynamicvec_init()
{
    printf("\n");
    printf("-----------------\n"
           "Dynamic Vector\n"
           "A Dynamic Memory Manager.\n"
           "Created By Kiritow.\n"
           "Version: V0.3x (x means a OpenSource Version)\n"
           "-----------------\n"
           );
}


/** V0.4x Update: AutoRelease.
*   A C++ Class-like Memory Manager.
*Updates:
*V0.3x --> V0.4x : Release Function Pointer Added.
*/

#define CONFIG_AUTORELEASE_STACK_MAX 64

typedef void (*RFUNC)(DYHANDLE);

typedef struct
{
    DYHANDLE todo[CONFIG_AUTORELEASE_STACK_MAX];
    int counter;
    RFUNC func;
}AutoRelease;

AutoRelease GetAutoReleaser()
{
    AutoRelease a;
    a.counter=0;
    a.func=NULL;
    return a;
}

void AddRelease(AutoRelease* _auto_releaser,DYHANDLE handle_to_auto_release)
{
    _auto_releaser->todo[_auto_releaser->counter++]=handle_to_auto_release;
}
int PopRelease(AutoRelease* _auto_releaser)
{
    if(_auto_releaser->counter>0)
    {
        if(_auto_releaser->func)
        {
            _auto_releaser->func(_auto_releaser->todo[_auto_releaser->counter-1]);
        }
        FreeList(_auto_releaser->todo[_auto_releaser->counter-1]);
        -- _auto_releaser->counter;
        return 0;
    }
    return -1;
}
void DoRelease(AutoRelease* _auto_releaser)
{
    while(PopRelease(_auto_releaser)>=0);
}

#define USE_RELEASE AutoRelease _internal_releaser=GetAutoReleaser()
#define ADD_RELEASE(hand) AddRelease(&_internal_releaser,hand)
#define POP_RELEASE() PopRelease(&_internal_releaser)
#define DO_RELEASE DoRelease(&_internal_releaser)
