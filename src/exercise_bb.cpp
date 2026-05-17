/**
 * @file exercise_bb.cpp
 * @brief 列举一些用于学生熟悉黑盒测试方法的样例程序
 * @details 
 * 1. 如果你刚开始黑盒测试部分的作业，最好先不要阅览本文件，以假装自己不知道源码
 * 2. /include/exercise.h 文件中的函数声明前也有和本文件相同的注释，可供参考
 * @date 2026sp
 * @version 1.0
 */

#include"include/exercise.h"

/**
 * @brief 第一题，简单的等价类划分和边界值分析。
 * @details 
 * 此函数用于百分制分数到等级制分数的转换，具体来说：
 * 1. [85, 100] => 返回A
 * 2. [75, 85)  => 返回B
 * 3. [65, 75)  => 返回C
 * 4. [60, 65)  => 返回D
 * 5. [0, 60)   => 返回F
 * 6. 其他情况，返回E代表错误
 * @param grade 百分制分数，可以有小数
 */
char exercise_bb_1(double grade) {
    if (grade > 100 || grade < 0) {
        return 'E';
    } else if (grade >= 85) {
        return 'A';
    } else if (grade >= 75) {
        return 'B';
    } else if (grade >= 65) {
        return 'C';
    } else if (grade >= 60) {
        return 'D';
    } else {
        return 'F';
    }
}

/**
 * @brief 第二题，多元等价类划分
 * @details
 * 
 */

/**
 * @brief 第三题，多元边界值分析
 */


