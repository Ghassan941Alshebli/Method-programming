#include "graph.h"

#include <QMap>
#include <QSet>
#include <QStringList>

bool dfs(const QString& node,
         const QString& parent,
         QMap<QString, QList<QString>>& graph,
         QSet<QString>& visited)
{
    visited.insert(node);

    for (const QString& neighbor : graph[node]) {
        if (!visited.contains(neighbor)) {
            if (dfs(neighbor, node, graph, visited))
                return true;
        }
        else if (neighbor != parent) {
            return true;
        }
    }

    return false;
}

bool hasCycle(const QString& graphData)
{
    QMap<QString, QList<QString>> graph;

    QStringList edges = graphData.split(",", Qt::SkipEmptyParts);

    for (const QString& edge : edges) {
        QStringList nodes = edge.split("-");

        if (nodes.size() == 2) {
            QString a = nodes[0].trimmed();
            QString b = nodes[1].trimmed();

            graph[a].append(b);
            graph[b].append(a);
        }
    }

    QSet<QString> visited;

    for (const QString& node : graph.keys()) {
        if (!visited.contains(node)) {
            if (dfs(node, "", graph, visited))
                return true;
        }
    }

    return false;
}