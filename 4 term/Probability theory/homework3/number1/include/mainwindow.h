#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <vector>
#include <memory>

#include "cluster_analyzer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private slots:
    void onGenerateClicked();
    void onDistributionTypeChanged(int index);
    void onSaveResults();
    void onClearLog();

private:
    void setupUI();
    void setupConnections();
    void updateProbabilitiesInput(int alphabetSize);
    void displayResults();
    void logMessage(const QString& msg, bool isError = false);
    
    QSpinBox* clusterCountSpin;
    QSpinBox* clusterSizeSpin;
    QSpinBox* alphabetSizeSpin;
    QLineEdit* patternEdit;
    QComboBox* distributionCombo;
    QWidget* probInputWidget;
    std::vector<QDoubleSpinBox*> probInputs;
    QSpinBox* dValueSpin;
    QPushButton* generateBtn;
    QPushButton* saveBtn;
    QPushButton* clearLogBtn;
    QTableWidget* resultsTable;
    QTextEdit* logText;
    QLabel* allConnectableLabel;
    QLabel* dConnectableLabel;
    QLabel* allNonConnectableLabel;
    
    std::unique_ptr<ClusterAnalyzer> analyzer;
    std::vector<QString> generatedClusters;
};

#endif