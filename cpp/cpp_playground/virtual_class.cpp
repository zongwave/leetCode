/*****************************************************************************************************************
  Abstract Class and Virtual Functions Example

  1. 抽象类与纯虚函数：
	 - 含有至少一个纯虚函数（声明中带有 = 0）的类是抽象类，不能直接实例化。
	 - 派生类必须覆盖所有继承来的纯虚函数，才能成为可实例化的具体类。
	 - 纯虚函数定义基类接口，约定派生类需要提供的行为。

  2. 虚析构函数：
	 - 若可能通过基类指针销毁派生类对象，基类析构函数应声明为 virtual。
	 - 这样销毁对象时，执行顺序为：派生类析构函数体 → 成员按声明逆序析构 → 基类析构函数体。
	 - 若基类析构函数非虚，通过基类指针 delete 派生类对象属于未定义行为。

  3. 虚成员函数与运行时多态：
	 - 派生类可以覆盖基类的虚函数，为其提供特定实现。
	 - 通过基类指针或引用调用虚函数时，会按对象的实际类型（动态类型）选择实现，即运行时多态。

  4. 覆盖与隐藏：
	 - 覆盖（override）是派生类重新实现基类的虚函数；建议使用 override 让编译器检查。
	 - 派生类声明同名函数会隐藏基类同名函数；非虚函数不会因此产生运行时多态。
	 - 对比：虚函数按对象的动态类型分派（多态），非虚函数按表达式的静态类型分派——这正是隐藏产生的原因。

******************************************************************************************************************/



#include <iostream>
#include <memory>
#include <string>

namespace VirtualAbs {

	class Base {
	public:
		explicit Base(const std::string& name) : m_name(name) {
			std::cout << "====== Base constructor called for: " << m_name << std::endl;
			s_instance_count++;
		}
		virtual ~Base() {
			std::cout << "~~~~~~ ~Base destructor called for: " << m_name << std::endl;
			s_instance_count--;
		}

		Base(const Base&) = delete;
		Base& operator=(const Base&) = delete;

		void setName(const std::string& name) {
			m_name = name;
			std::cout << "SetName: " << m_name << ", instance count: " << s_instance_count << std::endl;
		}

		virtual void describe() const = 0; // 纯虚函数，派生类必须实现

	protected:
		const std::string& getName() const {
			return m_name;
		}

	    static int s_instance_count;

	private:
		std::string m_name;
	};

	int Base::s_instance_count = 0;


	class Derived : public Base {
	public:
		explicit Derived(const std::string& name) : Base(name) {
			std::cout << "====== Derived constructor called for: " << getName() << std::endl;
		}

		~Derived() override {
			std::cout << "~~~~~~ ~Derived destructor called for: " << getName() << std::endl;
		}

		void describe() const override {
			std::cout << "Derived class: " << getName() << std::endl;
		}
	};


	class AnotherDerived : public Base {
	public:
		explicit AnotherDerived(const std::string& name) : Base(name) {
			std::cout << "====== AnotherDerived constructor called for: " << getName() << std::endl;
		}

		~AnotherDerived() override {
			std::cout << "~~~~~~ ~AnotherDerived destructor called for: " << getName() << std::endl;
		}

		void describe() const override {
			std::cout << "AnotherDerived class: " << getName() << std::endl;
		}
	};

}

using namespace VirtualAbs;

int run_abstract_class(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	// make_unique 创建对象并由 unique_ptr 管理；以基类接口持有派生类对象。
	std::unique_ptr<Base> obj1 = std::make_unique<Derived>("Derived Object");
	std::cout << "obj1 describe: ";
	obj1->describe();

	obj1->setName("Derived Object 1");
	std::cout << "obj1 describe: ";
	obj1->describe();

	// auto 推导为 std::unique_ptr<AnotherDerived>，对象离开作用域时自动释放。
	auto obj2 = std::make_unique<AnotherDerived>("Another Derived Object");
	std::cout << "obj2 describe: ";
	obj2->describe();

	// 原始基类指针管理动态对象；必须在适当位置手动 delete。
	Base* obj3 = new Derived("Derived Object 2");
	std::cout << "obj3 describe: ";
	obj3->describe();

	obj1.reset();

	std::cout << "obj2 describe: ";
	obj2->describe();

	delete obj3;

	return 0;
}

