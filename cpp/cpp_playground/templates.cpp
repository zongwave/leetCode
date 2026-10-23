/*****************************************************************************************************************
  Templates & Specialization (interview freq. ****)

  1. Function template & argument deduction:
     - Template args are deduced from CALL ARGS (mostly). MyMax(3, 2.5) fails:
       T deduced as int AND double -> contradiction. Fix: two params / explicit
       <T> / common_type.
     - Template args must be compile-time known; types or constexpr values.

  2. Class template:
     - Must be instantiated explicitly (or via CTAD since C++17: Pair p(1,2.0)).
     - Can have default template args & partial member specialization.

  3. Specialization:
     - Full specialization: function & class templates both allowed.
     - PARTIAL specialization: ONLY class templates! Function templates
       CANNOT be partially specialized -> use OVERLOAD instead (interview Q).
     - Lookup order: full spec > partial spec > primary template.

  4. Compile-time branching:
     - SFINAE (enable_if): old style, overload-set filtering.
     - if constexpr (C++17): discarded branch NOT instantiated -> can differ
       radically per type; far more readable. Regular if would still compile
       ALL branches (error if a branch is invalid for T).
     - concepts (C++20): named requirements, readable errors.

  5. Variadic templates:
     - Pack expansion + fold expression ((args + ...) etc.) replace recursion.

******************************************************************************************************************/

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace Templates {

// ---------------- task 1: deduction & its traps ----------------
template <typename T>
T my_max(T a, T b) {
    return a > b ? a : b;
}

void task_deduction() {
    std::cout << "\n==> task 1 template argument deduction \n";
    std::cout << "my_max(3, 7)      = " << my_max(3, 7) << "\n";       // T=int, fine
    std::cout << "my_max<int>(3, 7) = " << my_max<int>(3, 7.5) << "\n";// explicit T=int: 7.5 converted
    // TODO 1.1 uncomment -> compile error. Read MSVC message, find WHICH two
    //          deductions contradict:
    // std::cout << my_max(3, 2.5) << "\n";
    // TODO 1.2 fix it THREE ways, all must compile:
    //   (a) explicit template arg  my_max<double>(...)
    //   (b) two template params: template <class A, class B> auto my_max2(A,B)
    //   (c) std::common_type_t<A,B> as return/param type
    // TODO 1.3 my_max("a", "b") compiles but result is pointer-compare garbage.
    //          Why? (const char* T, compares addresses) -> how to fix? (specialize
    //          or overload for const char* using strcmp) -- try (b) from task 3.
}

// ---------------- task 2: class template + CTAD ----------------
template <typename T, std::size_t Cap>   // type param + constexpr VALUE param
class MiniStack {
public:
    bool push(const T& v) {
        if (m_n >= Cap) return false;
        m_buf[m_n++] = v;
        return true;
    }
    bool pop(T& out) {
        if (!m_n) return false;
        out = m_buf[--m_n];
        return true;
    }
    std::size_t size() const { return m_n; }
private:
    T           m_buf[Cap]{};
    std::size_t m_n = 0;
};

void task_class_template() {
    std::cout << "\n==> task 2 class template \n";
    MiniStack<int, 3> st;
    st.push(1); st.push(2);
    int top = 0;
    st.pop(top);
    std::cout << "popped " << top << ", size now " << st.size() << "\n";
    // TODO 2.1 MiniStack<int,3> and MiniStack<int,4> are DIFFERENT types.
    //          Prove it: static_assert(!std::is_same_v<decltype(st), MiniStack<int,4>>);
    // TODO 2.2 why must Cap be a value template param (std::size_t) rather than
    //          a constructor argument? (answer: m_buf[Cap] is part of the TYPE's
    //          memory layout -- decided at compile time, zero heap)
    // TODO 2.3 MiniStack<double, 3> -- does any code get compiled twice?
    //          (yes: one instantiation per unique <T,Cap> -- template = code
    //          GENERATOR, MiniStack itself does not exist as a real class)
}

// ---------------- task 3: specialization ----------------
// primary template: the "fallback"
template <typename T>
struct TypeTag {
    static const char* name() { return "some other type"; }
};
// FULL specialization for int
template <>
struct TypeTag<int> {
    static const char* name() { return "int (full spec)"; }
};
// PARTIAL specialization: any pointer
template <typename T>
struct TypeTag<T*> {
    static const char* name() { return "pointer (partial spec)"; }
};
// PARTIAL specialization: any std::vector<T>
template <typename T>
struct TypeTag<std::vector<T>> {
    static const char* name() { return "vector (partial spec)"; }
};

void task_specialization() {
    std::cout << "\n==> task 3 specialization \n";
    std::cout << "int          -> " << TypeTag<int>::name() << "\n";
    std::cout << "double       -> " << TypeTag<double>::name() << "\n";
    std::cout << "int*         -> " << TypeTag<int*>::name() << "\n";
    std::cout << "vector<int>  -> " << TypeTag<std::vector<int>>::name() << "\n";
    // TODO 3.1 add FULL spec for double -> "double (full spec)". Run. What changes?
    // TODO 3.2 CRITICAL INTERVIEW Q: why does TypeTag<double*> pick the pointer
    //          partial spec while TypeTag<std::vector<int>> also picks partial?
    //          Add TypeTag<std::vector<int>*>: which wins? Why? (more specialized
    //          wins: pointer-of-vector matches BOTH T* and vector<T> partials)
    // TODO 3.3 function templates CANNOT be partially specialized:
    //          template<> void f(int*) {}          // FULL spec: legal
    //          template<T> void f(std::vector<T>){} // looks partial: actually an OVERLOAD
    //          Write both & call them; notice the "partial one" is just an
    //          overload and overload resolution picks it -- subtle difference
    //          when combined with explicit template args. Research: f<>(x) vs f(x).

    // TODO 3.4 const trap: TypeTag<const int>::name() -- full spec <int> does NOT
    //          match (top-level const changes the arg). Print it. How to make
    //          const int reuse the int spec? (std::remove_cv_t before dispatch)
}

// ---------------- task 4: if constexpr vs SFINAE ----------------
template <typename T>
void describe(T v) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "integral " << v << ", is even: " << (v % 2 == 0) << "\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "float    " << v << ", truncated: " << static_cast<long>(v) << "\n";
    } else {
        std::cout << "other    \"" << v << "\", length: " << std::string(v).size() << "\n";
    }
}

