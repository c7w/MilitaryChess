#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QImage>
#include <QVector>

enum ArmType {
    DiLei,
    GongBing,
    JunQi,
    JunZhang,
    LianZhang,
    LvZhang,
    PaiZhang,
    ShiZhang,
    SiLing,
    TuanZhang,
    YingZhang,
    ZhaDan
};

enum Faction {Blue, Red, None};

enum EnumChessPiece {
    DiLeiB,
    DiLeiR,
    GongBingB,
    GongBingR,
    JunQiB,
    JunQiR,
    JunZhangB,
    JunZhangR,
    LianZhangB,
    LianZhangR,
    LvZhangB,
    LvZhangR,
    PaiZhangB,
    PaiZhangR,
    ShiZhangB,
    ShiZhangR,
    SiLingB,
    SiLingR,
    TuanZhangB,
    TuanZhangR,
    YingZhangB,
    YingZhangR,
    ZhaDanB,
    ZhaDanR,
    Unknown,
    Empty,
};


class Constants : public QObject
{
    Q_OBJECT

private:
    static QVector<QImage> Images;
    static QVector<QVector<int>> Edges;


public:
    explicit Constants(QObject *parent = nullptr);

    static QVector<int> Camp; // x & x+25

    static void GenerateEdge();
    static int hasEdge(int a, int b);
    static QImage& getImage(EnumChessPiece piece) { return Images[(int)piece]; }



};





#endif // CONSTANTS_H
