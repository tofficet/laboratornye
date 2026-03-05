#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <cmath>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setupConnections();
    logMessage("Приложение запущено");
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    setWindowTitle("Моделирование движения точки в M-рном дереве");
    setMinimumSize(1200, 800);
    
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    QWidget* leftPanel = new QWidget();
    leftPanel->setMaximumWidth(400);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    
    QGroupBox* treeGroup = new QGroupBox("Параметры дерева");
    QFormLayout* treeLayout = new QFormLayout(treeGroup);
    
    branchingFactorSpin = new QSpinBox();
    branchingFactorSpin->setRange(2, 5);
    branchingFactorSpin->setValue(3);
    treeLayout->addRow("Коэффициент ветвления (M):", branchingFactorSpin);
    
    depthSpin = new QSpinBox();
    depthSpin->setRange(1, 4);
    depthSpin->setValue(2);
    treeLayout->addRow("Глубина (H):", depthSpin);
    
    leftLayout->addWidget(treeGroup);
    
    QGroupBox* probGroup = new QGroupBox("Параметры вероятностей");
    QVBoxLayout* probLayout = new QVBoxLayout(probGroup);
    
    QHBoxLayout* levelLayout = new QHBoxLayout();
    levelLayout->addWidget(new QLabel("Уровень:"));
    levelSpin = new QSpinBox();
    levelSpin->setRange(0, depthSpin->value());
    levelSpin->setValue(0);
    levelLayout->addWidget(levelSpin);
    levelLayout->addStretch();
    probLayout->addLayout(levelLayout);
    
    QHBoxLayout* stopLayout = new QHBoxLayout();
    stopLayout->addWidget(new QLabel("Вероятность остановки p:"));
    stopProbSpin = new QDoubleSpinBox();
    stopProbSpin->setRange(0.0, 1.0);
    stopProbSpin->setSingleStep(0.05);
    stopProbSpin->setValue(0.1);
    stopProbSpin->setDecimals(3);
    stopLayout->addWidget(stopProbSpin);
    stopLayout->addStretch();
    probLayout->addLayout(stopLayout);
    
    QHBoxLayout* distLayout = new QHBoxLayout();
    distLayout->addWidget(new QLabel("Распределение:"));
    distributionTypeCombo = new QComboBox();
    distributionTypeCombo->addItem("Равномерное", 0);
    distributionTypeCombo->addItem("Пользовательское", 1);
    distLayout->addWidget(distributionTypeCombo);
    distLayout->addStretch();
    probLayout->addLayout(distLayout);
    
    probInputWidget = new QWidget();
    QVBoxLayout* probInputLayout = new QVBoxLayout(probInputWidget);
    probInputLayout->setContentsMargins(0, 0, 0, 0);
    probLayout->addWidget(probInputWidget);
    
    leftLayout->addWidget(probGroup);
    
    QGroupBox* controlGroup = new QGroupBox("Управление");
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);
    
    generateTreeBtn = new QPushButton("1. Построить дерево");
    controlLayout->addWidget(generateTreeBtn);
    
    runExperimentBtn = new QPushButton("2. Запустить один эксперимент");
    controlLayout->addWidget(runExperimentBtn);
    
    QHBoxLayout* multiLayout = new QHBoxLayout();
    multiLayout->addWidget(new QLabel("Кол-во:"));
    experimentsCountSpin = new QSpinBox();
    experimentsCountSpin->setRange(10, 10000);
    experimentsCountSpin->setValue(1000);
    multiLayout->addWidget(experimentsCountSpin);
    runMultipleBtn = new QPushButton("Запустить множество");
    multiLayout->addWidget(runMultipleBtn);
    controlLayout->addLayout(multiLayout);
    
    clearLogBtn = new QPushButton("Очистить лог");
    controlLayout->addWidget(clearLogBtn);
    
    leftLayout->addWidget(controlGroup);
    
    QGroupBox* resultGroup = new QGroupBox("Результаты");
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);
    
    resultLabel = new QLabel("Нажмите 'Построить дерево' для начала");
    resultLabel->setWordWrap(true);
    resultLayout->addWidget(resultLabel);
    
    statsTable = new QTableWidget();
    statsTable->setColumnCount(2);
    statsTable->setHorizontalHeaderLabels({"Параметр", "Значение"});
    statsTable->horizontalHeader()->setStretchLastSection(true);
    resultLayout->addWidget(statsTable);
    
    leftLayout->addWidget(resultGroup);
    
    QGroupBox* logGroup = new QGroupBox("Лог");
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    logText = new QTextEdit();
    logText->setReadOnly(true);
    logText->setMaximumHeight(150);
    logLayout->addWidget(logText);
    
    leftLayout->addWidget(logGroup);
    
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    
    treeView = new QGraphicsView();
    treeScene = new QGraphicsScene(this);
    treeView->setScene(treeScene);
    treeView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(treeView);
    
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);
    
    updateProbabilityInputs(0);
}

