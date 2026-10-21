# C++ 练习 Playground（单工程多练习）

用一个 Visual Studio 工程承载多个独立的 C++ 练习专题。所有练习每次一起编译，
运行时通过调度器 `main.cpp` 选择其中一个执行。

## 运行方式
- **交互式菜单**：直接运行 `Project1.exe`，按提示输入序号选择练习。
- **命令行直达**：`Project1.exe <练习名> [arg...]`，直接运行并透传参数，例如
  `Project1.exe abstract_class`。

## 目录结构
- `main.cpp` —— 调度器入口（菜单 + 参数分发）
- `sample_hello.cpp` —— 示例练习，顺带展示 argc/argv 透传
- `virtual_class.cpp` —— 抽象类与虚函数练习

## 新增一个练习（3 步）
1. 新建 `topic_xxx.cpp`，内容不含 `main()`，改为入口函数：
   ```cpp
   int run_xxx(int argc, char* argv[]) { /* ... */ return 0; }
   ```
2. 在 `main.cpp` 顶部声明它在 `g_exercises` 表中注册一行：
   ```cpp
   int run_xxx(int argc, char* argv[]);
   // ...
   {"xxx", "一句话说明", run_xxx},
   ```
3. 在 VS 中：右键工程 → 添加 → 现有项，选中 `topic_xxx.cpp`；
   或在 `Project1.vcxproj` 的 `<ClCompile>` 里补一行 `topic_xxx.cpp`。

要点：每个练习文件必须有且仅有一个入口函数、**不能有 `main()`**；
不同练习之间不要定义同名全局符号（否则链接冲突）。

## 备选方案：保留各自 main() 的写法
若某个练习特别想保留自己的 `main()`（例如专门练习 argc/argv），可改用
「仅编译当前练习」的方式：右键工程中其它 `.cpp` → 从项目中排除（Exclude from
Project），只保留要运行的那个，编译运行后再改回来。`sample_hello.cpp` 已示范了
在 dispatcher 框架下如何用 `run_xxx(int argc, char* argv[])` 处理命令行参数。