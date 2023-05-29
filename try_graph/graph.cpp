#include "graph.h"


Graph::Graph(const int &size)
{
    this->labelList = QVector<int>(size);
    this->maxSize = size;
    this->adjMatrix = QVector<QVector<int>>(size, QVector<int>(size));
    for (int i = 0; i < this->maxSize; ++i) {
        for (int j = 0; j < this->maxSize; ++j) this->adjMatrix[i][j] = 0;
    };
}

Graph::Graph() : Graph(10) {}

inline bool Graph::isEmpty() { return this->vertexList.size() == 0; };

inline bool Graph::isFull() { return this->vertexList.size() == this->maxSize;};

inline int Graph::GetVertPos(const int& v) {
    for (int i = 0; i < this->vertexList.size(); ++i) {
        if (this->vertexList[i] == v) return i;
    }
    return -1;
};

inline int Graph::GetAmountVerts() { return this->vertexList.size(); };


bool Graph::insertVertex(int& vert) {
    if (this->isFull())
    {
        qDebug() << "Невозможно добавить вершину.";
        return false;
    };
    if (this->GetVertPos(vert) == -1) {
        this->vertexList.push_back(vert);
        return true;
    }
    qDebug() << vertexList;
    return false;
};

int Graph::GetAmountEdges() {
    int amount = 0;
    if (!this->isEmpty()){
        for (int i = 0; i < this->vertexList.size(); ++i) {
            for (int j = 0; j < this->vertexList.size(); ++j) if (this->adjMatrix[i][j] != 0) amount++;
        }
    }
    return amount;
};

inline int Graph::GetWeight(const int& v1, const int& v2) {
    if (this->isEmpty()) return 0;

    int v1_p = this->GetVertPos(v1);
    int v2_p = this->GetVertPos(v2);

    if (v1_p == -1 || v2_p == -1) {
        qDebug() << "Одного из узлов в графе не существует." << endl;
        return 0;
    };

    return this->adjMatrix[v1_p][v2_p];
};

QVector<int> Graph::GetNbrs(const int& vertex) {
    QVector<int> nbrsList; // создание списка соседей
    int pos = this->GetVertPos(vertex); /* вычисление позиции vertex в матрице смежности */
    if (pos != -1) { /* проверка, что vertex есть в матрице смежности */
        for (int i = 0; i < this->vertexList.size(); ++i) {
            if (this->adjMatrix[pos][i] != 0) nbrsList.push_back(this->vertexList[i]);
        }
    }
    return nbrsList; // возврат списка соседей
}

QVector<int> Graph::getVertexList()
{
    return this->vertexList;
};


void Graph::InsertEdge(const int& vertex1, const int& vertex2, const int& weight) {
    if (GetVertPos(vertex1) != (-1) && this->GetVertPos(vertex2) != (-1)) {
        int vertPos1 = GetVertPos(vertex1);
        int vertPos2 = GetVertPos(vertex2);

        if (this->adjMatrix[vertPos1][vertPos2] != 0) {
            qDebug() << "Ребро между вершинами уже есть" << endl;
            return;
        }
        else this->adjMatrix[vertPos1][vertPos2] = weight;

    }
    else {
        qDebug() << "Обеих вершин (или одной из них) нет в графе " << endl;
        return;
    }
};

void Graph::showAdj()
{
    for (int i = 0; i < this->maxSize; ++i) qDebug() << this->adjMatrix[i] << ' ';

    qDebug() << endl;

}

QVector<QVector<int> > Graph::getAdj()
{
    return this->adjMatrix;
};

void Graph::eraseEdge(const int& vert, const int& vert2)
{
    int p1 = this->GetVertPos(vert);
    int p2 = this->GetVertPos(vert2);
    if (p1 != -1 && p2 != -1)
    {
        this->adjMatrix[p1][p2] = 0;
    }
};

void Graph::eraseVertex(const int& vert)
{
    int pos = this->GetVertPos(vert);
    if ( pos != -1)
    {
        for (int i = 0; i < this->vertexList.size(); ++i)
        {
            this->adjMatrix[i].erase(this->adjMatrix[i].begin() + pos);
        }
        this->adjMatrix.erase(this->adjMatrix.begin() + pos);
        this->vertexList.erase(this->vertexList.begin() + pos);
    }
};
