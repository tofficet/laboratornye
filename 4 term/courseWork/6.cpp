#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <QFileDialog>

class StepChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit StepChartWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setData(const std::vector<double>& x, const std::vector<double>& y) {
        m_x = x;
        m_y = y;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);

        if (m_x.empty() || m_y.empty()) return;

        double maxVal = *std::max_element(m_y.begin(), m_y.end());
        double minVal = 0;
        double range = maxVal - minVal;
        if (range < 1e-9) range = 1.0;

        QRect r = rect().adjusted(40, 20, -20, -20);
        int n = m_x.size();

        QPolygonF poly;
        for (int i = 0; i < n; ++i) {
            double x = r.left() + (m_x[i] - m_x.front()) / (m_x.back() - m_x.front() + 1e-9) * r.width();
            double y = r.bottom() - (m_y[i] - minVal) / range * r.height();
            poly << QPointF(x, y);
            if (i < n - 1) {
                double xNext = r.left() + (m_x[i+1] - m_x.front()) / (m_x.back() - m_x.front() + 1e-9) * r.width();
                poly << QPointF(xNext, y);
            }
        }

        painter.setPen(QPen(Qt::blue, 2));
        painter.drawPolyline(poly);

        painter.setPen(Qt::black);
        painter.drawText(r.left(), r.bottom() + 15, "x");
        painter.drawText(r.right() - 10, r.top() - 5, "y");
    }

private:
    std::vector<double> m_x, m_y;
};

class Generator : public QObject {
    Q_OBJECT
public:
    Generator(QObject *parent = nullptr) : QObject(parent) {}

    void setParams(int stepCount, double tau, int n, const QString& distType, const QJsonObject& params) {
        m_stepCount = stepCount;
        m_tau = tau;
        m_n = n;
        m_distributionType = distType;
        m_distributionParams = params;
        m_rng.seed(std::random_device{}());
        m_figures.clear();
        m_currentIndex = 0;
    }

    void generateAll(int count) {
        m_figures.clear();
        for (int k = 0; k < count; ++k) {
            std::vector<double> vals;
            for (int i = 0; i < m_stepCount; ++i) {
                vals.push_back(generateXi() * m_tau);
            }
            m_figures.push_back(vals);
        }
        m_currentIndex = 0;
        emit figureChanged();
    }

    int64_t generateXi() {
        if (m_distributionType == "uniform") {
            std::uniform_int_distribution<int64_t> dist(0, m_n);
            return dist(m_rng);
        }
        else if (m_distributionType == "binomial") {
            int trials = m_distributionParams.value("trials").toInt(10);
            double p = m_distributionParams.value("p").toDouble(0.5);
            std::binomial_distribution<int> binom(trials, p);
            return binom(m_rng) % (m_n + 1);
        }
        else if (m_distributionType == "finite_geometric") {
            double p = m_distributionParams.value("p").toDouble(0.5);
            std::vector<double> weights(m_n + 1);
            double sum = 0.0;
            for (int i = 0; i <= m_n; ++i) {
                weights[i] = std::pow(1.0 - p, i) * p;
                sum += weights[i];
            }
            for (auto& w : weights) w /= sum;
            std::discrete_distribution<int> dist(weights.begin(), weights.end());
            return dist(m_rng);
        }
        else if (m_distributionType == "discrete_triangular") {
            int a = m_distributionParams.value("a").toInt(0);
            int b = m_distributionParams.value("b").toInt(m_n);
            int c = m_distributionParams.value("c").toInt((a + b) / 2);
            std::vector<double> weights(m_n + 1, 0.0);
            for (int i = a; i <= b; ++i) {
                if (i <= c) weights[i] = (double)(i - a) / (c - a + 1e-9);
                else weights[i] = (double)(b - i) / (b - c + 1e-9);
            }
            double sum = 0.0;
            for (auto& w : weights) sum += w;
            for (auto& w : weights) w /= sum;
            std::discrete_distribution<int> dist(weights.begin(), weights.end());
            return dist(m_rng);
        }
        return 0;
    }

    bool isStrictlyIncreasing(const std::vector<double>& vals) const {
        for (size_t i = 1; i < vals.size(); ++i) {
            if (vals[i] <= vals[i-1]) return false;
        }
        return true;
    }

