#include <iostream>
#include <vector>
#include <math.h>

#ifndef _LEETCODE_PROBLEMS_EASY_H_
#define _LEETCODE_PROBLEMS_EASY_H_

using namespace std;

class CreateData {
public:
    static string str(const char c, uint32_t num) {
        string result;
        result.reserve(num);
        for (uint32_t i = 0; i < num; i++) {
            result.push_back(c);
        }
        return result;
    }
};

class PrintData {
public:
    static void print(vector<int> vec) {
        size_t count = vec.size();
        printf("\ninput vector size:%zd \n", count);
        printf("[ ");
        for (uint32_t i = 0; i < count; i++) {
            printf("%5d  ", vec[i]);
        }
        printf(" ]\n");
    }

    static void print(vector<int> vec, uint32_t printCount) {
        size_t vectorCount = vec.size();
        printf("\noutput vector size:%zd, printf size:%d \n", vectorCount, printCount);
        printf("[ ");
        for (uint32_t i = 0; i < vectorCount && i < printCount; i++) {
            printf("%5d  ", vec[i]);
        }
        printf(" ]\n");
    }
};

/* 1. Two Sum
*
*    Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.
*    You may assume that each input would have exactly one solution, and you may not use the same element twice.
*    You can return the answer in any order.
*
*    Constraints:
*        2 <= nums.length <= 104
*        -109 <= nums[i] <= 109
*        -109 <= target <= 109
*        Only one valid answer exists.
*/
class TwoSum {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> result;
        result.reserve(2);
        size_t count = nums.size();
        if (count <= 1) {
            return result;
        }
        uint32_t i = 1;
        uint32_t j = 0;

        for (i = 0; i < count - 1; i++) {
            for (j = i + 1; j < count; j++) {
                if (nums[i] + nums[j] == target) {
                    result.push_back(i);
                    result.push_back(j);
                    break;
                }
            }
        }
        PrintData::print(result);
        return result;
    }
};

/* 9. Palindrome Number
*
*    Given an integer x, return true if x is palindrome integer.
*    An integer is a palindrome when it reads the same backward as forward.
*    For example, 121 is palindrome while 123 is not.
*
*    Example 1:
*        Input: x = 121
*        Output: true
*
*    Example 2:
*        Input: x = -121
*        Output: false
*        Explanation: From left to right, it reads -121. From right to left, it becomes 121-. Therefore it is not a palindrome.
*
*    Constraints: -2^31 <= x <= 2^31 - 1
*/
class Palindrome {
public:
    bool isPalindrome(int x) {
        if (x < 0) {
            return false;
        }

        int32_t max = (int32_t)pow(2, 31) - 1;
        int32_t digits = 0;
        for (digits = 0; digits < max; digits++) {
            if ((x - pow(10, digits)) < 0) {
                printf("input number is %d digits \n", digits);
                break;
            }
        }

        int num = x;
        for (int32_t i = 0; i < digits / 2; i++) {
            uint32_t high = num / (uint32_t)pow(10, digits - 2*i - 1);
            uint32_t low = num % 10;
            num = (num - high * (uint32_t)pow(10, digits - 2*i - 1)) / 10;
            if (high != low) {
                return false;
            }
        }

        return true;
    }
};

/* 66. Plus One
*
*    You are given a large integer represented as an integer array digits,
*    where each digits[i] is the ith digit of the integer.
*    The digits are ordered from most significant to least significant in left-to-right order.
*    The large integer does not contain any leading 0's.
*
*    Increment the large integer by one and return the resulting array of digits.
*
*    Constraints:
*        1 <= digits.length <= 100
*        0 <= digits[i] <= 9
*        digits does not contain any leading 0's.
*/
class PlusOne {
public:
    vector<int> plusOne(vector<int>& digits) {
        vector<int> result = digits;
        uint32_t addend = 1;

        for (int32_t i = (int32_t)digits.size() - 1; i >= 0; i--) {
            if (1 == addend) {
                if (result[i] + addend > 9) {
                    result[i] = 0;
                    if (i == 0) {
                        result.insert(result.begin(), 1);
                        break;
                    } else {
                        addend = 1;
                    }
                } else {
                    result[i] += 1;
                    addend = 0;
                    break;
                }
            }
        }
        PrintData::print(digits);
        PrintData::print(result);
        return result;
    }
};

