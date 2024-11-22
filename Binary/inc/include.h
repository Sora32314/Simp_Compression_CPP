#pragma once

#include <iostream>



/**
 * @namespace Binary
 * @brief 包含二进制操作相关的功能。
 */
namespace Binary
{



    /**
     * @namespace Operator
     * @brief 提供基本的二进制位操作函数。
     */
    namespace Operator
    {



        /**
         * @brief 设置指定位置的位值。
         *
         * @tparam Ty 数据类型。
         * @param Num 要操作的数值。
         * @param pos 位的位置（从1开始计数）。
         * @param value 要设置的位值（true或false）。
         * @return 返回设置位后的数值。
         */
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


        /**
         * @brief 翻转指定位置的位。
         *
         * @tparam Ty 数据类型。
         * @param Num 要操作的数值。
         * @param pos 位的位置（从1开始计数）。
         * @return 返回翻转位后的数值。
         */
        template<typename Ty>
        Ty Bit_Flip(Ty Num, int pos)
        {
            Ty ret = 0;
            ret = Num ^ (1 << (pos - 1));
        }


        /**
         * @brief 检查指定位置的位值。
         *
         * @tparam Ty 数据类型。
         * @param Num 要操作的数值。
         * @param pos 位的位置（从0开始计数）。
         * @return 如果指定位置的位为1则返回true，否则返回false。
         */
        template<typename Ty>
        bool Bit_Check(Ty Num, int pos)
        {
            return static_cast<bool>(Num & (1 << pos));
        }


    } 
    



}





















