/*****************************************************************************************************************
  RAII 与智能指针（面试频率 ★★★★★）

  1. RAII（Resource Acquisition Is Initialization）：
     - 把资源的生命周期绑定到栈上对象的生命周期：构造函数获取，析构函数释放。
     - 好处：离开作用域、甚至抛异常时都保证释放；new/delete 手动配对的问题正在于此。

  2. std::unique_ptr —— 独占所有权：
     - 不可拷贝（拷贝构造/赋值被 delete），只能 std::move 转手；转手后原指针为 nullptr。
     - 大小与裸指针相同，零开销；默认删除器是 delete。

  3. std::shared_ptr —— 共享所有权：
     - 引用计数在"控制块"里（另一块堆内存），拷贝 +1、析构 -1，归零时释放资源。
     - make_shared 把对象与控制块一次分配；shared_ptr(new T) 是两次分配 —— 高频面试题。

  4. 循环引用与 std::weak_ptr：
     - 两个 shared_ptr 互指 → 计数永远不归零 → 资源泄漏（本练习会"亲眼看到"析构没发生）。
     - weak_ptr 不增加引用计数，用 lock() 升级为 shared_ptr 再使用。

  5. 自定义删除器：
     - 释放动作不是 delete 的场景（fclose、free、连接池归还）用删除器定制。
     - unique_ptr 的删除器进类型（decltype 会变）；shared_ptr 的删除器进控制块（类型不变）—— 也是面试题。

******************************************************************************************************************/

#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <type_traits>

#include <string>
#include <utility>