    int countIncreasing() const {
        int cnt = 0;
        for (const auto& fig : m_figures) {
            if (isStrictlyIncreasing(fig)) ++cnt;
        }
        return cnt;
    }

    int totalFigures() const { return m_figures.size(); }
    int currentIndex() const { return m_currentIndex; }

    std::vector<double> getCurrentFigure() const {
        if (m_figures.empty() || m_currentIndex >= (int)m_figures.size()) return {};
        return m_figures[m_currentIndex];
    }

    void nextPage(int step = 10) {
        int newIndex = m_currentIndex + step;
        if (newIndex < (int)m_figures.size()) {
            m_currentIndex = newIndex;
            emit figureChanged();
        }
    }

    void prevPage(int step = 10) {
        int newIndex = m_currentIndex - step;
        if (newIndex >= 0) {
            m_currentIndex = newIndex;
            emit figureChanged();
        }
    }

    void setCurrentIndex(int idx) {
        if (idx >= 0 && idx < (int)m_figures.size()) {
            m_currentIndex = idx;
            emit figureChanged();
        }
    }

signals:
    void figureChanged();

private:
    int m_stepCount = 10;
    double m_tau = 1.0;
    int m_n = 5;
    QString m_distributionType = "uniform";
    QJsonObject m_distributionParams;
    std::mt19937_64 m_rng;
    std::vector<std::vector<double>> m_figures;
    int m_currentIndex = 0;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent), m_generator(new Generator(this)) {
        setWindowTitle("Step Figures Generator");
        resize(900, 700);

        QWidget *central = new QWidget(this);
        setCentralWidget(central);
        QVBoxLayout *mainLayout = new QVBoxLayout(central);

        m_chartWidget = new StepChartWidget(this);
        mainLayout->addWidget(m_chartWidget, 1);

        QHBoxLayout *controlLayout = new QHBoxLayout();

        QLabel *distLabel = new QLabel("Distribution:", this);
        controlLayout->addWidget(distLabel);
        m_distCombo = new QComboBox(this);
        m_distCombo->addItems({"uniform", "binomial", "finite_geometric", "discrete_triangular"});
        controlLayout->addWidget(m_distCombo);

        QLabel *stepLabel = new QLabel("Steps:", this);
        controlLayout->addWidget(stepLabel);
        m_stepSpin = new QSpinBox(this);
        m_stepSpin->setRange(2, 50);
        m_stepSpin->setValue(10);
        controlLayout->addWidget(m_stepSpin);

        QLabel *tauLabel = new QLabel("Tau:", this);
        controlLayout->addWidget(tauLabel);
        m_tauSpin = new QDoubleSpinBox(this);
        m_tauSpin->setRange(0.1, 10.0);
        m_tauSpin->setValue(1.0);
        controlLayout->addWidget(m_tauSpin);

        QLabel *nLabel = new QLabel("n:", this);
        controlLayout->addWidget(nLabel);
        m_nSpin = new QSpinBox(this);
        m_nSpin->setRange(1, 20);
        m_nSpin->setValue(5);
        controlLayout->addWidget(m_nSpin);

        QLabel *countLabel = new QLabel("Generations:", this);
        controlLayout->addWidget(countLabel);
        m_countSpin = new QSpinBox(this);
        m_countSpin->setRange(1, 1000);
        m_countSpin->setValue(100);
        controlLayout->addWidget(m_countSpin);

        QPushButton *genBtn = new QPushButton("Generate", this);
        controlLayout->addWidget(genBtn);
        connect(genBtn, &QPushButton::clicked, this, &MainWindow::onGenerate);

        QPushButton *configBtn = new QPushButton("Load Config", this);
        controlLayout->addWidget(configBtn);
        connect(configBtn, &QPushButton::clicked, this, &MainWindow::loadConfig);

        mainLayout->addLayout(controlLayout);

        QHBoxLayout *navLayout = new QHBoxLayout();
        m_prevBtn = new QPushButton("Prev", this);
        m_nextBtn = new QPushButton("Next", this);
        m_pageLabel = new QLabel("0 / 0", this);
        navLayout->addWidget(m_prevBtn);
        navLayout->addWidget(m_pageLabel);
        navLayout->addWidget(m_nextBtn);
        navLayout->addStretch();

        m_probLabel = new QLabel("Probability (increasing): 0.00%", this);
        navLayout->addWidget(m_probLabel);

        mainLayout->addLayout(navLayout);

        connect(m_prevBtn, &QPushButton::clicked, [this]() { m_generator->prevPage(10); });
        connect(m_nextBtn, &QPushButton::clicked, [this]() { m_generator->nextPage(10); });
        connect(m_generator, &Generator::figureChanged, this, &MainWindow::updateDisplay);

        loadDefaultConfig();
        onGenerate();
    }

