#include "zombie_move.h"
#include<thread>
#include<chrono>
//생성자
Zombie::Zombie(int startX, int startY, int dX, int dY) : x(startX), y(startY), dx(dX), dy(dY) {}


void Zombie::move(char (&game_map)[20][35]) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	int next_x = x + dx;
	int next_y = y + dy;


	game_map[y][x] = ' ';
	//벽에 부딪히면 이동방향 반대로, 벽에 부?H힌다면 1초간 정지
	if (game_map[next_y][next_x] == '#' || game_map[next_y][next_x] == 'P') {
		dx = -dx;
		dy = -dy;
		next_x = x;
		next_y = y;
	}
	//위치 업뎃
	x = next_x;
	y = next_y;

	//좀비 위치 정보 업뎃
	game_map[y][x] = 'x';

}