#include <iostream>
#include <windows.h>
#include "GameFramework.h"

int main()
{
	// UTF-8を使用するための設定
	SetConsoleOutputCP(65001);

	std::cout << "Hello World." << std::endl;
	std::cout << "こんにちは 世界。" << std::endl;
	std::cout << "你好 世界。" << std::endl;
	GameFramework::Hello();
	return 0;
}
