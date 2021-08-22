#pragma once

#define NameRootFileSystemDirectory "RootFilesystem"
#define NameMetaInfoFilesDirectoriy "RootFilesystem/MetaFilesInfo"
#define NameFileMetaInfo "/Meta.bin"
#define NameFileData "/Data.txt"

#define SizeChunk 1024
#define IndentOneChunk (SizeChunk + sizeof(size_t) + sizeof(ssize_t))