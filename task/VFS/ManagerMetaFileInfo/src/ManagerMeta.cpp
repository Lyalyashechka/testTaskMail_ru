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

    File *ManagerMeta::findFileAndSetStatusIfFind(const char *name, statusFile setStatusIfFind)
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
                File currentFile = readOneFileInfo(metaFile);
                if (currentFile.fileName == name)
                {
                    if (currentFile.status == statusFile::default_ ||
                        currentFile.status == setStatusIfFind)
                    {
                        setStatusFile(metaFile, setStatusIfFind);
                        currentFile.status = setStatusIfFind;
                        metaFile.close();
                        return new File(currentFile);
                    }
                    else
                    {
                        std::cerr << "File " << currentFile.fileName
                                  << "is already open in a different mode" << std::endl;
                        metaFile.close();
                        return nullptr;
                    }
                }
            }
            metaFile.close();
        }
        return nullptr;
    }

    void ManagerMeta::setStatusFile(std::fstream &metaFile, statusFile status)
    {
        size_t currentPosition = metaFile.tellg();
        metaFile.seekg(currentPosition - sizeof(size_t) - sizeof(int), metaFile.beg);
        int newStatus = static_cast<int>(status);
        metaFile.write(reinterpret_cast<char *>(&newStatus), sizeof(int));
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
            metaFile.write(reinterpret_cast<char *>(&file.numberFirstChunk), sizeof(size_t));
            size_t countFiles = getCountFile();
            metaFile.seekg(0, metaFile.beg);
            countFiles++;
            metaFile.write(reinterpret_cast<char *>(&countFiles), sizeof(size_t));
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
            metaFile.close();
            return countChunks;
        }
        return 0;
    }

    void ManagerMeta::incrementCountChunk()
    {
        std::fstream metaFile;

        metaFile.open(metaDirectory_.string() + NameFileMetaInfo,
                      std::ios_base::binary | std::ios_base::out | std::ios_base::in);
        if (metaFile.is_open())
        {
            metaFile.seekg(sizeof(size_t), metaFile.beg);
            size_t currentCountChunk = getCountChunk();
            currentCountChunk++;
            metaFile.write(reinterpret_cast<char *>(&currentCountChunk), sizeof(size_t));
            metaFile.close();
        }
    }

    File ManagerMeta::readOneFileInfo(std::fstream &metaFile)
    {
        File currentFile;
        metaFile.read(reinterpret_cast<char *>(&currentFile.lenFileName), sizeof(size_t));
        char *buf = new char[currentFile.lenFileName + 1];
        metaFile.read(reinterpret_cast<char *>(buf), currentFile.lenFileName);
        buf[currentFile.lenFileName] = 0;
        currentFile.fileName = buf;
        delete buf;
        int status;
        metaFile.read(reinterpret_cast<char *>(&status), sizeof(int));
        currentFile.status = static_cast<statusFile>(status);
        metaFile.read(reinterpret_cast<char *>(&currentFile.numberFirstChunk), sizeof(size_t));
        return currentFile;
    }
}