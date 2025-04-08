/*
 * Math.cpp
 *
 *  Created on: Apr 22, 2024
 *      Author: jason
 */

#include <Math.h>

Math::Math()
{
    // TODO Auto-generated constructor stub

}

int Math::min(int a, int b) {
    if (a < b)
        return a;
    return b;
}

int Math::max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

