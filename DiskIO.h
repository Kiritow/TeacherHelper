
/// Override: 0 Not Allowed. 1 Override. 2 Append
FILE* _DiskIO_OpenWriteFile(const char* File,int Mode)
{
    FILE* fp;
    if(Mode==2)
    {
        return fopen(File,"wb");
    }
    fp=fopen(File,"rb");
    if(fp)
    {
        fclose(fp);
        if(Mode==0)
        {
            return NULL;
        }
    }
    fp=fopen(File,"wb");
    return fp;
}

FILE* _DiskIO_OpenReadFile(const char* File)
{
    return fopen(File,"rb");
}
int _DiskIO_CloseFile(FILE* fp)
{
    return fclose(fp);
}
int _DiskIO_VecToFile(FILE* fp,DYHANDLE handle)
{
    if(!fp||!handle) return -2;/// NULL Handle
    int sz=GetListSize(handle);
    int done=0;
    fwrite(&sz,sizeof(sz),1,fp);
    while(done<sz)
    {
        int ret=fwrite(handle->pdata,handle->typesz,sz-done,fp);
        if(ret<0)
        {
            done=-1;
            break;
        }
        done+=ret;
    }
    if(done<0)
    {
        return -4;/// Error While Writing to File.
    }

    return 0;
}

int _DiskIO_LoadVec(FILE* fp,DYHANDLE handle)
{
    if(!fp||!handle) return -2;/// NULL Handle or Pointer

    void* tmpmem=malloc(handle->typesz);
    if(tmpmem==NULL) return -3;/// Out of Memory.
    int sz;
    int ret=fread(&sz,sizeof(int),1,fp);
    if(ret<1)
    {
        fclose(fp);
        return -4;/// Error While Read From File
    }
    for(int i=0;i<sz;i++)
    {
        ret=fread(tmpmem,handle->typesz,1,fp);
        if(ret<1)
        {
            fclose(fp);
            return -4;/// Error While Read From File
        }
        if(PushBack(handle,tmpmem)==NULL)
        {
            fclose(fp);
            return -5;/// Error While Pushing Back
        }
    }

    return 0;
}

