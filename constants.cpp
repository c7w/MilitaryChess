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

QVector<QVector<int>> Constants::Edges = {};
QVector<int> Constants::Camp = {11, 13, 17, 21, 23};

QVector<int> GetEdge(int a, int b, int type) {
    if ( a > b ) return QVector<int>({b, a, type});
    else return QVector<int>({a, b, type});
}

void Constants::GenerateEdge() {
    int delta[] = {5,6,1,-4,-5,-6,-1,4};
    // 8 Direction
    int camp[] = {11, 13, 21, 23};
    for(int i = 0; i < 8; i++){
        for(auto x : camp) {
            Edges.push_back(GetEdge(x, x+delta[i], 1));
            Edges.push_back(GetEdge(x+25, x+25+delta[i], 1));
        }
    }

    int line[] = {0, 15, 40, 55};
    for (auto x : line) {
        for(int i = x; i < x+4; ++i) Edges.push_back(GetEdge(i, i+1, 1));
    }

    for(int i = 0; i < 5; ++i) Edges.push_back(GetEdge(i, i+5, 1));
    for(int i = 50; i < 55; ++i) Edges.push_back(GetEdge(i, i+5, 1));

    for(int i = 7; i <= 22; i+=5) Edges.push_back(GetEdge(i, i+5, 1));
    for(int i = 32; i <= 47; i+=5) Edges.push_back(GetEdge(i, i+5, 1));

    int line2[] = {5, 25, 30, 50};
    for (auto x : line2) {
        for(int i = x; i < x+4; ++i) Edges.push_back(GetEdge(i, i+1, 2));
    }

    for(int i = 5; i <= 45; i+=5) Edges.push_back(GetEdge(i, i+5, 2));
    for(int i = 9; i <= 49; i+=5) Edges.push_back(GetEdge(i, i+5, 2));

    Edges.push_back(GetEdge(27, 32, 2));

}

int Constants::hasEdge(int a, int b) {
    if ( a > b) {
        int temp = a; a = b; b = temp;
    }
    if ( a < 0 || b > 59 ) return 0;
    for (auto& x : Edges){
        if(x[0] == a && x[1] == b) return x[2];
    }
    return 0;
}
