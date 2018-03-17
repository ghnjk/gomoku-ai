from chess_board import *
import re

def sgf_to_board(sgfStr):
    """
    parse sgf board string to ChessBoard
    """
    pattern = re.compile("[BW]\[[0-9a-f]{2}\]")
    chess = ChessBoard()
    for step in sgfStr.split(";"):
        step = step.strip()
        if pattern.match(step) is None:
            continue
        row = int(step[2], 16)
        col = int(step[3], 16)
        color = step[0]
        chess.board[row][col] = color    
    chess.reset(chess.board)
    return chess