/* 69. Sqrt(x)
*
*    Given a non-negative integer x, compute and return the square root of x.
*    Since the return type is an integer, the decimal digits are truncated,
*    and only the integer part of the result is returned.
*
*    Note: You are not allowed to use any built-in exponent function or operator, such as pow(x, 0.5) or x ** 0.5.
*
*    Constraints:
*        0 <= x <= 231 - 1
*/
class Sqrt {
public:
    int mySqrt(int x) {
        if (x < 0 || x > 0x7fffffff) {
            printf("sqrt of %d is invalide \n", x);
            return 0;
        }

        printf("verify sqrt of %d is %f \n", x, sqrt(x));
        for (int32_t i = 0; i <= x; i++) {
            double temp = (double)i * i;

            if (temp > x) {
                printf("sqrt of %d(HEX:0x%x) is %d(HEX:0x%x) \n", x, x, i - 1, i - 1);
                return i - 1;
            } else if (temp == x) {
                printf("sqrt of %d(HEX:0x%x) is %d(HEX:0x%x) \n", x, x, i, i);
                return i;
            }
        }

        return 0;
    }
};

/* 367. Valid Perfect Square
*
*    Given a positive integer num, write a function which returns True if num is a perfect square else False.
*    Follow up: Do not use any built-in library function such as sqrt.
*
*    Constraints:
*        1 <= num <= 2^31 - 1
*/
class PerfectSquare {
public:
    bool isPerfectSquare(int num) {
        if (num < 0) {
            printf("sqrt of %d is invalide \n", num);
            return false;
        }

        printf("verify sqrt of %d is %f \n", num, sqrt(num));
        for (int32_t i = 0; i <= num; i++) {
            double temp = (double)i * i;

            if (temp == num) {
                printf("%d(HEX:0x%x) is a perfect square number, sqrt is:%d(HEX:0x%x) \n", num, num, i, i);
                return true;
            } else if (temp > num) {
                printf("%d(HEX:0x%x) is a NOT perfect square number, sqrt is:%d(HEX:0x%x) \n", num, num, i, i);
                return false;
            }
        }

        printf("%d(HEX:0x%x) is a NOT perfect square number \n", num, num);
        return false;
    }
};

/* 58. Length of Last Word
*
*    Given a string s consisting of some words separated by some number of spaces,
*    return the length of the last word in the string.
*    A word is a maximal substring consisting of non-space characters only.
*
*    Constraints:
*        1 <= s.length <= 104
*        s consists of only English letters and spaces ' '.
*        There will be at least one word in s.
*/
class LastWord {
public:
    int lengthOfLastWord(string s) {
        uint32_t index = 0;
        size_t strLen = s.length();
        uint32_t wordLen = 0;
        bool detectSpace = false;

        while (index < strLen) {
            if (s.at(index) != ' ') {
                if (detectSpace) {
                    wordLen = 0;
                    detectSpace = false;
                }
                wordLen++;
            } else {
                if (wordLen > 0) {
                    detectSpace = true;
                }
            }
            index++;
        }
        printf("the last word length:%d \n", wordLen);
        return wordLen;
    }
};

/* 26. Remove Duplicates from Sorted Array
*
*    Given an integer array nums sorted in non-decreasing order,
*    remove the duplicates in-place such that each unique element appears only once.
*    The relative order of the elements should be kept the same.
*    Since it is impossible to change the length of the array in some languages,
*    you must instead have the result be placed in the first part of the array nums.
*    More formally, if there are k elements after removing the duplicates,
*    then the first k elements of nums should hold the final result.
*    It does not matter what you leave beyond the first k elements.
*    Return k after placing the final result in the first k slots of nums.
*
*    Do not allocate extra space for another array.
*    You must do this by modifying the input array in-place with O(1) extra memory.
*
*    Constraints:
*        0 <= nums.length <= 3 * 104
*        -100 <= nums[i] <= 100
*        nums is sorted in non-decreasing order.
*/
class RemoveDup {
public:
    int removeDuplicates(vector<int>& nums) {
        uint32_t backupIndex = 0;
        uint32_t seekIndex = 1;

        PrintData::print(nums);

        if (0 == nums.size()) {
            return 0;
        }

        for (seekIndex = 1; seekIndex < nums.size(); seekIndex++) {
            if (nums[backupIndex] != nums[seekIndex]) {
                backupIndex++;
                nums[backupIndex] = nums[seekIndex];
            }
        }

        PrintData::print(nums, backupIndex + 1);
        printf("vector remove duplicates remian:%d \n", backupIndex + 1);
        return backupIndex + 1;
    }
};

