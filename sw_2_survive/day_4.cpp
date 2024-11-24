#include "expire_tool.h"
#include "player.h"
#include <string>
#include <conio.h>
#include <thread>
#include <iostream>
#include "bad_ending.h"
#include "game_map.h"
#include <vector>
#define MAP_WIDTH 34
#define MAP_HEIGHT 20

// day 4 대화문
std::vector<std::vector<std::string>> dialogue_4 = {
	{
	"플레이어: 여기에 연구원이 말한 치료제가 있다고 했지...",
	"???: 이 목소린 사람? 여기에요! 여기에 사람이 있어요!",
	"플레이어: 누구시죠? 어째서 여기에 있나요?",
	"np3: 앗 저는 그저 좀비를 피해다니다가 넘어져서 상처 치료할 만한게 없나 확인 하려고...",
	"플레이어: 여기도 좀비가 많은데 괜찮나요?",
	"np3: 네 이미 적응했거든요. 이래봐도 이미 산전수전 다 겪은 몸이랍니다.",
	"np3: 이미 좀비를 피해다닌지 3주가 넘어가요. 심지어 이젠 제가 이 상황을 즐기고 있다고도 생각돼요.",
	"플레이어: ...탈출할 생각은 있나요?",
	"np3: 당연하죠. 저도 살고 싶은 걸요. 다만 좀비들의 특성을 알기 위해 조금 위험하게 다닌 겁니다.",
	"np3: 게다가 탈출하고 싶어도 도무지 답이 안 나와서...",
	"플레이어: 제가 탈출 방법과 근처의 피난처를 알고 있습니다. 같이 가시죠.",
	"np3: 네? 그게 정말이에요? 그걸 어떻게?",
	"플레이어: 저도 이것저것 돌아다니다 알게 되었습니다. 일단 피난처는 이 근처이고, 탈출법은 무전기 부품을 구해다 만들면 돼요.",
	"np3: 무전기 재료요?",
	"플레이어: 네 자세하게는 설명하기 힘들지만 아무튼 그렇습니다.",
	"np3: 그렇군요... 알려줘서 감사합니다. 저도 도울게요.",
	"플레이어: 네 알겠습니다.",
	"np3: 저기 혹시 한가지만 더..... 제가 배고파서 그런데 식량하고 물을 나눠줄 수 있나요?",
	"플레이어: .....물론이죠 여기 여분의 식량을 드릴게요.",
	"np3: 정말 감사합니다. 분명 힘든 상황인 줄 알았는데 이렇게 나눠주시다니. 조금은 여유가 있나 보군요.",
	"플레이어: 착각하지 마세요. 저는 그저 상황이 안타까워 보여서 조금 제 것을 나눠주는 겁니다.",
	"np3: 마음씨가 착하신 분이군요.",
	"np3: ...하지만 충고하건데 그런 마음씨는 여기 좀비 세상에서는 쓸모가 없을 겁니다.",
	"np3: 오히려 감정을 버리는 게 나을 정도로요. 여기는 오로지 자신의 생존만 생각하는 게 나아요.",
	"플레이어: ..충고 감사합니다. 일단 병원에서는 같이 다니면서 탐색하죠.",
	"플레이어: 아무리 그래도 혼자보단 여럿이 행동하는 게 생존에 유리하지 않습니까.",
	"np3: 그것도 믿을 만한 사람에 한정된 얘기긴 한데.. 당신의 마음씨를 보니 믿어도 되겠어요. 좋아요 같이 다녀요.",
	"플레이어: 그래도 여기에 오래 적응한 사람과 같이 다니니 안심입니다. 잘 부탁드립니다.",
	"np3: 네 잘 부탁해요.",
},
{
	"np3: 제가 말했잖아요. 여기는 감정을 버리고 오로지 자기 자신의 생존만 위해 행동하라고, 믿을 만한 사람인지 몇 번이고 확인하라고.",
	"np3: 저는 힌트를 드렸는데 눈치채지 못한 건 당신입니다. 당신이 무능해서 이렇게 다 뺐기는 거에요.",
	"np3: 감사합니다. 저도 이제 알아볼 건 다 알아봤으니 슬슬 나가야겠어요. 정보 감사합니다.",
	"np3: 그리고 당신이 가진 모든 것도 잘 쓸게요~ 살아서 만나요.",
	"플레이어: 배신당했다... 어떻게 이럴 수가... 내가 그 사람한테 뭘 잘못했길래...",
	"플레이어: 내 물건이 다 사라졌다. 동정인지 식량과 물 하루치만 놔두고...",
	"플레이어: 용서할 수 없다. 이런 감정은 처음 느껴본다.",
	"플레이어: 감정을 버리라고 했지... 웃기지마 나는 분노의 감정을 가지고 np3를 쫓으리라 다짐했다.",
	"플레이어: 네... 다시 꼭 살아서 만나요.... 반드시... 만나요.",
},
{
	"생존자: 저기요 계세요?",
	"플레이어: 누구시죠?",
	"생존자: 저 사람이에요!! 저 좀 도와주세요!",
	"플레이어: 정보를 제공하면 도와드리겠습니다. 혹시 여기에 많은 물건을 들고 도망가는 사람을 못 보셨습니까?",
	"생존자: 사람? 어...제가 여기에 계속 숨어 있어서 모르겠는데 발자국 소리가 가파르게 저쪽 방향으로 들리긴 했어요.",
	"생존자: 아마 그 사람일 저에요.",
	"플레이어: ...감사합니다만. 도와드릴 순 없겠네요. 저도 지금 뭐 있는 게 아무것도 없어서. 제 코가 석자네요.",
	"생존자: 아무것도 없다구요. 일단 식량과 물 하나는 드릴 수 있고... 혹시 좀비를 물리칠 무기는 있나요?",
	"플레이어: 무기! 무기요? 그래 난 왜 그 생각을 못했지. 무기가 있나요?",
	"생존자: 여기에 무기는 없어요. 애초에 있어도 저는 무섭고 용기가 안 나서 계속 여기에 있었을 거에요.",
	"생존자: 아무튼 여기로 도망쳐온 길에 총 같은 무기가 있을 만한 장소를 발견했어요. 바로 경찰서죠!",
	"플레이어: 그니까 경찰서에 가면 총을 얻을 수 있을 것이다?",
	"생존자: 네 도망치면서 돌러보니 주위에 탄창은 많았지만 정작 총기가 없어서 쓸 수가 없었는데 거기서 총기를 얻으면 좀비를 쓰러뜨릴 수 있을지 몰라요!",
	"플레이어: 그렇군요. 바로 가보겠습니다. 모든 일이 끝나고 저랑 같이 가시죠. 피난처로요.",
	"생존자: !!네 감사합니다. 정말 감사합니다.",
	"플레이어: 네 그럼 이만 떠나겠습니다."
}
};

void start_day4(player* user) {
	system("cls");
	char map[MAP_HEIGHT][MAP_WIDTH + 1];
	copy_map(4, map);
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
	int dialogue_num = 0;
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
					if (dialogue_line < dialogue_4[dialogue_num].size()) {
						updateDialogue(dialogue_4[dialogue_num][dialogue_line++]);
					}
					else {
						// 대화 종료
						dialogue_clear();
						in_dialogue = false;
						set_isExplore(true);
						dialogue_line = 0;
						dialogue_num++;
						if (dialogue_num == 1) {
							map[1][11] = ' ';
							map[10][32] = 'N';
						}
						else if (dialogue_num == 2) {
							map[10][32] = ' ';
						}
						else if (dialogue_num == 3) {
							dialogue_num = 2;
						}
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
					updateTextBox("햇빛에 데미지를 받았다");
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
				updateDialogue(dialogue_4[dialogue_num][dialogue_line++]);
				in_dialogue = true; // 대화 상태 진입
			}
		}
		if (user->heart <= 0) {
			bad_ending();
		}
		if (user->mental <= 0) {
			bad_ending();
		}
		map[user->player_y][user->player_x] = 'P';
		draw_map(map);
	}
}