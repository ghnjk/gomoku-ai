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
typedef unsigned char TChessPos;
//评分类型
typedef float TScore;

const static int CHESS_BORD_SIZE = 15;

//旗子的颜色
const TChessColor COLOR_BLANK = '.';
const TChessColor COLOR_WHITE = 'W';
const TChessColor COLOR_BLACK = 'B';

#define IsValidPos(r, c) ( (r) >= 0 &&  (r) < CHESS_BORD_SIZE && (c) >= 0 && (c) < CHESS_BORD_SIZE )

}//namespace gomoku


#endif //gomoku__CONSTANT_H__
