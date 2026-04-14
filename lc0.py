#!/usr/bin/env python3
import re
import sys
import chess.pgn

score_re = re.compile(r'([+-]?\d+(?:\.\d+)?|#-?\d+)(?:/\d+)?')

def extract_score(comment: str):
    if not comment:
        return None
    comment = comment.replace("−", "-").strip()
    m = score_re.search(comment)
    return m.group(1) if m else None

def main(path):
    with open(path, "r", encoding="utf-8", errors="replace") as pgn:
        while True:
            game = chess.pgn.read_game(pgn)
            if game is None:
                break

            board = game.board()
            node = game

            while node.variations:
                node = node.variations[0]
                board.push(node.move)

                score = extract_score(node.comment)
                if score is None:
                    continue

                turn = "w" if board.turn == chess.WHITE else "b"
                print(f"{board.board_fen()} {turn} {score}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} input.pgn", file=sys.stderr)
        sys.exit(1)
    main(sys.argv[1])