#include "VFS.h"
#include "ManagerMeta.h"

int main()
{
    std::unique_ptr<TestTask::IVFS> IVFSPtr(new TestTask::VFS());
    TestTask::File* first = IVFSPtr->Create("perviyFile");
    TestTask::File* second = IVFSPtr->Create("vtoroyFile");
    char* buf = new char[1024];
    for (int i = 0; i < 1023; ++i)
    {
        buf[i] = ' ';
    }
    for (size_t i = 0; i < 10; i++)
    {
        buf[i] = '1';
    }
    
    IVFSPtr->Write(first, buf, 10);
    IVFSPtr->Write(second, buf, 10);
    std::cout << "Hello world\n";
}