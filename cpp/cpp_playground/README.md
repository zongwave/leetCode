# C++ 练习 Playground（单工程多练习）

用一个 CMake 工程承载多个独立的 C++ 练习专题。所有练习每次一起编译，
运行时通过调度器 `main.cpp` 选择其中一个执行。

## 编译

```powershell
cmake --preset vs                     # 首次配置（生成 build/vs/）
cmake --build build/vs --config Debug
```

工程根目录下有 `CMakePresets.json`：`vs` preset 用 VS 生成器（任何终端直接可用），
`ninja` preset 用 Ninja Multi-Config（更快，但需 Developer Shell / ninja 在 PATH）。
cmake 不在 PATH 时可用 VS 自带的：
`C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`

产物在 `build/vs/Debug/cpp_playground.exe`（Ninja 则在 `build/ninja/Debug/`）。

## 运行方式
- **交互式菜单**：直接运行 `cpp_playground.exe`，按提示输入序号选择练习。
- **命令行直达**：`cpp_playground.exe <练习名> [arg...]`，直接运行；练习从
  `argv[0]` 起收到自己的参数（练习名之后的部分，没有就是空列表）。

## 目录结构
- `CMakeLists.txt` / `CMakePresets.json` —— 全部构建配置（就这两个文件）
- `main.cpp` —— 调度器入口（菜单 + 参数分发）
- `sample_hello.cpp` —— 示例练习，顺带展示 argc/argv 透传
- `virtual_class.cpp` —— 抽象类与虚函数练习

## 新增一个练习（2 步）
1. 新建 `topic_xxx.cpp`（`CMakeLists.txt` 用 `file(GLOB ... CONFIGURE_DEPENDS "*.cpp")`
   通配包含，新文件自动参与编译，无需改任何构建文件）。内容不含 `main()`；
   主题符号包进具名 namespace，入口函数留在 namespace 外：
   ```cpp
   namespace TopicXxx { /* 类、辅助函数 */ }
   using namespace TopicXxx;

   int run_xxx(int argc, char* argv[]) { /* ... */ return 0; }
   ```
2. 在 `main.cpp` 顶部声明它，并在 `g_exercises` 表中注册一行：
   ```cpp
   int run_xxx(int argc, char* argv[]);
   // ...
   {"xxx", "一句话说明", run_xxx},
   ```

要点：每个练习文件必须有且仅有一个入口函数、**不能有 `main()`**；
练习内的类与辅助符号包进各自的 namespace，避免多个练习之间的全局名冲突。

## 备选方案：暂时移出某个练习
想把某个练习暂时排除编译（它写到一半编译不过、或想单跑某一个），把文件扩展名
临时改成 `*.cpp.off`——GLOB 只匹配 `*.cpp`，改名即排除，改回即恢复。
