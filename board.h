#pragma once
#include <QAbstractListModel>

class Board : public QAbstractListModel {
     Q_OBJECT

public:
    Board(QObject *parent = 0);

    enum BoardRoles {
        ValueRole = Qt::UserRole + 1,
    };

    struct Cell {
        int value {};
    };

    Q_INVOKABLE void mix();
    Q_INVOKABLE bool takeStep(const int index);
    Q_INVOKABLE bool userWon();

    Q_INVOKABLE int row() { return rowsCount; }
    Q_INVOKABLE int column() { return columnsCount; }

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                          const QModelIndex &destinationParent, int destinationChild) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

signals:
    void win();

private:

    int emptyCellIndex = -1;
    int rowsCount {4};
    int columnsCount {4};

    bool isSolvable();

    int getInversionsCount();
    int getRow(int index, int rowsCount, int columnsCount);
    int getColumn(int index, int rowsCount, int columnsCount);

    void moveEmptyItemDown();
    void moveEmptyItemLeft();
    void moveEmptyItemRight();
    void moveEmptyItemUp();

    QHash<int, QByteArray> m_roleNames;
    QVector<Cell> m_cells;
};



