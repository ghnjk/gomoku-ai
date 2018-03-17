#!/usr/bin/python
# -*- coding: UTF-8 -*- 
from chess import *
from chess.chess_board import *
from chess.chess_db import *

import numpy as np
import pandas as pd
from keras.models import Sequential, Model
from keras.layers import Input, ZeroPadding2D
from keras.layers.core import Dropout, Dense, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D, AveragePooling2D
from keras.layers.normalization import BatchNormalization
from keras.optimizers import Adam
from keras.layers.merge import concatenate
from keras.callbacks import ModelCheckpoint
from keras.utils import np_utils
from keras.models import model_from_json
import matplotlib  
import matplotlib.pyplot as plt

CHANNEL_SIZE = 1
DATA_FORMAT = "channels_last"

class DbDataLoader(object):
    """
    训练数据加载
    """

    def __init__(self):
        pass

    def __del__(self):
        pass

    def load_data(self, db_filePath, evaluate_version):
        """
        从chess db中加载数据
        返回(arr_x, arr_y)
        """
        arr_x = []
        arr_y = []
        db = ChessDb(db_filePath)
        cnt = 0
        while True:
            if cnt % 1000 == 0:
                print "loading chess %d" % (cnt)
            record = db.read_next()
            if record is None:
                break
            info = record.get_chess_info()
            if info.get("evaluateVersion", "") != evaluate_version:
                continue
            if info.get("nextMove.row", "") == "" or info.get("nextMove.col") == "" or info.get("nextMove.color") != info.get("nextPlayerColor"):
                continue
            board = record.get_board()
            # 将下一个棋子填充到board中
            move_color = info.get("nextMove.color")
            row = int(info.get("nextMove.row"))
            col = int(info.get("nextMove.col"))
            board.board[row][col] = move_color
            x, y = self.transfer_db_record(board, info)
            arr_x.append(x)
            arr_y.append(y)
            cnt += 1
        return (np.array(arr_x), np.array(arr_y))

    def transfer_db_record(self, board, info):
        """
        将db记录转成输入和输出
        (
        x // 棋局信息 [15][15][18]
            [0] 当前位置是否是本方棋子 0/1
            [1] 当前位置是否是对方的棋子 0/1
            [2，9】 八个方向上和该棋子相同颜色的个数
            [10, 17] 八个方向上和该能放该棋子的颜色最大个数0-4
        , y // 当前局面的胜率，针对下一个棋子的颜色
        )
        """
        mp = board.board
        color = board.next_color
        delta = [
            (1, 0)
            , (1, 1)
            , (0, 1)
            , (-1, 1)
            , (-1, 0)
            , (-1, -1)
            , (0, -1)
            , (1, -1)
        ]
        y = int(float(info.get("boardScore", "0")) * 10000)
        x = []
        for i in range(0, CHESS_BOARD_SIZE):
            x.append([])
            for j in range(0, CHESS_BOARD_SIZE):
                x[i].append([])
                if mp[i][j] == COLOR_BLANK:
                    x[i][j].append(3)
                elif mp[i][j] == color:
                    x[i][j].append(1)
                else:
                    x[i][j].append(2)
                # if mp[i][j] == COLOR_BLANK:
                #     x[i][j] = [0] * CHANNEL_SIZE
                #     continue
                # if mp[i][j] == color:
                #     x[i][j].append(1)
                # else:
                #     x[i][j].append(0)
                # x[i][j].append(1 - x[i][j][0])
                # for d in delta:
                #     cnt = self.__get_same_cnt(mp, mp[i][j], i, j, d)
                #     x[i][j].append(cnt)
                # for d in delta:
                #     cnt = self.__get_max_same_cnt(mp, mp[i][j], i, j, d)
                #     x[i][j].append(cnt)       
        return (x, y)

    def __get_same_cnt(self, mp, color, i, j, d):
        """
        计算i,j 在d方向相同颜色的棋子个数
        """
        cnt = 0
        ni = i
        nj = j
        for k in range(0, 4):
            ni += d[0]
            nj += d[1]
            if ni < 0 or nj < 0 or ni >= CHESS_BOARD_SIZE or nj >= CHESS_BOARD_SIZE:
                break
            if mp[ni][nj] == color:
                cnt += 1
            else:
                break
        return cnt

    def __get_max_same_cnt(self, mp, color, i, j, d):
        """
        计算i,j 在d方向相同颜色的棋子最多可以放的个数
        """
        cnt = 0
        ni = i
        nj = j
        for k in range(0, 4):
            ni += d[0]
            nj += d[1]
            if ni < 0 or nj < 0 or ni >= CHESS_BOARD_SIZE or nj >= CHESS_BOARD_SIZE:
                break
            if mp[ni][nj] == color or mp[ni][nj] == COLOR_BLANK:
                cnt += 1
            else:
                break
        return cnt

def CsvDataLoader(object):
    """
    从csv加载数据
    """

    def __init__(self):
        pass

    def __del__(self):
        pass

class DnnEvaluator(object):
    """
    dnn 神经网络学习评估棋
    """

    def __init__(self):
        pass

    def build_model(self):
        model = Sequential()
        model.add(
            ZeroPadding2D(padding = (1, 1)
                , input_shape = (CHESS_BOARD_SIZE, CHESS_BOARD_SIZE, CHANNEL_SIZE), data_format=DATA_FORMAT)
        )
        model.add(
            Convolution2D(32, (3, 3), padding="same", data_format=DATA_FORMAT
                , activation="tanh")
        )
        model.add(
            Convolution2D(32, (3, 3), padding="same", data_format=DATA_FORMAT
                , activation="tanh")
        )
        model.add(MaxPooling2D(pool_size=(2, 2), padding="same"))
        model.add(Dropout(0.1))

        model.add(
            Convolution2D(64, (3, 3), padding="same", data_format=DATA_FORMAT
                , activation="tanh")
        )
        model.add(
            Convolution2D(64, (3, 3), padding="same", data_format=DATA_FORMAT
                , activation="tanh")
        )
        model.add(MaxPooling2D(pool_size=(2, 2), padding="same"))
        model.add(Dropout(0.2))

        model.add(Flatten())
        model.add(Dense(512, activation='tanh'))
        model.add(Dropout(0.2))
        model.add(Dense(256, activation='tanh'))
        model.add(Dense(128, activation='tanh'))
        model.add(Dropout(0.2))
        model.add(Dense(64, activation='tanh'))
        model.add(Dense(1, activation='sigmoid'))

        model.compile(loss='mean_squared_error', optimizer="adadelta"
            , metrics=["mse"]
        )

        return model

    def calc_prediction_acc(self, y_true, y_pred):
        acc_cnt = [0] * 10
        for i in range(0, len(y_true)):
            grade = int ( abs(y_true[i] - y_pred[i]) * 10 )
            if grade > 10:
                grade = 10
            acc_cnt[grade] += 1
        print "prediction acc grade:"
        for i in range(0, 10):
            print "[%d]: %d" % (i, acc_cnt[i])

    def save_model(self, model, filePrefix):
        # serialize model to JSON
        model_json = model.to_json()
        with open(filePrefix + ".json", "w") as json_file:
            json_file.write(model_json)
        # serialize weights to HDF5
        model.save_weights(filePrefix + ".h5")
        print("Saved model to disk")

    def load_model(self, modelJsonFile, weightH5File):
        """
        load model
        """
        model = model_from_json(modelJsonFile)
        model.load_weights(weightH5File)
        return model

