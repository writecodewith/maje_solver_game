#include <bits/stdc++.h>
#include <unistd.h>
#include <random>
using namespace std;

// 🎨 ANSI COLORS
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define WHITE "\033[37m"

#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'

int n = 21, m = 41;
vector<vector<char>> maze;

// 🔹 Random generator (FIX for warning)
mt19937 rng(time(0));

// 🔹 Directions
int dr[] = {0, 0, 2, -2};
int dc[] = {2, -2, 0, 0};

// 🔹 Clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 🔹 Print Maze (COLORFUL)
void printMaze(int pr=-1, int pc=-1, vector<pair<int,int>> path={}) {
    set<pair<int,int>> pathSet(path.begin(), path.end());

    for(int i=0;i<n;i++) {
        for(int j=0;j<m;j++) {

            if(i==pr && j==pc)
                cout << BLUE << "O" << RESET;

            else if(i==1 && j==1)
                cout << GREEN << "S" << RESET;

            else if(i==n-2 && j==m-2)
                cout << RED << "E" << RESET;

            else if(maze[i][j]==WALL)
                cout << WHITE << "#" << RESET;

            else if(pathSet.count({i,j}))
                cout << YELLOW << "*" << RESET;

            else
                cout << " ";
        }

        // 👉 SIDE PANEL
        if(i==2) cout << "     🎮 MAZE GAME";
        if(i==4) cout << "     1. New Game";
        if(i==5) cout << "     2. DFS Solve";
        if(i==6) cout << "     3. BFS Solve";
        if(i==7) cout << "     4. Exit";

        cout << endl;
    }
}

// 🔹 Generate Maze (FIXED shuffle)
void generateMaze() {
    maze.assign(n, vector<char>(m, WALL));

    function<void(int,int)> dfs = [&](int r, int c) {
        maze[r][c] = PATH;

        vector<int> dirs = {0,1,2,3};
        shuffle(dirs.begin(), dirs.end(), rng); // ✅ FIX

        for(int i : dirs) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if(nr>0 && nc>0 && nr<n-1 && nc<m-1 && maze[nr][nc]==WALL) {
                maze[r + dr[i]/2][c + dc[i]/2] = PATH;
                dfs(nr,nc);
            }
        }
    };

    dfs(1,1);

    maze[1][1] = START;
    maze[n-2][m-2] = END;
}

// 🔹 BFS
vector<pair<int,int>> bfs() {
    vector<vector<bool>> vis(n, vector<bool>(m,false));
    map<pair<int,int>, pair<int,int>> parent;

    queue<pair<int,int>> q;
    q.push({1,1});
    vis[1][1] = true;

    int d4r[] = {0,0,1,-1};
    int d4c[] = {1,-1,0,0};

    while(!q.empty()) {
        auto [r,c] = q.front(); q.pop();

        if(r==n-2 && c==m-2) break;

        for(int i=0;i<4;i++) {
            int nr=r+d4r[i], nc=c+d4c[i];

            if(nr>=0 && nc>=0 && nr<n && nc<m &&
               !vis[nr][nc] && maze[nr][nc]!=WALL) {

                vis[nr][nc]=true;
                parent[{nr,nc}] = {r,c};
                q.push({nr,nc});
            }
        }
    }

    vector<pair<int,int>> path;
    pair<int,int> cur = {n-2,m-2};

    while(cur != make_pair(1,1)) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back({1,1});
    reverse(path.begin(), path.end());

    return path;
}

// 🔹 DFS
vector<pair<int,int>> dfsSolve() {
    vector<vector<bool>> vis(n, vector<bool>(m,false));
    map<pair<int,int>, pair<int,int>> parent;

    stack<pair<int,int>> st;
    st.push({1,1});

    int d4r[] = {0,0,1,-1};
    int d4c[] = {1,-1,0,0};

    while(!st.empty()) {
        auto [r,c] = st.top(); st.pop();

        if(vis[r][c]) continue;
        vis[r][c]=true;

        if(r==n-2 && c==m-2) break;

        for(int i=0;i<4;i++) {
            int nr=r+d4r[i], nc=c+d4c[i];

            if(nr>=0 && nc>=0 && nr<n && nc<m &&
               !vis[nr][nc] && maze[nr][nc]!=WALL) {

                parent[{nr,nc}] = {r,c};
                st.push({nr,nc});
            }
        }
    }

    vector<pair<int,int>> path;
    pair<int,int> cur = {n-2,m-2};

    while(cur != make_pair(1,1)) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back({1,1});
    reverse(path.begin(), path.end());

    return path;
}

// 🔹 Animation
void animate(vector<pair<int,int>> path, string title) {
    for(auto &p : path) {
        clearScreen();
        cout << title << "\n\n";
        printMaze(p.first, p.second);
        usleep(50000);
    }

    clearScreen();
    cout << title << " (Final Path)\n\n";
    printMaze(-1, -1, path);
}

// 🎮 MAIN
int main() {
    generateMaze();

    while(true) {
        clearScreen();
        printMaze();

        int choice;
        cout << "\nEnter choice: ";
        cin >> choice;

        if(choice == 1) generateMaze();

        else if(choice == 2) {
            auto path = dfsSolve();
            animate(path, "DFS Solving...");
            cin.get(); cin.get();
        }

        else if(choice == 3) {
            auto path = bfs();
            animate(path, "BFS Shortest Path...");
            cin.get(); cin.get();
        }

        else if(choice == 4) break;
    }
}
