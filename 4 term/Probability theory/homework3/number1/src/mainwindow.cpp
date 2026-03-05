#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QLabel>
#include <QDoubleSpinBox>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setupConnections();
    logMessage("Application started");
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    setWindowTitle("Cluster Connectability Analyzer");
    setMinimumSize(1000, 700);
    
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    QGroupBox* inputGroup = new QGroupBox("Input Parameters");
    QFormLayout* inputLayout = new QFormLayout(inputGroup);
    
    clusterCountSpin = new QSpinBox();
    clusterCountSpin->setRange(2, 1000);
    clusterCountSpin->setValue(100);
    clusterCountSpin->setToolTip("Number of clusters to generate (M)");
    inputLayout->addRow("Number of clusters (M):", clusterCountSpin);
    
    clusterSizeSpin = new QSpinBox();
    clusterSizeSpin->setRange(1, 100);
    clusterSizeSpin->setValue(5);
    clusterSizeSpin->setToolTip("Number of cells per cluster (n)");
    inputLayout->addRow("Cluster size (n):", clusterSizeSpin);
    
    alphabetSizeSpin = new QSpinBox();
    alphabetSizeSpin->setRange(2, 10);
    alphabetSizeSpin->setValue(2);
    alphabetSizeSpin->setToolTip("Alphabet size (r)");
    inputLayout->addRow("Alphabet size (r):", alphabetSizeSpin);
    
    patternEdit = new QLineEdit("101");
    patternEdit->setToolTip("k-pattern to check");
    inputLayout->addRow("Pattern:", patternEdit);
    
    distributionCombo = new QComboBox();
    distributionCombo->addItem("Uniform", static_cast<int>(ClusterAnalyzer::DistributionParams::Type::UNIFORM));
    distributionCombo->addItem("Custom", static_cast<int>(ClusterAnalyzer::DistributionParams::Type::CUSTOM));
    distributionCombo->setToolTip("Symbol distribution type");
    inputLayout->addRow("Distribution:", distributionCombo);
    
    probInputWidget = new QWidget();
    QHBoxLayout* probLayout = new QHBoxLayout(probInputWidget);
    probLayout->setContentsMargins(0, 0, 0, 0);
    inputLayout->addRow("Probabilities:", probInputWidget);
    
    dValueSpin = new QSpinBox();
    dValueSpin->setRange(0, 1000);
    dValueSpin->setValue(0);
    dValueSpin->setToolTip("D value for probability of D connectable clusters");
    inputLayout->addRow("D value:", dValueSpin);
    
    mainLayout->addWidget(inputGroup);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    generateBtn = new QPushButton("Generate and Analyze");
    saveBtn = new QPushButton("Save Results");
    clearLogBtn = new QPushButton("Clear Log");
    
    buttonLayout->addWidget(generateBtn);
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(clearLogBtn);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    QGroupBox* resultsGroup = new QGroupBox("Results");
    QVBoxLayout* resultsLayout = new QVBoxLayout(resultsGroup);
    
    QHBoxLayout* probLayout2 = new QHBoxLayout();
    allConnectableLabel = new QLabel("P(all connectable): -");
    dConnectableLabel = new QLabel("P(D connectable): -");
    allNonConnectableLabel = new QLabel("P(all non-connectable): -");
    
    probLayout2->addWidget(allConnectableLabel);
    probLayout2->addWidget(dConnectableLabel);
    probLayout2->addWidget(allNonConnectableLabel);
    probLayout2->addStretch();
    
    resultsLayout->addLayout(probLayout2);
    
    resultsTable = new QTableWidget();
    resultsTable->setColumnCount(3);
    resultsTable->setHorizontalHeaderLabels({"Cluster #", "Value", "Connectable with next"});
    resultsTable->horizontalHeader()->setStretchLastSection(true);
    resultsLayout->addWidget(resultsTable);
    
    mainLayout->addWidget(resultsGroup);

    QGroupBox* logGroup = new QGroupBox("Log");
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    logText = new QTextEdit();
    logText->setReadOnly(true);
    logLayout->addWidget(logText);
    
    mainLayout->addWidget(logGroup);
    
    updateProbabilitiesInput(alphabetSizeSpin->value());
}

