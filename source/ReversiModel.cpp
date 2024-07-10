#include "ReversiModel.h"
#include <map>

#ifdef QT_DEBUG
#include <iostream>
#endif

ReversiModel::ReversiModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    restart();
    updateGameMessage();
}

void ReversiModel::updateGameMessage()
{
    if (m_gameResult == GameResult::Unknown) {
        // The game is on. Show how turn.
        m_gameMessage = m_isWhiteStep ? "White moves" : "Black moves";
    } else {
        std::map<GameResult, QString> messagesForGameResults = {
            { GameResult::Draw, "DRAW!" },
            { GameResult::BlackWin, "BLACK WON!" },
            { GameResult::WhiteWin, "WHITE WON!" },
        };

        m_gameMessage = messagesForGameResults[m_gameResult];
    }

    emit gameMessageChanged();
}

QVariant ReversiModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QString("%1, %2").arg(index.column()).arg(index.row());
        break;
    case Qt::BackgroundRole:
        return QVariant::fromValue(m_field[index.column()][index.row()]);
    default:
        break;
    }

    return QVariant();
}

bool ReversiModel::setData(const QModelIndex& /*index*/,
    const QVariant& /*value*/,
    int /*role*/)
{
    emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()),
        { Qt::BackgroundRole });
    return true;
}

void ReversiModel::trySetDisc(const QModelIndex& index)
{
    if (m_field[index.column()][index.row()] != CellState::Empty)
        return;

    std::vector<QPoint> directions = getStepDirection(index);

#ifdef QT_DEBUG
    std::cout << "---" << index.column() << "_" << index.row() << "---"
              << std::endl;
    for (auto& i : directions)
        std::cout << index.column() + i.x() << "\t" << index.row() + i.y()
                  << std::endl;
#endif

    bool isSet = false;
    for (auto& i : directions) {
        isSet = true;
        QPoint currentPosition(index.column(), index.row());

        do {
            m_field[currentPosition.x()][currentPosition.y()] = m_isWhiteStep ? CellState::White : CellState::Black;
            currentPosition.setX(currentPosition.x() + i.x());
            currentPosition.setY(currentPosition.y() + i.y());
        } while (m_field[currentPosition.x()][currentPosition.y()] != (m_isWhiteStep ? CellState::White : CellState::Black));
    }
    if (isSet) {
        m_isWhiteStep = !m_isWhiteStep;
        emit isWhiteStepChanged();
    }

    emit dataChanged(createIndex(0, 0), createIndex(columnCount(), rowCount()),
        { Qt::BackgroundRole });

    updateGameMessage();

    tryEndGame();
};

std::vector<QPoint> ReversiModel::getStepDirection(
    const QModelIndex& targetIndex) const
{
    QPoint maxDistanceIndex(0, 0);
    std::map<size_t, std::vector<QPoint>> distances = {};
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0)
                continue;
            size_t distance = getDistance(
                QPoint(targetIndex.column() + i, targetIndex.row() + j), i, j);
            distances[distance].emplace_back(i, j);
        }
    }

    if (distances.empty() || distances.rbegin()->first <= 1)
        return {};

    return distances[distances.rbegin()->first];
}

size_t ReversiModel::getDistance(QPoint currentIndex,
    int xIncrement,
    int yIncrement) const
{
    if (currentIndex.x() < 0 || currentIndex.x() >= columnCount() || currentIndex.y() < 0 || currentIndex.y() >= rowCount())
        return 0;

    if (m_field[currentIndex.x()][currentIndex.y()] == CellState::Empty)
        return 0;

    if (m_field[currentIndex.x()][currentIndex.y()] == (!m_isWhiteStep ? CellState::White : CellState::Black)) {
        size_t currentDistance = getDistance(
            QPoint(currentIndex.x() + xIncrement, currentIndex.y() + yIncrement),
            xIncrement, yIncrement);

        return currentDistance == 0 ? currentDistance : currentDistance + 1;
    }

    if (m_field[currentIndex.x()][currentIndex.y()] == (m_isWhiteStep ? CellState::White : CellState::Black))
        return 1;

    return 0;
}

void ReversiModel::restart()
{
    beginResetModel();
    for (int i = 0; i < m_field.size(); ++i) {
        for (int j = 0; j < m_field[0].size(); ++j)
            m_field[i][j] = CellState::Empty;
    }

    m_field[3][3] = CellState::White;
    m_field[4][4] = CellState::White;
    m_field[3][4] = CellState::Black;
    m_field[4][3] = CellState::Black;

    m_isWhiteStep = true;
    m_gameResult = GameResult::Unknown;
    updateGameMessage();

    endResetModel();
}

void ReversiModel::tryEndGame()
{
    if (countCellsWithState(CellState::Empty) != 0)
        return;

    size_t whiteCount = countCellsWithState(CellState::White);
    size_t blackCount = countCellsWithState(CellState::Black);

    m_gameResult = blackCount == whiteCount ? GameResult::Draw
        : blackCount > whiteCount           ? GameResult::BlackWin
                                            : GameResult::WhiteWin;

    updateGameMessage();
}

size_t ReversiModel::countCellsWithState(CellState state) const
{
    size_t count = 0;
    for (int i = 0; i < m_field.size(); ++i) {
        for (int j = 0; j < m_field[0].size(); ++j) {
            if (m_field[i][j] == state)
                ++count;
        }
    }

    return count;
}