void MainWindow::setupConnections() {
    connect(branchingFactorSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int) { levelSpin->setRange(0, depthSpin->value()); });
    
    connect(depthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int d) { levelSpin->setRange(0, d); });
    
    connect(levelSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onLevelChanged);
    
    connect(distributionTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onDistributionTypeChanged);
    
    connect(generateTreeBtn, &QPushButton::clicked,
            this, &MainWindow::onGenerateTreeClicked);
    
    connect(runExperimentBtn, &QPushButton::clicked,
            this, &MainWindow::onRunExperimentClicked);
    
    connect(runMultipleBtn, &QPushButton::clicked,
            this, &MainWindow::onRunMultipleExperimentsClicked);
    
    connect(clearLogBtn, &QPushButton::clicked,
            this, &MainWindow::onClearLogClicked);
}

void MainWindow::updateProbabilityInputs(int level) {
    for (auto* spin : moveProbInputs) {
        delete spin;
    }
    moveProbInputs.clear();
    
    QLayout* layout = probInputWidget->layout();
    while (layout->count()) {
        delete layout->takeAt(0)->widget();
    }
    
    int M = branchingFactorSpin->value();
    
    for (int i = 0; i < M; ++i) {
        QHBoxLayout* rowLayout = new QHBoxLayout();
        rowLayout->addWidget(new QLabel(QString("К потомку %1:").arg(i + 1)));
        
        QDoubleSpinBox* spin = new QDoubleSpinBox();
        spin->setRange(0.0, 1.0);
        spin->setSingleStep(0.05);
        spin->setDecimals(3);
        spin->setValue(1.0 / M);
        spin->setEnabled(distributionTypeCombo->currentIndex() == 1);
        
        rowLayout->addWidget(spin);
        rowLayout->addStretch();
        
        probInputWidget->layout()->addItem(rowLayout);
        moveProbInputs.push_back(spin);
    }
}

void MainWindow::onDistributionTypeChanged(int index) {
    bool enabled = (index == 1);
    for (auto* spin : moveProbInputs) {
        spin->setEnabled(enabled);
    }
}

void MainWindow::onLevelChanged(int level) {
    if (levelMoveProbabilities.size() > static_cast<size_t>(level) &&
        levelMoveProbabilities[level].size() == static_cast<size_t>(branchingFactorSpin->value())) {
        
        for (size_t i = 0; i < moveProbInputs.size(); ++i) {
            moveProbInputs[i]->setValue(levelMoveProbabilities[level][i]);
        }
    }
    
    if (levelStopProbabilities.size() > static_cast<size_t>(level)) {
        stopProbSpin->setValue(levelStopProbabilities[level]);
    }
}

void MainWindow::onGenerateTreeClicked() {
    try {
        int M = branchingFactorSpin->value();
        int H = depthSpin->value();
        
        int totalLevels = H + 1;
        levelStopProbabilities.resize(totalLevels);
        levelMoveProbabilities.resize(totalLevels);
        
        for (int lvl = 0; lvl < totalLevels; ++lvl) {
            levelSpin->setValue(lvl);
            
            levelStopProbabilities[lvl] = stopProbSpin->value();
            
            levelMoveProbabilities[lvl].resize(M);
            for (int i = 0; i < M; ++i) {
                levelMoveProbabilities[lvl][i] = moveProbInputs[i]->value();
            }
            
            if (distributionTypeCombo->currentIndex() == 1) {
                double sum = 0.0;
                for (double p : levelMoveProbabilities[lvl]) {
                    sum += p;
                }
                if (std::abs(sum - 1.0) > 1e-6) {
                    throw std::invalid_argument(
                        QString("На уровне %1 сумма вероятностей должна быть 1").arg(lvl).toStdString()
                    );
                }
            }
        }
        
        model = std::make_unique<TreeModel>(M, H, levelStopProbabilities, levelMoveProbabilities);
        
        drawTree();
        
        logMessage("Дерево построено успешно");
        resultLabel->setText("Дерево построено. Запустите эксперименты.");
        
    } catch (const std::exception& e) {
        logMessage(QString("Ошибка: %1").arg(e.what()), true);
    }
}

