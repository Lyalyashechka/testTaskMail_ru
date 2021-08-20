#pragma once

#include "IManagerMeta.h"

namespace TestTask
{
    struct StatusFile;

    class ManagerMeta : public IManagerMeta
    {
        File *findFile(const char *name, int setStatusIfFind) override;

        size_t getCountFile() override;

        void setStatusFile(statusFile status) override;
    };
}