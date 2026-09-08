#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialog>
#include <random>
#include <vector>
#include <cmath>
#include <map>

class RandomWalkWidget : public QWidget {
    Q_OBJECT
private:
    std::vector<double> positions;
    double currentPos;
    bool isRunning;
    QTimer *timer;
    std::mt19937_64 rng;
    std::discrete_distribution<int> dist;
    std::vector<double> values;
    int currentStep;
    int totalSteps;
    double startPos;
    QPushButton *startBtn, *stopBtn, *clearBtn;
    QLabel *statusLabel, *stepLabel, *posLabel;
    QDoubleSpinBox *startSpin;
    QSpinBox *stepsSpin;
    
public:
    RandomWalkWidget(QWidget *parent = nullptr) : QWidget(parent), 
        currentPos(0), isRunning(false), rng(std::random_device{}()),
        currentStep(0), totalSteps(50), startPos(0) {
        
        setMinimumSize(800, 400);
        setDefaultDistribution();
        
        QVBoxLayout *layout = new QVBoxLayout(this);
        
        QHBoxLayout *controls = new QHBoxLayout();
        
        QLabel *startLabel = new QLabel("Начальная позиция:", this);
        startSpin = new QDoubleSpinBox(this);
        startSpin->setRange(-100, 100);
        startSpin->setValue(0);
        startSpin->setSingleStep(0.5);
        connect(startSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
                [this](double v) { startPos = v; });
        
        QLabel *stepsLabel = new QLabel("Шагов:", this);
        stepsSpin = new QSpinBox(this);
        stepsSpin->setRange(1, 1000);
        stepsSpin->setValue(50);
        connect(stepsSpin, QOverload<int>::of(&QSpinBox::valueChanged), 
                [this](int v) { totalSteps = v; });
        
        startBtn = new QPushButton("Старт", this);
        startBtn->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        connect(startBtn, &QPushButton::clicked, this, &RandomWalkWidget::start);
        
        stopBtn = new QPushButton("Стоп", this);
        stopBtn->setStyleSheet("background-color: #f44336; color: white; font-weight: bold;");
        stopBtn->setEnabled(false);
        connect(stopBtn, &QPushButton::clicked, this, &RandomWalkWidget::stop);
        
        clearBtn = new QPushButton("Очистить", this);
        connect(clearBtn, &QPushButton::clicked, this, &RandomWalkWidget::clear);
        
        QPushButton *loadBtn = new QPushButton("Загрузить закон", this);
        connect(loadBtn, &QPushButton::clicked, this, &RandomWalkWidget::loadDistribution);
        
        controls->addWidget(startLabel);
        controls->addWidget(startSpin);
        controls->addWidget(stepsLabel);
        controls->addWidget(stepsSpin);
        controls->addWidget(startBtn);
        controls->addWidget(stopBtn);
        controls->addWidget(clearBtn);
        controls->addWidget(loadBtn);
        
        layout->addLayout(controls);
        
        QHBoxLayout *info = new QHBoxLayout();
        stepLabel = new QLabel("Шаг: 0", this);
        posLabel = new QLabel("Позиция: 0.00", this);
        statusLabel = new QLabel("Готов", this);
        info->addWidget(stepLabel);
        info->addWidget(posLabel);
        info->addStretch();
        info->addWidget(statusLabel);
        layout->addLayout(info);
        
        timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, &QTimer::timeout, this, &RandomWalkWidget::nextStep);
    }
    
    void setDefaultDistribution() {
        values = {-2, -1, 0, 1, 2};
        std::vector<double> probs = {0.1, 0.2, 0.4, 0.2, 0.1};
        dist = std::discrete_distribution<int>(probs.begin(), probs.end());
    }
    
    void loadDistribution() {
        QString file = QFileDialog::getOpenFileName(this, "Выберите JSON файл", "", "JSON (*.json)");
        if (file.isEmpty()) return;
        
        QFile f(file);
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
            return;
        }
        
        QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        if (doc.isNull()) {
            QMessageBox::warning(this, "Ошибка", "Неверный JSON");
            return;
        }
        
        QJsonObject obj = doc.object();
        if (obj.contains("values") && obj.contains("probabilities")) {
            QJsonArray vals = obj["values"].toArray();
            QJsonArray probs = obj["probabilities"].toArray();
            
            if (vals.size() != probs.size() || vals.isEmpty()) {
                QMessageBox::warning(this, "Ошибка", "Неверный формат");
                return;
            }
            
            values.clear();
            std::vector<double> probsVec;
            double sum = 0;
            for (int i = 0; i < vals.size(); ++i) {
                values.push_back(vals[i].toDouble());
                double p = probs[i].toDouble();
                probsVec.push_back(p);
                sum += p;
            }
            
            if (std::abs(sum - 1.0) > 0.001) {
                QMessageBox::warning(this, "Ошибка", "Сумма вероятностей != 1");
                return;
            }
            
            dist = std::discrete_distribution<int>(probsVec.begin(), probsVec.end());
            QMessageBox::information(this, "Успех", "Закон перемещения загружен");
        }
    }
    
    void start() {
        if (isRunning) return;
        
        positions.clear();
        currentPos = startPos;
        positions.push_back(currentPos);
        currentStep = 0;
        isRunning = true;
        
        startBtn->setEnabled(false);
        stopBtn->setEnabled(true);
        startSpin->setEnabled(false);
        stepsSpin->setEnabled(false);
        
        statusLabel->setText("Выполняется...");
        update();
        timer->start();
    }
    
    void stop() {
        timer->stop();
        isRunning = false;
        
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        startSpin->setEnabled(true);
        stepsSpin->setEnabled(true);
        
        statusLabel->setText("Остановлено");
        showStatistics();
        update();
    }
    
    void clear() {
        timer->stop();
        isRunning = false;
        positions.clear();
        currentPos = startPos;
        
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        startSpin->setEnabled(true);
        stepsSpin->setEnabled(true);
        
        statusLabel->setText("Готов");
        stepLabel->setText("Шаг: 0");
        posLabel->setText("Позиция: 0.00");
        update();
    }
    
    void nextStep() {
        if (currentStep >= totalSteps) {
            stop();
            return;
        }
        
        double step = values[dist(rng)];
        currentPos += step;
        positions.push_back(currentPos);
        currentStep++;
        
        stepLabel->setText(QString("Шаг: %1/%2").arg(currentStep).arg(totalSteps));
        posLabel->setText(QString("Позиция: %1").arg(currentPos, 0, 'f', 2));
        update();
    }
    
    void showStatistics() {
        if (positions.empty()) return;
        
        std::map<int, int> freq;
        for (double p : positions) {
            freq[static_cast<int>(std::round(p))]++;
        }
        
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Распределение конечных позиций");
        dialog->resize(400, 300);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        
        QLabel *title = new QLabel(QString("После %1 шагов").arg(positions.size() - 1), dialog);
        layout->addWidget(title);
        
        QTableWidget *table = new QTableWidget((int)freq.size(), 2, dialog);
        table->setHorizontalHeaderLabels({"Позиция", "Вероятность"});
        table->horizontalHeader()->setStretchLastSection(true);
        
        int row = 0;
        for (const auto& pair : freq) {
            double prob = static_cast<double>(pair.second) / positions.size();
            QTableWidgetItem *posItem = new QTableWidgetItem(QString::number(pair.first));
            QTableWidgetItem *probItem = new QTableWidgetItem(QString::number(prob, 'f', 4));
            table->setItem(row, 0, posItem);
            table->setItem(row, 1, probItem);
            row++;
        }
        layout->addWidget(table);
        
        QPushButton *closeBtn = new QPushButton("Закрыть", dialog);
        connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
        layout->addWidget(closeBtn);
        
        dialog->exec();
    }
    
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);
        
        QRect r = rect().adjusted(50, 30, -30, -30);
        
        if (positions.empty()) {
            painter.drawText(r, Qt::AlignCenter, "Нажмите 'Старт' для начала моделирования");
            return;
        }
        
        double minVal = *std::min_element(positions.begin(), positions.end());
        double maxVal = *std::max_element(positions.begin(), positions.end());
        double range = maxVal - minVal + 1;
        if (range < 1) range = 1;
        
        painter.drawLine(r.left(), r.bottom(), r.right(), r.bottom());
        painter.drawLine(r.left(), r.top(), r.left(), r.bottom());
        
        painter.setPen(QPen(Qt::black, 1));
        for (double x = std::ceil(minVal); x <= std::floor(maxVal); ++x) {
            if (std::abs(x) < 0.01) continue;
            int px = r.left() + (x - minVal) / range * r.width();
            painter.drawLine(px, r.bottom() - 5, px, r.bottom() + 5);
            painter.drawText(px - 10, r.bottom() + 20, QString::number(x, 'f', 0));
        }
        
        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        int zeroX = r.left() + (0 - minVal) / range * r.width();
        painter.drawLine(zeroX, r.top(), zeroX, r.bottom());
        
        if (positions.size() > 1) {
            if (isRunning) {
                painter.setPen(QPen(Qt::red, 3));
            } else {
                painter.setPen(QPen(Qt::blue, 2));
            }
            
            QPainterPath path;
            double startX = r.left() + (positions[0] - minVal) / range * r.width();
            path.moveTo(startX, r.bottom());
            
            for (size_t i = 1; i < positions.size(); ++i) {
                double x = r.left() + (positions[i] - minVal) / range * r.width();
                double y = r.bottom() - (static_cast<double>(i) / positions.size()) * r.height();
                path.lineTo(x, y);
            }
            painter.drawPath(path);
        }
        
        if (!positions.empty()) {
            double lastX = r.left() + (positions.back() - minVal) / range * r.width();
            painter.setPen(QPen(Qt::red, 8));
            painter.drawEllipse(QPointF(lastX, r.bottom()), 6, 6);
            
            painter.setPen(QPen(Qt::black, 1));
            painter.drawText(lastX + 10, r.bottom() - 10, 
                           QString("Текущая: %1").arg(positions.back(), 0, 'f', 2));
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RandomWalkWidget w;
    w.show();
    return app.exec();
}

#include "12.moc"


// qmake random_walk.pro
// make
// ./random_walk.app/Contents/MacOS/random_walk