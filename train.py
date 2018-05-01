#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
import sys
from gomoku_chess import *
from gomoku_model import *
from mcts_alpha import *
import numpy as np
import pandas as pd  
from player import *
from datetime import datetime
from collections import deque
import random
import pickle
import math
def sigmoid(x):
  return 1 / (1 + math.exp(-x))

def swap_color_state(state):
    """
    交换黑白两色
    """
    tmp = state.copy()
    tmp[:,:, [0, 1]] = tmp[:,:, [1,0]]
    return tmp

def get_equal_train_data(rowCount, colCount, state, winRate, moveRate):
    """
    由于旋转，对称特性， 增加训练数据
    """
    allData = []
    state = np.swapaxes(state, 1, 2)
    state = np.swapaxes(state, 0, 1)
    for i in [1, 2, 3, 4]:
        #rotation
        eqState = np.array([np.rot90(s, i) for s in state])
        eqMoveRate = np.rot90(moveRate.reshape(rowCount, colCount), i)
        tmpState = np.swapaxes(eqState, 0, 1)
        tmpState = np.swapaxes(tmpState, 1, 2)
        allData.append((tmpState, winRate, eqMoveRate.flatten()))
        #flip horizon
        eqState = np.array([np.fliplr(s) for s in eqState])
        eqMoveRate = np.fliplr(eqMoveRate)
        tmpState = np.swapaxes(eqState, 0, 1)
        tmpState = np.swapaxes(tmpState, 1, 2)
        allData.append((tmpState, winRate, eqMoveRate.flatten()))
    return allData
    
class TrainDataStore(object):
    """
    store and read trainning data
    """

    def __init__(self, fileName, maxQueSize):
        self.fileName = fileName
        self.bufferFileName = self.fileName + ".train.tmp"
        self.indexFileName = self.fileName + ".index"
        self.boardFeatureTable = deque(maxlen=maxQueSize)
        self.winRateTable = deque(maxlen=maxQueSize)
        self.moveRateTable = deque(maxlen=maxQueSize)
        self.selfPlayCnt = 0
        self.boardHistoryBuffer = []

    def load(self):
        """
        load data from file:
        store['shape'] = [1, 3]
        store['boardFeatureTable'] = [dataIndex, rowCount * colCount * channelCount]
        store['winRateTable'] = [dataIndex, 1]
        store['moveRateTable'] = [dataIndex, rowCount * colCount]
        store['modelVersionTable'] = [dataIndex, 1]
        """
        if os.path.isfile(self.indexFileName):
            fp = open(self.indexFileName, "r")
            s = fp.read()
            fp.close()
            self.selfPlayCnt = int(s)
        if os.path.isfile(self.bufferFileName):
            fp = open(self.bufferFileName, "rb")
            mp = pickle.load(fp)
            self.boardFeatureTable = mp["boardFeatureTable"]
            self.winRateTable = mp["winRateTable"]
            self.moveRateTable = mp["moveRateTable"]
            fp.close()
            print("load train date success. data size: ", len(self.winRateTable))

    def save(self):
        """
        save data to hdf5 file
        """
        fp = open(self.indexFileName, "w")
        fp.write("%d" % (self.selfPlayCnt))
        fp.close()
        fp = open(self.fileName, "a")
        for s in self.boardHistoryBuffer:
            fp.write(s)
            fp.write("\n")
        fp.close()
        self.boardHistoryBuffer = []
        fp = open(self.bufferFileName, "wb")
        mp = {
            "boardFeatureTable": self.boardFeatureTable,
            "winRateTable": self.winRateTable,
            "moveRateTable": self.moveRateTable
        }
        pickle.dump(mp, fp)
        fp.close()

    def append(self, boardFeature, winRate, moveRate):
        """
        append a train case data to store
        """
        self.boardFeatureTable.append(boardFeature)
        self.winRateTable.append(winRate)
        self.moveRateTable.append(moveRate)

    def append_board(self, board):
        self.selfPlayCnt += 1
        s = "%d\n" % (self.selfPlayCnt)
        s += board.to_sgf_str()
        s += "\n"
        self.boardHistoryBuffer.append(s)

    def size(self):
        return len(self.boardFeatureTable)

    def get_last_self_play_count(self):
        return self.selfPlayCnt


def now_string():
    return datetime.now().strftime('%Y-%m-%d %H:%M:%S')