void MainWindow::drawTree() {
    if (!model) return;
    
    treeScene->clear();
    
    const auto& nodes = model->getNodes();
    if (nodes.empty()) return;
    
    int H = depthSpin->value();
    int M = branchingFactorSpin->value();
    
    double startX = 400;
    double startY = 50;
    double levelHeight = 80;
    double nodeRadius = 20;
    
    std::vector<QPointF> positions(nodes.size());
    
    for (const auto& node : nodes) {
        int level = node.level;
        int nodesAtLevel = static_cast<int>(pow(M, level));
        
        int nodesBefore = 0;
        for (int l = 0; l < level; ++l) {
            nodesBefore += static_cast<int>(pow(M, l));
        }
        int index = node.id - nodesBefore;
        
        double x = startX + (index - (nodesAtLevel - 1) / 2.0) * (400.0 / nodesAtLevel);
        double y = startY + level * levelHeight;
        
        positions[node.id] = QPointF(x, y);
    }
    
    for (const auto& node : nodes) {
        QPointF from = positions[node.id];
        
        for (int childId : node.children) {
            QPointF to = positions[childId];
            
            treeScene->addLine(
                from.x(), from.y(), to.x(), to.y(),
                QPen(Qt::gray, 1, Qt::DashLine)
            );
        }
    }
    
    for (const auto& node : nodes) {
        QPointF pos = positions[node.id];
        
        QColor color;
        if (node.level == 0) {
            color = Qt::darkGreen;
        } else if (node.children.empty()) {
            color = Qt::darkYellow;
        } else {
            color = Qt::lightGray;
        }
        
        treeScene->addEllipse(
            pos.x() - nodeRadius, pos.y() - nodeRadius,
            nodeRadius * 2, nodeRadius * 2,
            QPen(Qt::black), QBrush(color)
        );
        
        QString label;
        if (node.level == 0) {
            label = "Корень";
        } else {
            int nodesBefore = 0;
            for (int l = 0; l < node.level; ++l) {
                nodesBefore += static_cast<int>(pow(M, l));
            }
            int idx = node.id - nodesBefore;
            label = QString("L%1_%2").arg(node.level).arg(idx);
        }
        
        QGraphicsTextItem* text = treeScene->addText(label);
        text->setPos(pos.x() - 15, pos.y() - 25);
        text->setScale(0.7);
    }
    
    treeView->fitInView(treeScene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::drawPath(const std::vector<PathPoint>& path) {
    if (!model) return;
    
    drawTree();
    
    for (size_t i = 0; i < path.size(); ++i) {
        const PathPoint& point = path[i];
        
        QList<QGraphicsItem*> items = treeScene->items();
        for (QGraphicsItem* item : items) {
            if (item->type() == QGraphicsEllipseItem::Type) {
                QGraphicsEllipseItem* ellipse = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
                QPointF pos = ellipse->rect().center();
                ellipse->setBrush(QBrush(point.color));
                ellipse->setPen(QPen(Qt::black, 2));
            }
        }
    }
    
    logMessage(QString("Длина пути: %1 узлов").arg(path.size()));
}

void MainWindow::onRunExperimentClicked() {
    if (!model) {
        QMessageBox::warning(this, "Предупреждение", "Сначала постройте дерево!");
        return;
    }
    
    try {
        auto path = model->runSimulation();
        drawPath(path);
        
        QString lastNode = path.back().nodeName;
        QString stopColor = (path.back().color == Qt::red) ? "остановилась" : "дошла до листа";
        
        logMessage(QString("Эксперимент: точка %1 в узле %2")
                      .arg(stopColor).arg(lastNode));
        
    } catch (const std::exception& e) {
        logMessage(QString("Ошибка: %1").arg(e.what()), true);
    }
}

void MainWindow::onRunMultipleExperimentsClicked() {
    if (!model) {
        QMessageBox::warning(this, "Предупреждение", "Сначала постройте дерево!");
        return;
    }
    
    int count = experimentsCountSpin->value();
    
    try {
        logMessage(QString("Запуск %1 экспериментов...").arg(count));
        
        model->runMultipleSimulations(count);
        
        QString stats = model->getStatistics();
        resultLabel->setText(stats);
        
        statsTable->setRowCount(0);
        
        statsTable->insertRow(0);
        statsTable->setItem(0, 0, new QTableWidgetItem("Всего экспериментов"));
        statsTable->setItem(0, 1, new QTableWidgetItem(QString::number(count)));
        
        statsTable->insertRow(1);
        statsTable->setItem(1, 0, new QTableWidgetItem("Всего листьев"));
        statsTable->setItem(1, 1, new QTableWidgetItem(QString::number(model->getTotalLeaves())));
        
        int row = 2;
        for (int leafId : model->getLeafIds()) {
            double prob = model->getProbabilityToLeaf(leafId);
            if (prob > 0) {
                statsTable->insertRow(row);
                statsTable->setItem(row, 0, 
                    new QTableWidgetItem(QString("Вероятность попадания в лист %1")
                        .arg(leafId)));
                statsTable->setItem(row, 1, 
                    new QTableWidgetItem(QString("%1%").arg(prob * 100, 0, 'f', 2)));
                row++;
            }
        }
        
        logMessage("Эксперименты завершены");
        
    } catch (const std::exception& e) {
        logMessage(QString("Ошибка: %1").arg(e.what()), true);
    }
}

void MainWindow::onClearLogClicked() {
    logText->clear();
}

void MainWindow::logMessage(const QString& msg, bool isError) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formatted = QString("[%1] %2").arg(timestamp).arg(msg);
    
    QTextCharFormat format;
    if (isError) {
        logText->setTextColor(Qt::red);
    } else {
        logText->setTextColor(Qt::black);
    }
    
    logText->append(formatted);
}