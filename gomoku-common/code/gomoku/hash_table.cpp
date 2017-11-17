/**
 * @File gomoku/hash_table.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-09 09:55:16
 */
#include "gomoku/hash_table.h"

#include <string>
using namespace std;

namespace gomoku
{

HashType hashChessBoardApAlg(const ChessBoard & board)
{
    HashType hash = 0;
    for(TChessPos r = 0; r < CHESS_BOARD_SIZE; r++)
    {
        for(TChessPos c = 0; c < CHESS_BOARD_SIZE; c++)
        {
            if( (c & 1) == 0)
            {
                hash ^= ((hash << 7) ^ board.m_board[r][c] ^ (hash >> 3));
            }
            else
            {
                hash ^= (~((hash << 11) ^ board.m_board[r][c] ^ (hash >> 5)));
            }
        }
    }
    return (hash & 0x7FFFFFFF);
}

HashType hashChessBoardJenkinsAlg(const ChessBoard & board)
{
    HashType hash = 0;
    for(TChessPos r = 0; r < CHESS_BOARD_SIZE; r++)
    {
        for(TChessPos c = 0; c < CHESS_BOARD_SIZE; c++)
        {
            hash += board.m_board[r][c];
            hash += hash << 10;
            hash ^= hash >> 6;
        }
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

}//namespace gomoku