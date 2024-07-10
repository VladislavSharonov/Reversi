#pragma once

#include <QAbstractItemModel>
#include <QColor>
#include <QPoint>
#include <QSize>
#include <QTimer>
#include <array>
#include <vector>

class ReversiModel : public QAbstractItemModel {
    Q_OBJECT

    Q_PROPERTY(bool isWhiteStep READ isWhiteStep NOTIFY isWhiteStepChanged)
    Q_PROPERTY(QString gameMessage READ getGameMessage NOTIFY gameMessageChanged)

public:
    enum struct CellState { Empty,
        Black,
        White };
    Q_ENUM(CellState)

    enum struct GameResult { Unknown,
        Draw,
        BlackWin,
        WhiteWin };
    Q_ENUM(GameResult)

    explicit ReversiModel(QObject* parent = nullptr);

    Q_INVOKABLE void restart();

    Q_INVOKABLE void trySetDisc(const QModelIndex& index);

    size_t countCellsWithState(CellState state) const;

    bool isWhiteStep() const { return m_isWhiteStep; }

    QString getGameMessage() const { return m_gameMessage; }

    void tryEndGame();

private:
    void updateGameMessage();

signals:
    void isWhiteStepChanged();

    void gameMessageChanged();

private:
    size_t getDistance(QPoint currentIndex, int xIncrement, int yIncrement) const;

    std::vector<QPoint> getStepDirection(const QModelIndex& targetIndex) const;

public:
    //---- QAbstractItemModel interface ----
    QModelIndex index(
        int row,
        int column,
        const QModelIndex& /*parent*/ = QModelIndex()) const override
    {
        return createIndex(row, column);
    };

    QModelIndex parent(const QModelIndex& /*index*/) const override
    {
        return QModelIndex();
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override { return 8; }

    int columnCount(const QModelIndex& = QModelIndex()) const override
    {
        return 8;
    }

    QVariant data(const QModelIndex& index, int role) const override;

    bool setData(const QModelIndex& /*index*/,
        const QVariant& /*value*/,
        int /*role*/ = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override
    {
        return { { Qt::DisplayRole, "display" },
            { Qt::BackgroundRole, "modelCellState" } };
    }

private:
    std::array<std::array<CellState, 8>, 8> m_field = {}; // ToDo single- demention array

    bool m_isWhiteStep = true;

    GameResult m_gameResult = GameResult::Unknown;

    QString m_gameMessage;
};
