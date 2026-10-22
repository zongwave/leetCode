// C++ Exercise Playground - dispatcher
// 运行方式：
//   1. 直接运行 -> 交互式菜单选择练习
//   2. cpp_playground.exe <exercise-name> [args...] -> 直接运行指定练习并透传参数
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// 各练习的入口函数声明（新练习在此声明 + 在 g_exercises 表中注册）
int run_hello(int argc, char* argv[]);
int run_abstract_class(int argc, char* argv[]);

struct Exercise {
    const char* name;
    const char* desc;
    int (*fn)(int, char*[]);
};

static const std::vector<Exercise> g_exercises = {
    {"hello",          "Sample playground entry point",       run_hello},
    {"abstract_class", "Abstract class & virtual functions",  run_abstract_class},
};

static const Exercise* find_exercise(const std::string& name) {
    auto it = std::find_if(g_exercises.begin(), g_exercises.end(),
        [&](const Exercise& e) { return name == e.name; });
    return it == g_exercises.end() ? nullptr : &(*it);
}

int main(int argc, char* argv[]) {
    // 方式 2: 提供练习名则直接运行（并把后续参数透传给该练习）
    if (argc > 1) {
        if (const Exercise* e = find_exercise(argv[1])) {
            return e->fn(argc - 2, argv + 2);
        }
        std::cerr << "Unknown exercise: " << argv[1] << "\n";
        return 1;
    }

    // 方式 1: 交互式菜单
    std::cout << "=== C++ Exercise Playground ===\n";
    for (size_t i = 0; i < g_exercises.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << g_exercises[i].name
                  << " - " << g_exercises[i].desc << "\n";
    }
    std::cout << "Select exercise (1-" << g_exercises.size() << "): ";

    std::string line;
    if (!std::getline(std::cin, line)) {
        return 0;
    }
    int choice = 0;
    try {
        choice = std::stoi(line);
    } catch (...) {
        std::cerr << "Invalid input.\n";
        return 1;
    }
    if (choice < 1 || static_cast<size_t>(choice) > g_exercises.size()) {
        std::cerr << "Out of range.\n";
        return 1;
    }
    // 菜单路径：命令行上没有任何参数，传空列表（argv[argc] 是合法的 nullptr 收尾位）
    return g_exercises[choice - 1].fn(0, argv + argc);
}