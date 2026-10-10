// LeetCode 35 · Search Insert Position — 编译验证 review.md 中的实现
// cl /nologo /EHsc /W4 /utf-8 /std:c++17 test_35.cpp && test_35.exe
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

// ===== review.md 原样抄录 =====
int searchInsertPos(const std::vector<int>& nums, const int target) {
    int count = static_cast<int>(nums.size());
    int left = 0;
    int right = count - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else {
            return mid;
        }
    }
    return left;
}

// ===== 参考实现：std::lower_bound（第一个 >= target 的位置） =====
static int refInsert(const std::vector<int>& nums, int target) {
    return static_cast<int>(std::lower_bound(nums.begin(), nums.end(), target) - nums.begin());
}

// ===== 测试辅助 =====
static void printVec(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) std::cout << (i ? "," : "") << v[i];
    std::cout << "]";
}

static bool check(const std::vector<int>& in, int target, int expect) {
    int got = searchInsertPos(in, target);
    int ref = refInsert(in, target);
    bool ok = (got == expect) && (ref == expect);
    if (!ok) {
        std::cout << "  FAIL in="; printVec(in);
        std::cout << " target=" << target << " expect=" << expect
                  << " got=" << got << " ref=" << ref << "\n";
    }
    return ok;
}

int main() {
    bool ok = true;
    ok &= check({1, 3, 5, 6}, 5, 2);   // 示例 1 命中
    ok &= check({1, 3, 5, 6}, 2, 1);   // 示例 2 插中间
    ok &= check({1, 3, 5, 6}, 7, 4);   // 示例 3 插最后（left==n）
    ok &= check({1, 3, 5, 6}, 0, 0);   // 插最前（left==0）
    ok &= check({5}, 5, 0);            // 单元素命中
    ok &= check({5}, 1, 0);            // 单元素插前
    ok &= check({5}, 9, 1);            // 单元素插后
    ok &= check({1, 2}, 2, 1);         // 双元素命中右
    ok &= check({1, 2}, 0, 0);         // 双元素插最前
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：升序无重复 × lower_bound，target 扫过界内+界外
    std::mt19937 rng(35);
    bool rand_ok = true;
    for (int t = 0; t < 3000 && rand_ok; t++) {
        int n = std::uniform_int_distribution<int>(1, 40)(rng);
        std::vector<int> v(n);
        int x = -5;
        for (int& e : v) { x += std::uniform_int_distribution<int>(1, 3)(rng); e = x; }
        int target = std::uniform_int_distribution<int>(-8, x + 5)(rng);
        int a = searchInsertPos(v, target), c = refInsert(v, target);
        if (a != c) {
            std::cout << "  RANDOM MISMATCH case " << t << " target=" << target << " n=" << n << ":\n  ";
            printVec(v);
            std::cout << "\n  got=" << a << " ref=" << c << "\n";
            rand_ok = false;
        }
    }
    std::cout << "random 3000 cross-check: " << (rand_ok ? "OK" : "FAILED") << "\n";
    return (ok && rand_ok) ? 0 : 1;
}