void MainWindow::setupConnections() {
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveResults);
    connect(clearLogBtn, &QPushButton::clicked, this, &MainWindow::onClearLog);
    connect(distributionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onDistributionTypeChanged);
    connect(alphabetSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updateProbabilitiesInput);
}

void MainWindow::updateProbabilitiesInput(int alphabetSize) {
    for (auto* input : probInputs) {
        delete input;
    }
    probInputs.clear();
    
    QLayout* layout = probInputWidget->layout();
    while (layout->count()) {
        delete layout->takeAt(0)->widget();
    }
    
    QHBoxLayout* probLayout = qobject_cast<QHBoxLayout*>(layout);
    for (int i = 0; i < alphabetSize; ++i) {
        QDoubleSpinBox* spin = new QDoubleSpinBox();
        spin->setRange(0.0, 1.0);
        spin->setSingleStep(0.1);
        spin->setDecimals(3);
        spin->setValue(1.0 / alphabetSize);
        spin->setToolTip(QString("Probability for symbol '%1'").arg('0' + i));
        
        QLabel* label = new QLabel(QString("'%1':").arg('0' + i));
        QHBoxLayout* itemLayout = new QHBoxLayout();
        itemLayout->addWidget(label);
        itemLayout->addWidget(spin);
        
        probLayout->addLayout(itemLayout);
        probInputs.push_back(spin);
    }
    
    probLayout->addStretch();
}

void MainWindow::onDistributionTypeChanged(int index) {
    bool isCustom = distributionCombo->currentData().toInt() == 
                   static_cast<int>(ClusterAnalyzer::DistributionParams::Type::CUSTOM);
    
    for (auto* input : probInputs) {
        input->setEnabled(isCustom);
    }
}

void MainWindow::onGenerateClicked() {
    try {
        logMessage("Starting cluster generation and analysis...");
        
        QString pattern = patternEdit->text();
        if (pattern.isEmpty()) {
            throw std::invalid_argument("Pattern cannot be empty");
        }
        
        int k = pattern.length();
        if (k < 2) {
            throw std::invalid_argument("Pattern length must be at least 2");
        }
 
        ClusterAnalyzer::DistributionParams params;
        params.type = static_cast<ClusterAnalyzer::DistributionParams::Type>(
            distributionCombo->currentData().toInt());
        
        if (params.type == ClusterAnalyzer::DistributionParams::Type::CUSTOM) {
            params.probabilities.reserve(probInputs.size());
            for (auto* spin : probInputs) {
                params.probabilities.push_back(spin->value());
            }
        }

        analyzer = std::make_unique<ClusterAnalyzer>(
            clusterSizeSpin->value(),
            alphabetSizeSpin->value(),
            pattern.toStdString(),
            params
        );
        
        analyzer->analyzeSeries(clusterCountSpin->value());
        
        int totalPairs = clusterCountSpin->value() * (clusterCountSpin->value() - 1) / 2;
        dValueSpin->setMaximum(totalPairs);

        displayResults();
        
        logMessage("Analysis completed successfully");
        
    } catch (const std::exception& e) {
        logMessage(QString("Error: %1").arg(e.what()), true);
    }
}

