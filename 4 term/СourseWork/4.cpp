#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <random>
#include <vector>
#include <cmath>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupUI();
        setWindowTitle("Trajectory Simulation");
        resize(1000, 700);
    }

private slots:
    void loadConfig() {
        QString file = QFileDialog::getOpenFileName(this, "Open JSON config", "", "*.json");
        if (file.isEmpty()) return;
        QFile f(file);
        if (!f.open(QIODevice::ReadOnly)) return;
        QByteArray data = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) return;
        QJsonObject obj = doc.object();
        hEdit->setText(QString::number(obj["h"].toDouble()));
        nEdit->setText(QString::number(obj["n"].toInt()));
        YEdit->setText(QString::number(obj["Y"].toDouble()));
        KEdit->setText(QString::number(obj["K"].toInt()));
        lEdit->setText(QString::number(obj["l"].toInt()));
        QString dist = obj["distribution"].toString();
        int idx = distCombo->findText(dist, Qt::MatchFixedString);
        if (idx >= 0) distCombo->setCurrentIndex(idx);
        QJsonObject p = obj["params"].toObject();
        QStringList vals;
        for (auto key : p.keys()) vals << QString::number(p[key].toDouble());
        paramsEdit->setText(vals.join(", "));
    }

    void runSimulation() {
        double h = hEdit->text().toDouble();
        int n = nEdit->text().toInt();
        double Y = YEdit->text().toDouble();
        K = KEdit->text().toInt();

        QStringList ps = paramsEdit->text().split(',', Qt::SkipEmptyParts);
        std::vector<double> dp;
        for (auto &s : ps) dp.push_back(s.trimmed().toDouble());

        int distType = distCombo->currentIndex();
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<> unif(0,1);
        std::binomial_distribution<int> binom;
        std::geometric_distribution<int> geom;
        double triA=0, triB=1, triC=0.5;
        if (distType == 0 && dp.size()>=2) unif = std::uniform_real_distribution<>(dp[0], dp[1]);
        if (distType == 1 && dp.size()>=2) binom = std::binomial_distribution<int>((int)dp[0], dp[1]);
        if (distType == 2 && dp.size()>=1) geom = std::geometric_distribution<int>(dp[0]);
        if (distType == 3 && dp.size()>=3) { triA=dp[0]; triB=dp[1]; triC=dp[2]; }

        std::vector<std::pair<double,double>> traj;
        traj.reserve(n+1);
        double x=0, y=Y;
        traj.push_back({x,y});
        int crossings = 0;
        bool wasPos = (y > 0);
        for (int i=0; i<n; ++i) {
            double s = 0;
            switch (distType) {
                case 0: s = unif(rng); break;
                case 1: s = binom(rng); break;
                case 2: s = geom(rng); break;
                case 3: {
                    double u = unif(rng);
                    double F = (triC - triA) / (triB - triA);
                    s = (u < F) ? triA + std::sqrt(u * (triB - triA) * (triC - triA))
                                : triB - std::sqrt((1-u) * (triB - triA) * (triB - triC));
                    break;
                }
            }
            x += h;
            y += s;
            traj.push_back({x,y});
            bool nowPos = (y > 0);
            if (wasPos != nowPos && i>0) crossings++;
            wasPos = nowPos;
        }

        updateChart(traj);
        addLogEntry(QString("h=%1 n=%2 Y=%3").arg(h).arg(n).arg(Y), crossings);
    }

    void estimateProbability() {
        int target = lEdit->text().toInt();
        double h = hEdit->text().toDouble();
        int n = nEdit->text().toInt();
        double Y = YEdit->text().toDouble();
        QStringList ps = paramsEdit->text().split(',', Qt::SkipEmptyParts);
        std::vector<double> dp;
        for (auto &s : ps) dp.push_back(s.trimmed().toDouble());
        int distType = distCombo->currentIndex();

        int trials = 10000, success=0;
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<> unif(0,1);
        std::binomial_distribution<int> binom;
        std::geometric_distribution<int> geom;
        double triA=0, triB=1, triC=0.5;
        if (distType == 0 && dp.size()>=2) unif = std::uniform_real_distribution<>(dp[0], dp[1]);
        if (distType == 1 && dp.size()>=2) binom = std::binomial_distribution<int>((int)dp[0], dp[1]);
        if (distType == 2 && dp.size()>=1) geom = std::geometric_distribution<int>(dp[0]);
        if (distType == 3 && dp.size()>=3) { triA=dp[0]; triB=dp[1]; triC=dp[2]; }

        for (int t=0; t<trials; ++t) {
            std::vector<std::pair<double,double>> traj;
            traj.reserve(n+1);
            double x=0, y=Y;
            traj.push_back({x,y});
            int cr=0;
            bool wasPos = (y > 0);
            for (int i=0; i<n; ++i) {
                double s=0;
                switch (distType) {
                    case 0: s = unif(rng); break;
                    case 1: s = binom(rng); break;
                    case 2: s = geom(rng); break;
                    case 3: {
                        double u = unif(rng);
                        double F = (triC - triA) / (triB - triA);
                        s = (u < F) ? triA + std::sqrt(u * (triB - triA) * (triC - triA))
                                    : triB - std::sqrt((1-u) * (triB - triA) * (triB - triC));
                        break;
                    }
                }
                x += h; y += s;
                bool nowPos = (y > 0);
                if (wasPos != nowPos && i>0) cr++;
                wasPos = nowPos;
            }
            if (cr == target) success++;
        }
        double prob = (double)success/trials;
        QMessageBox::information(this, "Probability", QString("P(crossings = %1) = %2%")
                                 .arg(target).arg(prob*100, 0, 'f', 2));
    }

