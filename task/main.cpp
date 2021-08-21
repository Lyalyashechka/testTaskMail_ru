#include "VFS.h"
#include "ManagerMeta.h"

int main()
{
    std::unique_ptr<TestTask::IVFS> IVFSPtr(new TestTask::VFS());
    IVFSPtr->Create("vtoroyFile");
    std::cout << "Hello world\n";
}