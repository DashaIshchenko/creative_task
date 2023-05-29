#include "dialog.h"
#include "ui_dialog.h"

#include "graph.h"
#include "TSPalgorithm.h"

#include <QDebug>
#include <QMouseEvent>

//#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog())
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->btnAddEdge->setStyleSheet("background-color:" + QColor(165, 118, 222).name());
    ui->btnAddVertex->setStyleSheet("background-color:" + QColor(165, 118, 222).name());
    ui->btnAdj->setStyleSheet("background-color:" + QColor(165, 118, 222).name());
    ui->btnDelete->setStyleSheet("background-color:" + QColor(165, 118, 222).name());
    ui->btnTSP->setStyleSheet("background-color:" + QColor(165, 118, 222).name());
    //connect(ui->graphicsView, &Dialog::mousePressedSignal, this, &Dialog::graphWidgetClicked);

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_btnAddVertex_clicked() {
    int num = vertices.size();
    vert = new graphVertex(num);
    vert->setGraph(&graph);
    vertices.push_back(vert);
    scene->addItem(vert);
    if (graph.insertVertex(num)) qDebug() << "success!";
}


void Dialog::on_btnAdj_clicked()
{

    adj = new Adjacency;
    //qDebug() << graph.getAdj();
    adj->setWindowTitle("Матрица смежности ._.");
    adj->setWindowIcon(QIcon(":/myIcon.ico"));
    QPalette Pal(QColor(146, 248, 252));
    adj->setPalette(Pal);
    adj->setAutoFillBackground(true);
    adj->setAdj(graph.getAdj());
    adj->setSize(graph.getVertexList().size());
    QVector<QString> names;
    for (auto elem : graph.getVertexList()){
        names.push_back(QString::number(elem));
    }
    adj->rebuild(names);
    adj->show();

}


void Dialog::on_btnAddEdge_clicked()
{

    graphVertex *node1, *node2;
    int vert1, vert2;
    //QString str1, str2;
    //qDebug() << ui->graphicsView->scene()->selectedItems();
    //если ничего не выбрано
    if (ui->graphicsView->scene()->selectedItems().size() == 1) {
        node1 = dynamic_cast<graphVertex *> (ui->graphicsView->scene()->selectedItems().at(0));
        node1->setPressed(true);
        if (for_draw.isEmpty()) for_draw.push_back(node1);
        else if (for_draw.front() != node1 && for_draw.front() != nullptr) {
            /// ВЫЗОВ ДИАЛОГОВОГО ОКНА ///
            //setWeight("1");
            //getWeight.exec();
            //connect(&getWeight, SIGNAL(sendWeight), this, SLOT(setWeight(QString)));
            int result = 1;
            GetWeight gw;
            gw.setWindowTitle("Вес ребра^^");
            gw.setWindowIcon(QIcon(":/myIcon.ico"));
            QPalette Pal(QColor(173, 252, 146));
            gw.setPalette(Pal);
            gw.setAutoFillBackground(true);
            if (gw.exec()) result = gw.weight;
            //qDebug() << result;
            node2 = for_draw.front();
            // str1 = node1->getContent(); str2 = node2->getContent();
            //str1 = str1.mid(1,1); str2 = str2.mid(1,1);
            vert1 = node1->getNum(); vert2 = node2->getNum();
            graph.InsertEdge(vert2, vert1, result);  //graph.InsertEdge(vert1, vert2, result);
            edge = new Edge(node1, node2);
            edge->setGraph(&graph);
            if (node1->pressed() == true) node1->setPressed(false);
            if (node2->pressed() == true) node2->setPressed(false);
            edge->setContent(result);
            scene->addItem(edge);
            edges.push_back(edge);
            for_draw.pop_back();
        }

    }
    ui->graphicsView->scene()->clearSelection();
}

void Dialog::on_btnDelete_clicked()
{
    if (ui->graphicsView->scene()->selectedItems().size()!=0){
        int last = ui->graphicsView->scene()->selectedItems().size()-1;
        auto current = ui->graphicsView->scene()->selectedItems().at(last);
        delete current;
    }

}


void Dialog::on_btnTSP_clicked()
{

    QString res = TSP(graph);
    ShowInfo info;
    info.setWindowTitle("AAAAAAAA");
    info.setWindowIcon(QIcon(":/myIcon.ico"));
    QPalette Pal(QColor(252, 199, 146));
    info.setPalette(Pal);
    info.setAutoFillBackground(true);

    info.setInfo(res);
    if (!info.exec()) qDebug() << "Error";
}

