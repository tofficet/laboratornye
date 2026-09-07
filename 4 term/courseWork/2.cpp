#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <QHeaderView>

using namespace std;

struct Person {
    string name;
    enum State { HEALTHY, INFECTED, RECOVERED } state = HEALTHY;
};

class DiseaseModel {
public:
    bool loadGraph(const string& filename) {
        ifstream f(filename);
        if (!f) return false;
        int N; f >> N;
        if (N <= 0) return false;
        people.resize(N);
        adj.assign(N, {});
        for (int i = 0; i < N; ++i) f >> people[i].name;
        int u, v;
        while (f >> u >> v) {
            if (u >= 0 && u < N && v >= 0 && v < N) {
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
        reset();
        return true;
    }

    void reset() {
        for (auto& p : people) p.state = Person::HEALTHY;
        infected.clear();
    }

    void setParams(double p1_, double p2_) { p1 = p1_; p2 = p2_; }

    void startRandom() {
        reset();
        uniform_int_distribution<int> dist(0, people.size()-1);
        int start = dist(rng);
        people[start].state = Person::INFECTED;
        infected.push_back(start);
    }

    bool step() {
        if (infected.empty()) return false;
        vector<int> current = infected;
        for (int v : current) {
            for (int nb : adj[v]) {
                if (people[nb].state == Person::HEALTHY && uniform_real_distribution<>(0,1)(rng) < p1) {
                    people[nb].state = Person::INFECTED;
                    infected.push_back(nb);
                }
            }
        }
        for (int v : current) {
            if (people[v].state == Person::INFECTED && uniform_real_distribution<>(0,1)(rng) < p2) {
                people[v].state = Person::RECOVERED;
                infected.erase(remove(infected.begin(), infected.end(), v), infected.end());
            }
        }
        return !infected.empty();
    }

    vector<int> notInfected() const {
        vector<int> res;
        for (int i = 0; i < (int)people.size(); ++i)
            if (people[i].state == Person::HEALTHY) res.push_back(i);
        return res;
    }
    vector<int> recovered() const {
        vector<int> res;
        for (int i = 0; i < (int)people.size(); ++i)
            if (people[i].state == Person::RECOVERED) res.push_back(i);
        return res;
    }
    vector<int> recoveredWithNonRecoveredNeighbors() const {
        vector<int> res;
        for (int v : recovered()) {
            bool hasNonRec = false;
            for (int nb : adj[v])
                if (people[nb].state != Person::RECOVERED) { hasNonRec = true; break; }
            if (hasNonRec) res.push_back(v);
        }
        return res;
    }
    vector<int> notInfectedButAllNeighborsInfected() const {
        vector<int> res;
        for (int v : notInfected()) {
            if (adj[v].empty()) continue;
            bool allInfected = true;
            for (int nb : adj[v])
                if (people[nb].state == Person::HEALTHY) { allInfected = false; break; }
            if (allInfected) res.push_back(v);
        }
        return res;
    }

    // Доступ к данным
    int size() const { return people.size(); }
    const string& name(int i) const { return people[i].name; }
    Person::State state(int i) const { return people[i].state; }

private:
    vector<Person> people;
    vector<vector<int>> adj;
    vector<int> infected;
    double p1 = 0.3, p2 = 0.1;
    mt19937 rng{ random_device{}() };
};


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent), model(nullptr), timer(nullptr), running(false) {
        setupUI();
        setWindowTitle("Симуляция распространения заболевания");
        resize(800, 500);
    }

private slots:
    void loadGraph() {
        QString file = QFileDialog::getOpenFileName(this, "Выберите файл графа", "", "*.txt");
        if (file.isEmpty()) return;
        DiseaseModel* newModel = new DiseaseModel();
        if (!newModel->loadGraph(file.toStdString())) {
            delete newModel;
            QMessageBox::critical(this, "Ошибка", "Не удалось загрузить граф");
            return;
        }
        delete model;
        model = newModel;
        if (timer) { delete timer; timer = nullptr; }
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::doStep);
        running = false;
        startBtn->setText("Старт");
        startBtn->setEnabled(true);
        updateTable();
        statusLabel->setText("Граф загружен. Нажмите Старт.");
    }

    void toggleSimulation() {
        if (!model) return;
        if (!running) {
            bool ok1, ok2;
            double p1 = p1Edit->text().toDouble(&ok1);
            double p2 = p2Edit->text().toDouble(&ok2);
            if (!ok1 || p1 <= 0 || p1 > 1 || !ok2 || p2 < 0 || p2 > 1) {
                QMessageBox::critical(this, "Ошибка", "p1 ∈ (0,1], p2 ∈ [0,1]");
                return;
            }
            model->setParams(p1, p2);
            model->startRandom();
            running = true;
            startBtn->setText("Стоп");
            statusLabel->setText("Моделирование запущено...");
            timer->start(150);
        } else {
            running = false;
            timer->stop();
            startBtn->setText("Старт");
            statusLabel->setText("Остановлено пользователем");
        }
        updateTable();
    }

    void doStep() {
        if (!model || !running) return;
        bool cont = model->step();
        updateTable();
        int infected = 0;
        for (int i = 0; i < model->size(); ++i)
            if (model->state(i) == Person::INFECTED) ++infected;
        statusLabel->setText(QString("Заражённых: %1").arg(infected));
        if (!cont) {
            running = false;
            timer->stop();
            startBtn->setText("Старт");
            statusLabel->setText("Моделирование завершено");
            updateTable();
        }
    }

    void filterChanged(int) { updateTable(); }

