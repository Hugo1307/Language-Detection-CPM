cmake_minimum_required(VERSION 2.16)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(lang lang.cpp IO/reader/FileInfoReader.cpp IO/reader/ReferenceReader.cpp args/LangInputArguments.cpp cpm/ModelGenerator.cpp utils/utils.cpp utils/utils.h cpm/CopyModelExecutor.cpp cpm/CopyModelExecutor.h
        cpm/HitsMissesInfo.h cpm/HitsMissesInfo.cpp IO/reader/CopyModelReader.cpp IO/reader/CopyModelReader.h IO/reader/RandomAccessReader.cpp IO/writer/CopyModelOutputWriter.cpp IO/writer/CopyModelOutputWriter.h models/CopyModelOutput.cpp models/CopyModelOutput.h IO/IOEntity.h IO/reader/Reader.cpp IO/reader/Reader.h IO/writer/Writer.cpp IO/writer/Writer.h models/GeneratedModel.cpp models/GeneratedModel.h IO/reader/SequentialReader.cpp IO/reader/SequentialReader.h)