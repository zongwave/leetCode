/*****************************************************************************************************************
  移动语义与完美转发（面试频率 ★★★★☆）

  1. 值类别（简化模型）：
     - 能取地址、有名字的是左值（lvalue）；临时的、快死的、不能取地址的是右值（rvalue）。
     - 函数重载决议：const T& 收一切；T&& 只收右值。同名重载同时存在时，右值走 T&&。

  2. 右值引用 T&& 与 std::move：
     - std::move 本身一个字节都不搬，它只是把左值强转成右值（xvalue），发出"我可以被掏空"的信号。
     - 真正的"移动"发生在移动构造/移动赋值里：偷资源 + 把对方置成可安全析构的空状态。

  3. 规则五（Rule of Five）：
     - 管裸资源的类，析构/拷贝构造/拷贝赋值/移动构造/移动赋值 五个特殊成员要一起面对。
     - 只要用户声明了析构或拷贝操作，编译器就不再自动合成移动操作 —— 移动会静默退化成拷贝。

  4. 移动构造要标 noexcept：
     - vector 扩容搬迁元素时，若 move ctor 不是 noexcept，为保证强异常安全会退回逐个拷贝。
     - "移动比拷贝快"的优势要靠 noexcept 才能兑现 —— 高频面试题。

  5. 完美转发：
     - 模板参数中的 T&& 是转发引用（万能引用），不是右值引用；配合 std::forward 原样保留值类别。
     - make_shared/make_unique 就是这么把构造参数"毫发无损"转给 new T 的。

******************************************************************************************************************/

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace MoveSem {

// ---------------- 任务 1：一次调用，三种命运 ----------------
// 同名重载：左值走 const&，右值走 &&
void use(const std::string& s) { std::cout << "[lvalue] copied : " << s << "\n"; }
void use(std::string&& s)      { std::cout << "[rvalue] moved  : " << s << "\n"; }

void task_value_category() {
    std::cout << "\n==> task 1 value category \n";
    std::string named = "hello! this string is way too long for SSO.";
    use(named);          // 具名变量 = 左值 -> const& 版本
    use("world");        // 字符串面量 -> 临时 string -> && 版本

    // TODO 1.1 写下 use(std::move(named));  看走哪个版本；随后打印 named，观察它变成什么样
    use(std::move(named));
    std::cout << "named after move: " << named << "\n"; // named 可能被掏空，值不确定
    // TODO 1.2 接一个具名右值引用 std::string&& r = std::string("tmp"); 再 use(r);
    //          —— r 有名字，是左值还是右值？（具名右值引用仍是左值，面试常考）
    std::string&& r = std::string("tmp");
    use(r);

    std::string stolen = std::move(named);
    std::cout << "stolen = [" << stolen << "]  named = [" << named << "]\n";
}

// ---------------- 任务 2：给裸资源类补齐"五个特殊成员" ----------------
class Owner {
public:
    explicit Owner(std::size_t n = 4) : m_n(n), m_data(new int[n]) {
        std::cout << "[ctor Owner n=" << n << "]\n";
    }
    ~Owner() { delete[] m_data; std::cout << "[dtor Owner]\n"; }

    // 拷贝操作（已给出：深拷贝）
    Owner(const Owner& o) : m_n(o.m_n), m_data(new int[o.m_n]) {
        for (std::size_t i = 0; i < m_n; ++i) m_data[i] = o.m_data[i];
        std::cout << "[copy ctor] deep copy " << m_n << " ints\n";
    }
    Owner& operator=(const Owner& o) {
        if (this == &o) return *this;
        delete[] m_data;
        m_n = o.m_n;
        m_data = new int[m_n];
        for (std::size_t i = 0; i < m_n; ++i) m_data[i] = o.m_data[i];
        std::cout << "[copy assign] deep copy " << m_n << " ints\n";
        return *this;
    }

    // TODO 2.1 实现移动构造：偷指针 + 把对方置空，必须标 noexcept
    // Owner(Owner&& o) noexcept : m_n(o.m_n), m_data(o.m_data) {
    //     o.m_n = 0; o.m_data = nullptr;
    //     std::cout << "[move ctor]\n";
    // }
    Owner(Owner&& o) noexcept : m_n(o.m_n), m_data(o.m_data) {
        o.m_n = 0; o.m_data = nullptr;
        std::cout << "[move ctor] " << m_n << " ints\n";
    }
    // TODO 2.2 实现移动赋值：先防自移动，释放自己的资源，再偷
    // Owner& operator=(Owner&& o) noexcept { ... }
    Owner& operator=(Owner&& o) noexcept {
        if (this == &o) return *this; // 防自移动
        delete[] m_data;
        m_n = o.m_n;
        m_data = o.m_data;
        o.m_n = 0; o.m_data = nullptr;
        std::cout << "[move assign] " << m_n << " ints\n";
        return *this;
    }

    std::size_t size() const { return m_n; }

private:
    std::size_t m_n;
    int*        m_data;
};

void task_rule_of_five() {
    std::cout << "\n==> task 2 rule of five \n";
    Owner a(3);
    Owner b = a;              // 深拷贝 [copy ctor]
    Owner c = std::move(a);   // 未实现移动构造前：这一步其实还在走拷贝！（编译器不再自动合成）
    std::cout << "after move: a.size()=" << a.size()
              << "  c.size()=" << c.size() << "\n";
    // TODO 2.3 先按现状运行：a 被"移动"后 size 还是多少？为什么说这是"拷贝假扮的移动"？
    // size = 3 没有实现移动构造，编译器调用拷贝构造。
    // TODO 2.4 实现 2.1/2.2 后再运行：应看到 [move ctor]，且 a.size()=0 c.size()=3
}

// ---------------- 任务 3：noexcept —— 移动快不快，容器说了算 ----------------
void task_noexcept_vector() {
    std::cout << "\n==> task 3 noexcept \n";
    std::vector<Owner> v;
    v.reserve(1);
    v.push_back(Owner(1));            // 临时对象 -> 需要移动构造（没实现则拷贝）
    Owner big(10);
    v.push_back(big);                 // 左值 -> 拷贝
    v.push_back(std::move(big));      // std::move -> 移动
    v.reserve(100);                   // 扩容搬迁 3 个元素：走移动还是拷贝？看日志！
    std::cout << "v.size() = " << v.size() << "\n";
    // TODO 3.1 对比"实现带 noexcept 的移动构造"前后，reserve 扩容那一步的日志差异
    // 带 noexcept 的移动构造会走移动构造函数，否则走拷贝构造函数。
    // TODO 3.2 写注释回答：vector 为什么要求移动构造 noexcept？（提示：强异常安全保证，
    //          扩容中途抛异常时"拷贝"可以整体回滚，"移动"会把旧容器掏成半成品）
    // 所以移动构造到底好还是不好呢？标了 noexcept 快，但抛异常时可能会破坏原来的对象数据。
}

// ---------------- 任务 4：函数返回本地对象 ----------------
Owner make_owner() {
    Owner local(7);
    return local;   // 这里没有 std::move —— 为什么不用？
    //return std::move(local);
}

void task_return_value() {
    std::cout << "\n==> task 4 return local \n";
    Owner o = make_owner();
    // TODO 4.1 数一数日志：对象构造了几次？拷贝/移动出现了几次？（提示：NRVO 直接原地造）
    // 对象构造了一次，没有移动构造或者拷贝构造。这个可能时编译器做的优化吧。
    // TODO 4.2 把 return local 改成 return std::move(local); 再跑 —— 反而可能变慢，为什么？
    //          （提示：显式 move 会废掉 NRVO，退回"移动构造"这条次优路）
    // 一次构造，一次移动构造。编译器无法再优化了。
    // TODO 4.3 回忆 unique_ptr 练习的 1.4：返回 unique_ptr 不用写 move 是同一个机制
    (void)o;
}

// ---------------- 任务 5（选做）：完美转发，手写 make_unique ----------------
// TODO 5.1 在 make_owner 上方合适的位置实现：
// template <class T, class... Args>
// std::unique_ptr<T> my_make_unique(Args&&... args) {
//     return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
// }
// TODO 5.2 用 Owner 测试：my_make_unique<Owner>(5)；
//          再给 Owner 加一个 Owner(const std::string&, std::size_t) 构造函数，
//          试 my_make_unique<Owner>("cfg", 5) —— 体会 string 左值/右值被转发时的区别：
//          std::forward<Args> 对左值实参转发成 const&，对临时量转发成 &&，一次拷贝都不多。

} // namespace MoveSem

using namespace MoveSem;

int run_move_semantics(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    task_value_category();
    task_rule_of_five();
    task_noexcept_vector();
    task_return_value();
    return 0;
}