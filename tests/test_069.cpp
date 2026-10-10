// LeetCode 69 · Sqrt(x) — 编译验证 review.md 中的实现
// cl /nologo /EHsc /W4 /utf-8 /std:c++17 test_069.cpp && test_069.exe
#include <iostream>
#include <random>
#include <cmath>
#include <climits>

// ===== review.md 原样抄录 =====
int sqrtInt(int x) {
    int left = 0;
    int right = x;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        long long sqr = (long long)mid * mid;

        if (sqr > x) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return (left + right) / 2;
}

// ===== 参考实现：double sqrt 初值 + 整数校准（结构完全不同） =====
static int refSqrt(int x) {
    long long r = static_cast<long long>(std::sqrt(static_cast<double>(x)));
    while ((r + 1) * (r + 1) <= x) r++;
    while (r * r > x) r--;
    return static_cast<int>(r);
}

static bool check(int x, int expect) {
    int got = sqrtInt(x);
    int ref = refSqrt(x);
    bool ok = (got == expect) && (ref == expect);
    if (!ok) {
        std::cout << "  FAIL x=" << x << " expect=" << expect
                  << " got=" << got << " ref=" << ref << "\n";
    }
    return ok;
}

int main() {
    bool ok = true;
    ok &= check(0, 0);                       // 边界 0
    ok &= check(1, 1);                       // 边界 1
    ok &= check(2, 1);
    ok &= check(3, 1);
    ok &= check(4, 2);                       // 示例 1 完全平方
    ok &= check(8, 2);                       // 示例 2 截断
    ok &= check(9, 3);
    ok &= check(15, 3);
    ok &= check(16, 4);                      // 恰好命中 mid
    ok &= check(2147395599, 46339);          // 46340^2 - 1
    ok &= check(2147395600, 46340);          // 46340^2 完全平方贴上限
    ok &= check(INT_MAX, 46340);             // 2^31-1 溢出弹区
    ok &= check(1073741824, 32768);          // 2^30 完全平方
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：全域 [0, INT_MAX] × double-sqrt 校准版
    std::mt19937 rng(69);
    bool rand_ok = true;
    for (int t = 0; t < 5000 && rand_ok; t++) {
        int x = std::uniform_int_distribution<int>(0, INT_MAX)(rng);
        int a = sqrtInt(x), c = refSqrt(x);
        if (a != c) {
            std::cout << "  RANDOM MISMATCH case " << t << " x=" << x
                      << " got=" << a << " ref=" << c << "\n";
            rand_ok = false;
        }
    }
    std::cout << "random 5000 cross-check: " << (rand_ok ? "OK" : "FAILED") << "\n";
    return (ok && rand_ok) ? 0 : 1;
}
