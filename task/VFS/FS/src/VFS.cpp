#include "VFS.h"

namespace TestTask
{
    VFS::VFS(std::filesystem::path locationFileSystemDirectory) 
    {
        std::string root = "";
        if (locationFileSystemDirectory.string() != "")
        {
            root = locationFileSystemDirectory.string() + "/";
        }
        initFilesystemStructure(root);
        rootDirectory_ = root + NameRootFileSystemDirectory;
        metaDirectory_ = root + NameMetaInfoFilesDirectoriy;
        ManagerMeta_ = std::make_unique<ManagerMeta>();
    }

    File *VFS::Open(const char *name)
    {
        if (numberOfFilesInDirectory(metaDirectory_) == 0)
        {
            return nullptr;
        }
        else
        {
            return ManagerMeta_->findFile(name, statusFile::readOnly_);            
        }
    }

    File *VFS::Create(const char *name)
    {
        if (numberOfFilesInDirectory(metaDirectory_) == 0)
        {
            initMetaDataFile();
        }
        File *currentFIle = ManagerMeta_->findFile(name, statusFile::writeOnly_);
        if (currentFIle == nullptr)
        {
            std::cout << "File create from VFS" << std::endl;
            return ManagerMeta_->addFile(name);
        }
        else
        {
            std::cout << "File found" << std::endl;
            return new File(*currentFIle);
        }
    }

    size_t VFS::Read(File *f, char *buff, size_t len)
    {
        return 0;
    }

    size_t VFS::Write(File *f, char *buff, size_t len)
    {
        return 0;
    }

    void VFS::Close(File *f)
    {
        //ManagerMeta::setStatus(default)
    }

    void VFS::initFilesystemStructure(const std::string &locationFileSystemDirectory)
    {
        std::error_code ec;
        std::filesystem::create_directories(locationFileSystemDirectory + NameMetaInfoFilesDirectoriy, ec);
        if (ec)
        {
            std::cerr << "Error: create MetaFilesInfo" << std::endl;
            exit(1);
        }
    }

    size_t VFS::numberOfFilesInDirectory(std::filesystem::path path)
    {
        return std::distance(std::filesystem::directory_iterator(path),
                             std::filesystem::directory_iterator{});
    }

    void VFS::initMetaDataFile()
    {
        std::ofstream file;
        file.open(metaDirectory_.string() + NameFileMetaInfo, std::ios_base::binary);
        size_t countFiles = 0;
        file.write(reinterpret_cast<char *>(&countFiles), sizeof(size_t));
        file.close();
    }
}