/* 14. Longest Common Prefix
*
*    Write a function to find the longest common prefix string amongst an array of strings.
*    If there is no common prefix, return an empty string "".
*
*    Constraints:
*        1 <= strs.length <= 200
*        0 <= strs[i].length <= 200
*        strs[i] consists of only lower-case English letters.
*/
class LongestCommonPrefix {
public:
    string longestCommonPrefix(vector<string>& strs) {

        string result;
        bool samePrefix = false;
        char c1;
        char c2;
        if (1 == strs.size()) {
            result = strs[0];
            printf("longest common prefix:%s, Length:%zd \n", result.c_str(), result.size());
            return result;
        }
        for (size_t charIdx = 0; ; charIdx++) {
            for (size_t strIdx = 0; strIdx < strs.size() - 1; strIdx++) {
                if (charIdx < strs[strIdx].size() && charIdx < strs[strIdx + 1].size()) {
                    c1 = strs[strIdx].at(charIdx);
                    c2 = strs[strIdx + 1].at(charIdx);

                    if (!(samePrefix = (c1 == c2))) {
                        break;
                    }
                } else {
                    samePrefix = false;
                    break;
                }
            }
            if (samePrefix) {
                printf("insert char:%c at index:%zd into str size:%zd\n", c1, charIdx, result.size());
                result.push_back(c1);
            } else {
                break;
            }
        }
        printf("longest common prefix:%s, Length:%zd \n", result.c_str(), result.size());
        return result;
    }
};

/* 20. Valid Parentheses
*
*    Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
*    An input string is valid if:
*
*    Open brackets must be closed by the same type of brackets.
*    Open brackets must be closed in the correct order.
*
*    Constraints:
*        1 <= s.length <= 104
*        s consists of parentheses only '()[]{}'.
*/
class ValidBrackets {
public:
    bool isValidBrackets(string s) {
        vector<char> charStack;
        for (uint32_t charIdx = 0; charIdx < s.size(); charIdx++) {
            if (s[charIdx] == '(' || s[charIdx] == '[' || s[charIdx] == '{') {
                charStack.push_back(s[charIdx]);
            }
            if (s[charIdx] == ')') {
                if (charStack.size() > 0 && '(' == charStack[charStack.size() - 1]) {
                    charStack.pop_back();
                } else {
                    return false;
                }
            }
            if (s[charIdx] == ']') {
                if (charStack.size() > 0 && '[' == charStack[charStack.size() - 1]) {
                    charStack.pop_back();
                } else {
                    return false;
                }
            }
            if (s[charIdx] == '}') {
                if (charStack.size() > 0 && '{' == charStack[charStack.size() - 1]) {
                    charStack.pop_back();
                } else {
                    return false;
                }
            }
        }

        if (0 == charStack.size() && 0 != s.size()) {
            return true;
        } else {
            return false;
        }
    }
};

