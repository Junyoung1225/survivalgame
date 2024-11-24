#include "windows.h"
#include <iostream>
#include <string>
#include "player.h"
#include <chrono>
#include <atomic>
#include <thread>
#include <string>
#include "mutex_helper.h"
#define STAGE_WIDTH 68  // 스테이지의 너비
#define STAGE_HEIGHT 20 // 스테이지의 높이 


std::atomic<bool> timerRunning(true); // 시간 흐름 유무
std::atomic<bool> isExplore(true);
std::atomic<int> hour;
// 커서 위치 변경
void setCursorPosition(int x, int y) {
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// 콘솔 텍스트 색상 설정
void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// 대화창 테두리 출력 + 간단한 문장
void updateTextBox(const char* message) {
	int xOffset = 0;
	int yOffset = STAGE_HEIGHT + 1; // 맵 창 하단에 텍스트 창 위치 설정  
	int width = STAGE_WIDTH;    // 텍스트 창 가로 길이
	int height = 6;                 // 텍스트 창 세로 길이

	// 텍스트 창 테두리와 제목 출력
	std::lock_guard<std::mutex> lock(bufferMutex);
	// 텍스트 창 테두리 그리기
	for (int y = 0; y < height; y++) {
		for (int x = 0; x <= width; x++) {
			setCursorPosition(xOffset + x, yOffset + y + 1);
			if (y == 0) {
				if (x == 0) printf("┌");
				else if (x == width) printf("┐");
				else printf("─");
			}
			else if (y == height - 1) {
				if (x == 0) printf("└");
				else if (x == width) printf("┘");
				else printf("─");
			}
			else {
				if (x == 0 || x == width) printf("│");
				else printf(" ");
			}
		}
	}
	setCursorPosition(1, 23);
	std::cout << "                                                                  ";
	setCursorPosition(1, 23);
	std::cout << message;

}
// 대화창 백지화
void dialogue_clear() {
	std::lock_guard<std::mutex> lock(bufferMutex);
	setCursorPosition(1, 23);
	std::cout << "                                                                  ";
	setCursorPosition(1, 24);
	std::cout << "                                                                  ";
	setCursorPosition(1, 25);
	std::cout << "                                                                  ";
	setCursorPosition(1, 26);
	std::cout << "                                                                  ";
}
// 대화 내용 출력
void updateDialogue(std::string& message) {
	dialogue_clear();
	std::lock_guard<std::mutex> lock(bufferMutex);
	if (message.length() > 192) {
		std::string part1 = message.substr(0, 64);
		std::string part2 = message.substr(64, 64);
		std::string part3 = message.substr(128,64);
		std::string part4 = message.substr(192);
		setCursorPosition(1, 23);
		std::cout << part1;
		setCursorPosition(1, 24);
		std::cout << part2;
		setCursorPosition(1, 25);
		std::cout << part3;
		setCursorPosition(1, 26);
		std::cout << part4;
	}
	else if (message.length() > 128) {
		std::string part1 = message.substr(0, 64);
		std::string part2 = message.substr(64, 64);
		std::string part3 = message.substr(128);
		setCursorPosition(1, 23);
		std::cout << part1;
		setCursorPosition(1, 24);
		std::cout << part2;
		setCursorPosition(1, 25);
		std::cout << part3;
	}
	else if (message.length() > 64) {
		std::string part1 = message.substr(0, 64);
		std::string part2 = message.substr(64);
		setCursorPosition(1, 23);
		std::cout << part1;
		setCursorPosition(1, 24);
		std::cout << part2;
	}
	else {
		setCursorPosition(1, 23);
		std::cout << message;
	}
}
// 상태창 출력
void printstat(player* user) {
	std::lock_guard<std::mutex> lock(bufferMutex);
	//가로 27 세로 9
	SHORT y = 1;
	SHORT x = 75;
	COORD pos1 = { x ,y };
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOut, pos1);

	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	std::cout << "┌─────────────────────────┐";
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	std::cout << "│ 체력 :     ";
	for (int i = 0; i < user->heart; i++) {
		setColor(4);
		std::cout << " ♥";
		setColor(7);
	}
	for (int i = 0; i < 5 - user->heart; i++) {
		std::cout << " ♡";
	}
	std::cout << "   │";
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);

	std::cout << "│ 스트레스 : ";
	std::cout << user->mental;
	pos1.X = pos1.X+ 26;
	SetConsoleCursorPosition(hConsoleOut, pos1);
	std::cout << "│";
	pos1.X = pos1.X - 26;
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	std::cout << "│                         │";
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	std::cout << "│ 식량 X "<< user->food<< "                │";
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	std::cout << "│ 물 X "<<user->water<<"                  │";
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	if (user->bullet != 0) {
		std::cout << "│ 탄환 X ", user->bullet, "                │";
	}
	else {
		std::cout << "│ ---                     │";
	}
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	if (user->medicine != 0) {
		std::cout << "│ 치료제 X ", user->medicine, "              │";
	}
	else {
		std::cout << "│ ---                     │";
	}
	pos1 = { x ,y++ };
	SetConsoleCursorPosition(hConsoleOut, pos1);
	std::cout << "└─────────────────────────┘";
}
// npc 얼굴 출력 * 미완성 *
void draw_face() {
	std::lock_guard<std::mutex> lock(bufferMutex);
	setCursorPosition(80, 10);
	std::cout << "      ____,     ";
	setCursorPosition(80, 11);
	std::cout << "   .       -    ";
	setCursorPosition(80, 12);
	std::cout << "   --------- !  ";
	setCursorPosition(80, 13);
	std::cout << "   ;       ;-.  ";
	setCursorPosition(80, 14);
	std::cout << "   :--/'---- ,. ";
	setCursorPosition(80, 15);
	std::cout << "    ,      .;   ";
	setCursorPosition(80, 16);
	std::cout << "     .-,,!~     ";
	setCursorPosition(80, 17);
	std::cout << "    .~,,~~~  ~  ";
	setCursorPosition(80, 18);
	std::cout << "  ~. :.~,,   ., ";
	setCursorPosition(80, 19);
	std::cout << " .  ,. ,,.    ..";
	setCursorPosition(80, 20);
	std::cout << ",  -,. ..      -";
	setCursorPosition(80, 21);
	std::cout << " .  .~-,.      ~";
	setCursorPosition(80, 22);
	std::cout << ".-~~~:---~---_ _";

}
// 타이머 (시간 + 분) 출력
void timer() {
	COORD timer_pos = { 1,0 };
	std::string erase = { "                                     " };
	int current_time = 0;
	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		setColor(12);
		setCursorPosition(timer_pos.X, timer_pos.Y);
		std::cout << erase;
		setCursorPosition(timer_pos.X, timer_pos.Y);
		std::cout << hour<<"시" << current_time << "분";
		setColor(7);
	}
	timerRunning = true;
	isExplore = true;
	int count = 0;
	while (timerRunning) {
		if (isExplore) {
			std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초마다 감소

			current_time+=20;
			if (current_time >= 60) {
				current_time = 0;
				hour += 1;
				if (hour == 21) {
					break;
				}
			}
			{
				std::lock_guard<std::mutex> lock(bufferMutex);
				setColor(12);
				setCursorPosition(timer_pos.X, timer_pos.Y);
				std::cout << erase;
				setCursorPosition(timer_pos.X, timer_pos.Y);
				std::cout << hour << "시" << current_time << "분";
				setColor(7);
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}
// hour 반환
int get_hour() {
	return hour;
}
// hour 설정
void set_hour(int h) {
	hour = h;
}
// timer 종료
void stop_timer_running() {
	timerRunning = false;
}
// timer 시작
void start_timer_running() {
	timerRunning = true;
}

// 맵 내 햇빛 위치 초기화
void erase_sunlight(char map[][35]) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 35; j++) {
			if (map[i][j] == '%') {
				map[i][j] = ' ';
			}
		}
	}
}
// 맵에 새로운 햇빛 위치 저장
void draw_sunlight(char map[][35]) {
	switch (hour) {
	case 9:
		erase_sunlight(map);
		for (int h = 18; h >= 12; h--) {
			int w = 32;
			while (w >= 0 && map[h][w] != '#') {
				if(map[h][w]==' ')
				map[h][w] = '%';
				w--;
			}
		}
		break;
	case 10:
		erase_sunlight(map);
		for (int h = 18; h >= 6; h--) {
			int w = 32;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')
				map[h][w] = '%';
				w--;
			}
		}
		break;
	case 11:
		erase_sunlight(map);
		for (int h = 18; h >= 1; h--) {
			int w = 32;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')
				map[h][w] = '%';
				w--;
			}
		}
		break;
	case 12:
		erase_sunlight(map);
		for (int h = 12; h >= 1; h--) {
			int w = 32;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				w--;
			}
		}
		for (int w = 32; w >= 24; w--) {
			int h = 1;
			while (h >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				h++;
			}
		}
		break;
	case 13:
		erase_sunlight(map);
		for (int h = 6; h >= 1; h--) {
			int w = 32;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				w--;
			}
		}
		for (int w = 32; w >= 16; w--) {
			int h = 1;
			while (h >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				h++;
			}
		}
		break;
	case 14:
		erase_sunlight(map);
		for (int w = 32; w >= 8; w--) {
			int h = 1;
			while (h >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				h++;
			}
		}
		break;
	case 15:
		erase_sunlight(map);
		for (int w = 32; w >= 1; w--) {
			int h = 1;
			while (h >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				h++;
			}
		}
		break;
	case 16:
		erase_sunlight(map);
		for (int h = 6; h >= 1; h--) {
			int w = 1;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				w++;
			}
		}
		for (int w = 24; w >= 1; w--) {
			int h = 1;
			while (h >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				h++;
			}
		}
		break;
	case 17:
		erase_sunlight(map);
		for (int h = 12; h >= 1; h--) {
			int w = 1;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				w++;
			}
		}
		for (int w = 16; w >= 1; w--) {
			int h = 1;
			while (h >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				h++;
			}
		}
		break;
	case 18:
		erase_sunlight(map);
		for (int h = 18; h >= 1; h--) {
			int w = 1;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				w++;
			}
		}
		for (int w = 8; w >= 1; w--) {
			int h = 1;
			while (h >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				h++;
			}
		}
		break;
	case 19:
		erase_sunlight(map);
		for (int h = 18; h >= 6; h--) {
			int w = 1;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				w++;
			}
		}
		break;
	case 20:
		erase_sunlight(map);
		for (int h = 18; h >= 12; h--) {
			int w = 1;
			while (w >= 0 && map[h][w] != '#') {
				if (map[h][w] == ' ')map[h][w] = '%';
				w++;
			}
		}
		break;
	case 21:
		erase_sunlight(map);
		break;
	default:
		break;
	}
}
//timer 일시정지
void set_isExplore(bool x) {
	isExplore = x;
}