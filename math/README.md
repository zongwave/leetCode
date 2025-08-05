
<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>


# 数学算法专题总结

## 核心解题原则


---

## 题目

* [Math 数学算法总结](#LeetCode-math-总结)
  * [目录](#目录)
  * [9. 回文数字](#9-palindrome-number)

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

## 9. palindrome number

**判断一个整数是否正读反读相同（如121是回文，123不是），要求不使用字符串转换，直接操作数字。**  

**关键点**：  
- 负数直接排除  
- 通过数学运算反转一半数字进行比较  
- 注意处理以0结尾的非零数（如10）的特殊情况
---

```cpp
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0) return false;

        vector<int> temp;
        int num = x;
        while (num >= 10) {
            int mod = num % 10;
            num = num / 10;
            temp.push_back(mod);
        }
        temp.push_back(num);

        int bits = temp.size();
        for (int i = 0, j = bits - 1; i < j; i++, j--) {
            if (temp[i] != temp[j]) {
                return false;
            }
        }
        return true;
    }
};

```

你的代码实现了判断一个整数是否是回文数的功能，但有几个地方可以优化和改进。以下是详细分析和建议：

### 代码分析
1. **负数处理**：直接返回 `false`，正确。
2. **数字分解**：将数字逐位分解并存入 `vector`，然后比较首尾数字。
3. **回文判断**：使用双指针法比较数字是否对称。

### 存在的问题
1. **不必要的存储**：使用 `vector` 存储各位数字，增加了空间复杂度。
2. **边界条件**：`x = 0` 时也能正确处理，但可以更简洁。
3. **效率问题**：反转一半数字即可判断，不需要全部反转。

### 优化建议
1. **直接反转一半数字**：不需要存储所有位数，只需反转一半数字并与另一半比较。
2. **提前终止**：在反转过程中可以提前判断是否非回文。

### 优化后的代码
```cpp
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0 || (x % 10 == 0 && x > 0)) {
          return false;
        }

        int reverse = 0;
        while (reverse < x) {
          reverse = reverse * 10 + x % 10;
          x = x / 10;
        }

        // 偶数位：reversed == x
        // 奇数位：reversed/10 == x（去掉中间数字）
        return (reverse == x) || (reverse / 10 == x); 
    }
};
```

---
<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>


### 优化点说明
1. **负数和非零的10的倍数**：直接返回 `false`。
2. **反转一半数字**：
   - 当原始数字小于或等于反转后的数字时，表示已经处理了一半位数。
   - 例如：`1221`，反转后得到 `12` 和 `12`；`12321`，反转后得到 `12` 和 `123`，此时 `x == reversed / 10`。
3. **空间复杂度**：O(1)，不需要额外空间存储数字。

### 复杂度分析
- **时间复杂度**：O(log10(n))，因为每次迭代数字减少一位。
- **空间复杂度**：O(1)，只使用了常数空间。

### 测试用例
| 输入     | 预期结果 | 优化后代码 |
|----------|----------|------------|
| 121      | true     | true       |
| -121     | false    | false      |
| 10       | false    | false      |
| 0        | true     | true       |
| 12321    | true     | true       |
| 123456   | false    | false      |

### 总结
- **原始代码**：正确但不够高效，使用了额外的空间。
- **优化代码**：更高效，空间复杂度为 O(1)，逻辑更简洁。
- **推荐**：使用优化后的方法，更适合面试和实际应用。