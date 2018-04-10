#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import re

TIDE = 2
class GomokuBoard(object):
    """
    五子棋，棋盘
    """

    def __init__(self, rowCount, colCount, nInRow):
        """
        五子棋棋盘
        rowCount 行数
        colCount 列数
        nInRow 在一行能赢得棋子数目，五子棋为5 四子棋为4
        """
        self.rowCount = rowCount
        self.colCount = colCount
        self.states = {}
        self.curPlayer = 0
        self.availables = set(range(0, self.rowCount * self.colCount))
        self.moveHistory = []
        self.playerName = ['B', 'W', 'T']
        self.isGameOver = False
        self.winColor = None
        self.nInRow = nInRow

    def get_cur_step_no(self):
        return len(self.moveHistory)

    def move_to_location(self, r, c):
        """
        convert move(r,c) to location x
        """
        return r * self.colCount + c

    def location_to_move(self, x):
        """
        convert location x to move (r, c)
        """
        r = x / self.colCount
        c = x % self.colCount
        return (r, c)

    def play(self, r, c, color):
        """
        在(r, c)处下子
        """
        x = self.move_to_location(r, c)
        if self.curPlayer != color:
            raise Exception("invalid move. current player is " + self.playerName[self.curPlayer])
        if self.states.has_key(x):
            raise Exception("invalid move (%d, %c) already has chess" % (r, c))
        self.availables.remove(x)
        self.states[x] = color
        self.moveHistory.append( (x, color) )
        self.curPlayer = 1 - self.curPlayer

    def load_from_sgf(self, sgfStr):
        """
        load chess from sgf string like B[a9];W[99]
        """
        pattern = re.compile("[BW]\[[0-9a-f]{2}\]")
        for step in sgfStr.split(";"):
            step = step.strip()
            if pattern.match(step) is None:
                continue
            row = int(step[2], 16)
            col = int(step[3], 16)
            color = step[0]
            for i in range(0, len(self.playerName)):
                if color == self.playerName[i]:
                    color = i
            self.play(row, col, color)

    def to_sgf_str(self):
        """
        convert chess to sgf string
        """
        s = ""
        for step in self.moveHistory:
            x = step[0]
            color = step[1]
            (r, c) = self.location_to_move(x)
            s += "%s[%s%s];" % ( self.playerName[color]
                , hex(r)[2:3]
                , hex(c)[2:3])
        return s

    def __str__(self):
        """
        transfer to chess board for pringting
        """
        res = "  "
        for c in range(0, self.colCount):
            res += hex(c)[2:]
            res += " "
        res += '\n'
        if len(self.moveHistory) > 0:
            lastMove = self.moveHistory[-1]
            lastMove = self.location_to_move(lastMove[0])
        else:
            lastMove = (-1, -1)
        for r in range(0, self.rowCount):
            res += hex(r)[2:]
            if (r, 0) == lastMove:
                res += "("
            else:
                res += " "
            for c in range(0, self.colCount):
                i = self.move_to_location(r, c)
                if self.states.has_key(i):
                    color = self.playerName[self.states[i]]
                else:
                    color = '.'
                res += color
                if (r, c) == lastMove:
                    res += ")"
                elif (r, c + 1) == lastMove:
                    res += "("
                else:
                    res += " "
            res += '\n'
        return res

    def check_game_over(self):
        """
        check if game is over
        """
        dr = [-1, -1, 0, 1]
        dc = [0, 1, 1, 1]
        if len(self.moveHistory) == 0:
            self.winColor = None
            self.isGameOver = False
            return False
        (x, color) = self.moveHistory[-1]
        (r, c) = self.location_to_move(x)
        for i in range(0, len(dr)):
            cnt = 1
            nr = r
            nc = c
            for j in range(0, 4):
                nr += dr[i]
                nc += dc[i]
                if nr < 0 or nr >= self.rowCount or nc < 0 or nc >= self.colCount:
                    break
                nx = self.move_to_location(nr, nc)
                ncolor = self.states.get(nx, -1)
                if color != ncolor:
                    break
                cnt += 1
            nr = r
            nc = c
            for j in range(0, 4):
                nr -= dr[i]
                nc -= dc[i]
                if nr < 0 or nr >= self.rowCount or nc < 0 or nc >= self.colCount:
                    break
                nx = self.move_to_location(nr, nc)
                ncolor = self.states.get(nx, -1)
                if color != ncolor:
                    break
                cnt += 1
            if cnt >= self.nInRow:
                self.isGameOver = True
                self.winColor = color
                return self.isGameOver
        if len(self.availables) == 0:
            self.winColor = TIDE
            self.isGameOver = True
        else:
            self.winColor = None
            self.isGameOver = False
        return self.isGameOver

    def is_valid_move(self, r, c):
        """
        check if r,c is a valid move
        """
        if r < 0 or c < 0 or r >= self.rowCount or c >= self.colCount:
            return False
        x = self.move_to_location(r, c)
        return x in self.availables

    def undo(self):
        if len(self.moveHistory) == 0:
            return False
        (x, color) = move = self.moveHistory[-1]
        (r, c) = self.location_to_move(x)
        self.availables.add(x)
        del self.states[x]
        self.curPlayer = color
        self.moveHistory.remove(move)
        return True






