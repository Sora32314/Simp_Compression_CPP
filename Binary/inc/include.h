#pragma once

#include <iostream>

namespace Binary
{

    namespace Operator
    {
        template<typename Ty>
        Ty Bit_Set(Ty Num, int pos, bool value)
        {
            Ty ret = 0;
            if(value == 0)
            {
                ret = Num & ~(1 << (pos - 1));
            }
            else
            {
                ret = Num | (1 << (pos - 1));
            }

            return ret;
        }

        template<typename Ty>
        Ty Bit_Flip(Ty Num, int pos)
        {
            Ty ret = 0;
            ret = Num ^ (1 << (pos - 1));
        }

        template<typename Ty>
        bool Bit_Check(Ty Num, int pos)
        {
            return static_cast<bool>(Num & (1 << pos));
        }


    } 
    







}





















