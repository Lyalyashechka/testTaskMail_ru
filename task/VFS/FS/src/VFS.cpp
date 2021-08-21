#include "VFS.h"

namespace TestTask
{
    VFS::VFS(std::filesystem::path locationFileSystemDirectory) :
                             ManagerMeta_(std::make_unique<ManagerMeta>(locationFileSystemDirectory))
    {
        std::string root = "";
        if (locationFileSystemDirectory.string() != "")
        {
            root = locationFileSystemDirectory.string() + "/";
        }
        initFilesystemStructure(root);
        rootDirectory_ = root + NameRootFileSystemDirectory;
        metaDirectory_ = root + NameMetaInfoFilesDirectoriy;
    }

    File *VFS::Open(const char *name)
    {
        if (numberOfFilesInDirectory(metaDirectory_) == 0)
        {
            return nullptr;
        }
        else
        {
            File* currentFile = ManagerMeta_->findFile(name, statusFile::readOnly_);

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
        std::fstream fileWithData;
        std::cout << rootDirectory_.string();
        fileWithData.open(rootDirectory_.string() + NameFileData);
        if (fileWithData.is_open())
        {
            std::cout << "File write open"<<std::endl;
            fileWithData.seekg(f->numberFirstChunk * SizeChunk, fileWithData.beg);
            std::cout << fileWithData.tellg() << "tellp\n";
            //fileWithData.write(buff, 1024);
            fileWithData << buff;
            fileWithData.close();
        }
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
        size_t countFilesAndCountChunk = 0;
        file.write(reinterpret_cast<char *>(&countFilesAndCountChunk), sizeof(size_t));
        file.write(reinterpret_cast<char *>(&countFilesAndCountChunk), sizeof(size_t));
        file.close();
    }
}