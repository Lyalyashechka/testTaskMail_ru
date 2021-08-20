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
    }

    File *VFS::Open(const char *name)
    {
        std::cout << numberOfFilesInDirectory(metaDirectory_) << std::endl;
        if (numberOfFilesInDirectory(metaDirectory_) == 0)
        {
            return nullptr;
        }
        else
        {
            std::ifstream metaFile;
            metaFile.open(metaDirectory_.string() + "/meta1.txt", std::ios_base::binary);
        }
        return nullptr;
    }

    File *VFS::Create(const char *name)
    {
        if (numberOfFilesInDirectory(metaDirectory_) == 0)
        {
            initMetaDataFile();
        }
        std::fstream metaFile;

        metaFile.open(metaDirectory_.string() + "/meta1.bin",
                      std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        if (metaFile.is_open())
        {
            size_t countFiles;
            metaFile.read(reinterpret_cast<char *>(&countFiles), sizeof(size_t));
            
            /*if (ManagerMeta::findFile())
            {
                return FIle;
            }
            else{
                ManagerMeta::createFile())
            }*/
            std::cout << countFiles;
            metaFile.seekg(0, metaFile.beg);
            countFiles++;
            metaFile.write(reinterpret_cast<char *>(&countFiles), sizeof(size_t));
            metaFile.close();
        }
        return nullptr;
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
        file.open(metaDirectory_.string() + "/meta1.bin", std::ios_base::binary);
        size_t countFiles = 0;
        file.write(reinterpret_cast<char *>(&countFiles), sizeof(size_t));
        file.close();
    }
}