void MainWindow::displayResults() {
    if (!analyzer) return;
    
    const auto& clusters = analyzer->getGeneratedClusters();
    const auto& matrix = analyzer->getConnectabilityMatrix();
    int M = clusters.size();
    
    allConnectableLabel->setText(QString("P(all connectable): %1")
        .arg(analyzer->getProbabilityAllConnectable(), 0, 'f', 6));
    
    double probD = analyzer->getProbabilityDConnectable(dValueSpin->value());
    dConnectableLabel->setText(QString("P(D=%1 connectable): %2")
        .arg(dValueSpin->value())
        .arg(probD, 0, 'f', 6));
    
    allNonConnectableLabel->setText(QString("P(all non-connectable): %1")
        .arg(analyzer->getProbabilityAllNonConnectable(), 0, 'f', 6));
    
    resultsTable->setRowCount(M);
    
    int matrixIndex = 0;
    for (int i = 0; i < M; ++i) {
        QTableWidgetItem* numItem = new QTableWidgetItem(QString::number(i + 1));
        numItem->setFlags(numItem->flags() & ~Qt::ItemIsEditable);
        resultsTable->setItem(i, 0, numItem);

        QTableWidgetItem* valItem = new QTableWidgetItem(QString::fromStdString(clusters[i]));
        valItem->setFlags(valItem->flags() & ~Qt::ItemIsEditable);
        resultsTable->setItem(i, 1, valItem);
        
        if (i < M - 1) {
            bool connectable = matrix[matrixIndex++];
            QTableWidgetItem* connItem = new QTableWidgetItem(connectable ? "Yes" : "No");
            connItem->setForeground(connectable ? Qt::darkGreen : Qt::darkRed);
            connItem->setFlags(connItem->flags() & ~Qt::ItemIsEditable);
            resultsTable->setItem(i, 2, connItem);
        } else {
            QTableWidgetItem* connItem = new QTableWidgetItem("-");
            connItem->setFlags(connItem->flags() & ~Qt::ItemIsEditable);
            resultsTable->setItem(i, 2, connItem);
        }
    }
    
    resultsTable->resizeColumnsToContents();
}

void MainWindow::onSaveResults() {
    if (!analyzer) {
        QMessageBox::warning(this, "Warning", "No results to save. Generate first.");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, 
        "Save Results", 
        QString("cluster_analysis_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        "Text Files (*.txt);;CSV Files (*.csv)");
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open file for writing");
        return;
    }
    
    QTextStream out(&file);
 
    out << "=== Cluster Connectability Analysis Results ===\n";
    out << "Generated: " << QDateTime::currentDateTime().toString() << "\n\n";
    out << "Parameters:\n";
    out << "  Number of clusters (M): " << clusterCountSpin->value() << "\n";
    out << "  Cluster size (n): " << clusterSizeSpin->value() << "\n";
    out << "  Alphabet size (r): " << alphabetSizeSpin->value() << "\n";
    out << "  Pattern (k=" << patternEdit->text().length() << "): " << patternEdit->text() << "\n";
    out << "  Distribution: " << distributionCombo->currentText() << "\n";
    
    if (distributionCombo->currentData().toInt() == 
        static_cast<int>(ClusterAnalyzer::DistributionParams::Type::CUSTOM)) {
        out << "  Symbol probabilities: ";
        for (size_t i = 0; i < probInputs.size(); ++i) {
            out << QString("'%1':%2 ").arg('0' + i).arg(probInputs[i]->value());
        }
        out << "\n";
    }
    
    out << "\nResults:\n";
    out << "  P(all connectable): " << analyzer->getProbabilityAllConnectable() << "\n";
    out << "  P(D=" << dValueSpin->value() << " connectable): " 
        << analyzer->getProbabilityDConnectable(dValueSpin->value()) << "\n";
    out << "  P(all non-connectable): " << analyzer->getProbabilityAllNonConnectable() << "\n\n";
    
    out << "Clusters:\n";
    const auto& clusters = analyzer->getGeneratedClusters();
    const auto& matrix = analyzer->getConnectabilityMatrix();
    
    int matrixIndex = 0;
    for (size_t i = 0; i < clusters.size(); ++i) {
        out << QString("%1").arg(i + 1, 4) << ": " << QString::fromStdString(clusters[i]);
        if (i < clusters.size() - 1) {
            out << "  -> " << (matrix[matrixIndex++] ? "Yes" : "No") << "\n";
        } else {
            out << "\n";
        }
    }
    
    file.close();
    logMessage(QString("Results saved to: %1").arg(fileName));
}

void MainWindow::onClearLog() {
    logText->clear();
    logMessage("Log cleared");
}

void MainWindow::logMessage(const QString& msg, bool isError) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formatted = QString("[%1] %2").arg(timestamp).arg(msg);
    
    if (isError) {
        logText->setTextColor(Qt::red);
    } else {
        logText->setTextColor(Qt::black);
    }
    
    logText->append(formatted);
    logText->setTextColor(Qt::black);
}