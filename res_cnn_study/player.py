#!/usr/bin/python
# -*- coding: UTF-8 -*- 


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
        from mcts_pure import PureMctsEngine
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