namespace SmartPtr {

// 被追踪的资源：靠构造/析构的打印，观察"资源何时被真正释放"
class Buffer {
public:
    explicit Buffer(std::string name) : m_name(std::move(name)) {
        std::cout << "[+] Buffer(" << m_name << ") Construct \n";
    }
    ~Buffer() {
        std::cout << "[-] Buffer(" << m_name << ") Deconstruct  <-- run into this line = resource is free\n";
    }
    const std::string& name() const { return m_name; }

private:
    std::string m_name;
};

// ---------------- 任务 1：unique_ptr 独占所有权 ----------------
void task_unique_ptr() {
    // 1.1 用 std::make_unique<Buffer> 创建名为 "u1" 的对象
    std::unique_ptr<Buffer> u1 = std::make_unique<Buffer>("U1");
    // 1.2 写 `auto u2 = u1;` 试编译 —— 报 use of deleted function：被 delete 的是拷贝构造函数（复制被禁用）；确认后注释掉
    // auto u2 = u1;
    // 1.3 用 std::move 把 u1 转手给 u2；打印转移后 u1 的 get() 值（提示：nullptr）
    std::unique_ptr<Buffer> u2 = std::move(u1);
    std::cout << "u1.get() = " << u1.get() << "\n";
    // 1.4 思考（不用写）：函数返回本地 unique_ptr 时为什么不需要显式 move？
}

// ---------------- 任务 2：shared_ptr 引用计数 ----------------
void task_shared_ptr() {
    std::cout << "\n==> task 2 shared_ptr \n"; 
    // TODO 2.1 用 std::make_shared<Buffer> 创建 "s1"，打印 use_count()（预期 1）
    std::shared_ptr<Buffer> s1;

    s1 = std::make_shared<Buffer>("S1");
    std::cout << " Buffer(" << s1->name() << ") use count: " << s1.use_count() << std::endl;

    // TODO 2.2 拷贝 s1 到 s2，打印两者各自的 use_count()（预期都是 2）
    {
        std::shared_ptr<Buffer> s2 = s1;
        std::cout << " Buffer(" << s2->name() << ") use count: " << s2.use_count() << std::endl;
        std::cout << "s1 get: " << s1.get() << std::endl;
        std::cout << "s2 get: " << s2.get() << std::endl;
    }
    std::cout << " Buffer(" << s1->name() << ") use count: " << s1.use_count() << std::endl;

    // TODO 2.3 让 s2 先离开作用域（写个内层 {}），观察析构了吗？计数变几？
    //          再让 s1 出作用域 —— 析构出现在哪一刻？
    // TODO 2.4 在代码旁写一句注释：make_shared 与 shared_ptr(new Buffer) 的区别（至少 1 条）
    // make_shared<Buffer> 一次分配控制块与对象，shared_ptr(new Buffer) 分别分配对象与控制块。allocator 调用少一倍。
    // 内存对齐，访问效率高。
}

// ---------------- 任务 3：循环引用 —— 先制造泄漏，再修好它 ----------------
// 注意观察：任务 3 结束时两个 Buffer 的析构行有没有出现？
struct Node {
    explicit Node(std::string name) : m_name(std::move(name)) {
        std::cout << "[+] Node(" << m_name << ") Construct\n";
    }
    ~Node() {
        std::cout << "[-] Node(" << m_name << ") Deconstruct\n";
    }
    //std::shared_ptr<Node> next;   // TODO 3.2 修泄漏时：把这里改成 std::weak_ptr<Node>，
                                  //         使用处配合 lock() 升级（3.1 的代码不用动）
    std::weak_ptr<Node> next;
    std::string m_name;
};

void task_circular_ref() {
    std::cout << "\n==> task 3 shared_ptr circular ref \n"; 

    // TODO 3.1 建两个 make_shared<Node>("A") 和 ("B")，让 a->next = b、b->next = a，
    //          然后让两者出作用域 —— 观察：析构行没出现 = 泄漏实锤
    std::shared_ptr<Node> a = std::make_shared<Node>("A");
    std::cout << "a count: " << a.use_count() << std::endl;

    std::shared_ptr<Node> b = std::make_shared<Node>("B");
    std::cout << "b count: " << b.use_count() << std::endl;

    a->next = b;
    std::cout << "a count: " << a.use_count() << std::endl;
    std::cout << "b count: " << b.use_count() << std::endl;

    b->next = a;
    std::cout << "a count: " << a.use_count() << std::endl;
    std::cout << "b count: " << b.use_count() << std::endl;

    // TODO 3.2 把 Node::next 改成 weak_ptr，重跑 —— 析构回来了
    // TODO 3.3 在 3.2 版本里调用 a->next：先判断 expired()，再 lock() 拿到 shared_ptr 用
    // 写法一：先 expired() 判断，再 lock() 升级（面试思路）
    if (!a->next.expired()) {
        std::shared_ptr<Node> sp = a->next.lock();   // 升级：强计数 +1
        std::cout << "got node: " << sp->m_name
                  << ", count while using: " << sp.use_count() << std::endl;
    }   // sp 出作用域，强计数 -1

    // 写法二：工程推荐 —— 直接 lock() 判空（原子完成检查+加一，无竞态窗口）
    if (auto sp2 = a->next.lock()) {
        std::cout << "lock() got: " << sp2->m_name << std::endl;
    } else {
        std::cout << "lock() returned nullptr\n";
    }

    // 模拟 B 死亡：B 强计数归零立即析构；a->next（weak）之后仍可安全探测
    b.reset();                                       // 会打印 [-] Node(B) Deconstruct
    std::cout << "after b.reset(): expired() = "
              << std::boolalpha << a->next.expired() << std::endl;
    if (auto sp3 = a->next.lock()) {
        std::cout << "should NOT see this line\n";
    } else {
        std::cout << "lock() = nullptr, safe\n";
    }
}

// ---------------- 任务 4：自定义删除器 ----------------
void task_custom_deleter() {
    // TODO 4.1 创建 unique_ptr，删除器用 lambda 打印 "[custom delete]" 再 delete；
    //          先试 `auto p = std::unique_ptr<Buffer>(new Buffer("d1"), ...)` ——
    //          编译会失败，想想为什么（提示：lambda 的类型每个都不一样），
    //          再改用 std::function<void(Buffer*)> 或具名函数指针让它通过
    // TODO 4.2 用 shared_ptr 挂同样的删除器 —— 这次 decltype(p) 变了吗？
    //          和 4.1 的结论对比，就是那道面试题的答案

    std::cout << "\n==> task 4 custom deleter \n";

    // ---- 4.1a 错误示范（保留注释，放开编译验证）----
    // 每个 lambda 都是唯一的匿名闭包类型，写不出来；而 unique_ptr 的删除器
    // 是第二个模板参数（属于类型的一部分）：std::unique_ptr<Buffer> 实际是
    // unique_ptr<Buffer, default_delete<Buffer>>，挂 lambda 类型对不上 -> 编译失败：
    // auto pBad = std::unique_ptr<Buffer>(
    //     new Buffer("bad"),
    //     [](Buffer* b) { std::cout << "[custom delete]\n"; delete b; });

    // ---- 4.1b 修复：给删除器一个"可命名的类型" ----
    // 方案1：std::function 类型擦除（灵活，但 unique_ptr 体积变大）
    using Deleter = std::function<void(Buffer*)>;
    auto p = std::unique_ptr<Buffer, Deleter>(
        new Buffer("d1"),
        [](Buffer* b) { std::cout << "[custom delete] unique: " << b->name() << "\n"; delete b; });
    std::cout << "sizeof unique_ptr<Buffer>           = "
              << sizeof(std::unique_ptr<Buffer>) << "\n";
    std::cout << "sizeof unique_ptr<Buffer, function> = " << sizeof(p)
              << "   <-- deleter is part of the TYPE: it occupies the object itself\n";

    // 方案2：函数指针（无捕获 lambda 可隐式转换，更轻）
    auto p2 = std::unique_ptr<Buffer, void (*)(Buffer*)>(
        new Buffer("d2"),
        [](Buffer* b) { std::cout << "[ptr delete]   unique: " << b->name() << "\n"; delete b; });

    // ---- 4.2 shared_ptr 挂同样的 lambda：类型不变 ----
    auto sp = std::shared_ptr<Buffer>(
        new Buffer("d3"),
        [](Buffer* b) { std::cout << "[custom delete] shared: " << b->name() << "\n"; delete b; });
    std::cout << "sizeof shared_ptr<Buffer>           = " << sizeof(sp)
              << "   <-- deleter lives in the control block (type-erased): type & size unchanged\n";
    std::cout << "decltype(sp) is still std::shared_ptr<Buffer>: containers / params unaffected\n";
    // 对比结论：unique_ptr 换删除器 -> decltype 变了（要写对第二个模板参数）；
    //           shared_ptr 换删除器 -> decltype 不变（删除器存在控制块里）。

    // ---- The differences are COMPILE-TIME, not in runtime output ----
    // Diff 1: decltype. p2's type changed (deleter in type); sp's did not.
    static_assert(!std::is_same<decltype(p2), std::unique_ptr<Buffer>>::value,
                  "unique_ptr + fn-ptr deleter => a DIFFERENT type");
    static_assert(std::is_same<decltype(sp), std::shared_ptr<Buffer>>::value,
                  "shared_ptr + lambda deleter => SAME plain type");
    std::cout << "sizeof(p2) unique_ptr<Buffer, fn*>  = " << sizeof(p2)
              << "  (this type is NOT unique_ptr<Buffer>!)\n";

    // Diff 2: capture. shared_ptr's deleter is type-erased, so it MAY capture;
    //         p2's lambda must stay capture-less to fit into void(*)(Buffer*).
    int tag = 42;
    auto sp2 = std::shared_ptr<Buffer>(
        new Buffer("d4"),
        [tag](Buffer* b) { std::cout << "[captured tag=" << tag
                                     << "] shared: " << b->name() << "\n"; delete b; });
    // Uncomment to see it FAIL: capturing lambda won't convert to a function pointer
    // auto p3 = std::unique_ptr<Buffer, void (*)(Buffer*)>(
    //     new Buffer("bad"),
    //     [tag](Buffer* b) { std::cout << tag << "\n"; delete b; });

    // Diff 3: put them in ONE container. shared_ptrs with different deleters
    //         share the same type -> fine. unique_ptr with fn-ptr deleter into a
    //         vector of unique_ptr<Buffer> -> type mismatch, compile error.
    std::vector<std::shared_ptr<Buffer>> pool;
    pool.push_back(std::move(sp));   // lambda deleter
    pool.push_back(std::move(sp2));  // capturing lambda deleter, same element type!
    // pool is empty now; each element's own deleter runs when its count hits 0
    // Uncomment to see it FAIL:
    // std::vector<std::unique_ptr<Buffer>> upool;
    // upool.push_back(std::move(p2)); // ✗ unique_ptr<Buffer,function-ptr> is NOT unique_ptr<Buffer>
    std::cout << "pool.size() = " << pool.size()
              << "  (both shared_ptrs coexist despite DIFFERENT deleters)\n";
}

// ---------------- 任务 5（选做，白板高频题）：手写 MiniSharedPtr ----------------
// 在 namespace 里实现一个最小共享指针：
//
// template <class T>
// class MiniSharedPtr {
// public:
//     explicit MiniSharedPtr(T* p);          // 分配计数块，count=1
//     MiniSharedPtr(const MiniSharedPtr& o); // 拷贝：指针+计数都共享，count++
//     MiniSharedPtr& operator=(const MiniSharedPtr& o);
//     MiniSharedPtr(MiniSharedPtr&& o) noexcept;  // 移动：偷指针，o 置空
//     ~MiniSharedPtr();                      // count--，归零时 delete 对象和计数块
//     T* operator->() const { return m_ptr; }
//     T& operator*() const { return *m_ptr; }
//     long use_count() const;
//
// private:
//     T*    m_ptr  = nullptr;
//     long* m_count = nullptr;   // 思考：为什么计数必须放在堆上、且被所有副本共享？
// };
//
// 写完在这里测试：拷贝两个副本打印 use_count，出作用域观察 Buffer 析构。

} // namespace SmartPtr

using namespace SmartPtr;

int run_smart_pointer(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    task_unique_ptr();
    task_shared_ptr();
    task_circular_ref();
    task_custom_deleter();
    return 0;
}
