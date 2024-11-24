#ifndef PLAYER_H
#define PLAYER_H

class player
{
public:// 아래 상태들은 user->heart--, user->mental++ 등을 통해 조절 가능합니다.
	int heart;			//체력	
	int mental;			//정신력
	int food;			//음식개수
	int water;			//물개수
	int bullet;			//총알개수
	int player_x;		//플레이어 x위치
	int player_y;		//플레이어 y위치
	int medicine;		//약개수
	int direction;
	int itemlist[10];	// 아이템 리스트, 각 숫자별로 아이템 할당하고 if 문 통해서 구현하면 될 거 같습니다.
	int gun;
	int radio;
	player();
	void player_face();
};

#endif // !플레이어