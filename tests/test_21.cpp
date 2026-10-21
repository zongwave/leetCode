// LeetCode 21 · Merge Two Sorted Lists — 编译验证 review.md 中的 迭代 + 递归 两版
// cl /nologo /EHsc /W4 /utf-8 /std:c++17 test_21.cpp && test_21.exe
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

struct linkedList {
    int value;
    linkedList* next;

    linkedList(int v) {
        value = v;
        next = nullptr;
    }
};

// ===== review.md 原样抄录：迭代版 =====
linkedList* mergeTwoListsIter(linkedList* list1, linkedList* list2) {
    if (list1 == nullptr) return list2;
    if (list2 == nullptr) return list1;

    linkedList* dummy = new linkedList(0);
    dummy->next = (list1->value < list2->value) ? list1 : list2;
    linkedList* curr = dummy;

    while (list1 != nullptr && list2 != nullptr) {
        if (list1->value <= list2->value) {
            curr->next = list1;
            list1 = list1->next;
        } else {
            curr->next = list2;
            list2 = list2->next;
        }
        curr = curr->next;
    }
    if (list1 != nullptr) curr->next = list1;
    if (list2 != nullptr) curr->next = list2;

    return dummy->next;
}

// ===== review.md 原样抄录：递归版 =====
linkedList* mergeTwoListsRec(linkedList* list1, linkedList* list2) {
    if (list1 == nullptr) return list2;
    if (list2 == nullptr) return list1;

    if (list1->value < list2->value) {
        list1->next = mergeTwoListsRec(list1->next, list2);
        return list1;
    } else {
        list2->next = mergeTwoListsRec(list1, list2->next);
        return list2;
    }
}

// ===== 工具 =====
static linkedList* build(const std::vector<int>& v) {
    linkedList head(0), *cur = &head;
    for (int x : v) { cur->next = new linkedList(x); cur = cur->next; }
    return head.next;
}
static std::vector<int> toVec(const linkedList* h) {
    std::vector<int> r;
    for (; h; h = h->next) r.push_back(h->value);
    return r;
}
static void freeList(linkedList* h) {
    while (h) { linkedList* t = h->next; delete h; h = t; }
}
static bool eqVec(const std::vector<int>& a, const std::vector<int>& b) {
    return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin());
}

static bool checkOne(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& expect, bool& anyFail) {
    linkedList* ia = build(a), * ib = build(b);
    linkedList* recHead  = mergeTwoListsRec(ia, ib);
    std::vector<int> gotRec = toVec(recHead);
    freeList(recHead);

    linkedList* la = build(a), * lb = build(b);
    linkedList* iterHead = mergeTwoListsIter(la, lb);
    std::vector<int> gotIter = toVec(iterHead);
    freeList(iterHead);

    bool okIter = eqVec(gotIter, expect);
    bool okRec  = eqVec(gotRec, expect);
    bool cross  = eqVec(gotIter, gotRec);
    bool okAll  = okIter && okRec && cross;
    anyFail = anyFail || !okAll;
    if (!okIter) { std::cout << "  ITER FAIL: got "; for (int x : gotIter) std::cout << x << " "; std::cout << "\n"; }
    if (!okRec)  { std::cout << "  REC  FAIL: got "; for (int x : gotRec)  std::cout << x << " "; std::cout << "\n"; }
    if (!cross)  { std::cout << "  CROSS MISMATCH iter vs rec\n"; }
    return okAll;
}

int main() {
    bool ok = true;

    // 固定用例
    struct Case { std::vector<int> a, b, expect; };
    std::vector<Case> cases = {
        {{}, {}, {}},
        {{}, {0}, {0}},
        {{1}, {}, {1}},
        {{1, 3, 5}, {2, 4, 6}, {1, 2, 3, 4, 5, 6}},   // 交错
        {{1, 2, 4}, {1, 3, 4}, {1, 1, 2, 3, 4, 4}},   // 相等开头
        {{5}, {1, 2, 3}, {1, 2, 3, 5}},
        {{1}, {1}, {1, 1}},                            // 全相等
        {{1, 2, 3, 4, 5}, {6}, {1, 2, 3, 4, 5, 6}},    // 单边全小
        {{-3, -1}, {-2, 0}, {-3, -2, -1, 0}},          // 负数
        {{0, 0, 0}, {0, 0}, {0, 0, 0, 0, 0}},          // 大量重复
    };
    for (size_t i = 0; i < cases.size(); i++) {
        ok &= checkOne(cases[i].a, cases[i].b, cases[i].expect, ok);
    }
    std::cout << "fixed cases: " << (ok ? "OK" : "FAILED") << "\n";

    // 随机对拍：随机有序输入，迭代 vs 递归 结果必须一致
    std::mt19937 rng(21);
    bool randOk = true;
    int tested = 0;
    for (int t = 0; t < 2000 && randOk; t++) {
        std::vector<int> a, b;
        int na = std::uniform_int_distribution<int>(0, 20)(rng);
        int nb = std::uniform_int_distribution<int>(0, 20)(rng);
        for (int i = 0; i < na; i++) a.push_back(std::uniform_int_distribution<int>(-50, 50)(rng));
        for (int i = 0; i < nb; i++) b.push_back(std::uniform_int_distribution<int>(-50, 50)(rng));
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        // 期望 = 合并排序
        std::vector<int> expect = a;
        expect.insert(expect.end(), b.begin(), b.end());
        std::sort(expect.begin(), expect.end());

        linkedList* la = build(a), * lb = build(b);
        linkedList* ia = build(a), * ib = build(b);
        std::vector<int> gotIter = toVec(mergeTwoListsIter(la, lb));
        std::vector<int> gotRec  = toVec(mergeTwoListsRec(ia, ib));
        bool c = eqVec(gotIter, expect) && eqVec(gotRec, expect) && eqVec(gotIter, gotRec);
        if (!c) {
            std::cout << "  RANDOM FAIL case " << t << "\n";
            randOk = false;
        }
        tested++;
    }
    std::cout << "random " << tested << " cross-check: " << (randOk ? "OK" : "FAILED") << "\n";
    std::cout << "==> " << ((ok && randOk) ? "ALL PASS" : "HAS FAILURE") << "\n";
    return (ok && randOk) ? 0 : 1;
}
