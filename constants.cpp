#include "constants.h"


Constants::Constants(QObject *parent) : QObject(parent)
{

}


// Initialize images
QVector<QImage> Constants::Images = {
    QImage(":/assets/image/chesspieces/DiLeiB.png"),
    QImage(":/assets/image/chesspieces/DiLeiR.png"),
    QImage(":/assets/image/chesspieces/GongBingB.png"),
    QImage(":/assets/image/chesspieces/GongBingR.png"),
    QImage(":/assets/image/chesspieces/JunQiB.png"),
    QImage(":/assets/image/chesspieces/JunQiR.png"),
    QImage(":/assets/image/chesspieces/JunZhangB.png"),
    QImage(":/assets/image/chesspieces/JunZhangR.png"),
    QImage(":/assets/image/chesspieces/LianZhangB.png"),
    QImage(":/assets/image/chesspieces/LianZhangR.png"),
    QImage(":/assets/image/chesspieces/LvZhangB.png"),
    QImage(":/assets/image/chesspieces/LvZhangR.png"),
    QImage(":/assets/image/chesspieces/PaiZhangB.png"),
    QImage(":/assets/image/chesspieces/PaiZhangR.png"),
    QImage(":/assets/image/chesspieces/ShiZhangB.png"),
    QImage(":/assets/image/chesspieces/ShiZhangR.png"),
    QImage(":/assets/image/chesspieces/SiLingB.png"),
    QImage(":/assets/image/chesspieces/SiLingR.png"),
    QImage(":/assets/image/chesspieces/TuanZhangB.png"),
    QImage(":/assets/image/chesspieces/TuanZhangR.png"),
    QImage(":/assets/image/chesspieces/YingZhangB.png"),
    QImage(":/assets/image/chesspieces/YingZhangR.png"),
    QImage(":/assets/image/chesspieces/ZhaDanB.png"),
    QImage(":/assets/image/chesspieces/ZhaDanR.png"),
    QImage(":/assets/image/chess_unknown.png"),
    QImage(),
};