/* 136. Single Number
*
*    Given a non-empty array of integers nums, every element appears twice except for one.
*    Find that single one.
*    You must implement a solution with a linear runtime complexity and use only constant extra space.
*
*    Constraints:
*        1 <= nums.length <= 3 * 104
*        -3 * 104 <= nums[i] <= 3 * 104
*        Each element in the array appears twice except for one element which appears only once.
*/
class SingleNumber {
public:
    int singleNumber(vector<int>& nums) {
        int base_idx = 0;
        int seek_idx = 1;

        PrintData::print(nums);
        vector<bool> singleMark;
        singleMark.resize(nums.size());
        for (size_t i = 0; i < singleMark.size(); i++) {
            singleMark[i] = true;
        }

        for (base_idx = 0; base_idx < nums.size() - 1; base_idx++) {
            for (seek_idx = base_idx + 1; seek_idx < nums.size(); seek_idx++) {
                if (singleMark[base_idx] == false) {
                    continue;
                }
                if (nums[base_idx] == nums[seek_idx]) {
                    singleMark[base_idx] = false;
                    singleMark[seek_idx] = false;
                    break;
                }
            }
        }

        for (size_t i = 0; i < singleMark.size(); i++) {
            if (singleMark[i] == true) {
                printf("found single number:%d at index:%zd \n", nums[i], i);
                return nums[i];
            }
        }
        return 0;
    }

    int singleNumberXor(vector<int>& nums) {
        int X = 0;

        for (auto num : nums) {
            X ^= num;
        }

        printf("found single number:%d  \n", X);

        return X;
    }
};

/* 27. Remove Element
*
*    Given an integer array nums and an integer val, remove all occurrences of val in nums in-place.
*    The relative order of the elements may be changed.
*
*    Since it is impossible to change the length of the array in some languages,
*    you must instead have the result be placed in the first part of the array nums.
*    More formally, if there are k elements after removing the duplicates,
*    then the first k elements of nums should hold the final result.
*    It does not matter what you leave beyond the first k elements.
*
*    Return k after placing the final result in the first k slots of nums.
*
*    Do not allocate extra space for another array. You must do this by modifying the input array in-place with O(1) extra memory.
*
*    Constraints:
*        0 <= nums.length <= 100
*        0 <= nums[i] <= 50
*        0 <= val <= 100
*/
class RemoveElement {
public:
    int removeElementEasy(vector<int>& nums, int val) {
        PrintData::print(nums);

        int pos = 0;
        for (size_t idx = 0; idx < nums.size(); idx++) {
            if (nums[idx] != val) {
                nums[pos++] = nums[idx];
            }
        }
        printf("remove element:%d, remain element number:%d \n", val, pos);
        return pos;
    }

    int removeElement(vector<int>& nums, int val) {
        uint32_t count = (uint32_t)nums.size();
        if (count == 0) {
            return 0;
        }
        int32_t front_idx = 0;
        int32_t end_idx = count - 1;

        PrintData::print(nums);

        for (front_idx = 0; front_idx <= end_idx; front_idx++) {
            if (nums[front_idx] == val) {
                while (nums[end_idx] == val && end_idx > front_idx) {
                    end_idx--;
                }
                if (end_idx == 0) {
                    printf("remove element:%d, remain element number:0 \n", val);
                    return 0;
                }
                nums[front_idx] = nums[end_idx];
                end_idx--;
            } else {
                if (nums[end_idx] == val) {
                    end_idx--;
                }
            }
        }

        printf("remove element:%d, remain element number:%d \n", val, end_idx + 1);
        PrintData::print(nums);

        return end_idx + 1;
    }
};

/* 28. Implement strStr()
*
*    Implement strStr().
*
*    Return the index of the first occurrence of needle in haystack, or -1 if needle is not part of haystack.
*
*    Clarification:
*        What should we return when needle is an empty string? This is a great question to ask during an interview.
*        For the purpose of this problem, we will return 0 when needle is an empty string.
*        This is consistent to C's strstr() and Java's indexOf().
*
*    Constraints:
*        0 <= haystack.length, needle.length <= 5 * 104
*        haystack and needle consist of only lower-case English characters.
*/
class MystrStr {
public:
    int strStr(string haystack, string needle) {

        if (needle.empty()) {
            return 0;
        }
        if (haystack.size() < needle.size()) {
            return -1;
        }

        printf("find needle str:%s from haystack str:%s \n", needle.c_str(), haystack.c_str());
        string sword;
        for (uint32_t idx = 0; idx < haystack.size(); idx++) {
            sword = haystack.substr(idx, haystack.size() - idx);
            if (0 == strncmp(needle.c_str(), sword.c_str(), needle.size())) {
                printf("found needle at:%d \n", idx);
                return idx;
            }
        }

        printf("Could NOT found needle str:%s \n", needle.c_str());
        return -1;
    }

