#include "expire_tool.h"
#include "player.h"
#include <string>
#include <conio.h>
#include <thread>
#include <iostream>
#include "bad_ending.h"
#include "game_map.h"
#define MAP_WIDTH 34
#define MAP_HEIGHT 20
// day 2 대화문
std::string dialogue_2[] = {
	"“원래 여기는 캠핑장 이었나보네... 하긴 이렇게 여기 강과 아름다운 자연의 흔적이 있으니까. 지금은 오염된 강과 말그대로 흔적뿐이지만..”",
	"“아무튼 여기서 무전기 부품으로 쓸만한 것이든, 지연제 재료든, 식량이든 물이든, 구할 수 있는 것은 다 구하는거야!”",
	"",
	"전단지 발견:",
	"“이건...앗 멀지 않은 곳에 도심이 있네? 게다가 거기에 피난처도 있다고? 미쳐 다 빠져 나가지 못한 사람들이 서로 도우며 임시 피난처를 만든 건가?”",
	"“좋아. 슬슬 도심으로 나가도 되겠지. 그래 거기엔 분명 사람도 있을꺼야!”",
	"“슬슬 희망이 보이기 시작했어!”"
};


void start_day2(player* user) {
	system("cls");
	char map[MAP_HEIGHT][MAP_WIDTH + 1];
	copy_map(2, map);
	draw_map(map);         // 탐험 맵
	printstat(user); // 아이템 창 
	updateTextBox("");

	user->player_x = 1;
	user->player_y = 1;
	draw_face();
	set_hour(9);
	std::thread timerThread(timer);

	int dialogue_line = 0;
	bool in_dialogue = false;
	int flag = 0;
	map[user->player_y][user->player_x] = 'P';
	while (true) {
		map[user->player_y][user->player_x] = ' ';
		draw_sunlight(map);
		if (get_hour() == 21) {
			if (timerThread.joinable()) {
				timerThread.join();
			}
			break;
		}
		if (in_dialogue) {
			// 대화 중일 때
			if (_kbhit()) {
				int key = _getch();
				if (key == ' ') { // 스페이스바로 대화 진행
					if (dialogue_line < sizeof(dialogue_2) / sizeof(dialogue_2[0])) {
						updateDialogue(dialogue_2[dialogue_line++]);
					}
					else {
						// 대화 종료

						dialogue_clear();
						in_dialogue = false;
						set_isExplore(true);
					}
				}
			}
			continue; // 대화 중에는 아래 로직을 무시하고 다음 루프로 이동
		}
		if (_kbhit()) {
			int key = _getch();

			if (key == 224) { // 방향키 입력 확인
				key = _getch();
				int newX = user->player_x, newY = user->player_y;
				switch (key) {
				case 72: newY--; break;
				case 80: newY++; break;
				case 75: newX--; break;
				case 77: newX++; break;
				}
				if (map[newY][newX] == '?') {
					stop_timer_running();
					if (timerThread.joinable()) {
						timerThread.join();
					}
					break;
				}
				// 이동 가능 여부 확인
				if (newY >= 0 && newY < MAP_HEIGHT && newX >= 0 && newX < MAP_WIDTH &&
					map[newY][newX] == ' ' && !is_zombie_position(newX, newY, map)) {
					user->player_x = newX;
					user->player_y = newY;
				}
				else if (newY >= 0 && newY < MAP_HEIGHT && newX >= 0 && newX < MAP_WIDTH &&
					map[newY][newX] == '%' && !is_zombie_position(newX, newY, map)) {
					user->player_x = newX;
					user->player_y = newY;
					user->mental--;
					updateTextBox("햇빛에 스트레스를 받았다");
				}
				else if (is_zombie_position(newX, newY, map)) {
					user->heart--;
					printstat(user);
					setCursorPosition(1, 23);
					updateTextBox("좀비에게 데미지를 입었다.");
				}
			}
			else if (key == ' ' && is_player_near_item(user, map)) { // 엔터키로 아이템 획득
				map[newY][newX] = ' ';
			}
			else if (key == ' ' && is_player_near_npc(user, map)) { // 스페이스바로 NPC와 대화
				set_isExplore(false);
				dialogue_clear();
				updateDialogue(dialogue_2[dialogue_line++]);
				in_dialogue = true; // 대화 상태 진입
			}
		}
		if (user->heart == 0) {
			bad_ending();
		}
		map[user->player_y][user->player_x] = 'P';
		draw_map(map);
	}
}