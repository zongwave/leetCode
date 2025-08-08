
<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>


# LeetCode 75 专题总结

## 核心解题原则


---

## 题目

* [Math 数学算法总结](#LeetCode-75-总结)
  * [目录](#目录)
  * [1768. 交替合并字符串](#1768-merge-strings-alternately)
  * [1071. 最大公约数](#1071-greatest-common-divisor-of-strings)
  * [605. 能种花吗](#605-can-place-flowers)
  * [283. 移动零](#283-move-zeroes)
  * [643. 最大平均字串](#643-maximum-average-subarray-i)
  * [1732. 找到最大高度](#1732-find-the-highest-altitude)
  * [206. 反转链表](#206-reverse-linked-list)
  * [374. 猜数大小](#374-guess-number-higher-or-lower)
  * [1207. 唯一重复数字](#1207-unique-number-of-occurrences)


---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

## 1768. Merge Strings Alternately

你的解答是 **LeetCode 1768. 交替合并字符串（Merge Strings Alternately）** 的标准解法之一，代码简洁高效，符合题目要求。我来分析一下你的解答质量，并给出优化建议。

---

### **你的解答分析**
### **优点 ✅**
1. **逻辑清晰**：
   - 使用 `for` 循环交替添加字符，直到较短字符串结束。
   - 用 `substr(i)` 处理剩余字符，避免了手动循环。
   - 三元运算符 `?:` 使代码更紧凑。

2. **时间复杂度最优**：
   - **O(max(n, m))**，其中 `n` 和 `m` 分别是 `word1` 和 `word2` 的长度。
   - 只需要遍历一次字符串，没有冗余操作。

3. **空间复杂度合理**：
   - **O(n + m)**，因为要存储合并后的字符串。

4. **边界条件处理正确**：
   - 当一个字符串比另一个长时，能正确追加剩余部分。

---

### **可能的优化建议 🔧**
1. **使用 `reserve` 提前分配空间**（小优化）：
   - `res` 的大小是 `word1.size() + word2.size()`，可以提前分配空间避免多次扩容：
   ```cpp
   string res;
   res.reserve(word1.size() + word2.size()); // 提前分配空间
   ```

2. **用 `while` 循环替代 `for` 循环**（个人风格）：
   - 有些人更喜欢 `while` 循环处理双指针：
   ```cpp
   while (i < word1.size() && i < word2.size()) {
       res.push_back(word1[i]);
       res.push_back(word2[i]);
       i++;
   }
   ```

3. **直接 `append` 剩余部分**（可读性稍好）：
   - 可以不用三元运算符，直接 `append`：
   ```cpp
   if (i < word1.size()) res.append(word1.substr(i));
   if (i < word2.size()) res.append(word2.substr(i));
   ```

---

### **优化后的代码**
```cpp
class Solution {
public:
    string mergeAlternately(string word1, string word2) {
        string res;
        res.reserve(word1.size() + word2.size()); // 预分配空间
        
        int i = 0;
        while (i < word1.size() && i < word2.size()) {
            res.push_back(word1[i]);
            res.push_back(word2[i]);
            i++;
        }
        
        // 追加剩余部分
        if (i < word1.size()) res.append(word1.substr(i));
        if (i < word2.size()) res.append(word2.substr(i));
        
        return res;
    }
};
```


---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

### **与其他方法的对比**
| 方法 | 时间复杂度 | 空间复杂度 | 代码简洁性 | 适用场景 |
|------|------------|------------|------------|----------|
| **你的方法** | O(n+m) | O(n+m) | ⭐⭐⭐⭐ | 标准解法 |
| **优化后（预分配空间）** | O(n+m) | O(n+m) | ⭐⭐⭐ | 更高效 |
| **双指针（不依赖 `substr`）** | O(n+m) | O(n+m) | ⭐⭐ | 面试官可能要求不用 `substr` |

---

### **总结**
你的解法已经是 **最优解**，符合题目要求，代码清晰。  
- 如果面试官允许使用 `substr`，你的方法完全没问题。  
- 如果要求不用 `substr`，可以改用双指针逐个追加剩余字符。  

**评分：⭐️⭐️⭐️⭐️⭐️（5/5）**  
✅ 正确性 ✔️  
✅ 高效性 ✔️  
✅ 可读性 ✔️  

继续刷题，加油！ 🚀


---

## 1071. Greatest Common Divisor of Strings

---

这是一道考察字符串结构规律与最大公约数结合的题目，要求找出能同时整除两个字符串并构成它们的最大重复因子子串。

你在这道题「字符串的最大公因子」（Leetcode 1071）上的思考与演进，展现出一个**从误区出发 → 理解问题本质 → 尝试暴力法 → 学会高效算法 → 理解欧几里得背后直觉**的清晰学习曲线。下面是对你整个过程的分析与总结。

---

### ✘ 第一版错误解答：基于字符频率的错误假设

### 🔍 错误核心：

你试图通过统计字符频率（`unordered_map<char, int>`）来推断两个字符串是否有相同的因子字符串。你假设：

* 如果两个字符串中字符组成相似、频率比例一致，那么它们可能有公共的因子子串。
* 最后尝试根据 `map` 推断出公共因子。

### ⚠️ 错误原因：

1. **字符频率 ≠ 子串结构**

   * 例如："ABABAB" 和 "AB" 有公共因子 "AB"，但频率 map 是 `{A: 3, B: 3}` vs `{A: 1, B: 1}`，不能直接推断结构。
   * 反例："ABAB" 和 "AABB"，字符频率一样，但结构完全不同。

2. **字符串因子需要保留顺序**

   * 子串因子不是无序袋子问题，而是「是否重复组成」的问题。
   * 所以必须比较**前缀是否能周期性拼接得到两个字符串**，而不是字符是否出现一样多。

### ✅ 教训总结：

> “字符统计”适合无序判断，但“公共因子字符串”要求结构和顺序一致。

---

### ✔️ 第二版暴力解法：从结构出发，逐步接近正解

### ✅ 正确点：

1. 使用 `str1 + str2 != str2 + str1` 提前判断两个字符串是否存在公共因子。

   * 这是关键剪枝！如果不能互相整除拼接，肯定没公共因子。

2. 枚举 `i = 1..min(len1, len2)` 来找所有可能作为因子的长度。

   * 并选出能同时整除两个字符串长度的最大值。

### ⚠️ 可改进点：

* 暴力枚举效率低。时间复杂度是 $O(\min(n, m))$
* 如果字符串很长会超时。

---

### 🧠 最终改进 + 欧几里得算法引入：


```cpp

class Solution {
public:
    string gcdOfStrings(string str1, string str2) {
        if (str1.empty() || str2.empty()) {
            return "";
        }
        if (str1 + str2 != str2 + str1) {
            return "";
        }

        int gcdLen = gcd(str1.size(), str2.size());

        return str1.substr(0, gcdLen);
    }

private:
    int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    int gcd_recu(int a, int b) {
        return b ? gcd_recu(b, a % b) : a;
    }
}


```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

你在和我讨论中发现：

* 两个字符串如果存在一个公共因子字符串 $X$，那么 $str1 = X + X + X + ...$，$str2 = X + X + ...$
* 所以公共因子字符串的长度必须是 $\gcd(\text{len1}, \text{len2})$

通过欧几里得算法：

```cpp
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}
```

我们可以一步求出这个长度，直接返回：

```cpp
return str1.substr(0, gcd(len1, len2));
```

### 🎉 优点：

* 逻辑严谨，源自数学定理
* 时间复杂度降低到 $O(\log n)$
* 程序结构清晰，思路清楚

---

### 📌 欧几里得算法直观意义 + 和你类比图的关系

你用“装糖分袋”的方式理解欧几里得算法，非常成功地将“数论”转化为“实际操作”：

* 小明有 48 颗糖，小强有 18 颗糖
* 想找到每袋糖颗数，使得两人都可以**分成整袋**
* 最后找到：每袋装 6 颗糖是最合理的（GCD）

[![分糖](diagram/1071-gcd.png)](https://raw.githubusercontent.com/zongwave/leetcode/main/top75/diagram/1071-gcd.png)


这就是：

> GCD = 能整除两个总量的**最大粒度单位**

用在字符串上也是类似的：

> GCD(字符串) = 能整除两个字符串长度，且构成字符串结构的最大重复单元

---

### 🧾 总结：你在这题中学习的知识链条

| 阶段  | 做法          | 收获                |
| --- | ----------- | ----------------- |
| 初版  | 字符频率统计      | 错误模型 → 理解结构重要性    |
| 暴力版 | 枚举所有可能因子长度  | 正确思路但效率低          |
| 最终  | 欧几里得 + 拼接判断 | 数学建模能力大提升，思路简洁高效  |
| 类比图 | 分糖袋模型解释 GCD | 数学与现实直觉联结，帮助记忆与讲解 |

---

如果你愿意，这道题完全可以写进你的算法学习笔记，作为一个经典案例：**从误区到理解，从暴力到高效**。也非常适合用作教学内容。

如果你想进一步扩展这道题，比如处理多个字符串的 GCD、或者定义「字符串最小公倍数」，我也可以帮你引导。是否继续？

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>
---

## 605. Can Place Flowers


你在解决 LeetCode 的「Can Place Flowers」（种花问题）这道题的过程中，经历了 **逐步优化、深入理解边界处理与编程习惯** 的完整解题流程。以下是对你整个解答过程的总结：

---

### 🌼 题目简述：

给定一个 0/1 数组 `flowerbed`，表示花坛中每个位置是否已有花（1 表示有，0 表示无），要求判断是否能再种下 `n` 朵花，**前提是不能在相邻位置种花**。

---

### 🧭 解题过程回顾：

### **Step 1：首次尝试——逻辑复杂的区段扫描法**

你最初尝试通过一段段连续 0 区域统计可以种的花朵数，示例代码如下：

```cpp
while (left < flowerbed.size()) {
    // 处理开头、结尾为 0 的特殊情况
    // 统计连续 0 的数量并据此计算能种几朵花
}
```

#### ✅ 优点：

* 理解了“每两个 0 可以种一朵花”的本质。

#### ⚠️ 问题：

* 逻辑复杂（手动统计 count），容易出错。
* 可能访问越界，尤其是 `flowerbed[left++]` 写法不够安全。
* 修改代码难度较大，调试困难。

---

### **Step 2：引入邻接状态判断，简化逻辑**

你采用了更加清晰的写法，判断当前点及其左右是否为 0：

```cpp
for (int i = 0; i < plots; i++) {
    if (flowerbed[i] == 0) {
        bool emptyLeft = (i == 0) || (flowerbed[i - 1] == 0);
        bool emptyRight = (i == plots - 1) || (flowerbed[i + 1] == 0);
        if (emptyLeft && emptyRight) {
            flowerbed[i] = 1;
            plants++;
        }
    }
}
```

#### ✅ 优点：

* 简洁明了，逻辑上更健壮。
* 有效处理了左右边界（通过 `(i == 0)` 等判断）。
* 更贴近面试中期望的写法。

#### ⚠️ 问题：

* 修改了原数组 `flowerbed`，**不可重用**。

---

### **Step 3：进一步优化 —— 不修改原数组**

你提出了关键问题：“如何**不修改 flowerbed** 实现？”

于是你引入了模拟修改的方式：

```cpp
if (flowerbed[i] == 0) {
    bool emptyLeft = (i == 0) || (flowerbed[i - 1] == 0);
    bool emptyRight = (i == size - 1) || (flowerbed[i + 1] == 0);
    if (emptyLeft && emptyRight) {
        plants++;
        i++;  // 跳过下一个位置，相当于“虚拟种下”
    }
}
```

#### ✅ 优点：

* 保持 flowerbed 不变，**无副作用**。
* 在工程实践中更安全、更通用。
* 空间复杂度仍为 O(1)，没有新建数组。

---

### **Step 4：思考与比较**

你主动提出了对比问题：

> “你觉得修改 flowerbed 的方式和不修改的方式哪个更好？”

这表明你已经不只是写出能跑通的代码，而是在进行**工程规范与编程哲学的思考**。

---

### 📌 总结：

你展示了解题的四个关键能力：

| 能力维度       | 表现                                             |
| ---------- | ---------------------------------------------- |
| **算法理解力**  | 能理解邻接限制的本质并设计出不同策略来统计种花数量。                     |
| **边界处理能力** | 精准处理了数组头尾位置的越界问题（使用 `i == 0` 或 `i == size-1`）。 |
| **代码优化意识** | 从冗长逻辑向清晰写法迭代，并避免副作用。                           |
| **工程思维**   | 主动思考是否应该修改原始输入数组，体现了高级编程素养。                    |

---

### ✅ 推荐写法（不修改 flowerbed）：

```cpp
class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        int plots = flowerbed.size();
        int count = 0;

        for (int i = 0; i < plots; i++) {
            if (flowerbed[i] == 0) {
                bool emptyLeft = (i == 0) || (flowerbed[i - 1] == 0);
                bool emptyRight = (i == plots - 1) || (flowerbed[i + 1] == 0);

                if (emptyLeft && emptyRight) {
                    i++;
                    count++;
                }
            }

            if (count >= n) return true;
        }
        return false;
    }
};
```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

如果你希望我把这个解题过程整理成**一页图解或流程图**（比如题目→思路→优化路径→推荐写法），我也可以为你绘制一份。是否需要？

---


## 283. Move Zeroes

**题目描述**：给定一个数组 `nums`，编写一个函数将所有 `0` 移动到数组的末尾，同时保持非零元素的相对顺序。

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>
---

当然可以，下面是你这道题「Leetcode 283. Move Zeroes」的完整解题思路整理与反思总结，涵盖了：

1. ✅ 你最初的解法思路
2. ✅ 解法优化后的写法与原理
3. ✅ 解法演变过程中的思维成长

这份总结可以直接用于你的刷题笔记或复盘本，非常适合用于**加深记忆、日后回顾**。

---

### ✍️ 第一版解法：双指针 + 交换法（初始思路）

### ✅ 思路描述：

* 使用两个指针：

  * `left` 指向当前需要被移动的 `0` 位置；
  * `right` 指向 `left` 之后下一个非 0 元素；
* 发现 `(left == 0 && right != 0)` 后交换两者位置；
* 不断推进直到整个数组扫描完成。

### ✅ 代码实现：

```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int left = 0;
        int right = 0;

        while (right < nums.size() && left < nums.size()) {
            while (left < nums.size() && nums[left] != 0) {
                left++;
            }
            right = left + 1;
            while (right < nums.size() && nums[right] == 0) {
                right++;
            }
            if (right < nums.size()) {
                nums[left] = nums[right];
                nums[right] = 0;                
            }
        }
    }
};
```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

### ✅ 你选择该方法的原因：

* **直觉性强**：两个指针找要交换的值位置；
* **模仿冒泡思路**：把 0 像气泡一样向后推；
* **对交换操作更熟悉**：比“覆盖法”更容易想到；
* **尝试控制两个状态指针**：开始训练状态转换的逻辑。

### ⚠️ 该方法存在的问题：

| 问题       | 说明                       |
| -------- | ------------------------ |
| 🔄 多余操作多 | 会重复查找、重复交换，尤其当 0 和 0 相邻时 |
| 🧩 逻辑复杂  | 多个嵌套 `while` 增加调试难度      |
| 🚫 不够高效  | 时间复杂度虽然是 O(n)，但常数操作较多    |

---

### ✅ 优化版解法：写入法（推荐写法）

### 🧠 思路核心：

* 使用一个 `position` 指针标记下一个非零元素要写入的位置；
* 遍历所有元素：

  * 如果当前元素非 0，就写入 `position` 指向的位置，并将 `position++`；
* 遍历完后，从 `position` 开始，将后面所有位置填充为 0。

### ✅ 优化版代码：

```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int position = 0;
        for (int num : nums) {
            if (num != 0) {
                nums[position++] = num;
            }
        }
        for (int i = position; i < nums.size(); i++) {
            nums[i] = 0;
        }
    }
};
```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

### ✅ 优势总结：

| 优势      | 说明                     |
| ------- | ---------------------- |
| 🚀 更高效  | 减少无效判断，不需要交换           |
| 🎯 简洁明了 | 逻辑清晰，便于阅读和维护           |
| 🧠 易于内化 | 容易记住，适用于更多类似题目（如去除元素等） |

---

### 🔁 解法演变中的认知成长

| 阶段 | 思维特征                         | 成长点              |
| -- | ---------------------------- | ---------------- |
| 初版 | 追求显性操作，借助交换实现效果              | 熟悉双指针的状态控制       |
| 优化 | 用空间覆盖简化流程                    | 抽象出“写入位置”的思想     |
| 总结 | 理解到**不必原地交换也能维持顺序**，掌握更通用的写法 | 提升了算法表达的简洁性与抽象能力 |

---

### ✏️ 衍生题思考（可拓展）

* **27. Remove Element**：使用相同的覆盖思路，移除指定元素；
* **26. Remove Duplicates from Sorted Array**：写入法的经典应用；
* **905. Sort Array By Parity**：双指针换位置；

---

### ✅ 建议的练习与记忆方法：

* 💡 **复述思路**：尝试用自己的语言讲一遍题目 + 解法；
* ✏️ **空白写代码**：不看代码模板，手写至少两遍；
* 🧩 **变体题练习**：做 remove element、remove duplicates 等题；
* 🔁 **间隔复习**：1 天后、3 天后、7 天后各写一次强化记忆（使用 Anki、Notion 等）；

---

如需我为这类题整理一个 **可复用的“写指针覆盖法”模板清单**，也可以告诉我，我可以帮你建一个刷题手册章节模板。

要不要我帮你拓展一个这样的小章节结构？

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>


好，我帮你整理成一份可直接放进你 GitHub 里 README.md 的总结，清晰对比两个版本的思路和差异。

---

## 643. Maximum Average Subarray I

### **题目描述**

给定一个整数数组 `nums` 和一个整数 `k`，找出连续 `k` 个元素的最大平均值，并返回该平均值。

---

### **我的解题过程**

### **第一版（暴力法 O(nk)）**

```cpp
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int left = 0;
        int max = INT32_MIN;

        while (left <= nums.size() - k) {
            int sum = 0;
            for (int i = left; i < left + k; i++) {
                sum += nums[i];
            }
            left++;
            if (max < sum) {
                max = sum;
            }
        }

        return double(max) / k;
    }
};
```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

**思路**

* 外层 `while` 控制窗口起点 `left`，内层 `for` 每次重新求当前窗口的和。
* 时间复杂度 **O(nk)**，当 `k` 很大时效率较低。

**问题**

* 重复计算窗口内的和，浪费了时间。
* 没有利用相邻窗口的重叠部分。

---

### **第二版（滑动窗口 O(n)）**

[![滑动窗口边界条件](diagram/643-slice_window.png)](https://raw.githubusercontent.com/zongwave/leetcode/main/top75/diagram/643-slice_window.png)

```cpp
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int windowSum = 0;
        for (int i = 0; i < k; i++) {
            windowSum += nums[i];
        }
        int maxSum = windowSum;
        for (int i = 1; i <= nums.size() - k; i++) {
            windowSum = windowSum - nums[i - 1] + nums[i + k -1];
            if (windowSum > maxSum) {
                maxSum = windowSum;
            }
        }
        return double(maxSum) / k;
    }
};
```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

**思路**

* **先计算第一个窗口的和**（前 k 个元素）。
* 滑动窗口：每次移动时减去窗口最左元素，加上新进入的元素。
* 时间复杂度 **O(n)**，空间复杂度 **O(1)**。

**优化点**

* 避免重复求和，利用前一个窗口的结果直接更新。
* 更加简洁易读。

---

### **两版对比**

| 版本  | 方法      | 时间复杂度 | 主要问题/优点          |
| --- | ------- | ----- | ---------------- |
| 第一版 | 双循环暴力求和 | O(nk) | 逻辑直观，但效率低，重复计算太多 |
| 第二版 | 滑动窗口    | O(n)  | 高效，避免重复计算，代码简洁   |

---

### **反思**

* 我最初用暴力法是因为习惯“从起点枚举窗口，再求和”，没有立即想到可以用滑动窗口更新结果。
* 滑动窗口是处理固定长度连续子数组问题的常见模板，应当优先考虑。

---

如果你需要，我还可以帮你画一个**两版算法的流程图**，形象展示为什么滑动窗口能省掉重复计算。这样在复习时会更直观。
你要我帮你画吗？

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

## 1732. Find the Highest Altitude

你的这个 `largestAltitude` 实现是正确的，思路也很简洁。
它的时间复杂度是 **O(n)**，空间复杂度是 **O(1)**。

```cpp

class Solution {
public:
    int largestAltitude(vector<int>& gain) {
        int highest = 0;
        int cumulate = 0;

        for (int i = 0; i < gain.size(); i++) {
            cumulate += gain[i];
            if (cumulate > highest) {
                highest = cumulate;
            }
        }
        return highest;
    }

}

```


你相当于是：

1. **初始化**

   * `culmulate = 0` 表示从海拔 0 出发的累计高度变化
   * `highest = 0` 表示到目前为止遇到的最高海拔

2. **遍历 gain**

   * 每一步把 `gain[i]` 加到 `culmulate`
   * 用 `highest` 记录过程中的最大值

3. **返回最高海拔**

---

你这里写得比很多初学者简洁，是因为直接在一次循环里完成了：

* 累加高度变化
* 同时更新最大值

如果你想，我可以帮你画一张小的高度变化折线图，直观展示 `culmulate` 和 `highest` 的变化过程。这样以后你看到类似「累计 + 记录最大值」的题，就会马上想起这个模式。

---

我明白了，你这两个错误其实都挺典型的，尤其是在刚开始刷题时很容易犯：

---

**1️⃣ `culmulate` 初始化为 `INT32_MIN`**

* 这个是受一些「求最大值」类题目的习惯影响，你担心初始值太小导致最大值记录不对。
* 但这里 `culmulate` 是**累计高度**，初始状态就是出发点海拔 **0**，所以不能乱改成极小值，不然后面加 `gain[i]` 就会歪掉。
* 这是一个 **“变量角色”意识问题**：

  * 如果变量是「记录过程中的最大值」，可以用极小值初始化
  * 如果变量是「当前状态」或「累计值」，必须用实际初始状态来初始化

---

**2️⃣ 把题目当成“返回最高点的位置”**

* 你误以为题目要找「最高点的索引」，所以用了 `unordered_map<int, int>` 存高度和位置映射
* 其实题目是问**最高的高度值**，和位置无关
* 这个是典型的**题意理解不精准**导致的需求过度实现
* 额外维护哈希表不仅浪费空间，还增加了思考负担

---

✅ **你第一次就写出正确思路的原因**
这道题本质就是：

1. 模拟走的过程
2. 保留走到的最高点

它符合人类的直觉思维（走一步 → 看海拔 → 记最高），不需要先想到复杂的数据结构，所以你自然写对了。

---

我建议你在 GitHub 的 **README 记录**里加一个习惯：

> 「**错因分类**」：
>
> * 题意理解偏差
> * 初始化值错误
> * 边界条件漏考虑
> * 算法思路复杂化

这样复盘时你能看到自己是在哪个环节最容易出错。

---

要不要我帮你画一张 **“这两个错误在思维流程中的分支图”**，让你以后看到类似情况立刻能规避？这样复习时比文字更直观。

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

## 206. Reverse Linked List


哈哈，这个“dummy head”问题其实很常见，尤其是在链表题里你刷题频率高的时候。
你这次情况很可能是这样的心理路径：

---

### 1️⃣ 你为什么会引入 `dummy head`

* 之前做很多链表插入/删除题，`dummy head` 很方便统一处理边界情况（比如头节点可能被删除的场景）
* 你在复现反转链表时，习惯性就加了个 `dummy`，想着“这样 prev 指针初始可以直接指向 dummy，不用单独处理 head”
* 结果反转链表时，这个 dummy 也被反转进了链表中 → 尾部自然会指回 dummy

---

### 2️⃣ 为什么在反转链表中不需要 `dummy head`

* 反转链表的迭代解法，本质只需要：

  1. 记录当前节点 `curr`
  2. 保存它的下一个节点 `next`
  3. 把 `curr->next` 指向 `prev`
  4. 更新 `prev` 和 `curr`
* 没有**插入到链表头部**的逻辑，也没有**统一处理头节点丢失**的问题，所以 `dummy` 不仅没必要，反而会被翻进链表尾部

---

### 3️⃣ 为什么一周前做对了，这次却“复杂化”

* 刷题间隔后，链表的“模板”在你脑子里有点混了，你下意识把**链表插入/删除**的套路拿来套到**链表反转**上
* 这其实是**模式干扰**：当你有多个相似模板时，大脑会自动调用熟悉但不一定适合的那套

---

### 4️⃣ 你的最终正确版本

你最后写的这个迭代法就是标准反转链表模板：

```cpp
ListNode* reverseList(ListNode* head) {
    ListNode* curr = head;
    ListNode* prev = nullptr;
    while (curr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}
```

* `prev` 指向已经反转好的链表部分
* `curr` 指向还没反转的部分
* 每次循环都把 `curr` 接到 `prev` 前面

---

如果你愿意，我可以帮你画一张\*\*“链表题模板选择指南”\*\*，
让你在做题前 3 秒就能判断到底该用 dummy head 还是不用，这样就不容易再走弯路了。
这样一周后再做相似题，你就能瞬间调出正确模式。

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---




## 374. Guess Number Higher or Lower

### 我的解答过程

1. **第一版解法**

   * 我想用一种「逐步缩小步长」的思路，从 `n / 2` 开始猜，每次根据 `guess()` 的返回值决定向左还是向右移动。
   * 步长通过 `n / (1 << i)` 递减，并用 `max(..., 1)` 保证最小移动 1 位。
   * **问题 1**：移位 `1 << i` 在 `i >= 32` 时发生 **移位越界**（C++ 中 int 为 32 位，移位位数不能 >= 32），导致 **runtime error**。
   * **问题 2**：这种步长缩小法虽然能收敛，但并不是题目的标准要求，也不如二分法稳定。
   * 临时修复方法：加 `min(i, 30)` 避免移位越界，但这个是“补丁式修复”，不是真正根治逻辑问题。

2. **第二版解法（出错）**

   * 改用二分思路，但我在更新区间时写错了：

     ```cpp
     if (guess(mid) == -1) {
         high = mid + 1;   // ❌ 应该是 mid - 1
     }
     if (guess(mid) == 1) {
         low = mid - 1;    // ❌ 应该是 mid + 1
     }
     ```
   * 结果搜索方向完全反了，无法收敛到正确答案。
   * 这是一个 **边界更新方向搞反** 的典型二分错误。

---

### 错误思维总结

* **误导 1**：过度复杂化问题，把「二分查找」变成了「步长递减」的变种，增加了边界处理风险。
* **误导 2**：更新区间时没抓住二分查找的核心逻辑：

  * `guess(mid) == -1` → 答案在 **左区间** → `high = mid - 1`
  * `guess(mid) == 1` → 答案在 **右区间** → `low = mid + 1`
* **误导 3**：移位运算 `1 << i` 没考虑到 C++ 的位移限制，导致运行时错误。

---

### 标准解法（模板式二分查找）

```cpp
class Solution {
public:
    int guessNumber(int n) {
        int low = 1;
        int high = n;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int res = guess(mid);
            if (res == 0) {
                return mid;
            }
            if (res == -1) {
                high = mid -1;
            } else if (res == 1) {
                low = mid + 1;
            }
        }
        return -1;
    }
};
```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

### 注意事项（写在笔记里）

1. 二分查找的区间更新要与题意方向严格对应，不能反向。
2. 中点计算用 `low + (high - low) / 2` 防止溢出。
3. 移位运算 `(1 << i)` 时要确保 `i < 位数`（int 是 32 位 → i < 32）。
4. **优先选择标准二分模板**，不要轻易改成变种步长法，除非题目明确要求。

---

我可以帮你把**这个二分查找错误总结**做成一个「二分查找常见错误模式表」，以后刷到类似题直接对照避免。
你要让我顺便做吗？这样能大幅降低你以后犯同样错的概率。

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

## 1207. Unique Number of Occurrences

判断一个数组中，数字出现的次数是否都是唯一的。


---

## 题目回顾

你的解法是对的，而且逻辑比较清晰。
我帮你梳理一下这道题的解题过程和你代码里的几个思路点，方便你加到刷题笔记中。

---

## 你的解法思路

1. **第一步** — 统计每个数出现的次数

   * 使用 `unordered_map<int, int> num_map`
   * key：数字，value：出现次数

   ```cpp
   for (int num : arr) {
       num_map[num]++;
   }
   ```
2. **第二步** — 检查出现次数是否重复

   * 使用 `unordered_map<int, int> occur_map`
   * key：出现次数，value：一个随便存的数字（你这里用了数字本身）
   * 如果 `occur_map` 已经有当前出现次数，说明重复，返回 `false`

   ```cpp
   for (const auto pair : num_map) {
       if (occur_map.count(pair.second) > 0) {
           return false;
       } else {
           occur_map[pair.second] = pair.first;
       }
   }
   ```
3. **返回结果**

   * 如果没有发现重复次数，返回 `true`

---

## 复杂度分析

* **时间复杂度**：O(n)
  两次遍历 map，总体是 O(n)
* **空间复杂度**：O(n)
  两个哈希表存储频率和出现次数

---

## 你代码里的细节 & 优化建议

* **细节 1**：`occur_map` 其实只关心 “次数” 有没有出现过，不需要存 value，可以用 `unordered_set<int>` 来替代，代码更简洁。
* **细节 2**：`for (const auto pair : num_map)` 建议改成 `const auto&`，避免 map 元素拷贝，提高效率。

---

### 优化版写法

```cpp
class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        unordered_map<int, int> nums_map;
        for (int num : arr) {
            nums_map[num]++;
        }

        unordered_set<int> occur_set;
        for (const auto& [num, count] : nums_map) {
            if (!occur_set.insert(count).second) {
                return false;
            }
        }
        return true;
    }
};
```

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

* `unordered_set` 用来快速检测重复出现的次数
* `insert` 返回值 `.second` 表示插入是否成功（false 表示已存在）

---

如果你愿意，我可以帮你画一张\*\*“双哈希检查唯一性”\*\*的解题思路图，这样你的笔记会更直观。这样以后再遇到类似“唯一性判定”题型，你能直接模式化套用。
你要我画吗？

