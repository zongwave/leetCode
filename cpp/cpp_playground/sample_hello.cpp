#include <iostream>

// 示例练习：hello + 展示 dispatcher 透传的 argc/argv
int run_hello(int argc, char* argv[]) {
    std::cout << "Hello from the C++ Playground!\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << "  arg[" << i << "] = " << argv[i] << "\n";
    }
    return 0;
}