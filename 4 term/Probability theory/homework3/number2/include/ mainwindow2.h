#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QTableWidget>
#include <vector>
#include <memory>

#include "tree_model.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGenerateTreeClicked();
    void onRunExperimentClicked();
    void onRunMultipleExperimentsClicked();
    void onClearLogClicked();
    void onDistributionTypeChanged(int index);
    void onLevelChanged(int level);

private:
    void setupUI();
    void setupConnections();
    void updateProbabilityInputs(int level);
    void drawTree();
    void drawPath(const std::vector<PathPoint>& path);
    void logMessage(const QString& msg, bool isError = false);
    
    QSpinBox* branchingFactorSpin;
    QSpinBox* depthSpin;
    QSpinBox* experimentsCountSpin;
    QSpinBox* levelSpin;
    QComboBox* distributionTypeCombo;
    QWidget* probInputWidget;
    std::vector<QDoubleSpinBox*> moveProbInputs;
    QDoubleSpinBox* stopProbSpin;
    
    QPushButton* generateTreeBtn;
    QPushButton* runExperimentBtn;
    QPushButton* runMultipleBtn;
    QPushButton* clearLogBtn;
    
    QGraphicsView* treeView;
    QGraphicsScene* treeScene;
    QTextEdit* logText;
    QLabel* resultLabel;
    QTableWidget* statsTable;
    
    std::unique_ptr<TreeModel> model;
    std::vector<std::vector<double>> levelMoveProbabilities;
    std::vector<double> levelStopProbabilities;
};

#endif