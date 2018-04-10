#!/usr/bin/python
# -*- coding: UTF-8 -*- 
from gomoku_model import *
from keras.utils import plot_model

if __name__ == '__main__':
    rowCount = 15
    colCount = 15
    policyModel = GomokuModel(rowCount, colCount)
    model = policyModel.build_model()
    plot_model(model, show_shapes = True, to_file='data/model.png')
    plot_model(model.get_layer(name = "resNet"), show_shapes = True, to_file='data/resNet.png')
