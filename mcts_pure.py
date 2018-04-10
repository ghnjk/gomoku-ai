#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import math
import random
from gomoku_chess import *

MCTS_UCB_C_ARG = 2

class PureMctsNode(object):
    """
    node for mstc search
    """

    def __init__(self):
        self.winColor = None
        self.winCnt = 0
        self.tideCnt = 0
        self.searchCnt = 0
        self.children = {}

    def expand(self, board):
        """
        expand node for mcts
        """
        board.check_game_over()
        if board.isGameOver:
            self.winColor = board.winColor
        else:
            self.winColor = None
        for x in board.availables:
            self.children[x] = None

    def get_score(self):
        """
        get curent node score
        """
        return 1 - (self.winCnt / float(self.searchCnt))

    def is_new_node(self):
        """
        whether it is a new node
        """
        return self.searchCnt == 0

    def is_leaf(self):
        """
        whether current node is a leaf node
        """
        return self.winColor != None

    def calc_move_ucb(self, x):
        """
        get the node ucb score
        """
        child = self.children.get(x, None)
        if child != None and child.searchCnt > 0:
            return child.get_score() + math.sqrt(MCTS_UCB_C_ARG * math.log(self.searchCnt) / child.searchCnt)
        else:
            return math.sqrt(MCTS_UCB_C_ARG * math.log(self.searchCnt))

    def get_best_move(self):
        if len(self.children) == 0:
            return -1
        selectX = -1
        maxVisitCnt = 0
        #print self.children
        for x in self.children.keys():
            child = self.children.get(x, None)
            if child is None:
                continue
            if maxVisitCnt < child.searchCnt:
                selectX = x
                maxVisitCnt = child.searchCnt
        #print "selectX : " + str(selectX) + " mast visit " + str(maxVisitCnt)
        return selectX

class PureMctsEngine(object):
    """
    pure mcts gomoku search engine
    """

    def search(self, board, searchCnt):
        """
        search a best move and return (r, c)
        """
        root = PureMctsNode()
        for i in range(0, searchCnt):
            self.dfs(board, root)
        return board.location_to_move(root.get_best_move())

    def simulate(self, board):
        """
        simulate the board and tell the win color
        """
        winColor = None
        playCnt = 0
        availables = list(board.availables)
        random.shuffle(availables)
        for x in availables:
            board.check_game_over()
            if board.isGameOver:
                winColor = board.winColor
                break
            (r, c) = board.location_to_move(x)
            board.play(r, c, board.curPlayer)
            playCnt += 1
        for i in range(0, playCnt):
            board.undo()
        return winColor

    def dfs(self, board, node):
        """
        run one mcts search and return the win color
        """
        winColor = None
        isNewNode = False
        if node.is_new_node():
            # expanssion
            node.expand(board)
            isNewNode = True
        if node.is_leaf():
            winColor = node.winColor
        elif isNewNode:
            winColor = self.simulate(board)
        else:
            # selection
            selectX = -1
            selectUcb = -1
            for  x in node.children.keys():
                ucb = node.calc_move_ucb(x)
                if selectX == -1 or ucb > selectUcb:
                    selectX = x
                    selectUcb = ucb
            # play
            x = selectX
            (r, c) = board.location_to_move(x)
            board.play(r, c, board.curPlayer)
            child = node.children[x]
            if child is None:
                child = PureMctsNode()
                node.children[x] = child
            # dfs
            winColor = self.dfs(board, child)
            # undo play
            board.undo()
        # backpropagation
        node.searchCnt += 1
        if winColor == TIDE:
            node.tideCnt += 1
        elif winColor == board.curPlayer:
            node.winCnt += 1
        return winColor





