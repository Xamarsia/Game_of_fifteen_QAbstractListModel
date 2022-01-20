#include "board.h"
#include <iostream>
#include <QRandomGenerator>
#include <QtMath>
#include <QDebug>

Board::Board(QObject *parent) : QAbstractListModel(parent) {
    m_roleNames[ValueRole] = "value";
    mix();
}

QHash<int, QByteArray> Board::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ValueRole] = "value";
    return roles;
}
int Board::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
       return m_cells.size();
}

QVariant Board::data(const QModelIndex &index, int role) const {
    int row = index.row();
    if(row < 0 || row >= m_cells.count()) {
        return QVariant();
    }

    const Cell &cell = m_cells[row];

    if (role == ValueRole) {
            return cell.value;
        }
    return QVariant();
}

bool Board::setData(const QModelIndex &index, const QVariant &value, int role) {
    int row = index.row();
    if(row < 0 || row >= m_cells.count()) {
        return false;
    }

    Cell &cell = m_cells[row];
    if (role == ValueRole) {
        cell.value = value.toInt();
        emit dataChanged(index, index, { role });
        return true;
    }
    return false;
}

bool Board::userWon() {
    if(emptyCellIndex == 0) {
        return 0;
    }
    for(int i = 0; i < (m_cells.count() - 2) ; i++) {
        if(m_cells[i].value > m_cells[i+1].value) {
            return 0;
        }
    }
    return 1;
}

int Board::getInversionsCount() {
    int invCount = 0;
    for(int i = 0; i < rowsCount * columnsCount ; i++) {
        if (m_cells[i].value) {
            for (int j = 0; j < i; ++j) {
                if(m_cells[i].value > m_cells[j].value) {
                    ++invCount;
                }
            }
        } else {
            invCount +=  i/rowsCount;
        }
    }
    return invCount;
}

bool Board::isSolvable() {
    for(int i = 0; i < (rowsCount * columnsCount - 1); i++) {
        int invCount = getInversionsCount();
        return !(invCount & 1);
    }
    return false;
}


void  Board::mix() {
    QVector<int> items {};
    for(int i = 0; i < rowsCount * columnsCount; i++) {
        items.push_back(i);
    }

    m_cells.clear();
    while(!items.isEmpty()) {
        int index = QRandomGenerator::global()->generate() % items.size();

        int value = items[index];
        if(value == 0) {
            emptyCellIndex = m_cells.count();
        }
        m_cells.append(Cell{value});

        items.remove(index);
    }
    if(userWon()) {
        mix();
    }
    if(!isSolvable()) {
        mix();
    }
    emit dataChanged(createIndex(0, 0), createIndex(rowsCount * columnsCount -1, 0), { ValueRole });
}

void Board::moveEmptyItemDown() {
    if(emptyCellIndex + columnsCount < (rowsCount * columnsCount)) {
        m_cells.move(emptyCellIndex, emptyCellIndex + (columnsCount-1));
        m_cells.move(emptyCellIndex + columnsCount, emptyCellIndex);
        emptyCellIndex = emptyCellIndex + columnsCount;

        emit dataChanged(createIndex(emptyCellIndex, 0), createIndex(emptyCellIndex, 0), { ValueRole });
        emit dataChanged(createIndex(emptyCellIndex - columnsCount, 0), createIndex(emptyCellIndex - columnsCount, 0), { ValueRole });
    }
}

void Board::moveEmptyItemLeft() {
    if((emptyCellIndex) % columnsCount != 0) {
        m_cells.move(emptyCellIndex - 1, emptyCellIndex);
        emptyCellIndex = emptyCellIndex - 1;

        emit dataChanged(createIndex(emptyCellIndex, 0), createIndex(emptyCellIndex, 0), { ValueRole });
        emit dataChanged(createIndex(emptyCellIndex + 1, 0), createIndex(emptyCellIndex + 1, 0), { ValueRole });
    }
}

void Board::moveEmptyItemRight() {
    if((emptyCellIndex+1) % columnsCount != 0) {
        m_cells.move(emptyCellIndex + 1, emptyCellIndex);
        emptyCellIndex = emptyCellIndex + 1;

        emit dataChanged(createIndex(emptyCellIndex, 0), createIndex(emptyCellIndex, 0), { ValueRole });
        emit dataChanged(createIndex(emptyCellIndex - 1, 0), createIndex(emptyCellIndex - 1, 0), { ValueRole });
    }
}

void Board::moveEmptyItemUp() {
    if(emptyCellIndex - columnsCount >= 0) {
        m_cells.move(emptyCellIndex, emptyCellIndex - (columnsCount-1));
        m_cells.move(emptyCellIndex - columnsCount, emptyCellIndex);
        emptyCellIndex = emptyCellIndex - columnsCount;

        emit dataChanged(createIndex(emptyCellIndex, 0), createIndex(emptyCellIndex, 0), { ValueRole });
        emit dataChanged(createIndex(emptyCellIndex + columnsCount, 0), createIndex(emptyCellIndex + columnsCount, 0), { ValueRole });
    }
}

int Board::getColumn(int index, int rowsCount, int columnsCount) {
    if(index > ((rowsCount*columnsCount)-1) || index < 0) {
        return -1;
    }
    return index % columnsCount;
}

int Board::getRow(int index, int rowsCount, int columnsCount) {
    if(index > ((rowsCount*columnsCount)-1) || index < 0) {
        return -1;
    }
    return (index / columnsCount) | 0;
}

bool Board::takeStep(int index){
    int rowDistance = getRow(index, rowsCount, columnsCount) - getRow(emptyCellIndex, rowsCount, columnsCount);
    int columnDistance = getColumn(index, rowsCount, columnsCount) - getColumn(emptyCellIndex, rowsCount, columnsCount);

    if( qFabs(rowDistance) == 1 && columnDistance == 0) {

        if(getRow(index, rowsCount, columnsCount) < getRow(emptyCellIndex, rowsCount, columnsCount)) {
            moveEmptyItemUp();
        } else {
            moveEmptyItemDown();
        }

        if(userWon() && emptyCellIndex != 0){
            emit win();
        }
        return 1;
    } else if( qFabs(columnDistance) == 1 && rowDistance == 0 ) {

        if(getColumn(index, rowsCount, columnsCount) < getColumn(emptyCellIndex, rowsCount, columnsCount)) {
            moveEmptyItemLeft();
        } else {
            moveEmptyItemRight();
        }

        if(userWon() && emptyCellIndex != 0){
            emit win();
        }
        return 1;
    }
    return 0;
}
