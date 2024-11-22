
#include "Simp_Compression\inc\include.h"

namespace SC = Simp_Compression;
namespace SCF = Simp_Compression::FileSystem;
namespace BO = Binary::Operator;




int main()
{
    SCF::FileSystem_Func FSFuncs;
    SC::Simp_Compression_Func SCFuncs;
    
    std::vector<bool> tmp = {true, false, false, true, true, false, false, true};
    // std::string str = "你好啊！";
    
    // std::vector<int> vec = {1, 2, 3, 4, 5};

    //auto ret = SCF::CallBack_Wrapper(FSFuncs.FileWrite, "new.txt", std::move(str), 1);
    SC::CallBack_Wrapper(SCFuncs.GeneCompression, "new.txt", true, "new_cast.txt", true);
    SC::CallBack_Wrapper(SCFuncs.GeneDecompression, "new_cast.txt", true, "new_save_1.txt", true);


    


    return 0;
}






