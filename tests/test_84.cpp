// LeetCode 84 · Largest Rectangle in Histogram — 编译验证 review.md 中的暴力法
// cl /nologo /EHsc /W4 /utf-8 /std:c++17 test_84.cpp && test_84.exe
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>

// ===== review.md 暴力法（原样抄录） =====
int largestRectHist(const std::vector<int>& hist) {
    int count = static_cast<int>(hist.size());
    int maxRect = 0;

    for (int base = 0; base < count; base++) {
        int right = base + 1;
        int left = base - 1;
        while (right < count && hist[base] <= hist[right]) {
            right++;
        }
        while (left >= 0 && hist[base] <= hist[left]) {
            left--;
        }
        int rect = hist[base] * (right - left - 1);
        maxRect = std::max(maxRect, rect);
    }
    return maxRect;
}

// ===== review.md 单调栈+尾哨兵（原样抄录） =====
int largestRectHist_opt(const std::vector<int>& hist) {
    int count = static_cast<int>(hist.size());
    int maxRect = 0;
    std::vector<int> stack;

    for (int base = 0; base <= count; base++) {

        while (!stack.empty() && hist[stack.back()] >= ((base == count) ? -1 : hist[base])) {
            int height = hist[stack.back()];
            int width = 1;
            stack.pop_back();
            if (!stack.empty()) {
                width = (base - stack.back() - 1);
            } else {
                width = base;
            }
            maxRect = std::max(maxRect, height * width);
        }

        if (base < count) stack.push_back(base);
    }
    return maxRect;
}

// ===== 参考实现：区间视角 O(n^2) 枚举 [L,R] 维护最小值（结构不同，用于对拍） =====
static int bruteWindow(const std::vector<int>& h) {
    int n = static_cast<int>(h.size());
    int best = 0;
    for (int L = 0; L < n; L++) {
        int mn = INT_MAX;
        for (int R = L; R < n; R++) {
            mn = std::min(mn, h[R]);
            best = std::max(best, mn * (R - L + 1));
        }
    }
    return best;
}

// ===== 测试辅助 =====
static void printVec(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) std::cout << (i ? "," : "") << v[i];
    std::cout << "]";
}

static bool check(const std::vector<int>& in, int expect) {
    int got = largestRectHist(in);
    int gotOpt = largestRectHist_opt(in);
    int ref = bruteWindow(in);
    bool ok = (got == expect) && (gotOpt == expect) && (ref == expect);
    if (!ok) {
        std::cout << "  FAIL in="; printVec(in);
        std::cout << " expect=" << expect << " anchor=" << got
                  << " stack=" << gotOpt << " window=" << ref << "\n";
    }
    return ok;
}

int main() {
    bool ok = true;
    ok &= check({2, 1, 5, 6, 2, 3}, 10);   // 题面示例
    ok &= check({2, 4}, 4);                // 题面示例 2
    ok &= check({5, 4, 5}, 12);            // 锚在正中间（上一版翻车点）
    ok &= check({5}, 5);                   // 单柱
    ok &= check({}, 0);                    // 空（题面 n>=1，防越界行为一致即可）
    ok &= check({4, 4, 4}, 12);            // 全相等
    ok &= check({0, 0}, 0);                // 零高度
    ok &= check({1, 2, 3, 4, 5}, 9);       // 递增（3*3）
    ok &= check({5, 4, 3, 2, 1}, 9);       // 递减（3*3）
    ok &= check({2, 1, 2}, 3);             // 矮柱横贯全场
    ok &= check({6}, 6);                   // 单柱非零（尾哨兵结算）
    ok &= check({1, 2, 3, 4}, 6);          // 严格递增：答案全靠尾哨兵开庭（2×3=6）
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：锚视角 vs 区间视角
    std::mt19937 rng(84);
    std::uniform_int_distribution<int> dn(0, 30), tv(0, 6); // 窄值域 -> 大量等高柱
    bool rand_ok = true;
    for (int t = 0; t < 5000 && rand_ok; t++) {
        int n = dn(rng);
        std::vector<int> v(n);
        for (int& x : v) x = tv(rng);
        int a = largestRectHist(v), b = largestRectHist_opt(v), c = bruteWindow(v);
        if (a != c || b != c) {
            std::cout << "  RANDOM MISMATCH case " << t << " n=" << n << ":\n  ";
            printVec(v);
            std::cout << "\n  anchor=" << a << " stack=" << b << " window=" << c << "\n";
            rand_ok = false;
        }
    }
    std::cout << "random 5000 cross-check: " << (rand_ok ? "OK" : "FAILED") << "\n";
    return (ok && rand_ok) ? 0 : 1;
}
