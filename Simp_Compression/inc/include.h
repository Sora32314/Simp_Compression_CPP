#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <filesystem>

#include "Binary\inc\include.h"
namespace BO = Binary::Operator;
namespace fs = std::filesystem;

#ifndef BYTESIZE
#define BYTESIZE 8
#endif


/**
 * @namespace Simp_Compression
 * @brief 包含文件系统操作和压缩/解压缩功能的命名空间。
 */

namespace Simp_Compression
{
    namespace FileSystem
    {
        namespace SCF = Simp_Compression::FileSystem;


        /**
         * @brief 文件系统命名空间内的函数包装器。
         *
         * @param func 传入需要包装的函数指针。
         * @param ... args 函数的参数。
         * @return 返回函数调用的结果。
         */

        template<typename Tt, typename ... Args>
        auto CallBack_Wrapper(std::function<Tt> func, Args&& ... args)
        { 
            auto ret = func(std::forward<Args>(args) ...);
            return ret;
        }


        /**
         * @brief 用来创建一个文件。
         *
         * @param argPath 需要创建的文件路径。
         * @param ifRelative flag，用来控制传入的路径是否为相对路径。
         */
        void File_Create(std::string&& argPath, bool ifRelative)
        {
            fs::path pa(argPath);
            if(pa.empty())
            {
                pa = fs::current_path().string();
            }

            //拼接路径
            if(ifRelative)
            {
                std::ofstream file(fs::current_path().string() + pa.string(), std::ios::ate);
                file.close();
            }
            else
            {
                std::ofstream file(pa, std::ios::ate);
                file.close();
            }

            return;
        }


        /**
         * @brief 向文件写入文本数据。
         *
         * @param argPath 文件路径。
         * @param buffer 要写入的数据缓冲区。
         * @param ifRelative flag，指示路径是否为相对路径。
         * @return 如果成功写入则返回true，否则返回false。
         */
        bool File_Write(std::string&& argPath, std::string&& buffer, bool ifRelative)
        {
            fs::path pa(std::forward<std::string>(argPath));

            //拼接路径
            if(ifRelative)
            {
                pa = fs::current_path() / pa;
            }

            //寻找文件是否存在，如果存在则以正常方式打开并且定位至文件尾，没有则打开失败
            std::ofstream file(pa, std::ios::ate|std::ios::in);

            if(!file)
            {
                return false;
            }
            
            //写入文件
            file.write(std::forward<std::string>(buffer).c_str(), std::forward<std::string>(buffer).size());

            file.close();

            return true;
        }


        /**
         * @brief 向文件写入二进制数据。
         *
         * @param argPath 文件路径。
         * @param buffer 要写入的数据缓冲区。
         * @param ifRelative flag，指示路径是否为相对路径。
         * @return 如果成功写入则返回true，否则返回false。
         */
        bool File_Write_Bin(std::string&& argPath, std::vector<int>&& buffer, bool ifRelative)
        {
            fs::path pa(std::forward<std::string>(argPath));

            //拼接路径
            if(ifRelative)
            {
                pa = fs::current_path() / pa;
            }

            //寻找文件是否存在，如果存在则以二进制方式打开并且定位至文件尾，没有则打开失败
            std::ofstream file(pa, std::ios::ate|std::ios::in|std::ios::binary);

            if(!file)
            {
                return false;
            }
            
            //写入文件
            file.write(reinterpret_cast<char*>(std::forward<std::vector<int>>(buffer).data()), std::forward<std::vector<int>>(buffer).size() * sizeof(int));

            file.close();

            return true;
        }



        /**
         * @struct FileSystem_Func
         * @brief 包含文件系统操作函数的对象。
         */
        struct FileSystem_Func
        {
            std::function<void(std::string&&, bool)> FileCreate = SCF::File_Create;
            std::function<bool(std::string&&, std::string&&, bool)> FileWrite = SCF::File_Write;
            std::function<bool(std::string&&, std::vector<int>&&, bool)> FileWriteBin = SCF::File_Write_Bin;
        };

    } 
    


    /**
     * @brief 文件系统命名空间内的函数包装器。
     *
     * @param func 传入需要包装的函数指针。
     * @param ... args 函数的参数。
     * @return 返回函数调用的结果。
     */
    template<typename Tt, typename ... Args>
    auto CallBack_Wrapper(std::function<Tt> func, Args&& ... args)
    { 
        auto ret = func(std::forward<Args>(args) ...);
        return ret;
    }