class SelfPlayTranner(object):
    """
    通过self-play训练模型
    """

    def __init__(self, rowCount, colCount, nInRow, trainBatchCnt, gameCntPerBatch
        , trainStoreFileName):
        """
        trainBatchCnt: 训练批次数
        gameCntPerBatch: 每个批次中， 需要完成self-play的棋局数
        """
        self.rowCount = rowCount
        self.colCount = colCount
        self.nInRow = nInRow
        self.trainBatchCnt = trainBatchCnt
        self.gameCntPerBatch = gameCntPerBatch
        self.maxRemainSize = 80000 # 训练时， 保留最大的历史棋局数
        self.modelPath = "data/model.json"
        self.weightPath = "data/weight.hdf5"
        self.trainStore = TrainDataStore(trainStoreFileName, self.maxRemainSize)
        self.trainStore.load()
        self.estimateHist = []
        self.selfPlayCnt = self.trainStore.get_last_self_play_count()
        self.fpLossData = open("data/loss_data.txt", "a")
        self.winHisotry = deque(maxlen=100)
        lr = self.calcLearningRate()
        self.policyModel = GomokuModel(rowCount, colCount, learningRate = lr)
        self.epochs = 5
        self.trainModelMinSize = 2048
        self.estimateInterval = 500
        self.expandTemerature = 1 # (0.0, 1.0]
        if os.path.isfile(self.modelPath) and os.path.isfile(self.weightPath):
            self.policyModel.load_model(self.modelPath, self.weightPath)
        else:
            self.policyModel.build_model()

    def run(self):
        """
        启动训练
        """
        for i in range(0, self.trainBatchCnt):
            if(i % self.estimateInterval == 0 and i > 0):
                self.estimate()
            for j in range(0, self.gameCntPerBatch):
                print "%s self play index %d" % (now_string(), self.selfPlayCnt)
                self.run_one_self_play()
                sys.stdout.flush()
            if self.trainStore.size() >= self.trainModelMinSize * 2:
                self.update_model()
            self.trainStore.save()

    def run_one_self_play(self):
        """
        执行一轮self play
        """
        board = GomokuBoard(self.rowCount, self.colCount, self.nInRow)
        alphaZeroEngine = AlphaZeroEngine(self.rowCount, self.colCount
            , mctsPlayout = 500
            , policyModel = self.policyModel
            , isSelfPlay = True
            , cPuct = 5)
        states = []
        outProbs = []
        winnerZs = []
        blackPlayout, whitePlayout = self.calcMctsPlayout()
        while True:
            boardState = board_to_state(board)
            if board.curPlayer == COLOR_BLACK:
                alphaZeroEngine.setMctsPlayout(blackPlayout)
            else:
                alphaZeroEngine.setMctsPlayout(whitePlayout)
            (bestMoveX, moveRates) = alphaZeroEngine.search_moves(board, self.expandTemerature)
            states.append(boardState)
            outProbs.append(moveRates)
            winnerZs.append(board.curPlayer)
            (r, c) = board.location_to_move(bestMoveX)
            board.play(r, c, board.curPlayer)
            print str(board)
            if board.check_game_over():
                self.winHisotry.append(board.winColor)
                break
        print "game over. winner : " + board.playerName[board.winColor]
        if board.winColor == TIDE:
            winnerZs = np.zeros(len(winnerZs))
        else:
            for i in range(0, len(winnerZs)):
                #x = len(winnerZs) - i
                #y = 1 - 2 * (1 -  sigmoid(0.2) / sigmoid((x+1) / 10.0) )
                if winnerZs[i] == board.winColor:
                    winnerZs[i] = 1
                else:
                    winnerZs[i] = -1
                # print "winnerZs[i]: " + str(winnerZs[i])
                # print "outProbs: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.3lf " % (outProbs[i][r * board.rowCount + c])
                #     print s
                # print "**** self chess: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][0])
                #     print s
                # print "**** optcal chess: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][1])
                #     print s
                # print "**** last chess: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][2])
                #     print s
                # print "**** is first: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][3])
                #     print s
        self.selfPlayCnt += 1
        self.append_train_data(states, outProbs, winnerZs)
        self.trainStore.append_board(board)

    def calcMctsPlayout(self):
        """
        基础500步， 根据赢率 方向加一下步骤，加强未来一段时间的搜索深度
        """
        blackPlayout = 500
        whitePlayout = 500
        blackWin = 0
        whiteWin = 0
        for i in range(len(self.winHisotry)):
            if self.winHisotry[i] == COLOR_BLACK:
                blackWin += 1
            elif self.winHisotry[i] == COLOR_WHITE:
                whiteWin += 1
        if blackWin > whiteWin:
            blackWin -= whiteWin
            whitePlayout += blackWin * 3
        else:
            whiteWin -= blackWin
            blackPlayout += whiteWin * 3
        print("epoch stradge: blackPlayout: ", blackPlayout, " whitePlayout: ", whitePlayout)
        return blackPlayout, whitePlayout

    def calcLearningRate(self):
        """
        训练1W次的话 lr 从 0.005 逐渐减到 0.00001
        """
        maxLr = 0.001
        minLr = 0.0001
        lrGradeCnt = 20
        lrMoment = (maxLr - minLr) / lrGradeCnt
        totalSelfPlayCnt = 10000
        curLr = 0
        if self.selfPlayCnt >= totalSelfPlayCnt:
            curLr = minLr
        else:
            curGrade = (self.selfPlayCnt * lrGradeCnt) / totalSelfPlayCnt
            curLr = maxLr - curGrade * lrMoment
        print("train learning rate: ", curLr)
        return curLr

    def append_train_data(self, states, outProbs, winnerZs):
        """
        将数据添加到训练数据池中
        """
        for i in range(0, len(states)):
            #self.trainStore.append(states[i], winnerZs[i], outProbs[i]);
            eqData = get_equal_train_data(self.rowCount, self.colCount, states[i], winnerZs[i], outProbs[i])
            for (eqState, eqWinRate, eqMoveRate) in eqData:
                self.trainStore.append(eqState, eqWinRate, eqMoveRate)
                # if eqWinRate == 1:
                #     tmpState = swap_color_state(eqState)
                #     self.trainStore.append(tmpState, eqWinRate, eqMoveRate)

    def update_model(self):
        """
        更新策略模型
        """
        #全部参与训练
        # startIdx = 0
        # if self.trainStore.size() > self.maxRemainSize:
        #     startIdx = self.trainStore.size() - self.maxRemainSize
        # cnt = self.trainStore.size() - startIdx
        # states = []
        # winRates = []
        # outProbs = []
        # for i in range(startIdx, startIdx + cnt):
        #     states.append(self.trainStore.boardFeatureTable[i])
        #     winRates.append(self.trainStore.winRateTable[i])
        #     outProbs.append(self.trainStore.moveRateTable[i])
        #随机选择
        states = []
        winRates = []
        outProbs = []
        idx = random.sample(range(self.trainStore.size()), self.trainModelMinSize)
        for i in idx:
            states.append(self.trainStore.boardFeatureTable[i])
            winRates.append(self.trainStore.winRateTable[i])
            outProbs.append(self.trainStore.moveRateTable[i])
        cnt = len(states)
        states = np.array(states)
        winRates = np.array(winRates)
        outProbs = np.array(outProbs)
        self.policyModel.fit(states, winRates, outProbs, epochs = self.epochs, batchSize = cnt)
        self.policyModel.save_model(self.modelPath, self.weightPath)
        if self.selfPlayCnt % 100 == 0:
            backupDir = "data/backup_model"
            backupFile = backupDir + "/%d" % (self.selfPlayCnt)
            if not os.path.isdir(backupDir):
                os.makedirs(backupDir)
            self.policyModel.save_model(backupFile + ".model", backupFile + ".weights")
        loseInfo = self.policyModel.evaluate(states, winRates, outProbs, batchSize = cnt)
        print >> self.fpLossData, "%d %1.4lf %1.4lf %1.4lf" % (
            self.selfPlayCnt, loseInfo[0], loseInfo[1], loseInfo[2]
            )
        self.fpLossData.flush()
        sys.stdout.flush()

    def estimate(self):
        """
        评估训练结果
        """
        print "%s start estimate: " % (now_string())
        winCnt = 0
        tideCnt = 0
        competeCnt = 10
        for i in range(competeCnt):
            z = self.estimate_once()
            print "[%d]: %d" % (i, z)
            if z > 0:
                winCnt += 1
            if z == 0:
                tideCnt += 1
        print "winRate: %0.1lf tideRate: %0.1lf" % (winCnt / float(competeCnt), tideCnt / float(competeCnt) )
        self.estimateHist.append(winCnt / float(competeCnt) )
        print str(self.estimateHist)
        # if winCnt == competeCnt:
        #     exit(0)


    def estimate_once(self):
        """
        评估训练结果
        """
        board = GomokuBoard(self.rowCount, self.colCount, self.nInRow)
        pureMctsPlayer = PureMctsPlayer(1000)
        alphaPlayer = AlphaZeroPlayer(200)
        alphaPlayer.init_player(board, 1, self.policyModel)
        pureMctsPlayer.init_player(board, 0)
        players = [pureMctsPlayer, alphaPlayer]
        while True:
            board.check_game_over()
            if board.isGameOver:
                if board.winColor == TIDE:
                    return 0
                elif board.winColor == alphaPlayer.playerColor:
                    return 1
                else:
                    return -1
            player = players[board.curPlayer]
            (r, c) = player.gen_next_move(board)
            board.play(r, c, board.curPlayer)
        


if __name__ == '__main__':
    trainner = SelfPlayTranner(
        rowCount = 15
        , colCount = 15
        , nInRow = 5
        , trainBatchCnt = 10000
        , gameCntPerBatch = 1
        , trainStoreFileName = "data/train_data.db"
        )
    trainner.run()