private:
    void setupUI() {
        QWidget* central = new QWidget(this);
        setCentralWidget(central);
        QVBoxLayout* mainLayout = new QVBoxLayout(central);

        QHBoxLayout* top = new QHBoxLayout();
        loadBtn = new QPushButton("Загрузить граф");
        top->addWidget(loadBtn);
        connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadGraph);

        QFormLayout* params = new QFormLayout();
        p1Edit = new QLineEdit("0.3");
        p2Edit = new QLineEdit("0.1");
        params->addRow("p1 (заражение):", p1Edit);
        params->addRow("p2 (выздоровление):", p2Edit);
        top->addLayout(params);

        startBtn = new QPushButton("Старт");
        startBtn->setEnabled(false);
        top->addWidget(startBtn);
        connect(startBtn, &QPushButton::clicked, this, &MainWindow::toggleSimulation);
        top->addStretch();
        mainLayout->addLayout(top);

        table = new QTableWidget();
        table->setColumnCount(2);
        table->setHorizontalHeaderLabels({"ID", "Имя"});
        table->horizontalHeader()->setStretchLastSection(true);
        mainLayout->addWidget(table);

        QHBoxLayout* bottom = new QHBoxLayout();
        bottom->addWidget(new QLabel("Фильтр:"));
        filterCombo = new QComboBox();
        filterCombo->addItems({"Все", "Не заразившиеся", "Исцелившиеся",
                               "Исцелившиеся, окружение не исцелилось",
                               "Не заразившиеся, всё окружение заразилось"});
        bottom->addWidget(filterCombo);
        connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::filterChanged);

        statusLabel = new QLabel("Готов");
        bottom->addWidget(statusLabel);
        mainLayout->addLayout(bottom);
    }

    void updateTable() {
        if (!model) {
            table->setRowCount(0);
            return;
        }
        int filterIdx = filterCombo->currentIndex();
        vector<int> ids;
        switch (filterIdx) {
            case 0: for (int i = 0; i < model->size(); ++i) ids.push_back(i); break;
            case 1: ids = model->notInfected(); break;
            case 2: ids = model->recovered(); break;
            case 3: ids = model->recoveredWithNonRecoveredNeighbors(); break;
            case 4: ids = model->notInfectedButAllNeighborsInfected(); break;
        }
        table->setRowCount(ids.size());
        for (size_t i = 0; i < ids.size(); ++i) {
            int v = ids[i];
            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(v));
            idItem->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, 0, idItem);

            QString name = QString::fromStdString(model->name(v));
            QTableWidgetItem* nameItem = new QTableWidgetItem(name);
            Person::State st = model->state(v);
            if (st == Person::INFECTED) nameItem->setBackground(Qt::red);
            else if (st == Person::RECOVERED) nameItem->setBackground(Qt::green);
            else nameItem->setBackground(Qt::white);
            table->setItem(i, 1, nameItem);
        }
    }

    DiseaseModel* model;
    QTimer* timer;
    bool running;
    QPushButton *loadBtn, *startBtn;
    QLineEdit *p1Edit, *p2Edit;
    QTableWidget *table;
    QComboBox *filterCombo;
    QLabel *statusLabel;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}


#include "2.moc"