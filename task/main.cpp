#include "VFS.h"

int main()
{
    TestTask::IVFS* fileSystem = new TestTask::VFS();
    fileSystem->Create("sdfsgdf");
    std::cout<<"Hello world\n";
}