#include <vector>
#include <iostream> 
#include <tuple>
#include <limits>

struct Edge {
  int from;
  int to;
  long long int wt;
};

long long int BellmanFord(int n, int start, int finish, std::vector<Edge> allEdges) {
  std::vector<long long int> dist(n, 1e18); 
  dist[start] = 0;

  while (true) {
    bool flag = false;
    for (int i = 0; i < allEdges.size(); i++) {
      int u = allEdges[i].from - 1;
      int v = allEdges[i].to - 1;
      long long int w = allEdges[i].wt; 

      if (dist[u] < 1e18 && dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w; 
        flag = true;
      }
    }

    if (!flag) {
      break;
    }
  }

  return dist[finish];
}

int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n = 0, m = 0, start = 0, finish = 0;

  std::cin >> n >> m >> start >> finish;
  std::vector<Edge> allEdges;

  int from = 0, to = 0;
  long long int len = 0;
  allEdges.resize(m);

  for (int i = 0; i < m; i++) {
    std::cin >> allEdges[i].from >> allEdges[i].to >> allEdges[i].wt;
  }

  long long int dist = BellmanFord(n, start - 1, finish - 1, allEdges);

  if (dist == 1e18) {
    std::cout << "No solution" << std::endl; 
  } else {
    std::cout << dist << std::endl; 
  } 
}