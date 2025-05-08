#include <iostream>
#include <windows.h>

int main()
{
	SetConsoleOutputCP(65001);
	std::cout << "Hello World." << std::endl;
	std::cout << "你好 世界。" << std::endl;
	std::cout << "こんにちは 世界。" << std::endl;
	return 0;
}
