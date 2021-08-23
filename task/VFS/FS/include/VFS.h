#pragma once

#include <filesystem>
#include <system_error>
#include <fstream>
#include <memory>
#include <algorithm>
#include <iterator>

#include "IVFS.h"
#include "ManagerMeta.h"
#include "ConstDefines.h"

namespace TestTask
{
    class VFS : public IVFS
    {
    public:
        VFS(std::filesystem::path locationFileSystemDirectory = "");

        File *Open(const char *name) override;

        File *Create(const char *name) override;

        size_t Read(File *f, char *buff, size_t len) override;

        size_t Write(File *f, char *buff, size_t len) override;

        void Close(File *f) override;

    private:
        void initMetaInfoAndDataFile();

        void initFilesystemStructure(const std::string &locationFileSystemDirectory);
        //выделяет место в Data под новый чанк
        void initNewChunk();
        //ставит каретку в начало последнего чанка файла, возвращает количество свободного места в чанке
        size_t setCarriageInLastChunkFile(std::fstream &fileWithData);

        size_t numberOfFilesInDirectory(std::filesystem::path path);

        std::unique_ptr<IManagerMeta> ManagerMeta_;

        std::filesystem::path rootDirectory_;

        std::filesystem::path metaDirectory_;

        std::mutex mutex_;
    };
}