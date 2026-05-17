#include<vector>

using std::vector;

void exercise_wb_1(int, int, int);
void exercise_wb_2(int, int);
void exercise_wb_3(vector<int>);

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
char exercise_bb_1(double);



