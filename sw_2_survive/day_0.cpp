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
// day 0 대화문
std::string dialogue_0[] = {
	   "npc: “아악! 누구세요? 엣 사람? 사람이 왜 여기에? 아니 어떻게 여기에?”",
	   "플레이어: “혹시 사람이세요? 그렇겐 안보이는데...”",
	   "npc: “....사람이거든요. 그쪽은 어떻게 여기에 왔는데요?”",
	   "플레이어: “...기억은 없어요. 그저.... 도망쳐왔다는 기억밖엔...”",
	   "npc: “...그렇군요. 그럼 일단 제가 도와줄게요.”",
	   "npc: “일단 배고프실테니 여기 있는 식량부터.. 앗 없어졌네? 혹시 이미 가져가신...것 같군요. 손버릇이 안좋네요.”",
	   "플레이어: “살기위해선 모든지 해야합니다.”",
	   "npc: “....네.  일단 여기는 제가 피난해있는 동굴이에요.”",
	   "npc: “여기라고 안전한 곳은 아니지만, 그래도 바깥 세상보단 나으니까요.”",
	   "npc: “뭐 일단 설명을 이것저것 드릴게요”",
	   "npc: “제 설명이 튜토리얼 같아보여도 참으세요”",
	   "npc: “일단 여기 주변의 좀비들은 움직이진 않으니까 안심하세요.”",
	   "npc: “그렇다고 너무 가까이 다가가면 공격하니까 조심하시고요.”",
	   "npc: “일단 생존하려면 당연히 잘자고, 잘 먹어야겠죠? 잘이라는 말을 붙이기엔 상황이 안좋지만..”",
	   "npc: “아무튼 일단 식량이 바닥났어요. 사실 아까께 마지막 물과 식량이었는데 당신한테 양보한거라구요?”",
	   "npc: “그래서 밖에 나가서 식량, 물 등을 구해와야해요. 물론 좀비들을 피해서요.”",
	   "npc: “여기는 잠을 자거나 휴식공간으로 쓰셔도 상관없어요.”",
	   "npc: “네 당연히 저도 식량과 물을 구하려 나가봐야죠.”",
	   "npc: “...밖은 위험해요... 당장에 좀비부터 왠지 모르겠지만 햇빛도 너무 쎄요. 오존층이 파괴된건지... 햇빛에 너무 오래 있지 마세요.”",
	   "npc: “일단 당신은 오늘 일어났다고 하셨으니 여기서 쉬세요. 저는 먼저 제가 먹을 물과 식량을 구하러 떠날게요.”",
	   "npc: “네? 식량이나 물을 나눠먹으면 되지 않냐고요?”",
	   "npc: “...일단 물은 하루에 500ml이상 먹지 않으면 진짜 위험해져요.. 제가 겪어봐서 알아요. 식량은 하루정돈 괜찮지만.. 정신적으로 뭔가...”",
	   "npc: “그러니 식량은 일단 그렇다 쳐도 물은 무조건 구해야하기 때문에 가야만해요.”",
	   "npc: “...그럼 이만”",
	   "플레이어: “후 그래 내가 어떻게 여기에 왔는지 기억도 안나지만, 일단 살아야지. 그래 나도 내일부터 다시 돌아다녀 보자.”",
	   "플레이어: “정리하자면. 매일 좀비를 피해 물과 식량을 얻고, 정보를 얻어서 좀비가 없는 안전한 곳으로 가면 되겠지?”"
};


void start_day0(player* user) {
	system("cls");
	char map[MAP_HEIGHT][MAP_WIDTH + 1];
	copy_map(0, map);
	draw_map(map);         // 탐험 맵
	printstat(user); // 아이템 창 
	updateTextBox("");

	user->player_x = 5;
	user->player_y = 6;
	draw_face();
	set_hour(9);
	std::thread timerThread(timer);

	int dialogue_line = 0;
	bool in_dialogue = false;
	int flag = 0;
	map[user->player_y][user->player_x] = 'P';
	while (true) {
		map[user->player_y][user->player_x] = ' ';
		//draw_sunlight(map);
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
					if (dialogue_line < sizeof(dialogue_0) / sizeof(dialogue_0[0])) {
						updateDialogue(dialogue_0[dialogue_line++]);
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
				updateDialogue(dialogue_0[dialogue_line++]);
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