
/*
    cube.cpp
    Yura Hernandez - Juan Jose Bolivar

        Operaciones con bits:
        Prender:
                mask |= (1 << pos_bit_a_encender)
        Apagar:
                mask &= ~(1 << pos_bit_a_apagar)
        Verificar:
                mask & (1 << pos_bit_a_verificar)
*/

#include <bits/stdc++.h>

using namespace std;

const int DR[] = {0,1,0,-1};
const int DC[] = {1,0,-1,0};

array<int,3> cubeOrientations[24];
int orientationIndex[6][6][6];
int moveResult[24][4];


// =========================== [ Cube functions ] ===========================

/*
    bottom, south,  east
    {1,     5,        4}
    {6,     2         3}


    moveSouth() {
        Original: {1,     5,      4}
        Aux:      {6,     2       3}
        ++++++++++++++++++++++++++++
        Original: {5,     6,      4}
        Aux:      {2,     1,      3}
        ++++++++++++++++++++++++++++
        Original: {6,     2,      4}
        Aux:      {1,     5,      4}
        ++++++++++++++++++++++++++++
        Original: {2,     1,      4}
        Aux:      {5,     6,      4}
        ++++++++++++++++++++++++++++
        Original: {1,     5,      4}
        Aux:      {6,     2,      4}
    }

    moveNorth() {
        Original: {1,     5,      4}
        Aux:      {6,     2       3}
        ++++++++++++++++++++++++++++
        Original: {2,     1,      4}
        Aux:      {5,     6,      3}
        ++++++++++++++++++++++++++++
        Original: {6,     2,      4}
        Aux:      {1,     5,      4}
        ++++++++++++++++++++++++++++
        Original: {5,     6,      4}
        Aux:      {2,     1,      4}
        ++++++++++++++++++++++++++++
        Original: {1,     5,      4}
        Aux:      {6,     2,      4}
    }


    moveEast() {
        Original: {1,     5,      3}
        Aux:      {6,     2       4}
        ++++++++++++++++++++++++++++
        Original: {3,     5,      6}
        Aux:      {4,     2,      1}
        ++++++++++++++++++++++++++++
        Original: {6,     5,      4}
        Aux:      {1,     2,      3}
        ++++++++++++++++++++++++++++
        Original: {4,     5,      1}
        Aux:      {3,     2,      6}
        ++++++++++++++++++++++++++++
        Original: {1,     5,      3}
        Aux:      {6,     2,      4}
    }

    moveWeast() {
        Original: {1,     5,      3}
        Aux:      {6,     2       4}
        ++++++++++++++++++++++++++++
        Original: {4,     5,      1}
        Aux:      {3,     2,      6}
        ++++++++++++++++++++++++++++
        Original: {6,     5,      4}
        Aux:      {1,     2,      3}
        ++++++++++++++++++++++++++++
        Original: {3,     5,      6}
        Aux:      {4,     2,      1}
        ++++++++++++++++++++++++++++
        Original: {1,     5,      3}
        Aux:      {6,     2,      4}
    }

*/



const int INVERSE[6] = {5,4,3,2,1,0};

void moveSouth(array<int,3> &cube) {
    int tmp = INVERSE[cube[0]];
    cube[0] = cube[1];
    cube[1] = tmp;
}
void moveNorth(array<int,3> &cube) {
    int tmp = INVERSE[cube[1]];
    cube[1] = cube[0];
    cube[0] = tmp;
}
void moveEast(array<int,3> &cube) {
    int tmp = INVERSE[cube[0]];
    cube[0] = cube[2];
    cube[2] = tmp;
}
void moveWeast(array<int,3> &cube) {
    int tmp = INVERSE[cube[2]];
    cube[2] = cube[0];
    cube[0] = tmp;
}

void precomputeCubeOrientations() {
    memset(orientationIndex, -1, sizeof(orientationIndex));
    int count = 0;

    queue<array<int,3>> q;
    array<int,3> inicial = {0, 4, 3};
    q.push(inicial);
    orientationIndex[0][4][3] = count;
    cubeOrientations[count++] = inicial;

    while (!q.empty()) {
        array<int,3> cur = q.front();
        q.pop();

        array<int,3> vecinos[4] = {cur, cur, cur, cur};
        moveEast(vecinos[0]);
        moveSouth(vecinos[1]);
        moveWeast(vecinos[2]);
        moveNorth(vecinos[3]);

        int i = 0;
        while (i < 4) {
            auto &v = vecinos[i];
            if (orientationIndex[v[0]][v[1]][v[2]] == -1) {
                orientationIndex[v[0]][v[1]][v[2]] = count;
                cubeOrientations[count++] = v;
                q.push(v);
            }
            i++;
        }
    }

    int ori = 0;
    while (ori < 24) {
        array<int,3> dirs[4] = {
            cubeOrientations[ori],
            cubeOrientations[ori],
            cubeOrientations[ori],
            cubeOrientations[ori]
        };
        moveEast(dirs[0]);
        moveSouth(dirs[1]);
        moveWeast(dirs[2]);
        moveNorth(dirs[3]);

        int d = 0;
        while (d < 4) {
            moveResult[ori][d] = orientationIndex[dirs[d][0]][dirs[d][1]][dirs[d][2]];
            d++;
        }
        ori++;
    }
}

