#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>

class Constants : public QObject
{
    Q_OBJECT
public:
    explicit Constants(QObject *parent = nullptr);

    enum ChessPiece {
        Empty,
        Unknown,
    };


};

#endif // CONSTANTS_H
