#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QObject>
#include "constants.h"

class ChessPiece : public QObject
{
    Q_OBJECT


public:
    // Settings
    const int InitializedID; // Fixed map between this init ID and chesspiece type.

    virtual ArmType getArmType() = 0;
    Faction getFaction() {return (Faction)(InitializedID % 2);}

    // If (chesspiece A canEat chesspiece B), then A is able to eat B.
    virtual int canEat(ChessPiece* B) = 0;

    // Ingame variables
    bool revealed = false;
    int currentPos; // [0, 60)
    bool eaten = false;
    bool isInCamp = false;


    explicit ChessPiece(int initID, QObject *parent = nullptr) : InitializedID(initID){}

    void setPos(int n) { currentPos = n; }
    int getPos() const {return currentPos;}

    QImage& getImage() {
        EnumChessPiece currPiece = (EnumChessPiece)(this->getArmType() * 2 + this->getFaction());
        return (this->revealed) ? Constants::getImage(currPiece) : Constants::getImage(Empty);
    }
};

// Generating code START here.

class ChessPieceDiLei : public ChessPiece
{
public:
    ArmType getArmType() override { return DiLei; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { };
        QVector<ArmType> canEatWhileSelfDestroy = { };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceGongBing : public ChessPiece
{
public:
    ArmType getArmType() override { return GongBing; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { DiLei, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = { GongBing, ZhaDan };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceJunQi : public ChessPiece
{
public:
    ArmType getArmType() override { return JunQi; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { };
        QVector<ArmType> canEatWhileSelfDestroy = { };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceJunZhang : public ChessPiece
{
public:
    ArmType getArmType() override { return JunZhang; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = { JunZhang, DiLei, ZhaDan };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceLianZhang : public ChessPiece
{
public:
    ArmType getArmType() override { return LianZhang; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { PaiZhang, GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = { LianZhang, DiLei, ZhaDan };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceLvZhang : public ChessPiece
{
public:
    ArmType getArmType() override { return LvZhang; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = { LvZhang, DiLei, ZhaDan };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPiecePaiZhang : public ChessPiece
{
public:
    ArmType getArmType() override { return PaiZhang; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = {GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = { PaiZhang, DiLei , ZhaDan};

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceShiZhang : public ChessPiece
{
public:
    ArmType getArmType() override { return ShiZhang; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = {LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = {ShiZhang, DiLei, ZhaDan };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceSiLing : public ChessPiece
{
public:
    ArmType getArmType() override { return SiLing; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = { SiLing, DiLei, ZhaDan };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceTuanZhang : public ChessPiece
{
public:
    ArmType getArmType() override { return TuanZhang; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { YingZhang, LianZhang, PaiZhang, GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = {  TuanZhang, DiLei, ZhaDan };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceYingZhang : public ChessPiece
{
public:
    ArmType getArmType() override { return YingZhang; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { LianZhang, PaiZhang, GongBing, JunQi };
        QVector<ArmType> canEatWhileSelfDestroy = { YingZhang, DiLei, ZhaDan};

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

class ChessPieceZhaDan : public ChessPiece
{
public:
    ArmType getArmType() override { return ZhaDan; }

    int canEat(ChessPiece* B) override {
        if (! B->revealed) return 0;
        if (B->isInCamp) return 0;
        if (B->getFaction() == this->getFaction()) return 0;

        // Judge if can eat... After generating codes, finish here.
        // ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei
        QVector<ArmType> canEat = { };
        QVector<ArmType> canEatWhileSelfDestroy = { ZhaDan, SiLing, JunZhang, ShiZhang, LvZhang, TuanZhang, YingZhang, LianZhang, PaiZhang, GongBing, JunQi, DiLei };

        // Return 1 if can eat
        // Return 2 if can eat while self-destroyed
        for (auto x : canEat) {
            if(B->getArmType() == x) return 1;
        }

        for (auto x : canEatWhileSelfDestroy) {
            if(B->getArmType() == x) return 2;
        }

        return 0;
    }
};

// Generating code END here.






#endif // CHESSPIECE_H
