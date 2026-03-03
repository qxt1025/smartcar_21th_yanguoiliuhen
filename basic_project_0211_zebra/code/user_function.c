#include "user_function.h"

//------------------------------------------------------------------------
// 函数简介     遍历数组最大值或最小值
// 参数说明     array_value        数组指针
// 参数说明     num0               起始范围
// 参数说明     num1               终止范围
// 参数说明     model              1 返回最大值  0 返回最小值
// 返回类型     uint8
// 使用示例     find_extreme_value(remote_distance, 10, SEARCH_IMAGE_W - 10, 0);
// 备注信息
//------------------------------------------------------------------------
uint8 find_extreme_value(uint8 *array_value, uint8 num0, uint8 num1, uint8 model)
{
    uint8 i = 0, temp = 0, temp1 = 0, temp2 = 0, value = 0;

    if(num0 > num1)
    {
        temp1 = num0 - num1;
        temp2 = num1;
        array_value += num0;
        value = *array_value;
        if(model)
        {
            for(i = 0; i <= temp1; i++)
            {
                temp = *(array_value - i);
                if(temp > value)
                {
                    temp2 = num0 - i;
                    value = temp;
                }
            }
        }
        else
        {
            for(i = 0; i <= temp1; i++)
            {
                temp = *(array_value - i);
                if(temp < value)
                {
                    temp2 = num0 - i;
                    value = temp;
                }
            }
        }
    }
    else
    {
        temp1 = num1 - num0;
        temp2 = num0;
        array_value += num0;
        value = *array_value;
        if(model)
        {
            for(i = 0; i <= temp1; i++)
            {
                temp = *(array_value + i);
                if(temp > value)
                {
                    temp2 = num0 + i;
                    value = temp;
                }
            }
        }
        else
        {
            for(i = 0; i <= temp1; i++)
            {
                temp = *(array_value + i);
                if(temp < value)
                {
                    temp2 = num0 + i;
                    value = temp;
                }
            }
        }
    }
    return temp2;
}