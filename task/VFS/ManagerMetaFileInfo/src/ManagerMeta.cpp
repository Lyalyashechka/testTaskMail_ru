#include "ManagerMeta.h"

namespace TestTask
{
    ManagerMeta::ManagerMeta(std::filesystem::path locationFileSystemDirectory)
    {
        std::string root = "";
        if (locationFileSystemDirectory.string() != "")
        {
            root = locationFileSystemDirectory.string() + "/";
        }
        metaDirectory_ = root + NameMetaInfoFilesDirectoriy;
    }

    File *ManagerMeta::findFile(const char *name, statusFile setStatusIfFind)
    {
        size_t countFiles = getCountFile();
        std::fstream metaFile;
        metaFile.open(metaDirectory_.string() + NameFileMetaInfo,
                      std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        if (metaFile.is_open())
        {
            metaFile.seekg(2 * sizeof(size_t), metaFile.beg);
            for (int i = 0; i < countFiles; ++i)
            {
                File currentFile;
                metaFile.read(reinterpret_cast<char *>(&currentFile.lenFileName), sizeof(size_t));
                char *buf = new char[currentFile.lenFileName + 1];
                metaFile.read(reinterpret_cast<char *>(buf), currentFile.lenFileName);
                buf[currentFile.lenFileName] = 0;
                currentFile.fileName = buf;
                int status;
                metaFile.read(reinterpret_cast<char *>(&status), sizeof(int));
                currentFile.status = static_cast<statusFile>(status);
                metaFile.read(reinterpret_cast<char *>(&currentFile.numberFirstChunk), sizeof(size_t));
                if (currentFile.fileName == name)
                {
                    //setStatusFile();
                    return new File(currentFile);
                }
            }
        }
        return nullptr;
    }

    void ManagerMeta::setStatusFile(statusFile status)
    {
        std::cout << "ya ne bespolezniy" << std::endl;
    }

    File *ManagerMeta::addFile(const char *name)
    {
        File currentFile;
        currentFile.setFileName(name);
        currentFile.status = statusFile::writeOnly_;
        currentFile.numberFirstChunk = getCountChunk();
        writeMetaInfo(currentFile);
        return new File(currentFile);
    }

    void ManagerMeta::writeMetaInfo(File &file)
    {
        std::fstream metaFile;
        metaFile.open(metaDirectory_.string() + NameFileMetaInfo,
                      std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        if (metaFile.is_open())
        {
            metaFile.seekg(0, metaFile.end);
            metaFile.write(reinterpret_cast<char *>(&file.lenFileName), sizeof(size_t));
            metaFile.write(reinterpret_cast<char *>(file.fileName.data()), file.lenFileName);
            int statFile = static_cast<int>(file.status);
            metaFile.write(reinterpret_cast<char *>(&statFile), sizeof(int));
            metaFile.write(reinterpret_cast<char *> (&file.numberFirstChunk), sizeof(size_t));
            size_t countFiles = getCountFile();
            metaFile.seekg(0, metaFile.beg);
            countFiles++;
            metaFile.write(reinterpret_cast<char *>(&countFiles), sizeof(size_t));
            size_t countChunks = getCountChunk();
            countChunks++;
            metaFile.write(reinterpret_cast<char *>(&countChunks), sizeof(size_t));
            metaFile.close();
        }
    }

    size_t ManagerMeta::getCountFile()
    {
        std::fstream metaFile;

        metaFile.open(metaDirectory_.string() + NameFileMetaInfo,
                      std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        if (metaFile.is_open())
        {
            size_t countFiles;
            metaFile.read(reinterpret_cast<char *>(&countFiles), sizeof(size_t));
            std::cout << countFiles << std::endl;
            metaFile.close();
            return countFiles;
        }
        return 0;
    }

    size_t ManagerMeta::getCountChunk()
    {
        std::fstream metaFile;

        metaFile.open(metaDirectory_.string() + NameFileMetaInfo,
                      std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        if (metaFile.is_open())
        {
            metaFile.seekg(sizeof(size_t), metaFile.beg);
            size_t countChunks;
            metaFile.read(reinterpret_cast<char *>(&countChunks), sizeof(size_t));
            std::cout << countChunks << std::endl;
            return countChunks;
        }
        return 0;
    }
}