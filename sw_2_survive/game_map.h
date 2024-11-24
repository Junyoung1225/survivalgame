#ifndef GAME_MAP_H
#define GAME_MAP_H
#include"player.h"
#include "expire_tool.h"
#define MAP_WIDTH 34
#define MAP_HEIGHT 20
extern int newX, newY;
extern char game_map[20][MAP_HEIGHT][MAP_WIDTH + 1];

bool is_player_near_npc(player* user, char map[][MAP_WIDTH + 1]);

bool is_player_near_item(player* user, char map[][MAP_WIDTH + 1]);

bool is_zombie_position(int x, int y, char map[][MAP_WIDTH + 1]);

void copy_map(int day_num, char map[][MAP_WIDTH+1]);
void draw_map(char map[][MAP_WIDTH + 1]);
#endif