    int strStrFast(string haystack, string needle) {

        if (needle.empty()) {
            return 0;
        }
        //printf("find needle str:%s from haystack str:%s \n", needle.c_str(), haystack.c_str());

        int found = (int)haystack.find(needle.c_str());

        if (found != std::string::npos) {
            std::cout << "first 'needle' found at: " << found << '\n';
            return found;
        } else {
            printf("Could NOT found needle str:%s \n", needle.c_str());
            return -1;
        }
    }
};

/* 461. Hamming Distance
*
*    The Hamming distance between two integers is the number of positions at which the corresponding bits are different.
*    Given two integers x and y, return the Hamming distance between them.
*
*    Constraints:
*        0 <= x, y <= 231 - 1
*/
class HammingDistance {
public:
    int hammingDistance(int x, int y) {
        int z = x ^ y;
        int result = 0;

        while (z != 0) {
            if (z & 0x01) {
                result++;
            }
            z = z >> 1;
        }
        printf("x:%d, y:%d, hamming distance:%d \n", x, y, result);
        return result;
    }
};

/* 191. Number of 1 Bits
*
*     Write a function that takes an unsigned integer and returns the number of '1' bits it has (also known as the Hamming weight).
*
*     Constraints:
*         The input must be a binary string of length 32.
*/
class HammingWeight {
public:
    int hammingWeight(uint32_t n) {
        int count = 0;

        while (n > 0) {
            if (n & 0x01) {
                count += 1;
            }
            n = n >> 1;
        }
        return count;
    }
};

/* 190. Reverse Bits
*
*     Reverse bits of a given 32 bits unsigned integer.
*    
*     Constraints:
*        The input must be a binary string of length 32
*/
class ReverseBits {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t reverse = 0;
        uint32_t bitIdx = 31;
        uint32_t input = n;

        while (n > 0) {
            reverse |= (n & 0x01) << bitIdx;
            n = n >> 1;
            bitIdx--;
        }
        printf("ReverseBits input:0x%x, output:0x%x \n", input, reverse);
        return reverse;
    }
};

/* 35. Search Insert Position
*
*     Given a sorted array of distinct integers and a target value,
*     return the index if the target is found. If not, return the index where it would be if it were inserted in order.
*     You must write an algorithm with O(log n) runtime complexity.
*
*     Constraints:
*         1 <= nums.length <= 104
*         -104 <= nums[i] <= 104
*         nums contains distinct values sorted in ascending order.
*         -104 <= target <= 104
*/
class SearchInsert {
public:
    int searchInsert(vector<int>& nums, int target) {
        int idx = 0;
        for (const auto& num : nums) {
            if (num >= target) {
                return idx;
            }
            idx++;
        }
        return idx;
    }
};

/* 70. Climbing Stairs
*
*     You are climbing a staircase. It takes n steps to reach the top.
*     Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
*
*     Constraints:
*         1 <= n <= 45
*/
class ClimbStairs {
public:
    double factorial(int n) {
        if (n <= 0) {
            return 1;
        }
        if (n == 1) {
            return 1;
        } else {
            return n * factorial(n - 1);
        }
    }

    double combination(int n, int m) {
        if (n <= m || m == 0) {
            return 1;
        }

        double pnm = n;
        double pmm = n - m;
        for (int i = 1; i < n - m; i++) {
            pnm *= (n - i);
            pmm *= (n - m - i);
        }
        return pnm / pmm;
    }

    double combination_recur(int n, int m) {
        if (n <= m || m == 0) {
            return 1;
        }

        double result = factorial (n) / (factorial(m) * factorial(n - m));

        return result;
    }

    int fibonacci(int n) {
        if (n <= 2) {
            return 1;
        } else {
            int result = fibonacci(n - 1) + fibonacci(n - 2);
            //printf("fibonacci %d, %d \n", n, result);
            return result;
        }
    }

    int climbStairsCombination(int n) {
        if (n <= 0) {
            return 0;
        }

        int min_digits = n / 2 + n % 2;
        int max_digits = n;
        double result = 0;

        for (int i = min_digits; i <= max_digits; i++) {
            result += combination(i, max_digits - i);
        }

        printf("ClimbStairs intput:%d, output:%d \n", n, (int)result);
        return (int)result;
    }

