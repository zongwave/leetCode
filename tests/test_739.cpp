// LeetCode 739 · Daily Temperatures — 编译验证 review.md 中的两份实现
// 用法（Developer PowerShell 或经 vcvars64 初始化后的环境）：
//   cl /EHsc /W4 /std:c++17 test_739.cpp && test_739.exe
#include <vector>
#include <iostream>
#include <random>

// ===== review.md 中的暴力法（原样抄录） =====
std::vector<int> dailyTemperatures(const std::vector<int>& temp) {
    int count = static_cast<int>(temp.size());

    std::vector<int> answer(count, 0);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (temp[j] > temp[i]) {
                answer[i] = j - i;
                break;
            }
        }
    }
    return answer;
}

// ===== review.md 中的单调栈法（原样抄录） =====
std::vector<int> dailyTemperaturesOpt(const std::vector<int>& temp) {
    int count = static_cast<int>(temp.size());

    std::vector<int> answer(count, 0);
    std::vector<int> index;
    for (int i = 0; i < count; i++) {
        while (!index.empty() && temp[index.back()] < temp[i]) {
            int day = index.back();
            answer[day] = i - day;
            index.pop_back();
        }
        index.push_back(i);
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
    auto got = dailyTemperaturesOpt(in);
    auto gotBrute = dailyTemperatures(in);
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
    ok &= check({73,74,75,71,69,72,76,73}, {1,1,4,2,1,1,0,0});  // 题面示例
    ok &= check({}, {});                                        // 空数组
    ok &= check({73}, {0});                                     // 单元素
    ok &= check({73,73}, {0,0});                                // 相等不弹
    ok &= check({73,72,71}, {0,0,0});                           // 一路递减
    ok &= check({71,72,73}, {1,1,0});                           // 一路递增
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：单调栈 vs 暴力
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dn(0, 40), tv(30, 34); // 窄值域 -> 大量相等边界
    bool rand_ok = true;
    for (int t = 0; t < 5000 && rand_ok; t++) {
        int n = dn(rng);
        std::vector<int> v(n);
        for (int& x : v) x = tv(rng);
        if (dailyTemperatures(v) != dailyTemperaturesOpt(v)) {
            std::cout << "  RANDOM MISMATCH case " << t << " n=" << n << ":\n  ";
            printVec(v); std::cout << "\n";
            rand_ok = false;
        }
    }
    std::cout << "random 5000 cross-check: " << (rand_ok ? "OK" : "FAILED") << "\n";
    return (ok && rand_ok) ? 0 : 1;
}
