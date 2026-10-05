
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