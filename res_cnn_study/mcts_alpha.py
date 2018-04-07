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
        for (x, child) in self.children.iteritems():
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
        #u = np.log(self.searchCnt) / (1 + child.searchCnt)
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
        rates = []
        visits = []
        for (x, child) in self.children.iteritems():
            moves.append(x)
            visits.append(child.searchCnt)
            rates.append(child.searchCnt / float(self.searchCnt))
        # print "all child value: "
        # for r in range(8):
        #     s = ""
        #     for c in range(8):
        #         x = r * 8 + c;
        #         if self.children.has_key(x) and self.children[x].searchCnt > 0:
        #             if self.children[x].totalValue >= 0:
        #                 s += " "
        #                 s += "%1.2lf " % (self.children[x].totalValue / float(self.children[x].searchCnt))
        #             else:
        #                 s += "%1.2lf " % (self.children[x].totalValue / float(self.children[x].searchCnt))
        #         else:
        #             s += " 0.00 "
        #     print s
        rates = softmax(1.0/expandTemerature * np.log(np.array(visits) + 1e-10))
        return (moves, np.array(rates))

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
            self.dfs(board, self.root)
        #self.dfs_print(board, self.root)
        return self.root.get_all_moves(expandTemerature)

    def dfs_print(self, board, node):
        print str(board)
        print "node.totalValue: " + str(node.totalValue)
        print "node.searchCnt: " + str(node.searchCnt)
        for (x, child) in node.children.iteritems():
            if child.searchCnt <= 4:
                continue
            (r, c) = board.location_to_move(x)
            board.play(r, c, board.curPlayer)
            self.dfs_print(board, child)
            board.undo()

    def dfs(self, board, node):
        value = 0
        needDfs = True
        if node.is_new_node():
            needDfs = False
            (value, moveRates) = self.policyModel.gen_policy(board)
            #print "value: " + str(value)
            # expanssion
            node.expand(board, moveRates)
        if node.is_game_over():
            if node.winColor == TIDE:
                value = 0.0
            elif node.winColor == board.curPlayer:
                value = 1.0
            else:
                value = -1.0
            #print value
        elif needDfs:
            # selection
            x = node.select(self.cPuct)
            # play
            (r, c) = board.location_to_move(x)
            board.play(r, c, board.curPlayer)
            child = node.children[x]
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
        self.randSelectStepNo = int(self.rowCount * self.colCount / 6)
        if policyModel is None:
            self.policyModel = GomokuModel(rowCount, colCount)
            self.policyModel.build_model()
        else:
            self.policyModel = policyModel
        self.mcts = AlphaZeroMcts(mctsPlayout, self.policyModel, cPuct)

    def reset(self):
        """
        棋局开始时，需要reset
        """
        self.mcts.reset_root()

    def search_moves(self, board, expandTemerature = 0.75):
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
            # print "move rates: "
            # for r in range(8):
            #     s = ""
            #     for c in range(8):
            #         s += "%1.3lf " % (moveRates[ r * 8 + c])
            #     print s
            if self.isSelfPlay and board.get_cur_step_no() <= self.randSelectStepNo:
                altRates = 0.75 * rates + 0.25 * np.random.dirichlet(0.3 * np.ones(len(rates)))
                bestMoveX = np.random.choice(moves, p = altRates)
                # 重新设置mcts的头， 等待下次下棋时， 复用mcts树
                self.mcts.update_root(bestMoveX)
            elif self.isSelfPlay:
                #bestMoveX = np.random.choice(moves, p = rates)
                altRates = 0.75 * rates + 0.25 * np.random.dirichlet(0.3 * np.ones(len(rates)))
                bestMoveX = np.random.choice(moves, p = altRates)
                # bestMoveX = None
                # maxRate = 0
                # for i in range(0, len(moves)):
                #     if bestMoveX is None or maxRate < rates[i]:
                #         bestMoveX = moves[i]
                #         maxRate = rates[i]
                # 重新设置mcts的头， 等待下次下棋时， 复用mcts树
                self.mcts.update_root(bestMoveX)
            else:
                bestMoveX = None
                maxRate = 0
                for i in range(0, len(moves)):
                    if bestMoveX is None or maxRate < rates[i]:
                        bestMoveX = moves[i]
                        maxRate = rates[i]
                self.mcts.reset_root()
        return (bestMoveX, moveRates)





