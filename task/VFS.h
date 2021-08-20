#include <IVFS.h>
namespace TestTask
{
    class VFS : public IVFS
    {
    public:
        VFS()
        {
            
        }
        File *Open(const char *name);
        File *Create(const char *name);
        size_t Read(File *f, char *buff, size_t len);
        size_t Write(File *f, char *buff, size_t len);
        void Close(File *f);
    };
}