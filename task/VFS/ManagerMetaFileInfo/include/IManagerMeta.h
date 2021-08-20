#pragma once

#include "StructFile.h"

namespace TestTask
{

    struct IManagerMeta
    {
        virtual File* findFile(const char *name ,int setStatusIfFind) = 0;

        virtual size_t getCountFile() = 0;

        virtual void setStatusFile(statusFile status) = 0;
    };
    




}