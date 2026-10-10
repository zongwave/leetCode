// LeetCode 42 · Trapping Rain Water — 编译验证 review.md 中的暴力法
// cl /nologo /EHsc /W4 /utf-8 /std:c++17 test_42.cpp && test_42.exe
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

// ===== review.md 暴力法（原样抄录） =====
int trappingWater(const std::vector<int>& bins) {
    int count = static_cast<int>(bins.size());
    int water = 0;

    for (int cur = 1; cur < count - 1; cur++) {
        int maxLeft = 0;
        int maxRight = 0;
        for (int left = cur - 1; left >= 0; left--) {
            maxLeft = std::max(maxLeft, bins[left]);
        }
        for (int right = cur + 1; right < count; right++) {
            maxRight = std::max(maxRight, bins[right]);
        }
        int w = std::min(maxLeft, maxRight) - bins[cur];
        water += (w > 0) ? w : 0;
    }
    return water;
}

// ===== review.md 双指针（原样抄录） =====
int trappingWater_opt(const std::vector<int>& bins) {
    int count = static_cast<int>(bins.size());
    int water = 0;

    int left = 0;
    int right = count - 1;
    int leftMax = 0;
    int rightMax = 0;

    while (left < right) {
        if (bins[left] < bins[right]) {
            leftMax = std::max(leftMax, bins[left]);
            water += leftMax - bins[left];
            left++;
        } else {
            rightMax = std::max(rightMax, bins[right]);
            water += rightMax - bins[right];
            right--;
        }
    }
    return water;
}

// ===== 参考实现：前后缀 max 预存 O(n)（结构不同，用于对拍） =====
static int prefixSuffix(const std::vector<int>& h) {
    int n = static_cast<int>(h.size());
    if (n == 0) return 0;
    std::vector<int> lm(n), rm(n);
    lm[0] = h[0];
    for (int i = 1; i < n; i++) lm[i] = std::max(lm[i - 1], h[i]);
    rm[n - 1] = h[n - 1];
    for (int i = n - 2; i >= 0; i--) rm[i] = std::max(rm[i + 1], h[i]);
    int water = 0;
    for (int i = 0; i < n; i++) water += std::max(0, std::min(lm[i], rm[i]) - h[i]);
    return water;
}

// ===== 测试辅助 =====
static void printVec(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) std::cout << (i ? "," : "") << v[i];
    std::cout << "]";
}

static bool check(const std::vector<int>& in, int expect) {
    int got = trappingWater(in);
    int opt = trappingWater_opt(in);
    int ref = prefixSuffix(in);
    bool ok = (got == expect) && (ref == expect) && (opt == expect);
    if (!ok) {
        std::cout << "  FAIL in="; printVec(in);
        std::cout << " expect=" << expect << " brute=" << got
                  << " opt=" << opt << " prefix=" << ref << "\n";
    }
    return ok;
}

int main() {
    bool ok = true;
    ok &= check({0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6);  // 题面示例
    ok &= check({4, 2, 0, 3, 2, 4, 3, 0}, 9);              // 两侧高墙夹多凹槽
    ok &= check({4, 2, 0, 3, 2, 5}, 9);                    // 题面示例 2
    ok &= check({5, 2, 1, 4, 3}, 5);                       // 上一版「最近高柱」思路的翻车点（正确值 2+3=5）
    ok &= check({3}, 0);                                   // 单柱
    ok &= check({}, 0);                                    // 空
    ok &= check({2, 0, 2}, 2);                             // 最小凹槽
    ok &= check({5, 4, 3, 2, 1}, 0);                       // 单调递减无积水
    ok &= check({1, 2, 3, 4, 5}, 0);                       // 单调递增无积水
    ok &= check({1, 1, 1, 1}, 0);                          // 全相等
    ok &= check({0, 0, 5, 0, 0}, 0);                       // 单柱无墙
    ok &= check({1, 3, 1}, 0);                             // cur 比两侧都高 -> 负水深被钳 0
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：暴力 vs 前后缀 max
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dn(0, 30), tv(0, 6); // 窄值域 -> 大量等高柱
    bool rand_ok = true;
    for (int t = 0; t < 5000 && rand_ok; t++) {
        int n = dn(rng);
        std::vector<int> v(n);
        for (int& x : v) x = tv(rng);
        int a = trappingWater(v), b = trappingWater_opt(v), c = prefixSuffix(v);
        if (a != c || b != c) {
            std::cout << "  RANDOM MISMATCH case " << t << " n=" << n << ":\n  ";
            printVec(v);
            std::cout << "\n  brute=" << a << " opt=" << b << " prefix=" << c << "\n";
            rand_ok = false;
        }
    }
    std::cout << "random 5000 cross-check: " << (rand_ok ? "OK" : "FAILED") << "\n";
    return (ok && rand_ok) ? 0 : 1;
}
