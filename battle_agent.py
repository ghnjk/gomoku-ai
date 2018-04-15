#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
import sys
sys.stderr = fpBattleLog = open("data/battle.log", "a")

import json
from gomoku_chess import *
from gomoku_model import *
from mcts_alpha import *

def play_chess(board, policyModel, color):
    """
    gen next color move with plicyModel
    """
    alphaZeroEngine = AlphaZeroEngine(board.rowCount, board.colCount
            , mctsPlayout = 400
            , policyModel = policyModel
            , isSelfPlay = False
            , cPuct = 5)
    (bestMoveX, moveRates) = alphaZeroEngine.search_moves(board)
    return board.location_to_move(bestMoveX)
    
def main():
    """
    gomoku battle entrance
    """
    rowCount = 15
    colCount = 15
    modelPath = "data/model.json"
    weightPath = "data/weight.hdf5"
    policyModel = GomokuModel(rowCount, colCount)
    if os.path.isfile(modelPath) and os.path.isfile(weightPath):
        policyModel.load_model(modelPath, weightPath, showSummary = False)
    else:
        policyModel.build_model(showSummary = False)
    while True:
        try:
            line = raw_input()
        except:
            break
        line = line.strip()
        if len(line) == 0:
            continue
        print >> fpBattleLog, "receive : " + line        
        req = json.loads(line)
        resp = {}
        command = req.get("command", "")
        board = GomokuBoard(req["rows"], req["columns"], nInRow = 5)
        board.load_from_sgf(req["chessboard"])
        if command == "NEXT_BLACK":
            #print "NEXT_BLACK"
            (row, col) = play_chess(board, policyModel, COLOR_BLACK)
            resp["rowIndex"] = row
            resp["columnIndex"] = col
        elif command == "NEXT_WHITE":
            #print "NEXT_WHITE"
            (row, col) = play_chess(board, policyModel, COLOR_WHITE)
            resp["rowIndex"] = row
            resp["columnIndex"] = col
        else:
            resp["error"] = "invalid line: " + line
        print json.dumps(resp)
        print >> fpBattleLog,json.dumps(resp)
        sys.stdout.flush()
        fpBattleLog.flush()


if __name__ == '__main__':
    main()