// old-style SFINAE: this overload EXISTS ONLY if T is integral
template <typename T>
typename std::enable_if<std::is_integral_v<T>, double>::type
safe_div(T a, T b) {                     // v % 2 in describe() above would BREAK
    return b ? static_cast<double>(a) / b : 0.0;   // compile for std::string if it
}                                       // were a normal if!

void task_compile_time_branch() {
    std::cout << "\n==> task 4 if constexpr / SFINAE \n";
    describe(42);
    describe(3.14);
    describe("hello");
    std::cout << "safe_div(7,2) = " << safe_div(7, 2) << "\n";
    // TODO 4.1 change describe() to a NORMAL if (drop constexpr) -> compile fails.
    //          Read the error: (v % 2) instantiated for const char* -> boom.
    //          This is the difference: if constexpr THROWS AWAY the dead branch.
    // TODO 4.2 uncomment: safe_div(3.5, 2.0); -> no matching function. Why?
    //          (enable_if removes the overload from the set; C++20 way:
    //           template<std::integral T> ... -- concepts, same effect, readable)
    // TODO 4.3 rewrite safe_div with C++20 concepts:
    //          double safe_div2(std::integral auto a, std::integral auto b);
}

// ---------------- task 5: variadic + fold ----------------
// old-school recursion (pre-C++17 style), kept as reference:
void my_print() { std::cout << "\n"; }
template <typename Head, typename... Rest>
void my_print(Head h, Rest... rest) {
    std::cout << h << ' ';
    my_print(rest...);
}

// TODO 5.1 rewrite it with a C++17 FOLD EXPRESSION (no base case needed):
// template <typename... Args>
// void fold_print(Args&&... args) {
//     ((std::cout << args << ' '), ...);   // left fold over comma operator
//     std::cout << "\n";
// }
// TODO 5.2 try other fold forms with a numeric pack:
//     template<typename... N> auto sum(N... n) { return (n + ...); }        // (n + ...)
//     template<typename... N> auto sum1(N... n) { return (1 + ... + n); }   // (init op ... op pack)
//     template<typename... N> bool all_even(N... n) { return ((n % 2 == 0) && ...); }
// TODO 5.3 why Args&&... (forwarding refs) matters here: pass a std::string
//          lvalue & a temporary, add a print_all that forwards to a sink taking
//          const std::string&. (connects to task 5 of move_semantics: forward)
} // namespace Templates

using namespace Templates;

int run_templates(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    task_deduction();
    task_class_template();
    task_specialization();
    task_compile_time_branch();
    my_print(1, 2.5, std::string("three"), "four");
    return 0;
}