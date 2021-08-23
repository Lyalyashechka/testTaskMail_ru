#include "VFS.h"
#include "ManagerMeta.h"

int main()
{
    std::unique_ptr<TestTask::IVFS> IVFSPtr(new TestTask::VFS());
    TestTask::File *first = IVFSPtr->Create("perviyFile");
    TestTask::File *second = IVFSPtr->Create("vtoroyFile");
    char buf[10] = {"012345678"};

    IVFSPtr->Write(first, buf, 10);
    IVFSPtr->Write(second, buf, 10);
    IVFSPtr->Write(first, buf, 10);
    char *bufNew = new char[1024];
    for (int i = 0; i < 1024; ++i)
    {
        bufNew[i] = 'a';
    }
    IVFSPtr->Write(first, bufNew, 1024);
    IVFSPtr->Write(first, bufNew, 1024);
    IVFSPtr->Write(second, bufNew, 1024);
    char *bufRead = new char[1080];
    IVFSPtr->Close(second);
    second = IVFSPtr->Open("vtoroyFile");
    std::cout << IVFSPtr->Read(second, bufRead, 1080) << " readed"<<std::endl;;
    /*for (int i = 0; i < 1080; ++i)
    {
        std::cout << bufRead[i];
    }*/
    //std::cout << "Hello world\n";
    delete bufNew;
    delete bufRead;
}