private slots:
    void onGenerate() {
        QJsonObject params;
        if (m_distCombo->currentText() == "binomial") {
            params["trials"] = 10;
            params["p"] = 0.5;
        } else if (m_distCombo->currentText() == "finite_geometric") {
            params["p"] = 0.5;
        } else if (m_distCombo->currentText() == "discrete_triangular") {
            params["a"] = 0;
            params["b"] = m_nSpin->value();
            params["c"] = (m_nSpin->value()) / 2;
        }

        m_generator->setParams(
            m_stepSpin->value(),
            m_tauSpin->value(),
            m_nSpin->value(),
            m_distCombo->currentText(),
            params
        );
        m_generator->generateAll(m_countSpin->value());
        updateDisplay();
    }

    void loadConfig() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Config", "", "JSON (*.json)");
        if (fileName.isEmpty()) return;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Cannot open file");
            return;
        }
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            QMessageBox::warning(this, "Error", "Invalid JSON");
            return;
        }
        QJsonObject obj = doc.object();
        if (obj.contains("distribution")) {
            QString dist = obj["distribution"].toString();
            int idx = m_distCombo->findText(dist);
            if (idx >= 0) m_distCombo->setCurrentIndex(idx);
        }
        if (obj.contains("steps")) m_stepSpin->setValue(obj["steps"].toInt(10));
        if (obj.contains("tau")) m_tauSpin->setValue(obj["tau"].toDouble(1.0));
        if (obj.contains("n")) m_nSpin->setValue(obj["n"].toInt(5));
        if (obj.contains("generations")) m_countSpin->setValue(obj["generations"].toInt(100));
        if (obj.contains("params")) {
            m_generator->setParams(
                m_stepSpin->value(),
                m_tauSpin->value(),
                m_nSpin->value(),
                m_distCombo->currentText(),
                obj["params"].toObject()
            );
        }
        onGenerate();
    }

    void loadDefaultConfig() {
        QJsonObject defaultConfig;
        defaultConfig["distribution"] = "uniform";
        defaultConfig["steps"] = 10;
        defaultConfig["tau"] = 1.0;
        defaultConfig["n"] = 5;
        defaultConfig["generations"] = 100;
        defaultConfig["params"] = QJsonObject();
    }

    void updateDisplay() {
        auto fig = m_generator->getCurrentFigure();
        if (fig.empty()) {
            m_chartWidget->setData({}, {});
            m_pageLabel->setText("0 / 0");
            return;
        }
        std::vector<double> x(fig.size());
        for (size_t i = 0; i < fig.size(); ++i) x[i] = i * m_tauSpin->value();
        m_chartWidget->setData(x, fig);

        int total = m_generator->totalFigures();
        int cur = m_generator->currentIndex();
        m_pageLabel->setText(QString("%1 / %2").arg(cur + 1).arg(total));

        int inc = m_generator->countIncreasing();
        double prob = total > 0 ? (double)inc / total * 100.0 : 0.0;
        m_probLabel->setText(QString("Probability (increasing): %1% (%2 / %3)")
                              .arg(prob, 0, 'f', 2).arg(inc).arg(total));
    }

private:
    Generator *m_generator;
    StepChartWidget *m_chartWidget;
    QComboBox *m_distCombo;
    QSpinBox *m_stepSpin, *m_nSpin, *m_countSpin;
    QDoubleSpinBox *m_tauSpin;
    QPushButton *m_prevBtn, *m_nextBtn;
    QLabel *m_pageLabel, *m_probLabel;
};

#include <QFileDialog>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "6.moc"



// qmake step_figures.pro
// make
// ./step_figures