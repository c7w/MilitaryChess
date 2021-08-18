#include "chesspiece.h"

// When generating the initalized list of chess pieces, just give a for-loop from 1 to 50
ChessPiece* ChessPiece::getChesePiece(int initID) {
    if (1 <= initID && initID <= 2) return new ChessPieceJunQi(initID);
    if (3 <= initID && initID <= 4) return new ChessPieceSiLing(initID);
    if (5 <= initID && initID <= 6) return new ChessPieceJunZhang(initID);
    if (7 <= initID && initID <= 10) return new ChessPieceShiZhang(initID);
    if (11 <= initID && initID <= 14) return new ChessPieceLvZhang(initID);
    if (15 <= initID && initID <= 18) return new ChessPieceTuanZhang(initID);
    if (19 <= initID && initID <= 22) return new ChessPieceYingZhang(initID);
    if (23 <= initID && initID <= 26) return new ChessPieceZhaDan(initID);
    if (27 <= initID && initID <= 32) return new ChessPieceLianZhang(initID);
    if (33 <= initID && initID <= 38) return new ChessPiecePaiZhang(initID);
    if (39 <= initID && initID <= 44) return new ChessPieceGongBing(initID);
    if (45 <= initID && initID <= 50) return new ChessPieceDiLei(initID);
    throw "Generate chess piece: ID OutOfBound Exception"; // Just to make IDE happy
}
