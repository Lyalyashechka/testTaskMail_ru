#pragma once

#include <iostream>
#include <string>

namespace TestTask
{
    enum class statusFile
    {
        default_,
        readOnly_,
        writeOnly_
    };

    struct File
    {
        File(){};

        File(const File &file)
        {
            this->lenFileName = file.lenFileName;

            this->fileName = file.fileName;
        }
        //size_t hashFileName;

        size_t lenFileName;

        std::string fileName;

        //size_t posFseekFirstChunk;

        statusFile status = statusFile::default_;

        void setFileName(std::string name)
        {
            lenFileName = name.length();

            fileName = name;
        }
    };
}