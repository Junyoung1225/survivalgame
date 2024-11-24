#ifndef EXPIRE_TOOL_H
#define EXPIRE_TOOL_H
#include "player.h"
#include<string>
#include <atomic>
#include<iostream>
//////////////////// 기본 사용 함수///////////////////////////
// 콘솔 커서 위치 설정
void setCursorPosition(int x, int y);

// 콘솔 텍스트 색상 설정
void setColor(int color);

//////////////////// 대화창 ///////////////////////////

// 텍스트 창 초기화 및 메시지 출력 함수  -> 짧은 대화에 사용
void updateTextBox(const char* message);
// 긴 대화에 사용
void updateDialogue(std::string& message);
//대화창 청소
void dialogue_clear();
//////////////////// 상태창 ///////////////////////////


void printstat(player* user);

//////////////////// 타이머 ///////////////////////////
void draw_face();
void stop_timer_running();
void timer();
void start_timer_running();
int get_hour();
void set_hour(int h);
void set_isExplore(bool x);
//////햇빛시스템
void draw_sunlight(char map[][35]);
void erase_sunlight(char map[][35]);
#endif