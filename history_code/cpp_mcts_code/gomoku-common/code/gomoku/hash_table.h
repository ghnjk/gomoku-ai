/**
 * @File gomoku/hash_table.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-08 23:39:18
 */
#ifndef gomoku__GOMOKU_HASHTABLE_H__
#define gomoku__GOMOKU_HASHTABLE_H__

#include "gomoku/chess_board.h"

namespace gomoku
{

typedef unsigned int HashType;

HashType hashChessBoardApAlg(const ChessBoard & board);
HashType hashChessBoardJenkinsAlg(const ChessBoard & board);

template<typename T>
struct HashDataNode
{
    bool hasData;
    HashType nCheckCode;
    T data;
    HashDataNode()
    {
        hasData = false;
    }
};

template<typename T>
class HashTable
{
public:
    HashTable(size_t nTableSize);
    virtual ~HashTable();
    bool contains(const ChessBoard & board)const;
    T * get(const ChessBoard & board);
    void put(const ChessBoard & board, const T & value);
protected:
    size_t m_nTableSize;
    HashDataNode<T> * m_arrNode;
};

template<typename T>
HashTable<T>::HashTable(size_t nTableSize)
{
    m_nTableSize = nTableSize;
    m_arrNode = new HashDataNode<T>[m_nTableSize];
}

template<typename T>
HashTable<T>::~HashTable()
{
    if(m_arrNode != NULL)
    {
        delete [] m_arrNode;
        m_arrNode = NULL;
    }
}
template<typename T>
bool HashTable<T>::contains(const ChessBoard & board)const
{
    HashType jenkiinsCode = hashChessBoardJenkinsAlg(board);
    size_t idx = jenkiinsCode % m_nTableSize;
    if(! m_arrNode[idx].hasData)
    {
        return false;
    }
    HashType apCode = hashChessBoardApAlg(board);
    if(apCode == m_arrNode[idx].nCheckCode)
    {
        return true;
    }
    else
    {
        return false;
    }
}
template<typename T>
T * HashTable<T>::get(const ChessBoard & board)
{
    HashType jenkiinsCode = hashChessBoardJenkinsAlg(board);
    size_t idx = jenkiinsCode % m_nTableSize;
    if(! m_arrNode[idx].hasData)
    {
        return NULL;
    }
    HashType apCode = hashChessBoardApAlg(board);
    if(apCode == m_arrNode[idx].nCheckCode)
    {
        return &(m_arrNode[idx].data);
    }
    else
    {
        return NULL;
    }
}
template<typename T>
void HashTable<T>::put(const ChessBoard & board, const T & value)
{
    HashType jenkiinsCode = hashChessBoardJenkinsAlg(board);
    size_t idx = jenkiinsCode % m_nTableSize;
    m_arrNode[idx].hasData = true;
    m_arrNode[idx].data = value;
    m_arrNode[idx].nCheckCode = hashChessBoardApAlg(board);
}

}// namespace gomoku

#endif