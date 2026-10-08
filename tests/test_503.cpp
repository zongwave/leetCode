// LeetCode 503 · Next Greater Element II — 编译验证 review.md 中的两份实现
// cl /nologo /EHsc /W4 /utf-8 /std:c++17 test_503.cpp && test_503.exe
#include <vector>
#include <iostream>
#include <random>

// ===== review.md 暴力法（原样抄录） =====
std::vector<int> nextGreaterElement(const std::vector<int>& nums) {
    int count = static_cast<int>(nums.size());
    std::vector<int> answer(count, -1);

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; (j % count) != i; j++) {
            if (nums[j % count] > nums[i]) {
                answer[i] = nums[j % count];
                break;
            }
        }
    }
    return answer;
}

// ===== review.md 单调栈法（原样抄录） =====
std::vector<int> nextGreaterElement_opt(const std::vector<int> nums) {
    int count = static_cast<int>(nums.size());
    std::vector<int> answer(count, -1);
    std::vector<int> stack;

    for (int i = 0; i < 2 * count - 1; i++) {
        while (!stack.empty() && nums[stack.back()] < nums[i % count]) {
            answer[stack.back()] = nums[i % count];
            stack.pop_back();
        }
        if (i < count) stack.push_back(i);
    }
    return answer;
}

// ===== 测试辅助 =====
static void printVec(const std::vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); i++) std::cout << (i ? "," : "") << v[i];
    std::cout << "]";
}

static bool check(const std::vector<int>& in, const std::vector<int>& expect) {
    auto got = nextGreaterElement_opt(in);
    auto gotBrute = nextGreaterElement(in);
    bool ok = (got == expect) && (gotBrute == expect);
    if (!ok) {
        std::cout << "  FAIL in="; printVec(in);
        std::cout << " expect="; printVec(expect);
        std::cout << " stack="; printVec(got);
        std::cout << " brute="; printVec(gotBrute);
        std::cout << "\n";
    }
    return ok;
}

int main() {
    bool ok = true;
    ok &= check({1,2,1}, {2,-1,2});            // 题面示例
    ok &= check({5}, {-1});                    // 单元素
    ok &= check({1,1,1}, {-1,-1,-1});          // 全相等
    ok &= check({1,2,3,4}, {2,3,4,-1});        // 最大值绕圈无解
    ok &= check({5,4,3,2,1}, {-1,5,5,5,5});    // 全靠绕回开头
    ok &= check({3,1,2}, {-1,2,3});            // 混合：头部最大
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：单调栈 vs 暴力
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dn(0, 20), tv(1, 6); // 窄值域 -> 相等相邻多
    bool rand_ok = true;
    for (int t = 0; t < 5000 && rand_ok; t++) {
        int n = dn(rng);
        if (n == 0) continue;                  // 暴力法 j%count 会除零，题面保证 n>=1
        std::vector<int> v(n);
        for (int& x : v) x = tv(rng);
        if (nextGreaterElement(v) != nextGreaterElement_opt(v)) {
            std::cout << "  RANDOM MISMATCH case " << t << " n=" << n << ":\n  ";
            printVec(v); std::cout << "\n";
            rand_ok = false;
        }
    }
    std::cout << "random 5000 cross-check: " << (rand_ok ? "OK" : "FAILED") << "\n";
    return (ok && rand_ok) ? 0 : 1;
}