    /**
     * @brief 压缩基因序列文件。
     *
     * @param argPath 输入文件路径。
     * @param isRelativeArg flag，指示输入文件路径是否为相对路径。
     * @param resPath 输出文件路径。
     * @param isRelativeRes flag，指示输出文件路径是否为相对路径。
     * @return 如果成功压缩则返回true，否则返回false。
     */
    bool Gene_Compression(std::string&& argPath, bool isRelativeArg, std::string&& resPath, bool isRelativeRes)
    {
        std::vector<int> streamTarget;
        //int位大小
        short intBitCount = sizeof(int) * 8;

        int target = 0;
        fs::path fsResPath(std::forward<std::string>(resPath));
        fs::path fsArgPath(std::forward<std::string>(argPath));
        //如果是相对路径，则补充为绝对路径
        if(isRelativeRes)
        {
            fsResPath = fs::current_path() / fsResPath;
        }

        if(isRelativeArg)
        {
            fsArgPath = fs::current_path() / fsArgPath;
        }

        //打开文件
        std::ifstream argFile(fsArgPath, std::ios::in);
        std::ofstream resFile(fsResPath, std::ios::out|std::ios::trunc|std::ios::binary);

        //文件长度
        size_t length = 0;
        resFile.write(reinterpret_cast<char*>(&length), sizeof(size_t));

        //逐渐读取文件直到文件末尾
        char buffer[128] = {0};
        while (!argFile.eof())
        {
            //创建缓冲区并初始化字符串
            argFile.read(buffer, 128);
            std::streamsize bytesRead = argFile.gcount();
            
            //将读取到的数据存储至数组中
            for(decltype(bytesRead) i = 0; i < bytesRead; i++, length += 2)
            {
                if(buffer[i] == 'A')
                {
                    streamTarget.push_back(0);
                    streamTarget.push_back(0);
                }
                else if(buffer[i] == 'T')
                {
                    streamTarget.push_back(1);
                    streamTarget.push_back(0);
                }
                else if(buffer[i] == 'C')
                {
                    streamTarget.push_back(0);
                    streamTarget.push_back(1);
                }
                else if(buffer[i] == 'G')
                {
                    streamTarget.push_back(1);
                    streamTarget.push_back(1);
                }
            }

            //处理并写入数据到int中，之后写入到文件里
            for (size_t i = 0; i < streamTarget.size();) 
            {
                target = 0;
                for (int j = 1; j <= intBitCount && i < streamTarget.size(); j++, i++) 
                {
                    target = BO::Bit_Set(target, j, streamTarget[i]);
                }
                resFile.write(reinterpret_cast<char*>(&target), sizeof(int));
            }
            
            streamTarget.clear();
        }   

        resFile.seekp(std::ios::beg);
        resFile.write(reinterpret_cast<char*>(&length), sizeof(size_t));

        argFile.close();
        resFile.close();
        
        return true;
    }



    /**
     * @brief 解压基因序列文件。
     *
     * @param argPath 输入文件路径。
     * @param isRelativeArg flag，指示输入文件路径是否为相对路径。
     * @param resPath 输出文件路径。
     * @param isRelativeRes flag，指示输出文件路径是否为相对路径。
     * @return 如果成功解压则返回true，否则返回false。
     */
    bool Gene_Decompression(std::string&& argPath, bool isRelativeArg, std::string&& resPath, bool isRelativeRes)
    {
        std::vector<bool> streamTarget;
        std::vector<char> streamInput;
        fs::path fsResPath(std::forward<std::string>(resPath));
        fs::path fsArgPath(std::forward<std::string>(argPath));
        //如果是相对路径，则补充为绝对路径
        if(isRelativeRes)
        {
            fsResPath = fs::current_path() / fsResPath;
        }

        if(isRelativeArg)
        {
            fsArgPath = fs::current_path() / fsArgPath;
        }

        std::ifstream ArgFile(fsArgPath, std::ios::in|std::ios::binary);
        std::ofstream ResFile(fsResPath, std::ios::ate|std::ios::trunc);


        //获取文件长度
        char lengthBuffer[sizeof(size_t)] = {0};
        ArgFile.read(lengthBuffer, sizeof(size_t));
        size_t size = *(reinterpret_cast<size_t*>(lengthBuffer));
        
        
        while(size > 0)
        {
            //创建并初始化字符串
            char buffer[128] = {0};
            ArgFile.read(buffer, 128);
            std::streamsize bytesRead = ArgFile.gcount();

            //读取每一位到bool类型数组中
            for(decltype(bytesRead) i = 0; i < bytesRead; i++)
            {
                for(decltype(bytesRead) j = 0; j < BYTESIZE; j++)
                {
                    streamTarget.push_back(BO::Bit_Check(buffer[i], j));
                }
            }
            


            for(size_t bufferCount = 0; bufferCount < streamTarget.size() && size > 0; bufferCount += 2, size -= 2)
            {
                if(streamTarget[bufferCount] == false)
                {
                    if(streamTarget[bufferCount + 1] == false)
                    {
                        streamInput.push_back('A');
                    }
                    else
                    {
                        streamInput.push_back('C');
                    }
                }
                else if(streamTarget[bufferCount] == true)
                {
                    if(streamTarget[bufferCount + 1] == false)
                    {
                        streamInput.push_back('T');
                    }
                    else
                    {
                        streamInput.push_back('G');
                    }
                }
            }

            streamTarget.clear();
            
            ResFile.write(streamInput.data(), streamInput.size());
            
            streamInput.clear();
        }
        
        ArgFile.close();
        ResFile.close();

        return true;
    }


    /**
     * @struct Simp_Compression_Func
     * @brief 包含压缩和解压缩函数的对象。
     */
    struct Simp_Compression_Func
    {
        std::function<bool(std::string&&, bool, std::string&&, bool)> GeneCompression = Gene_Compression;
        std::function<bool(std::string&&, bool, std::string&&, bool)> GeneDecompression = Gene_Decompression;
    };
    

} 