int encodeState1(int r, int c, int ori, int goldFaces) {
    return r | (c << 3) | (ori << 6) | (goldFaces << 11);
}

struct PairHash {
    size_t operator()(const pair<int,uint64_t> &p) const {
        size_t h1 = hash<int>()(p.first);
        size_t h2 = hash<uint64_t>()(p.second);
        return h1 ^ (h2 * 1000000007ULL);
    }
};

typedef pair<int,uint64_t> Estado;
typedef tuple<int,int,uint64_t> PQEntry;

// =========================== [ END cube functions ] ===========================


bool hasGold(uint64_t b, int r, int c) {
    return b & (1ULL << (r*8+c));
}

bool faceHasGold(int cubeFace, int goldMask) {
    return goldMask & (1 << cubeFace);
}

void solve(int sr, int sc, int R, int C, int A, int B, uint64_t initialBoardMask) {

    unordered_map<Estado, int, PairHash> dist;
    priority_queue<PQEntry, vector<PQEntry>, greater<PQEntry>> pq;

    int oriInicial = orientationIndex[0][4][3];
    int s1 = encodeState1(sr, sc, oriInicial, 0);
    uint64_t s2 = initialBoardMask;

    Estado estadoInicial = make_pair(s1, s2);
    dist[estadoInicial] = 0;
    pq.push(make_tuple(0, s1, s2));

    bool found = false;
    int ans = -1;

    while (!pq.empty() && !found) {

        int cost = get<0>(pq.top());
        int estado1 = get<1>(pq.top());
        uint64_t estado2 = get<2>(pq.top());
        pq.pop();

        int er = estado1 & 7;
        int ec = (estado1 >> 3) & 7;
        int eOri = (estado1 >> 6) & 31;
        int eGoldFaces = (estado1 >> 11) & 63;
        uint64_t eBoardMask = estado2;

        if (eBoardMask == 0) {
            found = true;
            ans = cost;
        }

        Estado claveActual = make_pair(estado1, estado2);
        if (!found && dist.count(claveActual) && dist[claveActual] < cost) {

        } else if (!found) {
            int d = 0;
            while (d < 4) {
                int nr = er + DR[d];
                int nc = ec + DC[d];

                if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                    int newOri = moveResult[eOri][d];
                    int newGoldFaces = eGoldFaces;
                    uint64_t newBoardMask = eBoardMask;
                    int newCost = cost;

                    int bottomFace = cubeOrientations[newOri][0];

                    bool cellHasGoldB = hasGold(newBoardMask, nr, nc);
                    bool faceHasGoldB = faceHasGold(bottomFace, newGoldFaces);

                    if (cellHasGoldB && !faceHasGoldB) {
                        newBoardMask &= ~(1ULL << (nr*8+nc));
                        newGoldFaces |= (1 << bottomFace);
                        newCost += B;
                    } else if (cellHasGoldB && faceHasGoldB) {
                        newCost += A;
                    } else if (!cellHasGoldB && faceHasGoldB) {
                        newBoardMask |= (1ULL << (nr*8+nc));
                        newGoldFaces &= ~(1 << bottomFace);
                        newCost += A;
                    } else {
                        newCost += A;
                    }

                    int newEstado1 = encodeState1(nr, nc, newOri, newGoldFaces);
                    Estado nuevaClave = make_pair(newEstado1, newBoardMask);

                    if (!dist.count(nuevaClave) || newCost < dist[nuevaClave]) {
                        dist[nuevaClave] = newCost;
                        pq.push(make_tuple(newCost, newEstado1, newBoardMask));
                    }
                }
                d++;
            }
        }
    }

    if (found)
        cout << "Screw you guys, I got all the gold for " << ans << " cost!" << endl;
    else
        cout << "Oh my God, they killed Kenny!" << endl;
}

int main() {
    precomputeCubeOrientations();

    int T, R, C, A, B;
    cin >> T;

    while (T-- > 0) {
        uint64_t boardMask = 0;
        int sr = -1, sc = -1;
        char aux0;

        cin >> R >> C >> A >> B;

        int i = 0;
        while (i < R) {
            int j = 0;
            while (j < C) {
                cin >> aux0;
                if (aux0 == 'G')
                    boardMask |= (1ULL << (i*8+j));
                else if (aux0 == 'S') {
                    sr = i;
                    sc = j;
                }
                j++;
            }
            i++;
        }

        solve(sr, sc, R, C, A, B, boardMask);
    }

    return 0;
}