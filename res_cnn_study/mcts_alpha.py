#!/usr/bin/python
# -*- coding: UTF-8 -*- 
from gomoku_chess import *
from gomoku_model import *
import numpy as np


def softmax(x):
    probs = np.exp(x - np.max(x))
    probs /= np.sum(probs)
    return probs


class AlphaMctsNode(object):
    """
    alpha zero mcts search node
    """
    
    def __init__(self, prioP = None):
        self.searchCnt = 0
        self.children = {} # map[moveX] = childNode
        self.winColor = None
        self.totalValue = 0.0 # 从该节点搜索过的所有子节点对于本节点获得的值得总和
        self.prioP = prioP # 由策略模型估算出来的先验概率


    def expand(self, board, moveRates):
        """
        expand node for mcts
        """
        board.check_game_over()
        if board.isGameOver:
            self.winColor = board.winColor
        else:
            self.winColor = None
            for (x, prioP) in moveRates:
                if x not in self.children:
                    self.children[x] = AlphaMctsNode(prioP)

    def select(self, cPuct):
        """
        通过 q + u 公式选择下一个扩展的子节点
        cPuct (0, inf) 控制对先验概率的依赖程度， 值越大，越依赖网络计算出来的概率
        return : 下一步的走法x
        """
        selectX = None
        selectValue = 0
        for (x, child) in selectX.children:
            v = self.calc_move_q_ucb(child, cPuct)
            if selectX is None or v > selectValue:
                selectX = x
                selectValue = v
        return selectX

    def is_new_node(self):
        """
        whether it is a new node
        """
        return self.searchCnt == 0

    def is_game_over(self):
        """
        whether current node is a leaf node
        """
        return self.winColor != None

    def calc_move_q_ucb(self, child, cPuct):
        """
        get the node q + ucb score
        """
        u = cPuct * child.prioP * np.sqrt(self.searchCnt) / (1 + child.searchCnt)
        if child.searchCnt == 0:
            q = 0.0
        else:
            q = - child.totalValue / float(child.searchCnt)
        return q + u


    def get_all_moves(self, expandTemerature):
        """
        return all moves and the rate
         (moves, rates)
        """
        moves = []
        visits = []
        for (x, child) in self.children.iteritems():
            moves.append(x)
            visits.append(child.searchCnt)
        rates = softmax(1.0/expandTemerature * np.log(np.array(visits) + 1e-10))
        return (moves, rates)

class AlphaZeroMcts(object):
    """
    mcts for aplha zero
    """

    def __init__(self, mctsPlayout, policyModel, cPuct):
        self.mctsPlayout = mctsPlayout
        self.root = None
        self.policyModel = policyModel
        self.cPuct = cPuct

    def reset_root(self):
        self.root = None

    def search_all_movs(self, board, expandTemerature):
        """
        执行一轮mcts搜索，每轮扩展mctsPlayout次
        board: 当前棋局
        expandTemerature: 控制探索广度
        返回最好的走法和所有可能走法和对应的概率列表(moves, rates)
        """
        if self.root is None:
            self.root = AlphaMctsNode()
        for i in range(0, self.mctsPlayout):
            self.dfs(board, root)
        return self.root.get_all_moves(expandTemerature)

    def dfs(self, board, node):
        value = 0
        if node.is_game_over():
            if node.winColor == TIDE:
                value = 0.0
            elif node.winColor == board.curPlayer:
                value = 1.0
            else:
                value = -1.0
        elif node.is_new_node():
            (value, moveRates) = self.policyModel.gen_policy(board)
            # expanssion
            node.expand(board, moveRates)
        else:
            # selection
            x = node.select(self.cPuct)
            # play
            (r, c) = board.location_to_move(x)
            board.play(r, c, board.curPlayer)
            child = node.children[x]
            if child is None:
                child = PureMctsNode()
                node.children[x] = child
            # dfs
            value = self.dfs(board, child)
            # undo play
            board.undo()
        # backpropagation
        node.searchCnt += 1
        node.totalValue += value
        return -value

    def update_root(self, x):
        """
        选择走法x的子节点作为root，以便后续复用mcts 树
        """
        node = self.root.children.get(x, None)
        if node is None:
            raise Exception("cannot find child node for " + str(x))
        self.root = node

class AlphaZeroEngine(object):
    """
    alpha zero mcts search engine
    """

    def __init__(self, rowCount, colCount, mctsPlayout = 1000, policyModel = None, isSelfPlay = True, cPuct = 5):
        """
        rowCount, colCount 指定棋盘大小
        mctsPlayout 指定每次mcts搜索时，扩展的次数
        policyModel 为训练的评估模型
        cPuct (0, inf)在mcts选择子节点时， 控制对由策略模型评估出来的先验概率依赖程度
        """
        self.rowCount = rowCount
        self.colCount = colCount
        self.isSelfPlay = isSelfPlay
        if policyModel is None:
            self.policyModel = GomokuModel(rowCount, colCount)
        else:
            self.policyModel = policyModel
        self.mcts = AlphaZeroMcts(mctsPlayout, self.policyModel, cPuct)

    def search_moves(self, board, expandTemerature = 1e-3):
        """
        搜索当前局面下的走法，给出最佳走法和其他走法的可能性
        expandTemerature: 决定在self-play， 用于控制搜索宽度的参数 取值区间(0,1]
        return (bestMoveX, moveRates)
        """
        allMoves = board.availables
        moveRates = np.zeros(self.rowCount * self.colCount)
        bestMoveX = None
        if len(allMoves) > 0:
            (moves, rates) = self.mcts.search_all_movs(board, expandTemerature)
            moveRates[list(moves)] = rates
            if self.isSelfPlay:
                altRates = 0.75 * rates + 0.25 * np.random.dirichlet(0.3 * np.ones(len(rates)))
                bestMoveX = np.random.choice(moves, p = altRates)
                # 重新设置mcts的头， 等待下次下棋时， 复用mcts树
                self.mcts.update_root(bestMoveX)
            else:
                bestMoveX = np.random.choice(moves, rates)
                self.mcts.reset_root()
        return (bestMoveX, moveRates)





