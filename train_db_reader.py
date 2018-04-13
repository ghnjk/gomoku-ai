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

def estimate_win_rate(batchSize):
    try:
        batchSize = int(batchSize)
        if batchSize < 1:
            batchSize = 1
        import matplotlib.pyplot as plt
        x = []
        y = []
        idx = 0
        while idx < len(historyBoardArray):
            cnt = 0
            winCnt = 0
            for i in range(batchSize):
                idx += 1
                cnt += 1
                if idx >= len(historyBoardArray):
                    break
                dbBoard = historyBoardArray[idx]
                if dbBoard.check_game_over() and dbBoard.winColor == 0:
                    winCnt += 1
            x.append(idx)
            y.append(winCnt * 100 / cnt)
        plt.figure()  
        plt.plot(x,y)  
        plt.xlabel("self play count(s)")  
        plt.ylabel("black win rate(%)")  
        plt.title("self play win rate estimation.")
        plt.grid(True)
        plt.ylim(0, 100)
        plt.show()
    except:
        print "estimate_win_rate failed."

def show_train_loss(lossFile):
    try:
        import matplotlib.pyplot as plt
        x = []
        allLoss = []
        winRateLoss = []
        moveRateLoss = []
        with open(lossFile) as fp:
            for line in fp:
                fields = line.strip().split(' ')
                if len(fields) != 4:
                    continue
                x.append(fields[0])
                allLoss.append(fields[1])
                winRateLoss.append(fields[2])
                moveRateLoss.append(fields[3])
        plt.figure() 
        plt.xlabel("self play count(s)")
        plt.ylabel("loss value")
        plt.plot(x, allLoss, color="blue", linewidth = 3, label = "totalLoss")
        plt.plot(x, winRateLoss, 'r-.', linewidth = 1, label = "winRateLoss")
        plt.plot(x, moveRateLoss, color="gray", linewidth = 1, label = "moveRateLoss")
        plt.legend() # 显示图例
        plt.grid(True)
        plt.show()
    except:
        print "show_train_loss failed."

if __name__ == '__main__':
    dataDir = "data"
    if len(sys.argv) == 2:
        dataDir = sys.argv[1]
    dbFile = os.path.join(dataDir, 'train_data.db')
    lossFile = os.path.join(dataDir, "loss_data.txt")
    read_history_chess_board(dbFile)
    while True:
        print "%s has %d chess." % (dbFile, len(historyBoardArray))
        try:
            line = raw_input("command: (play idx/set playIntervalMs/estimate batchSize)/loss: ")
        except:
            break
        line = line.strip()
        if line.startswith("play"):
            replay_chess(line[4:])
        elif line.startswith("set"):
            set_play_interval(line[3:])
        elif line.startswith("estimate"):
            estimate_win_rate(line[8:])
        elif line.startswith("loss"):
            show_train_loss(lossFile)
        else:
            print "invalid command."
