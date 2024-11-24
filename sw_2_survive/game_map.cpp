#include "expire_tool.h"
#include "player.h"
#include "Windows.h"
#include "mutex_helper.h"
#define MAP_WIDTH 34
#define MAP_HEIGHT 20
int newX, newY;
char game_map[20][MAP_HEIGHT][MAP_WIDTH + 1] =
{
//0ÀÏÂ÷ µ¿±¼
    {
    "##################################",
    "#              ####              #",
    "#             ######             #",
    "#    ####    ##xxx ##    ####    #",
    "#   ##  ##  ##      ##  ## F##   #",
    "#   #    ####        ####    #   #",
    "#                            #   #",
    "#   ##   x   ###  ###       ##   #",
    "#    ###    ##      ##    ###    #",
    "#      ## ###        ### ##      #",
    "#      ## ###        ### ##      #",
    "#    ###    ##      ##    ###    #",
    "#   ##       ###  ###       ##   #",
    "#    # x                    #    #",
    "#   #    ####        ####    #   #",
    "#   ## W##  ##      ##  ##N ##   #",
    "#    ####    ## xxx##    ##?#    #",
    "#             ######             #",
    "#              ####              #",
    "##################################"
},
//1ÀÏÂ÷ ¿Üµý°Ç¹°
   {
        "##################################",
        "#        ##     #           F#x !?",
        "#   ######F     ######  #   ### ##",
        "#   #Wxxx# ###  #    ###      #  #",
        "#   #  xx#      #    #        ## #",
        "#   #   x#### ###### # ####   #  #",
        "#   #                     #   # ##",
        "#   ####### x  #####  x  ###### x#",
        "#      xx #     # #      #       #",
        "#xxxxx x  #  x       x   # #######",
        "#x       x#     # #      #       #",
        "#  x#######  x  ###  x   ######  #",
        "#   #    x      x        #       #",
        "#x  #        #### ####   # #######",
        "#   ######   #       #   #       #",
        "# x      #   # #   # #   ####### #",
        "#   #######  #*#   #W#x  ##      #",
        "#           #### N ####     #### #",
        "##################################"
},
//2ÀÏÂ÷ Ä·ÇÎÀå
    {
        "##################################",
        "#      ###     x     ###  x     x#",
        "#     ##F##    x    ##W##     #  #",
        "#    ##   ##   x   ##   ##   #F# #",
        "#       #    xxx      #          #",
        "#  x   ###       #   ###    ##   #",
        "#    ## W ##    #x#  #     #  ## #",
        "#  ###x# #x### #   ## #      ##  #",
        "#   x##  x##     ####     ###    #",
        "#       ##   ###  x   ###   ##   #",
        "#      ####    ##    ##   ## #   #",
        "# x   #  xx#    ######    ##   # #",
        "#   x### x### ###F###x     # # ###",
        "#    x   #   ###x  ##x  xx  ##   #",
        "#   #     ## ##x    ##  ##   ###!#",
        "#  #W#     ###x      ## ##   ###?#",
        "# ## ##     #x         ###     ###",
        "###    # #         #       ##x   #",
        "##   #     x## #     x#####      #",
        "##################################"
},
    //day3 ,(11,1) : »ç±â²Û  -> (32,10);
     {
        "##################################",
        "#       #  N  #F x #  W          #",
        "#  ###  # ### #x  x# ### # x  ####",
        "# xx##  #     # x  #            F#",
        "#   ##  ### ###    ### ###    ## #",
        "##x ##        x     x      #  ## #",
        "#   ##   # ##   ##     ####   ## #",
        "# #x###### ##  #####   ## ###### #",
        "#    x   # ##  x     x           #",
        "# x      # ##     x              #",
        "#xx  #######     ###    #######  #",
        "# x      # ##  x     x  ##       #",
        "#    x   #x       x    ##N#      #",
        "# xx###### ##   ####   ## ###### #",
        "#   ##   ####     ##   ## #   ## #",
        "#xx ##x                 #  #  ## #",
        "#  W##x ### ###    #######    ## #",
        "######  ##   ##    #?!        ## #",
        "#M     x#######    #######       #",
        "##################################"
},
//4ÀÏÂ÷ : °æÂû¼­
 {
   "################################?#",
   "#     x #  #F   #   #   W# M#  x!#",
   "#  x       # xx   #   xx #x #   x#",
   "#    x  #  #    #   #    #  #  xx#",
   "#x  #####  ####### ####### x###xx#",
   "#  x#        ##x    x##x     x#  #",
   "#   #  ###   ##      ##  ###  # x#",
   "#x  # B###x       x   ##  ###B#  #",
   "#   ######    ### ###    ######x #",
   "#       x#   x##x  ##x   # x     #",
   "#x       #    F#  x#W   x#      x#",
   "######   #######   #######  ######",
   "#   #G   #x            #    #W xx#",
   "#   ######   x#######    ######  #",
   "#   x   x     #F   N#   x##x     #",
   "#x    x x     ### ###    ##   #  #",
   "#   #####  ###### ######x   #    #",
   "#  x#xx        ## ##             #",
   "#MB #F ## #    ##    ##          #",
   "##################################"
},
//day 7
{
        "##################################",
        "##################################",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##                              ##",
        "##################################",
        "##################################"
    }
};




