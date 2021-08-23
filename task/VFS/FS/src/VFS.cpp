#include "VFS.h"

namespace TestTask
{
    VFS::VFS(std::filesystem::path locationFileSystemDirectory) : ManagerMeta_(std::make_unique<ManagerMeta>(locationFileSystemDirectory))
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
            return ManagerMeta_->findFileAndSetStatusIfFind(name, statusFile::readOnly_);
        }
    }

    File *VFS::Create(const char *name)
    {
        if (numberOfFilesInDirectory(metaDirectory_) == 0)
        {
            initMetaInfoAndDataFile();
        }
        File *currentFIle = ManagerMeta_->findFileAndSetStatusIfFind(name, statusFile::writeOnly_);
        if (currentFIle == nullptr)
        {
            std::lock_guard<std::mutex> lock(mutex_);

            currentFIle = ManagerMeta_->addFile(name);
            ssize_t firstChunkCurrentFile = ManagerMeta_->getCountChunk();
            currentFIle->numberFirstChunk = firstChunkCurrentFile;
            initNewChunk();
            ManagerMeta_->incrementCountChunk();
            std::cout << "New file created" << std::endl;
            return new File(*currentFIle);
        }
        else
        {
            std::cout << "File found" << std::endl;
            return new File(*currentFIle);
        }
    }

    size_t VFS::Read(File *f, char *buff, size_t len)
    {
        size_t totalReaded = 0;
        if (f == nullptr || f->status != statusFile::readOnly_)
        {
            std::cerr << "file read error: " << std::endl;
            return totalReaded;
        }
        std::fstream fileWithData;
        fileWithData.open(rootDirectory_.string() + NameFileData /*, std::ios_base::binary | std::ios_base::out | std::ios_base::in*/);
        if (fileWithData.is_open())
        {
            fileWithData.seekg(f->numberFirstChunk * IndentOneChunk, fileWithData.beg);
            size_t countFreeBlockInChunk = 0;
            int leftForReaded = len;
            int readed = 0;
            while (readed != len)
            {
                fileWithData.read(reinterpret_cast<char *>(&countFreeBlockInChunk), sizeof(size_t));
                size_t countFilledBlock = SizeChunk - countFreeBlockInChunk;
                //если не надо переходить на след чанк
                if (leftForReaded < countFilledBlock)
                {
                    fileWithData.seekg(sizeof(ssize_t), std::ios_base::cur);
                    fileWithData.read(&buff[readed], leftForReaded);
                    readed += leftForReaded;
                    totalReaded += leftForReaded;
                }
                //если запросили прочитать больше чем есть на самом деле в файле
                else if (leftForReaded > countFilledBlock && countFilledBlock < SizeChunk)
                {
                    fileWithData.seekg(sizeof(ssize_t), std::ios_base::cur);
                    fileWithData.read(&buff[readed], countFilledBlock);
                    totalReaded += countFilledBlock;
                    readed = len;
                }
                //если надо прочитать весь чанк
                else
                {
                    ssize_t nextChunk = IdEmptyNextChunk;
                    fileWithData.read(reinterpret_cast<char *>(&nextChunk), sizeof(ssize_t));
                    fileWithData.read(&buff[readed], countFilledBlock);
                    leftForReaded -= countFilledBlock;
                    readed += countFilledBlock;
                    totalReaded += SizeChunk;
                    if (nextChunk == IdEmptyNextChunk)
                    {
                        fileWithData.close();
                        return totalReaded;
                    }
                    fileWithData.seekg(nextChunk * IndentOneChunk, fileWithData.beg);
                }
            }
            fileWithData.close();
        }
        else
        {
            std::cerr << "Error open fileWithData" << std::endl;
            exit(1);
        }
        return totalReaded;
    }

    size_t VFS::Write(File *f, char *buff, size_t len)
    {
        size_t totalWrited = 0;
        if (f == nullptr || f->status != statusFile::writeOnly_)
        {
            std::cerr << "file write error" << std::endl;
            return totalWrited;
        }
        std::fstream fileWithData;
        fileWithData.open(rootDirectory_.string() + NameFileData /*, std::ios_base::binary | std::ios_base::out | std::ios_base::in*/);
        if (fileWithData.is_open())
        {
            std::lock_guard<std::mutex> lock(mutex_);

            fileWithData.seekg(f->numberFirstChunk * IndentOneChunk, fileWithData.beg);
            size_t countFreeBlockInChunk = setCarriageInLastChunkFile(fileWithData);
            //std::cout << fileWithData.tellg() << "position for write" << std::endl;
            int leftToWrite = len;
            int writed = 0;
            while (writed != len)
            {
                //если данные поместятся в чанк
                if (leftToWrite < countFreeBlockInChunk)
                {
                    size_t newCountFreeBlockInChunk = countFreeBlockInChunk - leftToWrite;
                    fileWithData.write(reinterpret_cast<char *>(&newCountFreeBlockInChunk), sizeof(size_t));
                    fileWithData.seekg(sizeof(ssize_t) + SizeChunk - countFreeBlockInChunk, std::ios_base::cur);
                    fileWithData.write(&buff[writed], leftToWrite);
                    writed += leftToWrite;
                }
                else
                {
                    size_t newCountFreeBlockInChunk = 0;
                    fileWithData.write(reinterpret_cast<char *>(&newCountFreeBlockInChunk), sizeof(size_t));
                    fileWithData.seekg(sizeof(ssize_t) + SizeChunk - countFreeBlockInChunk, std::ios_base::cur);
                    fileWithData.write(&buff[writed], countFreeBlockInChunk);
                    leftToWrite -= countFreeBlockInChunk;
                    writed += countFreeBlockInChunk;
                    size_t currentPosition = fileWithData.tellg();
                    fileWithData.seekg(currentPosition - SizeChunk - sizeof(ssize_t), fileWithData.beg);
                    ssize_t numNewChunk = ManagerMeta_->getCountChunk();
                    initNewChunk();
                    ManagerMeta_->incrementCountChunk();
                    fileWithData.write(reinterpret_cast<char *>(&numNewChunk), sizeof(ssize_t));
                    fileWithData.seekg(numNewChunk * IndentOneChunk, fileWithData.beg);
                    countFreeBlockInChunk = SizeChunk;
                }
            }
            totalWrited = writed;
            fileWithData.close();
        }
        else
        {
            std::cerr << "Error open fileWithData" << std::endl;
            exit(1);
        }
        return totalWrited;
    }

    void VFS::Close(File *f)
    {
        ManagerMeta_->findFileAndSetStatusIfFind(f->fileName.data(), statusFile::default_);
        f->status = statusFile::default_;
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

    void VFS::initMetaInfoAndDataFile()
    {
        std::ofstream fileMeta;
        fileMeta.open(metaDirectory_.string() + NameFileMetaInfo, std::ios_base::binary);
        if (fileMeta.is_open())
        {
            size_t countFilesAndCountChunk = 0;
            fileMeta.write(reinterpret_cast<char *>(&countFilesAndCountChunk), sizeof(size_t));
            fileMeta.write(reinterpret_cast<char *>(&countFilesAndCountChunk), sizeof(size_t));
            fileMeta.close();
        }
        else
        {
            std::cerr << "Error initialization fileMeta" << std::endl;
        }
        std::ofstream fileData;
        fileData.open(rootDirectory_.string() + NameFileData);
        if (!fileData.is_open())
        {
            std::cerr << "Error initialization fileData" << std::endl;
        }
        fileData.close();
    }

    void VFS::initNewChunk()
    {
        //записывает размер чанка, номер следующего чанка -1. заполняет чанк пробелами 
        std::fstream fileWithData;
        fileWithData.open(rootDirectory_.string() + NameFileData /*, std::ios_base::binary | std::ios_base::out | std::ios_base::in*/);
        if (fileWithData.is_open())
        {
            ssize_t numLastChunk = ManagerMeta_->getCountChunk();
            fileWithData.seekg(numLastChunk * IndentOneChunk, fileWithData.beg);
            size_t countFreeInChunk = SizeChunk;
            fileWithData.write(reinterpret_cast<char *>(&countFreeInChunk), sizeof(size_t));
            ssize_t nextChunk = IdEmptyNextChunk;
            fileWithData.write(reinterpret_cast<char *>(&nextChunk), sizeof(ssize_t));
            char *masForElement = new char[SizeChunk];
            std::fill_n(masForElement, SizeChunk, ' ');
            fileWithData.write(masForElement, SizeChunk);
            fileWithData.close();
        }
        else
        {
            std::cerr << "Error open fileWithData" << std::endl;
            exit(1);
        }
    }

    size_t VFS::setCarriageInLastChunkFile(std::fstream &fileWithData)
    {
        size_t countFreeBlockInChunk = 0;

        while (countFreeBlockInChunk == 0)
        {
            fileWithData.read(reinterpret_cast<char *>(&countFreeBlockInChunk), sizeof(size_t));
            if (countFreeBlockInChunk == 0)
            {
                ssize_t nextChunk = IdEmptyNextChunk;
                size_t currentPosition = fileWithData.tellg();
                fileWithData.read(reinterpret_cast<char *>(&nextChunk), sizeof(ssize_t));
                if (nextChunk == IdEmptyNextChunk)
                {
                    ssize_t numNewChunk = ManagerMeta_->getCountChunk();
                    initNewChunk();
                    ManagerMeta_->incrementCountChunk();

                    fileWithData.seekg(currentPosition, fileWithData.beg);
                    fileWithData.write(reinterpret_cast<char *>(&numNewChunk), sizeof(ssize_t));
                    fileWithData.seekg(numNewChunk * IndentOneChunk, fileWithData.beg);
                    break;
                }
                else
                {
                    fileWithData.seekg(nextChunk * IndentOneChunk, fileWithData.beg);
                }
            }
            else
            {
                size_t positionOnBeginCurrentChunk = static_cast<size_t>(fileWithData.tellg()) -
                                                     static_cast<size_t>(sizeof(size_t));
                fileWithData.seekg(positionOnBeginCurrentChunk, fileWithData.beg);
            }
        }
        return countFreeBlockInChunk;
    }
}