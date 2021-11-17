// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "easy.hpp"

int main()
{
    std::cout << "Hello LeetCode!\n";

    std::cout << "LeetCode Soultuion 1 'TwoSum' ==============>\n\n";
    TwoSum s1;
    vector<int> input1 = { 2, 5, 5, 11 };
    int target = 10;
    vector<int> output = s1.twoSum(input1, target);
    std::cout << "LeetCode Soultuion 1 'TwoSum' <==============\n\n";

    std::cout << "LeetCode Soultuion 2 'Palindrome' ==============>\n\n";
    Palindrome s2;
    int32_t input_number = 1456106541;
    bool palindrome = s2.isPalindrome(input_number);
    printf("input number %d is palindrome %d \n", input_number, palindrome);
    std::cout << "LeetCode Soultuion 2 'Palindrome' <==============\n\n";

    std::cout << "LeetCode Soultuion 3 'PlusOne' ==============>\n\n";
    PlusOne s3;
    vector<int> input3 = { 3, 9, 9, 9 };
    s3.plusOne(input3);
    std::cout << "LeetCode Soultuion 3 'PlusOne' <==============\n\n";

    std::cout << "LeetCode Soultuion 4 'Sqrt' ==============>\n\n";
    Sqrt s4;
    s4.mySqrt(2147395599);
    s4.mySqrt(2147395600);
    s4.mySqrt(2147483647);
    std::cout << "LeetCode Soultuion 4 'Sqrt' <==============\n\n";

    std::cout << "LeetCode Soultuion 5 'PerfectSquare' ==============>\n\n";
    PerfectSquare s5;
    s5.isPerfectSquare(196);
    s5.isPerfectSquare(255);
    s5.isPerfectSquare(2147395599);
    s5.isPerfectSquare(2147395600);
    s5.isPerfectSquare(0x7fffffff);
    std::cout << "LeetCode Soultuion 5 'PerfectSquare' <==============\n\n";

    std::cout << "LeetCode Soultuion 6 'LastWord' ==============>\n\n";
    LastWord s6;
    s6.lengthOfLastWord("leetCode ");
    s6.lengthOfLastWord("leetCode NICE");
    s6.lengthOfLastWord("luffy is still joyboy");
    s6.lengthOfLastWord("   fly me   to   the moon  ");
    s6.lengthOfLastWord("   u  ");
    std::cout << "LeetCode Soultuion 6 'LastWord' <==============\n\n";

    std::cout << "LeetCode Soultuion 7 'RemoveDup' ==============>\n\n";
    RemoveDup s7;
    uint32_t count;
    vector<int> nums = { 0, 0, 1, 1, 1, 4, 4, 4, 8, 10 };
    count = s7.removeDuplicates(nums);

    nums = { 0, 0, 1 };
    s7.removeDuplicates(nums);

    nums = { 1, };
    s7.removeDuplicates(nums);

    nums = {  };
    count = s7.removeDuplicates(nums);
    std::cout << "LeetCode Soultuion 7 'RemoveDup' <==============\n\n";

    std::cout << "LeetCode Soultuion 8 'LongestCommonPrefix' ==============>\n\n";
    LongestCommonPrefix s8;
    vector<string> strs = { "flower", "flow", "flight" };
    s8.longestCommonPrefix(strs);

    strs = { "dog", "racecar", "car" };
    s8.longestCommonPrefix(strs);

    strs = { "r" };
    s8.longestCommonPrefix(strs);
    std::cout << "LeetCode Soultuion 9 'LongestCommonPrefix' <==============>\n\n";

    std::cout << "LeetCode Soultuion 9 'ValidBrackets' ==============>\n\n";
    ValidBrackets s9;
    string brackets = "()[]{}";
    bool isVal = false;
    isVal = s9.isValidBrackets(brackets);
    printf("brackets string:%s, is valid brackets:%d \n", brackets.c_str(), isVal);

    brackets = "(]";
    isVal = s9.isValidBrackets(brackets);
    printf("brackets string:%s, is valid brackets:%d \n", brackets.c_str(), isVal);

    brackets = "(";
    isVal = s9.isValidBrackets(brackets);
    printf("brackets string:%s, is valid brackets:%d \n", brackets.c_str(), isVal);

    brackets = "]";
    isVal = s9.isValidBrackets(brackets);
    printf("brackets string:%s, is valid brackets:%d \n", brackets.c_str(), isVal);

    brackets = "";
    isVal = s9.isValidBrackets(brackets);
    printf("brackets string:%s, is valid brackets:%d \n", brackets.c_str(), isVal);

    brackets = "([)]";
    isVal = s9.isValidBrackets(brackets);
    printf("brackets string:%s, is valid brackets:%d \n", brackets.c_str(), isVal);
    std::cout << "LeetCode Soultuion 9 'ValidBrackets' <==============\n\n";

}
