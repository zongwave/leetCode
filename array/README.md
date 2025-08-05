
<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>


# 数组算法专题总结

## 核心解题原则


---

## 题目

* [Array 数组算法总结](#LeetCode-array-总结)
  * [目录](#目录)
  * [1. 两数之和](#1-two-sum)
  * [26. 移除有序数组中的冗余元素](#26-remove-duplicates-from-sorted-array)

---

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

## 1. Two Sum

**问题描述**：  
给定一个整数数组 `nums` 和一个目标值 `target`，在数组中找出 **和为目标值** 的两个整数，并返回它们的下标。假设每个输入只有唯一解，且不能重复使用同一元素。

**示例**：  
输入：`nums = [2, 7, 11, 15]`, `target = 9`  
输出：`[0, 1]`（因为 `nums[0] + nums[1] = 2 + 7 = 9`）

**核心方法**：  
- **哈希表优化**：遍历数组，用哈希表记录已访问的值及其下标，检查 `target - nums[i]` 是否在表中。  
- 时间复杂度：`O(n)`，空间复杂度：`O(n)`。

---

#### **你的原始代码问题**
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> sum;  // 键值定义错误
        vector<int> res;

        for (int key = 0; key < nums.size(); key++) {
            int value = target - nums[key];
            if (sum.count(value)) {   // 查找的是值，但哈希表存的是索引→值
                res.push_back(key);
                res.push_back(sum[value]); // 错误：返回的是值而非索引
                return res;
            }
            sum[key] = nums[key];     // 错误：存储的是索引→值（反向）
        }
        return res;
    }
};
```

##### **主要错误**
1. **哈希表键值定义颠倒**  
   - 你使用 `sum[key] = nums[key]`（索引→值），但需要的是 **值→索引** 的映射。
   - 导致查找 `value` 时无法命中正确键。

2. **返回值逻辑错误**  
   - `sum[value]` 返回的是数值而非索引，与题目要求不符。

3. **未利用哈希表特性**  
   - 哈希表应直接通过数值快速定位索引，你的实现绕过了这一核心优势。

---

### **标准解法与核心思想**
#### **正确代码（哈希表优化）**
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> num_map; // 值 → 索引
        
        for (int i = 0; i < nums.size(); i++) {
            int complement = target - nums[i];
            if (num_map.count(complement)) {
                return {num_map[complement], i}; // 返回找到的索引和当前索引
            }
            num_map[nums[i]] = i; // 存储当前值 → 索引
        }
        return {}; // 题目保证有解，实际不会执行
    }
};
```

#### **关键设计**
1. **哈希表的作用**  
   - **键（Key）**：数组中的数值（用于快速查找补数 `target - nums[i]`）。  
   - **值（Value）**：该数值对应的索引（用于返回结果）。

2. **单次遍历**  
   - 边遍历边记录已访问的数值和索引，确保每个元素只处理一次。

3. **提前返回**  
   - 找到补数后立即返回结果，无需完成全部循环。

---

### **复杂度分析**
| 指标          | 暴力解法       | 哈希表优化     |
|---------------|---------------|---------------|
| **时间复杂度** | O(n²)        | O(n)          |
| **空间复杂度** | O(1)         | O(n)          |

---

### **应吸取的教训**
1. **明确哈希表的键值角色**  
   - 先确定需要通过什么查找什么（本题是通过值找索引）。

2. **理解数据结构的设计意图**  
   - 哈希表的核心是 **用空间换时间**，直接通过键定位值，避免嵌套循环。

3. **边界条件验证**  
   - 测试用例需覆盖：
     - 解在数组开头/末尾（如 `[2,7,11,15]`，target=9）
     - 重复值（如 `[3,3]`，target=6）
     - 大数差值（如 `[INT_MAX, 1]`，target=INT_MIN+1）

4. **避免过度设计**  
   - 初始解法用暴力法虽能通过，但面试中需展示对最优解的理解。

---

### **扩展思考**
1. **如果数组已排序？**  
   - 可用双指针法（时间复杂度 O(n)，空间 O(1)）：
     ```cpp
     int left = 0, right = nums.size() - 1;
     while (left < right) {
         int sum = nums[left] + nums[right];
         if (sum == target) return {left, right};
         else if (sum < target) left++;
         else right--;
     }
     ```

2. **如果存在多个解？**  
   - 修改返回类型为 `vector<vector<int>>`，继续遍历而非提前返回。

---

### **总结**
- **核心技巧**：哈希表存储 **值→索引** 的映射，将查找时间从 O(n) 降至 O(1)。
- **面试重点**：解释清楚为何选择值作为键，以及如何通过补数快速定位。
- **代码模板**：
  ```cpp
  unordered_map<ValueType, IndexType> map;
  for (遍历数组) {
      if (map.count(target - 当前值)) {
          return {map[target - 当前值], 当前索引};
      }
      map[当前值] = 当前索引;
  }
  ```

<a href="#题目" style="font-size: 16px; color: #666;">▲ 返回题目</a>

---

## 26. Remove Duplicates from Sorted Array

**题目简介**：原地删除有序数组中的重复项，返回去重后的新长度，且空间复杂度应为 O(1)。

---

#### **题目要求**
1. **原地修改**：必须在原数组上操作，不能使用额外的数组空间
2. **返回新长度**：返回去重后的数组长度，并保证数组前k个元素是不重复的
3. **时间复杂度**：O(n)
4. **空间复杂度**：最好为O(1)（针对已排序数组）

---

### **你的解法分析**
```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        unordered_map<int, bool> map;
        int k = 0;

        for (int num: nums) {
            if (!map.count(num)) {
                map[num] = true;
                nums[k++] = num;
            }
        }
        return k;
    }
};
```

#### **优点**
1. **正确性**：
   - 使用哈希表记录已出现的数字，确保去重逻辑正确
   - 通过`k`指针维护去重后的数组位置，满足"原地修改"的要求
2. **通用性**：
   - 适用于**未排序数组**的去重（题目假设数组已排序，但你的解法更通用）

#### **不足与改进点**
1. **空间复杂度偏高**：
   - 使用了`unordered_map`，空间复杂度为O(n)
   - **改进**：如果数组**已排序**，可以用双指针法优化到O(1)空间

2. **冗余的value存储**：
   - `unordered_map<int, bool>`中的`bool`是多余的，因为只需要判断key是否存在
   - **改进**：改用`unordered_set<int>`，更简洁且节省空间

3. **未利用题目条件**：
   - 题目明确说明数组是**升序排列**，但你的解法没有利用这一特性
   - **改进**：若数组已排序，双指针法是最优解（见下文）

---

### **最优解（双指针法）**
```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;

        int k = 1;
        for (int i = 1; i < nums.size(); i++) {
            if (nums[i] != nums[k-1]) {
                nums[k++] = nums[i];
            }
        }
        return k;
    }
};
```

#### **优势**
1. **空间复杂度O(1)**：仅用两个指针，无需额外空间
2. **时间复杂度O(n)**：单次遍历
3. **完全利用题目条件**：依赖数组已排序的特性

---

### **关键对比总结**
| 方法               | 时间复杂度 | 空间复杂度 | 适用场景          |
|--------------------|------------|------------|-------------------|
| 你的哈希表法        | O(n)       | O(n)       | 未排序数组        |
| 双指针法（最优解）  | O(n)       | O(1)       | 已排序数组（推荐）|

---

### **最终建议**
1. **如果数组已排序**：
   - 优先使用双指针法，满足O(1)空间复杂度
2. **如果数组未排序**：
   - 你的解法完全正确，但可改用`unordered_set`提升可读性
3. **面试注意点**：
   - 先确认数组是否已排序，再选择最优解法
   - 即使写出哈希表解法，也要能解释双指针法的优势

---
