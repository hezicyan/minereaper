#include <game.h>

int cnt[20][20], tot[20][20];
bool vis[20][20];
double p[20][20];
const int dx[] = {1, 0, -1, 0, 1, -1, 1, -1};
const int dy[] = {0, 1, 0, -1, 1, -1, -1, 1};
int n, m, sum;
std::vector<std::pair<int, int>> mine; 
std::vector<std::pair<int, int>> block;
void dfs(const game::Game& game, int x, int y) {
  if (x >= n) {
    sum++;
    for (auto u : mine) tot[u.first][u.second]++;
    return; 
  } 
  if (vis[x][y]) return;
  vis[x][y] = 1;
  auto now = game.GetCell(game::Coord(x, y));
  if (now.revealed) {
    if (y == m - 1) dfs(game, x + 1, 0);
    else dfs(game, x, y + 1);
  }
  block.push_back(std::make_pair(x, y));
  bool flag = 1;
  for (int i = 0; i < 8; i++) {
    int xx = x + dx[i], yy = y + dy[i];
    if (xx >= 0 and xx < n and yy >= 0 and yy <= m) {
      cnt[xx][yy]++;
      auto nnow = game.GetCell(game::Coord(xx, yy));
      if (cnt[xx][yy] > nnow.number) flag = 0;
    }
  }
  mine.push_back(std::make_pair(x, y));
  if (flag) {
    if (y == m - 1) dfs(game, x + 1, 0);
    else dfs(game, x, y + 1);
  }
  mine.back();
  for (int i = 0; i < 8; i++) {
    int xx = x + dx[i], yy = y + dy[i];
    if (xx >= 0 and xx < n and yy >= 0 and yy <= m) {
      cnt[xx][yy]--;
    }
  }
  if (flag) {
    if (y == m - 1) dfs(game, x + 1, 0);
    else dfs(game, x, y + 1);
  }
}
void ai::GetPossibility(const game::Game& game) {
  memset(cnt, 0, sizeof(cnt));
  memset(vis, 0, sizeof(vis));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (vis[i][j]) continue;
      sum = 0;
      block.clear();
      dfs(game, i, j);
      for (auto u : block) p[u.first][u.second] = tot[u.first][u.second] * 1.0 / sum;
    }
  }
} 
