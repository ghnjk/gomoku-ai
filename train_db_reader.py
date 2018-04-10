#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
import sys
from gomoku_chess import *
import time

historyBoardArray = []

rowCount = 15
colCount = 15
nInRow = 5
playIntervalMs = 300

def read_history_chess_board(dbFile):
    with open(dbFile, "r") as fp:
        for line in fp:
            line = line.strip()
            if len(line) < 3:
                continue
            if line[0] != 'B' and line[0] != 'W':
                continue
            board = GomokuBoard(rowCount, colCount, nInRow)
            board.load_from_sgf(line)
            historyBoardArray.append(board)

def replay_chess(idx):
    try:
        idx = int(idx)
        if idx < 0 or idx >= len(historyBoardArray):
            raise "invalid idx"
        board = GomokuBoard(rowCount, colCount, nInRow)
        dbBoard = historyBoardArray[idx]
        for i in range(len(dbBoard.moveHistory)):
            (x, color) = dbBoard.moveHistory[i]
            (r, c) = dbBoard.location_to_move(x)
            board.play(r, c, color)
            print str(board)
            time.sleep(playIntervalMs / 1000.0)
        if board.check_game_over():
            print "game over. win color " + board.playerName[board.winColor]
    except:
        print "replay_chess [%s] failed." % (idx)

def set_play_interval(mSec):
    try:
        mSec = int(mSec)
        global playIntervalMs
        playIntervalMs = mSec
        print "set playIntervalMs " + str(mSec)
    except:
        print "set_play_interval [%s] failed." % (mSec)
if __name__ == '__main__':
    dbFile = 'data/train_data.db'
    if len(sys.argv) == 2:
        dbFile = sys.argv[1]
    read_history_chess_board(dbFile)
    while True:
        print "%s has %d chess." % (dbFile, len(historyBoardArray))
        try:
            line = raw_input("command: (play idx/set playIntervalMs): ")
        except:
            break
        line = line.strip()
        if line.startswith("play"):
            replay_chess(line[4:])
        elif line.startswith("set"):
            set_play_interval(line[3:])
        else:
            print "invalid command."
