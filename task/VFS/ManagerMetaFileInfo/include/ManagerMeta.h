#pragma once

#include <memory>
#include <filesystem>
#include <fstream>

#include "IManagerMeta.h"
#include "RootName.h"

namespace TestTask
{
    class ManagerMeta : public IManagerMeta
    {
    public:
        ManagerMeta(std::filesystem::path locationFileSystemDirectory);

        File *findFile(const char *name, statusFile setStatusIfFind) override;

        void setStatusFile(statusFile status) override;

        File *addFile(const char *name) override;

    private:
        std::filesystem::path metaDirectory_;

        void writeMetaInfo(File &file);
        
        size_t getCountFile();

        size_t getCountChunk();
    };
}