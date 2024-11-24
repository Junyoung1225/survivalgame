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
// day 1 대화문
std::string dialogue_1[] = {
	   "???: “으으 힘들어, 하지만 ...해야해..”",
	   "무언가에 열중하고 있는 모습이다.",
	   "npc2: “이 냄새는? 앗 당신 뭐요? 여길 어떻게?”",
	   "npc2: “...일단 이 빵은 감사히 잘 먹겠소. 너무 열중한 나머지 밥도 까먹고 있었네요.”",
	   "npc2: “여기는 연구... 아니 제가 지내는 휴식처에요.”",
	   "npc2: “아 뭐에 열중하고 있었냐구요? 그냥. 좀비에 대해서요. 사실 옛날엔 연구소 직원이었거든요.. 그 연구소는 이미 망했지만.”",
	   "npc2: “좀비사태가 터진지 이미 오랜 시간이 지났지만, 아직 인류는 해결하지 못한 것이 너무 많아요. 당장 원인부터, 백신까지요.”",
	   "npc2: “이미 여러 연구진이 이건 해결방법이 불가능하다고 못박았지만 저는 그래도 포기하지 않을려구요. 실제로 여기 좀비들이 많은 곳에서 생활하니 점점 진전도 되는 것 같구요.”",
	   "npc2: “...잡설이 길었죠. 여기에서 사람을 본적이 너무 드물어서 너무 반가워서 그만.”",
	   "npc2: “그래요 당신도 언젠간 피난처로 탈출을 바란다면 그때 제 연구 결과를 알려줘도 좋겠군요.”",
	   "npc2: “일단 좀비에 대해. 좀비는 일단 빛을 싫어하는 것 같아요 특히 햇빛 그래서 밤에 많이 활동하고 그림자 근처에서 돌아다니지.”",
	   "npc2: “....네? 햇빛은 오존층 파괴 때문에 따가운거 아니냐고요? 아뇨? 저는 멀쩡한데요?”",
	   "npc2: “아무튼 말을 계속하자면 수많은 좀비들의 움직임을 관찰한 결과, 그들은 ‘감정’에 이끌려 돌아다니는 것 같아.”",
	   "플레이어: “감....정?”",
	   "npc2: “네 기쁨, 슬픔, 분노 등 다양한 감정이요. 좀비들은 아무것도 못느끼는 것처럼 보였어요. 감정이 뭔지, 감정을 원하는지 그저 무의식적으로 쫓아다니는 것처럼”",
	   "npc2: “좀비의 종류도 다양한데, 가만히있는 좀비도 있는 반면, 그저 특정 패턴으로 움직임만 반복하거나. 감정에 이끌려 쫓아오는 좀비들 정도에요.”",
	   "npc2: “마지막으로, 백신에 대해선데.... 이건 아무리 알아봐도 도저히 답이 안나와요. 이미 죽은 자를 살릴 수 없듯이 감염되면 변하는 것을 막을 순 없는 것처럼.”",
	   "npc2: “하지만 지연제는 만들 수 있을 거 같아요. 그리고 제 가설에 의하면 지연제를 지속적으로 맞고 감정을 컨트롤을 잘해 감정을 잃지 않는다면, 아무리 좀비라도 인간처럼 살수 있다고 생각해요. 그것이 사람이라고 말 하기엔 애매하겠지만.”",
	   "npc2: “아무튼 그 지연제를 만들려면, 이런저런 약초가 필요해요. 혹시 나중에 기회 되신다면 구해다 주실수 있겠어요?”",
	   "npc2: “네. 저도 거의 다 연구가 끝났으니 슬슬 돌아가 봐야죠. 일단 식량 감사합니다.”",
	   "npc2: “우리 언젠간 또 볼수 있을 것 같아요.”",
	   "플레이어: “후 그래 내가 어떻게 여기에 왔는지 기억도 안나지만, 일단 살아야지. 그래 나도 내일부터 다시 돌아다녀 보자.”",
	   "플레이어: “정리하자면. 매일 좀비를 피해 물과 식량을 얻고, 정보를 얻어서 좀비가 없는 안전한 곳으로 가면 되겠지?”",
	   "플레이어: “일단 여기 동굴을 안식처로 삼고 행동하자.”"
};



void start_day1(player* user) {
	system("cls");
	char map[MAP_HEIGHT][MAP_WIDTH + 1];
	copy_map(1, map);
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
					if (dialogue_line < sizeof(dialogue_1) / sizeof(dialogue_1[0])) {
						updateDialogue(dialogue_1[dialogue_line++]);
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
				updateDialogue(dialogue_1[dialogue_line++]);
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