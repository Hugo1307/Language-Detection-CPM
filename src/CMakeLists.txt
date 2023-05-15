cmake_minimum_required(VERSION 2.16)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(lang lang.cpp reader/FileInfoReader.cpp reader/ReferenceReader.cpp input/LangInputArguments.cpp cpm/ModelGenerator.cpp utils/utils.cpp utils/utils.h cpm/CopyModelExecutor.cpp cpm/CopyModelExecutor.h
        cpm/HitsMissesInfo.h cpm/HitsMissesInfo.cpp reader/CopyModelReader.cpp reader/CopyModelReader.h reader/RandomAccessReader.cpp)