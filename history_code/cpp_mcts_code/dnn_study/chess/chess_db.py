#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
import logging
from chess_board import *

class ChessDbRecord(object):
    """
    db记录
    """

    def __init__(self):
        self.szHead = bytearray(64)
        self.szChessMap = bytearray(CHESS_BOARD_SIZE * (CHESS_BOARD_SIZE + 1))
        self.szInfo = bytearray(512)

    def get_record_size(self):
        """
        获取记录的字节数
        """
        return len(self.szHead) + len(self.szChessMap) + len(self.szInfo)

    def get_record_index(self):
        """
        获取记录的索引
        """
        return int(self.szHead[len("chessRecord"):].strip())

    def get_board(self):
        """
        获取chess_board对象
        """
        res = ChessBoard()
        white_cnt = 0
        black_cnt = 0
        res.board = []
        for i in range(0, CHESS_BOARD_SIZE):
            res.board.append([])
            for j in range(0, CHESS_BOARD_SIZE):
                color = str(self.szChessMap[i * (CHESS_BOARD_SIZE + 1) + j])
                #print "i %d j %d color [%s]" % (i, j, color)
                if color == COLOR_BLACK:
                    black_cnt += 1
                elif color == COLOR_WHITE:
                    white_cnt += 1
                else:
                    color = COLOR_BLANK
                res.board[i].append(color)
                #print "i %d j %d color [%s]" % (i, j, res.board[i][j])
        if black_cnt > white_cnt:
            res.next_color = COLOR_WHITE
        else:
            res.next_color = COLOR_BLACK
        return res

    def is_valid_record(self):
        if str(self.szHead).startswith("chessRecord"):
            return True
        else:
            return False

    def get_chess_info(self):
        """
        获取棋局map信息
        """
        res = {}
        msg = str(self.szInfo).strip()
        arrItem = msg.split(' ')
        for item in arrItem:
            item = item.strip()
            fields = item.split('=')
            if len(fields) == 1:
                fields[0].strip()
                res[fields[0]] = ''
            elif len(fields) == 2:
                fields[0].strip()
                fields[1].strip()
                res[fields[0]] = fields[1]
        return res

class ChessDb(object):
    """
    棋局信息数据库
    """

    def __init__(self, dbFilePath):
        self.dbFilePath = dbFilePath
        if os.path.isfile(self.dbFilePath):
            self.dbFp = open(self.dbFilePath, "rb+")
        else:
            self.dbFp = open(self.dbFilePath, "wb")
            self.dbFp.close()
            self.dbFp = open(self.dbFilePath, "rb+")

    def __del__(self):
        self.close()

    def close(self):
        """
        关闭连接
        """
        if not self.dbFp is None:
            self.dbFp.close()
            self.dbFp = None

    def read_next(self):
        """
        读吓一条记录
        """
        if self.dbFp is None:
            return None
        rec = ChessDbRecord()
        buf = self.dbFp.read(rec.get_record_size())
        if len(buf) == 0:
            return None
        if len(buf) != rec.get_record_size():
            logging.error("invalid record: (%s)", str(buf))
            return None
        idx = 0
        rec.szHead = buf[idx: idx + len(rec.szHead)]
        idx += len(rec.szHead)
        rec.szChessMap = buf[idx: idx + len(rec.szChessMap)]
        idx += len(rec.szChessMap)
        rec.szInfo = buf[idx:]
        if not rec.is_valid_record():
            return None
        return rec



