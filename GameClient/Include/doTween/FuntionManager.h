#include <vector>
#include <functional>
#include <iostream>

class FunctionManager 
{
public:
	// �پ��� �ñ״�ó�� �Լ��� �����ϱ� ���� ���ø� �޼ҵ�
	template<typename Func>
	void addFunction(Func func) 
	{
		functions.emplace_back([func](auto&&... args) {
			return func(std::forward<decltype(args)>(args)...);
			});
	}

	// �Լ� ȣ�� �޼ҵ�
	template<typename... Args>
	void callFunctions(Args&&... args) {
		for (auto& func : functions) {
			func(std::forward<Args>(args)...);
		}
	}

private:
	std::vector<std::function<void()>> functions;
};

int main() {
	FunctionManager manager;

	// �پ��� �ñ״�ó�� �Լ� �߰�
	manager.addFunction([]() {
		std::cout << "No argument function\n";
		});

	manager.addFunction([](int x) {
		std::cout << "Function with one int argument: " << x << "\n";
		});

	manager.addFunction([](int x, double y) {
		std::cout << "Function with int and double arguments: " << x << ", " << y << "\n";
		});

	// �Լ� ȣ��
	manager.callFunctions();        // No argument function ȣ��
	manager.callFunctions(10);      // Function with one int argument ȣ��
	manager.callFunctions(10, 20.5); // Function with int and double arguments ȣ��

	return 0;
}