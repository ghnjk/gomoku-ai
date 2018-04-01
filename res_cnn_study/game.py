#!/usr/bin/python
# -*- coding: UTF-8 -*- 
from gomoku_chess import *
from player import *

class GomokuGame(object):
    """
    terminal gomoku game
    """

    def __init__(self, players = [], rowCount = 15, colCount = 15, nInRow = 5):
        """
        init the game
        """
        self.players = players
        self.rowCount = rowCount
        self.colCount = colCount
        self.nInRow = nInRow
        self.create_board()

    def create_board(self):
        """
        create new board
        """
        self.board = GomokuBoard(self.rowCount, self.colCount, self.nInRow)
        for i in range(0, len(self.players)):
            player = self.players[i]
            player.init_player(self.board, i)
        return self.board

    def run(self):
        """
        run the game
        """
        board = self.board
        while True:
            print str(board)
            board.check_game_over()
            if board.isGameOver:
                print "game over: winColor %s" % (board.playerName[board.winColor])
                break
            player = self.players[board.curPlayer]
            #print "next player: " + str(board.curPlayer)
            x = player.gen_next_move(board)
            if x == "undo":
                board.undo()
            elif x == "to_sgf":
                print board.to_sgf_str()
            elif x == "from_sgf":
                line = raw_input("input sgf string: ")
                board = self.create_board()
                board.load_from_sgf(line.strip())
            elif x == "end":
                break
            elif isinstance(x, tuple):
                (r, c) = x
                board.play(r, c, board.curPlayer)
            else:
                print "invalid move: " + x

if __name__ == '__main__':
    game = GomokuGame(players = [
            AlphaZeroPlayer(200)
            , HumanPlayer()
        ], rowCount = 6
        , colCount = 6
        , nInRow = 4
        )
    game.run()
    
