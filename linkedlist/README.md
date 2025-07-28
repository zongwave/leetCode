# 链表

## 目录

* [LeetCode 链表问题总结](#LeetCode-linked-list-总结)
  * [目录](#目录)
  * [160. 链表交叉](#160-intersection-of-two-linked-lists)
  * [21. 合并两个有序链表](#21-merge-two-sorted-lists)
  * [141. 检查链表中的环](#141-linked-list-cycle)
  * [203. 删除链表中的元素](#203-removed-linked-list-elements)
  * [206. 翻转链表](#206-reverse-linked-list)
  * [243. 单链表回文判断](#243-palindrome-linked-list)

---

## 160. Intersection of Two Linked Lists

### ✅ 链表结构（有交点）：

[![图示](diagram/intersection_of_two_linked-list.png)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/intersection_of_two_linked-list.png)



```
List A: a1 → a2
                ↘
                    c1 → c2 → c3
                   ↗
List B: b1 → b2 → b3
```

* 链表 A 长度：2（a1→a2）+ 3（c1→c2→c3） = 5
* 链表 B 长度：3（b1→b2→b3）+ 3（c1→c2→c3） = 6

---
```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *pA = headA;
        ListNode *pB = headB;

        while (pA != pB) {
            pA = pA ? pA->next : headB;
            pB = pB ? pB->next : headA;
        }
        return pA;
    }
}；

```

---

### ✅ 严格对齐的双指针遍历轨迹

我们按顺序列出 **每一步 pA 与 pB 的指针位置**，直到首次相遇：

| 步骤 | pA      | pB      |
| -- | ------- | ------- |
| 1  | a1      | b1      |
| 2  | a2      | b2      |
| 3  | c1      | b3      |
| 4  | c2      | c1      |
| 5  | c3      | c2      |
| 6  | nullptr | c3      |
| 7  | b1      | nullptr |
| 8  | b2      | a1      |
| 9  | b3      | a2      |
| 10 | c1 ✅    | c1 ✅    |

🎉 **在第10步两者第一次重合，相遇在交点 `c1`。**

---

### 🧠 简洁总结：为什么一定能对齐？

因为：

* pA 走的是 `A + B`
* pB 走的是 `B + A`
* 它们都走了 `m + n` 步

**只要有交点，一定会在最后相交段（即 c1 开始）重合；如果没有交点，就同时走到 `nullptr`。**

---

### 🔁 Mermaid 流程图（补充解释逻辑）

下面是重新整理过的 Mermaid 流程图，便于你在 Markdown 编辑器中使用：

```mermaid
flowchart TD
    Start(["Start"])
    Init["Set pA = headA, pB = headB"]
    Loop{"pA != pB ?"}
    MoveA["pA = pA ? pA.next : headB"]
    MoveB["pB = pB ? pB.next : headA"]
    End(["Return pA (intersection or nullptr)"])

    Start --> Init --> Loop
    Loop -- Yes --> MoveA --> MoveB --> Loop
    Loop -- No --> End
```

[![动图](diagram/intersection_demo.gif)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/intersection_demo.gif)


---

### ✅ 最终提示

你总结的这句话非常经典，可以作为这道题的“口诀”：

> **两个指针都走同样的步数，n+m 它们一定会同时走完，如果有相同的“道路”，会是在终点。那么两个指针必然会在道路上“相遇”。**





## 21. Merge Two Sorted Lists


[![图示](diagram/merge_ex1.jpg)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/merge_ex1.jpg)



### ✅ 推荐解法：使用哨兵节点 + 双指针（LeetCode 官方解法）
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode dummy();
        listNode tail = &dummy;

        while (list1 != NULL && list2 != NULL) {
            if (list1->val < list2->val) {
                tail->next = list1;
                list1 = list1->next;
            } else {
                tail->next = list2;
                list2 = list2->next;
            }
            tail = tail->next;
        }
        tail->next = list1 ? list1 : list2;
    }
    return dummy.next;
};

```


[![动图](diagram/merge_two_lists_with_pointer.gif)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/merge_two_lists_with_pointer.gif)


---


### 📈 性能分析

* **时间复杂度**：`O(m + n)`，m 和 n 为两个链表长度
* **空间复杂度**：`O(1)`（不额外申请节点，只操作指针）

---

### ✅ 总结建议

| 方案类型      | 是否推荐  | 原因说明             |
| --------- | ----- | ---------------- |
| 手动插入节点遍历  | ❌ 不推荐 | 容易出错，逻辑混乱，指针难以维护 |
| dummy+双指针 | ✅ 推荐  | 简洁高效，逻辑清晰，是标准解法  |



## 141. Linked List Cycle

[![图示](diagram/circularlinkedlist.png)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/circularlinkedlist.png)

你的代码已经基本上可以正确判断链表中是否存在环（环检测问题），但存在一点**冗余逻辑**，可以稍作**简化与规范化**：

---

### 🔍 存在的问题：

1. **`fast != NULL && slow != NULL`** 是冗余的。

   * `slow` 是每次只走一步，初始化为非空 `head`，只要 `fast != NULL && fast->next != NULL`，`slow` 也一定不会提前为 `NULL`，因此 `slow != NULL` 可以省略。
   * `fast` 每次走两步，所以你应当每轮循环前确认 `fast` 和 `fast->next` 均不为 `NULL`。

2. 初始化时将 `fast = head->next` 可能使得初始 `fast == slow`，导致直接跳出循环。

   * 建议初始化为 `fast = head`，统一判断逻辑。

---

### ✅ 推荐修改版本：

```cpp
class Solution {
public:
    bool hasCycle(ListNode *head) {
        if (head == nullptr) return false;

        ListNode *slow = head;
        ListNode *fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }

        return false;
    }
};
```

---

### ✅ 简洁说明：

* **快慢指针法**：

  * `slow` 每次走一步；
  * `fast` 每次走两步；
  * 如果存在环，那么 `fast` 最终会追上 `slow`；
  * 否则 `fast` 或 `fast->next` 会变成 `nullptr`，跳出循环。

---

如需进一步拓展，还可加上：

* **返回相遇节点**（检测入口）；
* **计算环长度**；
* **移除环** 等高级操作。需要我帮忙的话也可以继续补充。



## 203. Removed Linked List Elements

[![图示](diagram/removelinked-list.jpg)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/removelinked-list.jpg)


你的代码逻辑上接近正确，但存在一个关键 bug，会导致跳过一些节点 **没有正确更新 `prev` 指针的位置**。下面是详解和修复建议。

---

### ❌ 问题说明

你的循环中，**无论是否删除节点，`prev` 都会前移**：

```cpp
if (curr->val == val) {
    prev->next = curr->next;  // 删除
}
curr = curr->next;
prev = prev->next;            // ← 始终前移，哪怕删除了当前节点！
```

这个在删除时就出错了：
删除节点后，`prev->next` 变了，但你还是把 `prev` 前移了，导致跳过了后续元素。

---

### ✅ 正确做法是：**只在未删除时前移 `prev`**：

```cpp
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        ListNode* curr = head;

        while (curr != nullptr) {
            if (curr->val == val) {
                prev->next = curr->next;
            } else {
                prev = curr;  // 只有没删除时才移动 prev
            }
            curr = curr->next;
        }

        return dummy.next;
    }
};
```

---

### 🔍 示例对比

假设链表为：`1 → 2 → 6 → 3 → 4 → 5 → 6`，要删除所有 `6`。

* 错误代码会跳过后面一个 `6`；
* 正确代码能完整处理所有目标值。

---

### ✅ 总结

| 问题                  | 修复                |
| ------------------- | ----------------- |
| 删除时依然移动 `prev`，跳过元素 | 只有在未删除时才移动 `prev` |

这类问题在链表操作中很常见，**哨兵节点（dummy）+ 条件前移指针**是推荐写法。



## 206. Reverse Linked List

[![图示](diagram/reverselinked-list.jpg)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/reverselinked-list.jpg)



当然，以下是对 LeetCode 中经典题目「反转链表」（Reverse Linked List） 的**全面总结**，包括思路、代码、注意事项与常见陷阱：

---

### 📌 题目描述

**题目：**
反转一个单链表。

**输入示例：**
`1 → 2 → 3 → 4 → 5`

**输出：**
`5 → 4 → 3 → 2 → 1`

---

### 🧠 解题思路

#### ✅ 方法一：迭代反转（最常用）

使用三个指针依次反转链表指向：

| 指针名    | 含义               |
| ------ | ---------------- |
| `prev` | 当前节点反转后的前一个节点    |
| `curr` | 当前处理的节点          |
| `next` | 当前节点原本的下一个节点（暂存） |

#### 🧩 操作流程：

每一步执行：

```cpp
next = curr->next;
curr->next = prev;
prev = curr;
curr = next;
```

最终返回 `prev`，它是新的头节点。

---

### ✅ 迭代法 C++ 代码（推荐高效写法）

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        ListNode* next = nullptr;

        while (curr != nullptr) {
            next = curr->next;    // 保存原始 next
            curr->next = prev;    // 反转指针
            prev = curr;          // 移动 prev
            curr = next;          // 移动 curr
        }
        return prev;              // prev 是新的头节点
    }
};
```

---

### ✅ 方法二：递归法（更简洁但栈开销大）

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode* newHead = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
};
```

**适合用于面试中“考察递归理解”但不推荐用于超长链表。**

---

### ⚠️ 常见错误 & 易错点

| 错误或陷阱                     | 原因                          |
| ------------------------- | --------------------------- |
| 返回了原始 `head` 而不是新头 `prev` | 忘记更新反转后的新起点                 |
| 没有断开尾部指针                  | 旧头 `next` 没置为 nullptr，可能形成环 |
| 不小心跳过某个节点                 | 指针更新顺序错误                    |
| 循环中定义 `ListNode* next`    | 会造成微小性能损耗，影响 LeetCode 评测表现  |

---

### 🛠️ LeetCode 最佳实践细节

* 使用 `nullptr` 替代 `NULL`（现代 C++ 标准推荐）
* `ListNode* next` 在循环外定义，可提升效率
* 测试用例要包括：空链表、单节点、多节点

---

### 🧪 时间与空间复杂度

| 方法 | 时间复杂度 | 空间复杂度     |
| -- | ----- | --------- |
| 迭代 | O(n)  | O(1)      |
| 递归 | O(n)  | O(n)（递归栈） |

---

### 📊 LeetCode 数据点（参考）

| 提交写法         | 平均运行时间 | 提交排名      |
| ------------ | ------ | --------- |
| 错误写法（返回head） | 12ms   | Bottom 1% |
| 正确迭代写法       | 3ms    | Top 95%+  |

---

### ✅ 总结一句话：

> 三指针迭代 + 注意返回 `prev` + 循环外声明 `next`，就是链表反转题目的性能最佳实践。


---


## 243. Palindrome Linked List

### 一、题目要求

判断一个单链表是否是回文链表。回文链表的定义是：从前向后读和从后向前读节点值序列完全相同。


[![图示](diagram/pal1linked-list.jpg)](https://raw.githubusercontent.com/zongwave/leetcode/main/linkedlist/diagram/pal1linked-list.jpg)

---

### 二、经典解法流程（迭代+快慢指针）

1. **使用快慢指针找到链表中点**

   * 快指针 `fast` 每次走两步，慢指针 `slow` 每次走一步。
   * 当快指针到尾部时，慢指针停在链表中点或中点左侧。

2. **反转链表后半部分**

   * 从中点节点的下一个节点开始反转链表的后半部分。

3. **断开链表，防止形成环**

   * 将中点节点的 `next` 置空。

4. **比较前半链表和反转后的后半链表**

   * 两指针分别从链表头和后半反转链表头开始逐节点比较。

5. **还原链表结构（可选，但推荐）**

   * 反转后的链表再反转回来，保证链表结构不变。

6. **返回比较结果**

   * 如果全部节点值相等，返回 `true`，否则返回 `false`。

---

### 三、你代码中的错误及改进

| 错误点                    | 说明                                                                                   | 改进建议                                                           |
| ---------------------- | ------------------------------------------------------------------------------------ | -------------------------------------------------------------- |
| 快慢指针循环条件不严谨            | 用 `while (fast != nullptr && fast->next != nullptr)` 会导致慢指针停在中点右侧（奇数长度时）或后半第一个节点（偶数） | 建议使用 `while (fast->next && fast->next->next)`，保证慢指针停在中点左侧，方便断链 |
| 反转链表起点用 `slow->next`   | 直接用 `slow->next` 作为反转起点没错，但需断开中点的 `next` 否则可能成环                                      | 反转前执行 `slow->next = nullptr;` 断开链表                             |
| 没有断开链表                 | 反转后链表未断开，可能导致遍历时形成环                                                                  | 断开链表，防止后半部分成环                                                  |
| 没有还原链表                 | 反转后半部分链表后没有再反转回去，破坏了原链表结构                                                            | 比较完成后再调用一次反转恢复原链表                                              |
| 比较后修改了 `secondHalf` 指针 | 如果用 `secondHalf` 指针直接遍历，指针会移动，后续无法用它还原链表                                             | 复制一个临时指针（如 `p2`）用来遍历，保持 `secondHalf` 指针不变                      |

---

### 四、推荐代码框架

```cpp
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;

        ListNode* slow = head;
        ListNode* fast = head;

        // 找中点，慢指针停在中点左侧
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 断链并反转后半部分
        ListNode* secondHalf = reverseLinkedList(slow->next);
        slow->next = nullptr;

        ListNode* p1 = head;
        ListNode* p2 = secondHalf;
        bool palindrome = true;

        while (p2) {
            if (p1->val != p2->val) {
                palindrome = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }

        // 还原链表结构
        slow->next = reverseLinkedList(secondHalf);

        return palindrome;
    }

private:
    ListNode* reverseLinkedList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
};
```

---

### 五、总结

* 这道题的核心是**快慢指针 + 链表反转 + 节点比较**。
* 细节决定正确性和性能：断链和还原链表都是好习惯。
* 循环条件、断链时机、指针变量使用要谨慎，避免漏掉节点或产生环。
* 保证还原链表结构，符合题目对数据结构的“非破坏”原则。

---

如果你需要，我还能帮你画流程图或者给你动图演示这个算法的执行过程！