bool is_player_near_npc(player* user, char map[][MAP_WIDTH + 1]) {
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; i++) {
        newX = user->player_x + dx[i];
        newY = user->player_y + dy[i];
        if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT &&
            map[newY][newX] == 'N') {
            return true;
        }
    }
    return false;
}

bool is_player_near_item(player* user, char map[][MAP_WIDTH + 1]) {
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; i++) {
        newX = user->player_x + dx[i];
        newY = user->player_y + dy[i];
        char item_type = map[newY][newX];
        switch (item_type) {
        case 'F': // ½Ä·®
            user->food++;
            map[newY][newX] = ' ';
            updateTextBox("½Ä·®À» È¹µæÇß½À´Ï´Ù!");
            printstat(user);
            break;
        case 'W': // ¹°
            user->water++;
            map[newY][newX] = ' ';
            updateTextBox("¹°À» È¹µæÇß½À´Ï´Ù!");
            printstat(user);
            break;
        case 'M': // Ä¡·áÁ¦
            user->medicine++;
            map[newY][newX] = ' ';
            updateTextBox("Ä¡·áÁ¦¸¦ È¹µæÇß½À´Ï´Ù!");
            printstat(user);
            break;
        case '!': // ¹«Àü±â
            user->radio++;
            map[newY][newX] = ' ';
            updateTextBox("¹«Àü±â¸¦ È¹µæÇß½À´Ï´Ù! Æ¯¼ö ¾ÆÀÌÅÛÀÔ´Ï´Ù!");
            printstat(user);
            break;
        case 'G': // ÃÑ
            user->gun++;
            map[newY][newX] = ' ';
            updateTextBox("ÃÑÀ» È¹µæÇß½À´Ï´Ù! ÀÌÁ¦ ÃÑÀ» »ç¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù!");
            printstat(user);
            break;
        case 'B': // ÃÑ¾Ë
            user->bullet++;
            map[newY][newX] = ' ';
            updateTextBox("ÅºÃ¢À» È¹µæÇß½À´Ï´Ù!");
            printstat(user);
            break;
        }
    }
    return false;
}

bool is_zombie_position(int x, int y, char map[][MAP_WIDTH + 1]) {
    return map[y][x] == 'x';
}

void colorSetBack(int backColor, int textColor) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, (backColor << 4) + textColor);
}

void draw_map(char map[][MAP_WIDTH + 1]) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            setCursorPosition(x * 2 + 1, y + 1);
            switch (map[y][x]) {
            case '#': printf("¡á"); break;
            case 'x': setColor(12); printf("¡á "); setColor(7); break;
            case 'F': setColor(14); printf("¡Ú "); setColor(7); break;
            case 'W': setColor(14); printf("¡Ú "); setColor(7); break;
            case 'M': setColor(14); printf("¡Ú "); setColor(7); break;
            case '!': setColor(11); printf("¡Ú "); setColor(7); break; // ¹«Àü±â (ÇÏ´Ã»ö)
            case 'N': setColor(9);  printf("¡Ü "); setColor(7); break;
            /*case '%': colorSetBack(6, 6); std::cout << "  "; setColor(7); break;*/
            case '%': setColor(6); std::cout << "*"; setColor(7); break;
            case '?': setColor(11); printf("¡á"); setColor(7); break;
            case 'P': setColor(3); std::cout << "¿ô"; setColor(7); break;
            case '*': std::cout << "¢´"; break;
            case 'G': setColor(15); std::cout << "¡þ"; setColor(7); break;
            case 'B': setColor(15); std::cout << "¤ý"; setColor(7); break;
            default: printf("  "); break;
            }
        }
    }
}

void copy_map(int day_num, char map[][MAP_WIDTH+1]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j <= MAP_WIDTH; j++) {
            map[i][j] = game_map[day_num][i][j];
        }
    }
}