private:
    QLineEdit *hEdit, *nEdit, *YEdit, *KEdit, *lEdit, *paramsEdit;
    QComboBox *distCombo;
    QPushButton *loadBtn, *runBtn, *estBtn;
    QTableWidget *logTable;
    QChartView *chartView;
    int K;

    void setupUI() {
        QWidget *central = new QWidget(this);
        setCentralWidget(central);
        QVBoxLayout *main = new QVBoxLayout(central);

        QHBoxLayout *top = new QHBoxLayout();
        QFormLayout *form = new QFormLayout();
        hEdit = new QLineEdit("1.0");
        nEdit = new QLineEdit("10");
        YEdit = new QLineEdit("0.0");
        KEdit = new QLineEdit("5");
        lEdit = new QLineEdit("0");
        form->addRow("h:", hEdit);
        form->addRow("n:", nEdit);
        form->addRow("Y:", YEdit);
        form->addRow("K:", KEdit);
        form->addRow("l:", lEdit);
        distCombo = new QComboBox();
        distCombo->addItems({"Uniform", "Binomial", "Geometric", "Triangular"});
        form->addRow("Distribution:", distCombo);
        paramsEdit = new QLineEdit("-1, 1");
        form->addRow("Params:", paramsEdit);
        top->addLayout(form);

        QVBoxLayout *btns = new QVBoxLayout();
        loadBtn = new QPushButton("Load JSON");
        runBtn = new QPushButton("Run");
        estBtn = new QPushButton("Estimate Prob");
        btns->addWidget(loadBtn);
        btns->addWidget(runBtn);
        btns->addWidget(estBtn);
        btns->addStretch();
        top->addLayout(btns);
        main->addLayout(top);

        chartView = new QChartView(this);
        chartView->setRenderHint(QPainter::Antialiasing);
        main->addWidget(chartView);

        logTable = new QTableWidget(this);
        logTable->setColumnCount(3);
        logTable->setHorizontalHeaderLabels({"#", "Params", "Crossings"});
        logTable->horizontalHeader()->setStretchLastSection(true);
        logTable->setMaximumHeight(200);
        main->addWidget(logTable);

        connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadConfig);
        connect(runBtn, &QPushButton::clicked, this, &MainWindow::runSimulation);
        connect(estBtn, &QPushButton::clicked, this, &MainWindow::estimateProbability);
    }

    void updateChart(const std::vector<std::pair<double,double>>& pts) {
        QChart *chart = new QChart();
        QLineSeries *series = new QLineSeries();
        for (auto &p : pts) series->append(p.first, p.second);
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->axes(Qt::Horizontal).first()->setTitleText("X");
        chart->axes(Qt::Vertical).first()->setTitleText("Y");
        chartView->setChart(chart);
    }

    void addLogEntry(const QString& params, int crossings) {
        int row = logTable->rowCount();
        if (row >= K) logTable->removeRow(0);
        logTable->insertRow(logTable->rowCount());
        int r = logTable->rowCount() - 1;
        logTable->setItem(r, 0, new QTableWidgetItem(QString::number(r+1)));
        logTable->setItem(r, 1, new QTableWidgetItem(params));
        logTable->setItem(r, 2, new QTableWidgetItem(QString::number(crossings)));
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "4.moc"

// qmake trajectory.pro
// make
// ./trajectory_sim.app/Contents/MacOS/trajectory_sim