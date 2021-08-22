#pragma once

#include "StructFile.h"

namespace TestTask
{
    struct IManagerMeta
    {
        virtual File *findFile(const char *name, statusFile setStatusIfFind) = 0;

        virtual File *addFile(const char *name) = 0;

        virtual size_t getCountChunk() = 0;

        virtual void incrementCountChunk() = 0;
    };
}