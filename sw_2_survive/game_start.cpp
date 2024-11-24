#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include "expire_tool.h"
void setConsoleSize(int width, int height) {
	std::string command = "mode " + std::to_string(width) + "," + std::to_string(height);
	system(command.c_str());
}



void game_start_screen() {
	// 콘솔 창 크기 설정 (가로 100, 세로 30)

	// 콘솔 화면 지우기
	system("CLS");

	// 제목 출력
	setCursorPosition(17, 5);
	printf("  ######   ##     ##  ########  ##       ##  ######### ##       ##  #########");
	setCursorPosition(17, 6);
	printf(" ##    ##  ##     ##  ##    ##   ##     ##      ###     ##     ##   ##");
	setCursorPosition(17, 7);
	printf(" ##        ##     ##  ##    ##   ##     ##      ###     ##     ##   ##");
	setCursorPosition(17, 8);
	printf("  ######   ##     ##  ########    ##   ##       ###      ##   ##    #########");
	setCursorPosition(17, 9);
	printf("       ##  ##     ##  ## ###      ##   ##       ###      ##   ##    ##");
	setCursorPosition(17, 10);
	printf(" ##    ##  ##     ##  ##  ###      ## ##        ###       ## ##     ##");
	setCursorPosition(17, 11);
	printf("  ######    #######   ##   ###      ###      #########     ###      #########");


	setCursorPosition(50, 23);
	// 추가 정보를 위한 여백 출력
	std::string subtitle = "Press any key to start...";
	std::cout << subtitle;
	//printCenteredText(subtitle, consoleWidth);

	// 사용자가 키를 누를 때까지 대기
	while (_kbhit() == 0) {
	}
}