// LeetCode 704 · Binary Search — 编译验证 review.md 中的二分实现
// cl /nologo /EHsc /W4 /utf-8 /std:c++17 test_704.cpp && test_704.exe
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

// ===== review.md 二分（原样抄录） =====
int binarySearch(const std::vector<int>& nums, const int target) {
    int count = static_cast<int>(nums.size());
    int left = 0;
    int right = count - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (target < nums[mid]) {
            right = mid - 1;
        } else if (target > nums[mid]) {
            left = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}

// ===== 参考实现：std::lower_bound（结构不同，用于对拍） =====
static int refSearch(const std::vector<int>& nums, int target) {
    auto it = std::lower_bound(nums.begin(), nums.end(), target);
    if (it != nums.end() && *it == target) return static_cast<int>(it - nums.begin());
    return -1;
}

// ===== 测试辅助 =====
static void printVec(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) std::cout << (i ? "," : "") << v[i];
    std::cout << "]";
}

static bool check(const std::vector<int>& in, int target, int expect) {
    int got = binarySearch(in, target);
    int ref = refSearch(in, target);
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
    ok &= check({-1, 0, 3, 5, 9, 12}, 9, 4);    // 题面示例 1
    ok &= check({-1, 0, 3, 5, 9, 12}, 2, -1);   // 题面示例 2
    ok &= check({5}, 5, 0);                     // 单元素命中
    ok &= check({5}, 3, -1);                    // 单元素小于
    ok &= check({5}, 8, -1);                    // 单元素大于
    ok &= check({1, 2, 3}, 1, 0);               // 左端点
    ok &= check({1, 2, 3}, 3, 2);               // 右端点
    ok &= check({1, 2, 3}, 0, -1);              // 全体偏小
    ok &= check({1, 2, 3}, 4, -1);              // 全体偏大
    ok &= check({}, 5, -1);                     // 空数组（约定推演：right=-1 首轮即假）
    ok &= check({1, 2}, 1, 0);                  // 双元素取左
    ok &= check({1, 2}, 2, 1);                  // 双元素取右
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：升序无重复 vs std::lower_bound
    std::mt19937 rng(704);
    bool rand_ok = true;
    for (int t = 0; t < 2000 && rand_ok; t++) {
        int n = std::uniform_int_distribution<int>(1, 40)(rng);
        std::vector<int> v(n);
        int x = -50;
        for (int& e : v) { x += std::uniform_int_distribution<int>(1, 3)(rng); e = x; }  // 严格递增，符合题面无重复约定
        int target = std::uniform_int_distribution<int>(-60, 80)(rng);
        int a = binarySearch(v, target), c = refSearch(v, target);
        if (a != c) {
            std::cout << "  RANDOM MISMATCH case " << t << " target=" << target << " n=" << n << ":\n  ";
            printVec(v);
            std::cout << "\n  got=" << a << " ref=" << c << "\n";
            rand_ok = false;
        }
    }
    std::cout << "random 2000 cross-check: " << (rand_ok ? "OK" : "FAILED") << "\n";
    return (ok && rand_ok) ? 0 : 1;
}
