#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
from gomoku_model import *
from mcts_alpha import *
from mcts_pure import PureMctsEngine

class IPlayer(object):
    """
    player interface
    """
    def __init__(self):
        self.name = "player"
        self.playerColor = None

    def init_player(self, board, playerColor):
        """
        init the player with the board and the playerColor
        """
        self.playerColor = playerColor

    def gen_next_move(self, board):
        """
        gen next move and return (r, c)
        """
        pass

class HumanPlayer(IPlayer):
    """
    terminal human player
    """
    def init_player(self, board, playerColor):
        """
        init the player with the board and the playerColor
        """
        self.playerColor = playerColor
        self.name = "human player " + board.playerName[self.playerColor]

    def gen_next_move(self, board):
        """
        gen next move and return (r, c)
        """
        while True:
            try:
                line = raw_input("input move (r, c) for color %s: " % (board.playerName[board.curPlayer]))
            except:
                return "end"
            line = line.strip()
            f = line.split(' ')
            if len(f) != 2:
                return line
            try:
                r = int(f[0], 16)
                c = int(f[1], 16)
            except:
                print "invalid move: " + line
                continue
            if not board.is_valid_move(r, c):
                print "invalid move: " + line
                continue
            return (r, c)

class PureMctsPlayer(IPlayer):
    """
    pure mcts search engin player
    """
    def __init__(self, searchCount = 100):
        self.engine = PureMctsEngine()
        self.searchCount = searchCount

    def init_player(self, board, playerColor):
        """
        init the player with the board and the playerColor
        """
        self.playerColor = playerColor
        self.name = "mcts player " + board.playerName[self.playerColor]

    def gen_next_move(self, board):
        """
        gen next move and return (r, c)
        """ 
        return self.engine.search(board, self.searchCount)


class AlphaZeroPlayer(IPlayer):
    """
    alpha zero player
    """
    def __init__(self, playOut = 100, modelPath = "data/model.json", weightPath = "data/weight.hdf5"):
        self.playOut = playOut
        self.modelPath = modelPath
        self.weightPath = weightPath
        

    def init_player(self, board, playerColor, policyModel = None):
        """
        init the player with the board and the playerColor
        """
        self.playerColor = playerColor
        self.name = "alpha zero player " + board.playerName[self.playerColor]
        if policyModel is None:
            self.policyModel = GomokuModel(board.rowCount, board.colCount)
            if os.path.isfile(self.modelPath) and os.path.isfile(self.weightPath):
                self.policyModel.load_model(self.modelPath, self.weightPath)
            else:
                self.policyModel.build_model()
        else:
            self.policyModel = policyModel
        self.alphaZeroEngine = AlphaZeroEngine(board.rowCount, board.colCount
            , mctsPlayout = self.playOut
            , policyModel = self.policyModel
            , isSelfPlay = False
            , cPuct = 5)

    def gen_next_move(self, board):
        """
        gen next move and return (r, c)
        """ 
        boardState = board_to_state(board)
        (bestMoveX, moveRates) = self.alphaZeroEngine.search_moves(board)
        return board.location_to_move(bestMoveX)