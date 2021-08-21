#pragma once

#include <filesystem>
#include <system_error>
#include <fstream>
#include <memory>

#include "IVFS.h"
#include "ManagerMeta.h"
#include "RootName.h"

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
        void initMetaDataFile();

        void initFilesystemStructure(const std::string &locationFileSystemDirectory);

        size_t numberOfFilesInDirectory(std::filesystem::path path);

        std::unique_ptr<IManagerMeta> ManagerMeta_;

        std::filesystem::path rootDirectory_;

        std::filesystem::path metaDirectory_;
    };
}