    int climbStairs(int n) {
        if (n <= 0) {
            return 0;
        }

        int* fibonacciList = (int*)malloc((n + 1) * sizeof(int));
        if (NULL == fibonacciList) {
            return 0;
        }

        fibonacciList[0] = 1;
        fibonacciList[1] = 1;

        for (int i = 2; i <= n; i++) {
            fibonacciList[i] = fibonacciList[i - 1] + fibonacciList[i - 2];
        }

        int result = fibonacciList[n];
        free(fibonacciList);

        printf("ClimbStairs intput:%d, output:%d \n", n, result);
        return result;
    }
};

/* 88. Merge Sorted Array
*
*     You are given two integer arrays nums1 and nums2, sorted in non-decreasing order,
*     and two integers m and n, representing the number of elements in nums1 and nums2 respectively.
*     Merge nums1 and nums2 into a single array sorted in non-decreasing order.
*     The final sorted array should not be returned by the function, but instead be stored inside the array nums1.
*     To accommodate this, nums1 has a length of m + n, where the first m elements denote the elements that should be merged,
*     and the last n elements are set to 0 and should be ignored. nums2 has a length of n.
*
*     Constraints:
*         nums1.length == m + n
*         nums2.length == n
*         0 <= m, n <= 200
*         1 <= m + n <= 200
*        -109 <= nums1[i], nums2[j] <= 109
*/
class MergeVector {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int idx1 = m - 1;
        int idx2 = n - 1;
        int idx0 = m + n - 1;

        PrintData::print(nums1);
        PrintData::print(nums2);

        while (idx2 >= 0) {
            if (idx1 >= 0 && nums1[idx1] >= nums2[idx2]) {
                nums1[idx0] = nums1[idx1];
                idx1--;
            } else {
                nums1[idx0] = nums2[idx2];
                idx2--;
            }
            idx0--;
        }
        PrintData::print(nums1);
    }
};

/* 118. Pascal's Triangle
*
*    Given an integer numRows, return the first numRows of Pascal's triangle.
*    In Pascal's triangle, each number is the sum of the two numbers directly above it as shown:
*
*    Example 1:
*        Input: numRows = 5
*        Output: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]
*
*    Constraints:
*        1 <= numRows <= 30
*/
class PascalTriangle {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> triangle;
        triangle.resize(numRows);
        triangle[0] = { 1 };

        if (numRows > 1) {
            triangle[1] = { 1, 1 };

            for (int num = 2; num < numRows; num++) {
                vector<int> row;
                row.resize(num + 1);
                row[0] = 1;
                row[num] = 1;
                for (int idx = 1; idx < num; idx++) {
                    row[idx] = triangle[num - 1][idx - 1] + triangle[num - 1][idx];
                }
                triangle[num] = row;

                PrintData::print(row);
            }
        }

        return triangle;
    }
};

/* 119. Pascal's Triangle II
*
*     Given an integer rowIndex, return the rowIndexth (0-indexed) row of the Pascal's triangle.
*     In Pascal's triangle, each number is the sum of the two numbers directly above it as shown:
*
*     Example 1:
*         Input: rowIndex = 3
*         Output: [1,3,3,1]
*
*     Constraints:
*         0 <= rowIndex <= 33
*/
class PascalTriangleRow {
public:
    vector<int> getRow(int rowIndex) {
        vector<vector<int>> triangle;
        triangle.resize(rowIndex + 1);

        if (rowIndex < 1) {
            PrintData::print({ 1 });
            return { 1 };
        }

        triangle[0] = { 1 };
        triangle[1] = { 1, 1 };

        for (int i = 2; i <= rowIndex; i++) {
            vector<int> row;
            triangle[i].resize(i + 1);
            triangle[i][0] = 1;
            triangle[i][i] = 1;
            for (int j = 1; j < i; j++) {
                triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
            }
        }

        PrintData::print(triangle[rowIndex]);
        return triangle[rowIndex];
    }
};

#endif

