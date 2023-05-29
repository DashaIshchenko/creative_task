#ifndef TSPALGORITHM_H
#define TSPALGORITHM_H
#pragma once
#include "graph.h"

void RestoreWay(QVector<QVector<int>>& adj, QVector<int>& path, int size, QVector<QVector<float>>& masked) {
    size_t i = 0;
    int mask = (1 << size) - 1;
    path.push_back(0); //starting vertex is always the first one :)
    while (mask != 0) {
        for (size_t j = 0; j < size; j++) {
            if (adj[i][j]) { //if edge exists
                if ((mask & (1 << j))) {
                    if (masked[mask][i] == adj[i][j] + masked[mask ^ (1 << j)][j]) { // ^ stands for XOR
                        path.push_back(j);
                        i = j;
                        mask = mask ^ (1 << j); // ^ stands for XOR
                    }
                }
            }
        }
    }
}

float MaskTSP(int i, int mask, QVector<QVector<int>>& adj, int size, QVector<QVector<float>>& masked) {
    // vertex should be handeled once. unprocessed vertexes have infinity value
    if (masked[mask][i] != Q_INFINITY) return masked[mask][i];

    for (size_t j = 0; j < size; j++) {
        if (adj[i][j]) { //if edge exists
            if ((mask & (1 << j))) { //mask without j
                //finding minimal weight by comparing current weight and summary of path to any neighbor and their weight
                masked[mask][i] = std::min(masked[mask][i], MaskTSP(j, mask - (1 << j), adj, size, masked) + adj[i][j]);

            }
            /*

            */
        }
    }
    return masked[mask][i];
}

QString TSP(Graph graph) {
    QString res = "Shortest way: ";
    int size = graph.getVertexList().size();

    //2**size = 1 << size (left shift)
    QVector<QVector<float>>masked((1 << size), QVector<float>(size));
    //has to be system data type (not template data type) in order to compare correctly

    QVector<QVector<int>> adj = graph.getAdj();

    // === making masks for every path === //

    for (size_t i = 0; i < size; i++)
        for (size_t mask = 0; mask < (1 << size); mask++)
            masked[mask][i] = Q_INFINITY;
    /*
    moving the bits of an int (or type T in general)
    */

    // === processing masked adjacency matrix === //
    masked[0][0] = 0; //starting vertex is always the first one :)

    // === answer === //
    int ans = MaskTSP(0, (1 << size) - 1, adj, size, masked);
    res += QString::number( ans );
    res += "\n\nTravelling salesman way is: ";

    // === print path === //
    QVector<int> path;
    RestoreWay(adj, path, size, masked);
    for (size_t i = 0; i < size; ++i) res  += QString::number(path[i]) + " -> ";
    res += "0"; //starting vertex is always the first one :)
    return res;
}
#endif // TSPALGORITHM_H
