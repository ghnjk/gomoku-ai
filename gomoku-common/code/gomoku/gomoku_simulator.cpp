/**
 * @File gomoku/gomoku_simulator.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-09 09:55:16
 */
#include "gomoku/gomoku_simulator.h"

#include <string>
using namespace std;

namespace gomoku
{

const static string arrLineTy[] = {
    //FIVE
    "AAAAA"
    //活四
    , "_AAAA_"
    //死四
    , "AAAA_"
    , "AAA_A"
    , "AA_AA"
    //活三
    , "_AAA__"
    //死三
    , "AAA__"
    , "AA_A_"
    , "A_AA_"
    , "A__AA"
    , "A_A_A"
    // 活二
    , "__AA__"
    , "_AA___"
    , "_A_A__"
    , "_A__A_"
    , "A___A"
};
const int arrLineTypeScore[] = {
    //FIVE
    //"AAAAA"
    10000
    //活四
    //, "_AAAA_"
    , 600
    //死四
    //, "AAAA_"
    , 160
    //, "AAA_A"
    , 150
    //, "AA_AA"
    , 150
    //活三
    //, "_AAA__"
    , 130
    //死三
    //, "AAA__"
    , 40
    //, "AA_A_"
    , 25
    //, "A_AA_"
    , 25
    //, "A__AA"
    , 15
    //, "A_A_A"
    , 15
    // 活二
    //, "__AA__"
    , 15
    //, "_AA___"
    , 10
    //, "_A_A__"
    , 10
    //, "_A__A_"
    , 10
    //, "A___A"
    , 5
};
MoveEvaluator::MoveEvaluator()
{
    initLineTypeMap();
}
//评估某步的价值
TScore MoveEvaluator::evaluateMove(const ChessBoard & board, const ChessMove & move)
{
    int iPos;
    int iLineType;
    char szLine[9];

    ChessMove mv = move;
    TScore res = 0;
    static const TChessPos dr[] = {0, -1, -1, -1};
    static const TChessPos dc[] = {1, 1, 0, -1};
    for(int i = 0; i < 4; i ++)
    {
        //自己晴型
        mv.color = move.color;
        this->getLineFromBoard(board, mv, dr[i], dc[i]
            , iPos, szLine); 
        iLineType = this->getLineType(iPos, szLine);
        if(iLineType >= 0)
        {
            res += arrLineTypeScore[iLineType];
        }
        //对方棋型
        mv.color = GetReverseColor(move.color);
        this->getLineFromBoard(board, mv, dr[i], dc[i]
            , iPos, szLine); 
        iLineType = this->getLineType(iPos, szLine);
        if(iLineType >= 0)
        {
            res += arrLineTypeScore[iLineType] ;//* 4 / 5;
        }
    }
    return res;
}

const static int LINE_LEN = 9;
bool MoveEvaluator::isMatchLine(int iPos, char szLine[LINE_LEN], const string & str)
{
    int i, j;
    for(i = 0; i < (int)(str.length()); i++)
    {
        if(str[i] != 'A')
        {
            continue;
        }
        for(j = 0; j < (int)(str.length()); j++)
        {
            int k = iPos + j - i;
            if( k < 0 || k >= LINE_LEN )
            {
                break;
            }
            if(str[j] == '_' && szLine[k] != 0)
            {
                break;
            }
            if(str[j] == 'A' && szLine[k] != 1 && iPos != k)
            {
                break;
            }
        }
        if(j == (int) (str.length()) )
        {
            return true;
        }
        for(j = 0; j < (int)(str.length()); j++)
        {
            int k = iPos - j + i;
            if( k < 0 || k >= LINE_LEN)
            {
                break;
            }
            if(str[j] == '_' && szLine[k] != 0)
            {
                break;
            }
            if(str[j] == 'A' && szLine[k] != 1 && iPos != k)
            {
                break;
            }
        }
        if(j == (int) str.length())
        {
            return true;
        }
    }
    return false;
}
bool MoveEvaluator::isValidLineType(int iPos, char szLine[LINE_LEN])
{
    int i, j;
    if(szLine[iPos] != 0)
    {
        return false;//当前位置不为空
    }
    for(i = 0; i < 5; i++)
    {
        if(szLine[i] == 0)
        {
            continue;
        }
        for(j = 1; j < 5; j++)
        {
            if(szLine[i] != szLine[j])
            {
                break;
            }
        }
        if(j == 5 && szLine[i] == 2)
        {
            return false;//对方五子
        }
    }
    for(i = 0; i < 4; i++)
    {
        if(szLine[i] == 0)
        {
            continue;
        }
        for(j = 1; j < 6; j++)
        {
            if(szLine[i] != szLine[j])
            {
                break;
            }
        }
        if(j == 6)
        {
            return false;//超过六个子
        }
    }
    bool isBlock = false;
    for(i = 0; i < LINE_LEN; i++)
    {
        j = iPos - i;
        if(j < 0)
        {
            continue;
        }
        if(szLine[j] == 2)
        {
            if(isBlock)
            {
                return false;
            }
            else
            {
                isBlock = true;
            }
        }
    }
    isBlock = false;
    for(i = 0; i < LINE_LEN; i++)
    {
        j = iPos + i;
        if(j >= LINE_LEN)
        {
            continue;
        }
        if(szLine[j] == 2)
        {
            if(isBlock)
            {
                return false;
            }
            else
            {
                isBlock = true;
            }
        }
    }
    return true;
}
int MoveEvaluator::getLineType(int iPos, char szLine[9])
{
    int iType = 0;
    int pw = 1;
    for(int i = 0; i < LINE_LEN; i++)
    {
        iType += szLine[i] * pw;
        pw *= 3;
      //  printf("%d %d\n", pw, iType);
    }
    iType = iType * 10 + iPos;
   // printf("iType %d\n", iType);
    map<int, int>::const_iterator it;
    it = m_mpLineType.find(iType);
    if(it == m_mpLineType.end())
    {
        return -1;
    }
    else
    {
        return it->second;
    }
}
void MoveEvaluator::initLineTypeMap()
{
    m_mpLineType.clear();
    //line 表示法 每两个bit一个位置
    //bit 00 空 01 本方棋子 10 对方棋子/不可以走的
    int iTypeMax = 1;
    for(int i = 0; i < LINE_LEN; i++)
    {
        iTypeMax *= 3;
    }
    for(int iType = 0; iType < iTypeMax; iType++)
    {
        char szLine[LINE_LEN];
        for(int iTmp = iType, i = 0; i < (int) sizeof(szLine); i++)
        {
            szLine[i] = iTmp % 3;
            iTmp /= 3;
        }
        for(int iPos = 0; iPos < LINE_LEN; iPos++)
        {
            if(!isValidLineType(iPos, szLine))
            {
                continue;
            }
            for(size_t i = 0; i < sizeof(arrLineTy) / sizeof(arrLineTy[0]); i++)
            {
                if(isMatchLine(iPos, szLine, arrLineTy[i]))
                {
                    m_mpLineType[iPos + iType * 10] = i;
                    break;
                }
            }
        }
    } 
}
void MoveEvaluator::getLineFromBoard(const ChessBoard & board, const ChessMove & move, TChessPos dr, TChessPos dc
    , int & iPos, char szLine[LINE_LEN] )
{
    TChessPos tr, tc;
    tr = move.row;
    tc = move.col;
    int i;
    TChessColor reverseColor = GetReverseColor(move.color);
    for(i = 0; i < 5; i++)
    {
        tr -= dr;
        tc -= dc;
        if(! IsValidPos(tr, tc))
        {
            break;
        }
        if(board.m_board[tr][tc] == reverseColor)
        {
            break;
        }
    }
    bool isEnd = false;
    for(i = 0; i < 9; i++)
    {
        tr += dr;
        tc += dc;
        if(isEnd)
        {
            szLine[i] = 0;
            continue;
        }
        if(!IsValidPos(tr, tc))
        {
            isEnd = true;
            szLine[i] = 2;
            continue;
        }
        //printf("%d %d\n", tr, tc);
        if(tr == move.row && tc == move.col)
        {
            iPos = i;
        }
        if(board.m_board[tr][tc] == COLOR_BLANK)
        {
            szLine[i] = 0;
        }
        else if(board.m_board[tr][tc] == reverseColor)
        {
            szLine[i] = 2;
            isEnd = true;
        }
        else
        {
            szLine[i] = 1;
        }
    }
}

}//namespace gomoku