#pragma once

#include <memory>
#include <filesystem>
#include <fstream>
#include <mutex>

#include "IManagerMeta.h"
#include "ConstDefines.h"

namespace TestTask
{
    class ManagerMeta : public IManagerMeta
    {
    public:
        ManagerMeta(std::filesystem::path locationFileSystemDirectory);

        File *findFileAndSetStatusIfFind(const char *name, statusFile setStatusIfFind) override;

        File *addFile(const char *name) override;

        size_t getCountChunk() override;

        void incrementCountChunk() override;

    private:
        std::filesystem::path metaDirectory_;

        void writeMetaInfo(File &file);

        void setStatusFile(std::fstream &metaFile, statusFile status);

        File readOneFileInfo(std::fstream &metaFile);

        size_t getCountFile();

        std::mutex mutex_;
    };
}