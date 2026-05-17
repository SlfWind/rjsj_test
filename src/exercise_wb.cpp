/**
 * @file exercise_wb.cpp
 * @brief 列举一些用于学生熟悉白盒测试方法的样例程序
 * @date 2026sp
 * @version 1.0
 */

 #include<iostream>
 #include<vector>
 #include"include/exercise.h"

 using std::cout, std::endl;
 using std::vector;

/**
 * @brief 第一题，基础的控制流分析。
 */
void exercise_wb_1(int param1, int param2, int param3) {
    int res = -1;

    if (param1 > 0 && param2 > 0 && param3 > 0) {
        if (param1 > param2) {
            if (param2 > param3) {
                res = 3;
            } else if (param1 > param3) {
                res = 2;
            } else {
                res = 1;
            }
        } else {
            if (param2 > param3) {
                if (param1 > param3) {
                    res = 2;
                } else {
                    res = 1;
                }
            } else {
                res = 0;
            }
        }
    }

    cout << res << endl;
}

/**
 * @brief 第二题，引入了简单的数据流，更多的条件组合。
 */
void exercise_wb_2(int param1, int param2) {
    int param3 = param1 + param2;

    if (param3 > 0) {
        ++param1;
    }

    if (param3 < 0) {
        ++param2;
    }

    if (param1 > param2 || param1 > param3) {
        if (param1 <= param2 || param1 <= param3) {
            param3 += param1;            
        }
    }

    if (param3 != 0) {
        param3 += param2;
    }

    cout << param3 << endl;
}

/**
 * @brief 第三题，引入了循环和更复杂的输入。
 */
void exercise_wb_3(vector<int> params) {
    bool flag = true;
    int res = 0;
    int pos = 0;
    int sz = params.size();

    while(pos != sz) {
        if(flag) {
            res += params[pos];
        } else {
            res -= params[pos];
        }
        if (params[pos] > 0) {
            flag = !flag;
        }
        ++pos;
    }

    if(res > 0) {
        res = res * res;
    }
    
    if (res < 0) {
        res = - (res * res);
    }

    cout << res << endl;
}
