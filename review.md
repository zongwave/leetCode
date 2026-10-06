# LeetCode 刷题复习笔记

## 目录
1. [Two Sum](#two-sum)
2. [Valid Anagram](#valid-anagram)
3. [26. Remove Duplicates from Sorted Array](#26-remove-duplicates-from-sorted-array)
4. [Valid Palindrome](#valid-palindrome)
5. [Move Zeroes](#move-zeroes)
6. [Container with Most Water](#container-with-most-water)
7. [Longest Substring Without Repeating Characters](#longest-substring)
8. [Merge Intervals](#merge-intervals)
9. [Maximum Depth of Binary Tree](#maximum-depth-of-binary-tree)
10. [Invert Binary Tree](#invert-binary-tree)
11. [Binary Tree Right Side View](#binary-tree-right-side-view)
12. [Path Sum](#path-sum)

---

# Two Sum

## 思路
 - 双层循环遍历数组，符合要求提前跳出循环。
## 复杂度
 - 空间 O(1)，时间 O(n^2)

## 哈希表优化
 - 空间复杂度 O(n), 时间复杂度 O(n)
 - 哈希键值是用来查询的，因此键值是数组的元素，需要查询到满足要求的数组下标，哈希表的值是数组下标。
```cpp

std::vector<int> twoSum(std::vector<int>& nums, int target) {
    int count = nums.size();
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j ++) {
            if (nums[i] + nums[j] == target) {
                return {i, j};
            }
        }
    }
    return {};
}


std::vector<int> twoSum_hash(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> num_map;

    for (int i = 0; i < nums.size(); i++) {
        auto it = num_map.find(target - nums[i]);
        if (it != num_map.end()) {
            return {i, it->second};
        } else {
            num_map[nums[i]] = i;
        }
    }
    return {};
}

std::vector<int> twoSum_count(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> num_map;

    for (int i = 0; i < nums.size(); i++) {
        int v = target - nums[i];
        if (num_map.count(v)) {
            return {i, num_map[v]};
        } else {
            num_map[nums[i]] = i;
        }
    }
    return {};
}

```

# Valid Anagram

## 思路
 - 暴力解法，两层循环，内层循环如果找不到，直接返回 false
 - 内层循环找到了，外层循环递进。但上一轮找到的重复字符如何处理？
 - 提示说用数组表示 s 和 t，可以理解为遍历 s 一轮可以构造一个直方图，重复字符的 bin 数值大于 1。如果在 t 中找到一次就减一次。遍历结束后，s 直方图全为 0，就可以返回 true.


## 复杂度
 - 暴力解法：时间 O(n^2)，空间 O(1)，可以不额外分配数组空间，直接遍历输入数组。
 - 直方图解法: 时间 O(n)，空间 O(1) 直方图解法分配了固定的数组大小，不随 n 动态变化，因此是 O(1)

``` cpp
bool validAnagram(std::string& s, std::string& t) {
    if (s.size() != t.size()) {
        return false;
    }

    int src[26] = {0};
    for (char c: s) {
        src[c - 'a'] ++;
    }
    for (char c: t) {
        if (src[c - 'a'] != 0) {
            src[c - 'a'] --;
        }
    }
    for (int i = 0; i < 26; i++) {
        if (src[i] != 0) {
            return false;
        }
    }
    return true;
}

```

# 26. Remove Duplicates from Sorted Array

给一个非严格递增排序的整数数组 nums，请你原地删除重复出现的元素，使每个元素只出现一次，并返回删除后数组的新长度 k。
要求：
- 不要使用额外数组空间，必须在原数组上修改。
- 删除后，nums 的前 k 个位置应当保存的是唯一的那些元素（顺序与原数组一致）。
- 最终返回 k。

输入: nums = [0,0,1,1,1,2,2,3,3,4]
输出: k = 5，且 nums 前 5 位为 [0,1,2,3,4]

## 思路
 - 使用 front / end 两个指针，一次遍历数组
 - 移动 end 指针，与 front 指针对比，相同继续移动 end 指针，直到不同，把 end 指针之后的所有数组都整体移动到 front 指针之后。
 - end 指针遍历完成，任务结束。

## 修正
 - 不需要把 end 指针之后的所有元素都整体搬移，只需要复制当前 end 指针的一个元素到 front 之后，同时移动 front 指针。end 指针的位置不往前移动。

## 复杂度
 空间复杂度：O(1) 原位操作。时间复杂度：end 指针一次遍历 O(n)

```cpp
int removeDuplicates(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    int front = 0;
    int end = front + 1;

    for (; end < nums.size(); end++) {
        if (nums[end] != nums[front]) {
            nums[++front] = nums[end];
        }
    }
    return front + 1;
}

```

# Valid Palindrome

给定一个字符串 s，判断它是否是回文串。规则：
 - 只考虑字母和数字，忽略其他字符（标点、空格等）。
 - 忽略大小写（A 和 a 视为相同）。
 - 如果是回文返回 true，否则返回 false。

## 思路
 - 双指针，初始化一头 head，一尾 tail，一次遍历，相向移动。
 - while 循环跳过空格，标点，只比较字母和数字。其它特殊字符如何处理？
 - 比对出现不同立即跳出遍历，返回 false。

## 复杂度
  空间复杂度：没有额外引入空间分配 O(1)。
  时间复杂度：一次遍历 O(n).

```cpp

bool validPalindrome(std::string& s) {
    int head = 0;
    int tail = s.size() - 1;

    while (head < tail) {
        while (head < tail && !isalnum(s[head])) {
            head ++;
        }
        while (head < tail && !isalnum(s[tail])) {
            tail --;
        }
        if (tolower(s[head]) != tolower(s[tail])) {
            return false;
        }
        head ++;
        tail --;
    }

    return true;
}


```

# Move Zeroes

## 思路
 - 把数组中的非零数字向数组头部移动。
 - 两个指针，一个指针指向非零数填满的位置，另一个指针遍历数组。

## 复杂度
 - 空间复杂度，无需额外引入存储空间。O(1)
 - 时间复杂度，遍历一次数组。O(n)

```cpp

void moveZeroes(std::vector<int>& nums) {
    int head = 0;
    int tail = head;

    while (tail < nums.size()) {
        while (tail < nums.size() && nums[tail] == 0) {
            tail ++;
        }
        if (tail < nums.size()) {
            std::swap(nums[head], nums[tail]);
            head ++;
            tail ++;
        }

    }
}

void moveZeroes_for(std::vector<int>& nums) {
    int head = 0;

    for (int tail = 0; tail < nums.size(); tail++) {
        if (nums[tail] !=0) {
            if (head != tail) {
                std::swap(nums[head], nums[tail]);
            }
            head ++;
        }
    }
}


```

# Container with Most Water

## 思路
 - 按照矩形面积计算，暴力解法两层循环，算出所有 “上底”，“下底” 与 “高” 的组合。


## 复杂度
 - 暴力法：时间 O(n^2)，空间未引入新分配空间 O(1)


```cpp

int mostWater(std::vector<int>& height) {

    int max = 0;
    for (int left = 0; left < height.size() - 1; left ++) {
        for (int right = left + 1; right < height.size(); right++) {
            int water = std::min(height[right], height[left]) * (right - left);
            if (water > max) max = water;
        }
    }
    return max;
}

int mostWater_opt(std::vector<int>& height) {
    int left = 0;
    int right = height.size() - 1;
    int max = std::min(height[left], height[right]) * (right- left);

    while (left < right) {
        if (height[left] < height[right]) {
            left ++;
        } else {
            right --;
        }
        int water = std::min(height[left], height[right]) * (right- left);
        if (water > max) max = water;
    }
    return max;
}

```

# longest substring

## 思路
 - 暴力法：两个指针，left 和 right，双层循环遍历字符串。指针之间没有重复字符。记下最大字符数量。
 - 暴力法的难度在于如何标记右指针遍历过的字符？所有字母都用一个 bool 变量来标记。这里用一个 bool seen[128] 来做这个标记。128 长度足够英文字符。
 - 滑动窗口：不用双层遍历，left 和 right 指针之间始终保持没有重复字符，记下最大字符数量。

## 复杂度
 - 暴力法：空间复杂度，没有额外存储分配， O(1)。时间复杂度 O(n^2)
 - 滑动窗口：空间复杂度，无额外存储分配，O(1)。时间复杂度，一次遍历 O(n)。

```cpp

int longestSubstring(std::string& s) {
    int max = 0;
    int n = (int)s.size();

    for (int left = 0; left < n; left++) {
        bool seen[128] = { false };

        for (int right = left; right < n; right++) {
            if (seen[s[right]]) break;

            seen[s[right]] = true;
            max = std::max(max, right - left + 1);
        }
    }
    return max;
}


int longestSubstring_opt(std::string& s) {
    int max = 0;
    int n = (int)s.size();
    int left = 0;
    int seen[128] = { 0 };

    for (int right = left; right < n; right++) {
        char c = s[right];
        seen[c]++;
        while (seen[c] > 1) {
            seen[s[left]]--;
            left++;
        }
        max = std::max(max, right - left + 1);
    }
    return max;
}

```

# Merge Intervals

## 思路
 - 首先用二元组的第一个元素 l 对二元组排序，intervals = [[l0, r0], [l1, r1], ...]
 - 比较当前二元组的 c_r 与下一个二元组的 n_l 和 n_r，若 n_l 大，则移到下一个二元组，merge count 归零。若 n_l 小或者相等，比较 c_r 与 n_r，c_r = max(c_r, n_r)。 merge count 加 1。记录 max merge count，同时更新合并 interval

## 复杂度
 - 空间：不额外分配空间 O(1)
 - 时间：排序加一次遍历，一个指针指向当前二元组，另一个指针指向后面的对比二元组。O(n*logn)

 ```cpp

std::pair<int, int> mergeIntervals(std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) return {0, 0}

    int max_merged = 0;
    int count = 0;

    std::sort(intervals.begin(), intervals.end());
    std::pair<int, int> merged = intervals[0];
    std::pair<int, int> temp = merged;

    for (int cur = 0; cur < intervals.size() - 1; cur++) {

        if (temp.second >= intervals[cur + 1].first) {
            temp.second = std::max(temp.second, intervals[cur + 1].second);
            count ++;
            if (count > max_merged) {
                merged = temp;
            }
            max_merged = std::max(max_merged, count);
        } else {
            temp = intervals[cur + 1];
            count = 0;
        }
    }
    return merged;
}

 ```

 # Reverse Linked List

 ## 思路
  - 整个反转操作在原链表上进行。
  - 需要一个当前节点遍历链表，一个前节点用来保存链接换位，一个临时节点进行周转。

## 复杂度
  - 空间：O(1) 原链表执行
  - 时间：O(n) 一次遍历

```cpp
struct linkedList {
    int value;
    linkedList* next;

    linkedList(int v) {
        value = v;
        next = nullptr;
    }
};

linkedList* reverseLinkedList(linkedList* list) {
    if (list == nullptr || list->next == nullptr) return list;

    linkedList* cur = list;
    linkedList* prev = nullptr;

    while (cur) {
        linkedList* temp = cur->next;
        cur->next = prev;
        prev = cur;
        cur = temp;
    }

    return prev;
}

```

# Maximum Depth of Binary Tree

## 思路
 - 递归方法，入口检查如果是空叶子，返回 0.
 - 左右平等递归统计。每层有效递归，左与右大数层数累加 1.

## 复杂度
 - 空间 O(n) 递归调用栈的最大深度
 - 时间 O(n) 一次遍历

```cpp

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};


int maximumDepth(TreeNode* tree) {
    if (tree == nullptr) return 0;
    int left = maximumDepth(tree->left);
    int right = maximumDepth(tree->right);

    return 1 + std::max(left, right);
}

```

# Invert Binary Tree

## 思路
 - 需要向左右方向对称递归遍历树。
 - 原树遍历。

## 复杂度
 - 空间：需要全树递归遍历，O(n)
 - 时间：递归遍历一次，O(n)

```cpp
void invertBinaryTree(TreeNode* tree) {
    if (tree == nullptr) return;

    std::swap(tree->left, tree->right);
    invertBinaryTree(tree->left);
    invertBinaryTree(tree->right);
}

```

# Binary Tree Right Side View

## 思路
 - 按照提示，用层序遍历 (BFS) 右边节点是本层最右边的节点，但是如何找出同一层最右节点？用一个层号？
 - 按照提示把同一层的所有节点按序入队列
 - 层遍历不知怎么写。这道题直觉是先查找右侧叶子，如果右侧叶子不存在，左侧叶子也能从右边观察到，所以也必须从右往左检查。必须要用队列吗？
 - 层序遍历 (BFS) 两层循环，外层检查队列中是否还有节点，内层遍历当前层全部的节点。第一层 root 在 while 循环外加入队列。之后在 for 循环中处理当前层时可以把下一层的节点加入队列 push。因为 for 循环遍历的节点个数 n 是在进入循环之前通过检查队列大小得到的，因此可以保证每个 for 循环只遍历一层中的节点数。注意在每个 for 循环中，处理完的节点 front 应当及时 pop 从队列中清除。只有这样才能保证下一个 for 循环能够正确处理一整层的节点。

## 复杂度
 - 时间：按层遍历，n 层，O(n)
 - 空间：队列里加入节点

```cpp

std::vector<int> rightSideView(TreeNode* root) {
    std::vector<int> res;
    if (root == nullptr) return res;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int n = static_cast<int>(q.size());
        int rightMost = 0;
        
        for (int i = 0; i < n; i++) {
            TreeNode* node = q.front();
            q.pop();
            rightMost = node->val;

           if (node->left) q.push(node->left);
           if (node->right) q.push(node->right);
        }
        res.push_back(rightMost);
    }
    return res;
}

```

# Path Sum

# 思路
 - 采用二叉树的 DFS （深度遍历）
 - 遍历过程中累加数值，检查是否最终累加到尾部的和是否等于 target
 - 遍历过程用 target 减去当前值的方式更加简洁。
 - 主函数检查 root nullptr 还不够，需要在 dfs 递归函数中每次调用都检查。因为在 dfs 中只是对 left && right 同时为 nullptr 进行了处理，只有一方为 nullptr 的情况并没有处理。
 - 递归函数调用栈会对传入的参数在各自的调用栈独立处理，不会被混淆。因此值传递是安全的。如果传递引用，逻辑会变复杂，需要在每个调用栈分岔处对数值变量进行分身处理。

# 复杂度
 - 空间，如果用递归，那么最坏情况需要 n 层调用栈 O(n)
 - 时间，n 层递归 O(n)

 ```cpp

bool dfs(TreeNode* node, int target_val) {
    if (node == nullptr) return false;
    target_val -= node->val;

    if (node->left == nullptr && node->right == nullptr) {
        return target_val == 0;
    }
    return dfs(node->left, target_val) || dfs(node->right, target_val);
}

bool pathSum(TreeNode* root, int target_val) {
    if (root == nullptr) {
        return false;
    }
    return dfs(root, target_val);
}

 ```