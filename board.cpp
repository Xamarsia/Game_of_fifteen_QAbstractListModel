#include "board.h"
#include <iostream>
#include <QRandomGenerator>
#include <QtMath>
#include <QDebug>

Board::Board(QObject *parent) : QAbstractListModel(parent) {
    m_roleNames[ValueRole] = "value";
    insertRows(0, rowsCount * columnsCount, QModelIndex());
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

bool Board::moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
              const QModelIndex &destinationParent, int destinationChild) {

    if(destinationChild >= sourceRow && destinationChild <= (sourceRow + count)) {
        return false;
    }

    if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild)) {
        // TODO Error processing
        return false;
    }

    QVector<Cell> buffer {};

    for(int i = 0; i < count; i++) {
        buffer.push_back(m_cells[sourceRow]);
        m_cells.remove(sourceRow);
    }

    if(destinationChild > sourceRow) {
        destinationChild = destinationChild - buffer.count();
    }

    while(!buffer.isEmpty()) {
        m_cells.insert(destinationChild, buffer.last());
        buffer.remove(buffer.count() - 1);
    }

    endMoveRows();
    return true;
}

bool Board::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count - 1);
    m_cells.insert(row, count, Cell{});
    endInsertRows();

    return true;
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

    for(int i = 0; !items.isEmpty(); i++ ) {
        int itemIndex = QRandomGenerator::global()->generate() % items.size();

        int value = items[itemIndex];
        if(value == 0) {
            emptyCellIndex = i;
        }
        setData(index(i, 0), value, ValueRole);
        items.remove(itemIndex);
    }
    if(userWon()) {
        mix();
    }
    if(!isSolvable()) {
        mix();
    }
}

void Board::moveEmptyItemDown() {
    if(emptyCellIndex + columnsCount < (rowsCount * columnsCount)) {
        moveRow(QModelIndex(), emptyCellIndex, QModelIndex(), emptyCellIndex + (columnsCount));
        moveRow(QModelIndex(), emptyCellIndex + columnsCount, QModelIndex(), emptyCellIndex);
        emptyCellIndex = emptyCellIndex + columnsCount;
    }
}

void Board::moveEmptyItemLeft() {
    if((emptyCellIndex) % columnsCount != 0) {
        moveRow(QModelIndex(), emptyCellIndex - 1, QModelIndex(), emptyCellIndex + 1);
        emptyCellIndex = emptyCellIndex - 1;
    }
}

void Board::moveEmptyItemRight() {
    if((emptyCellIndex+1) % columnsCount != 0) {
        moveRow(QModelIndex(), emptyCellIndex + 1, QModelIndex(), emptyCellIndex);
        emptyCellIndex = emptyCellIndex + 1;
    }
}

void Board::moveEmptyItemUp() {
    if(emptyCellIndex - columnsCount >= 0) {
        moveRow(QModelIndex(), emptyCellIndex, QModelIndex(), emptyCellIndex - (columnsCount - 1));
        moveRow(QModelIndex(), emptyCellIndex - columnsCount, QModelIndex(), emptyCellIndex + 1);
        emptyCellIndex = emptyCellIndex - columnsCount;
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
