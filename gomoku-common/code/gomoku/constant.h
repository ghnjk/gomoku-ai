/**
 * @File gomoku/constant.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:50:43
 */
#ifndef gomoku__CONSTANT_H__
#define gomoku__CONSTANT_H__
#include <stdio.h>
#include <string>

using std::string;

namespace gomoku
{

//棋子颜色类型
typedef unsigned char TChessColor;
//棋子位置类型
typedef  int TChessPos;
//评分类型
typedef double TScore;

#define CHESS_BORD_SIZE 15

//棋子的颜色
const TChessColor COLOR_BLANK = '.';
const TChessColor COLOR_WHITE = 'W';
const TChessColor COLOR_BLACK = 'B';

//是否合法棋子
#define IsValidPos(r, c) ( (r) >= 0 &&  (r) < CHESS_BORD_SIZE && (c) >= 0 && (c) < CHESS_BORD_SIZE )
//获取对方的颜色
#define GetReverseColor(c) ( (c) == COLOR_BLACK ? COLOR_WHITE: COLOR_BLACK )

//最大走法
const size_t MAX_MOVE_COUNT = 15 * 15;



}//namespace gomoku


#endif //gomoku__CONSTANT_H__
