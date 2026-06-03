/*
    cube.cpp
    Yura Hernandez - Juan Jose Bolivar

    Operaciones con bits:
        Prender bit:
            mask |= (1 << pos_a_prender)
        Apagar bit:
            mask &= ~(1 << pos_a_apagar)
        verificar bit:
            mask & (1 << pos_a_verificar)

    Complejidad:
        O()
*/

#include <bits/stdc++.h>
using namespace std;

const int DR[] = {0, 1, 0, -1};
const int DC[] = {1, 0, -1, 0};

const int EAST_DIR = 0;
const int SOUTH_DIR = 1;
const int WEST_DIR = 2;
const int NORTH_DIR = 3;

const int DOWN = 0;
const int SOUTH = 1;
const int EAST = 2;
const int NORTH = 3;
const int WEST = 4;
const int UP = 5;

uint64_t cellBit[8][8];

bool bit(int mask, int p) {
    return (mask & (1 << p)) != 0;
}

void setBit(int &mask, int pos, bool val) {
    if (val)
        mask |= (1 << pos);
    else
        mask &= ~(1 << pos);
}

int rotateMask(int mask, int d) {
    int nm = mask;

    if (d == EAST_DIR) {
        setBit(nm, DOWN, bit(mask, WEST));
        setBit(nm, EAST, bit(mask, DOWN));
        setBit(nm, UP, bit(mask, EAST));
        setBit(nm, WEST, bit(mask, UP));
    }
    else if (d == WEST_DIR) {
        setBit(nm, DOWN, bit(mask, EAST));
        setBit(nm, WEST, bit(mask, DOWN));
        setBit(nm, UP, bit(mask, WEST));
        setBit(nm, EAST, bit(mask, UP));
    }
    else if (d == SOUTH_DIR) {
        setBit(nm, DOWN, bit(mask, NORTH));
        setBit(nm, SOUTH, bit(mask, DOWN));
        setBit(nm, UP, bit(mask, SOUTH));
        setBit(nm, NORTH, bit(mask, UP));
    }
    else {
        setBit(nm, DOWN, bit(mask, SOUTH));
        setBit(nm, NORTH, bit(mask, DOWN));
        setBit(nm, UP, bit(mask, NORTH));
        setBit(nm, SOUTH, bit(mask, UP));
    }

    return nm;
}

int encodeState1(int r, int c, int goldFaces) {
    return r | (c << 3) | (goldFaces << 6);
}

struct PairHash {
    size_t operator()(const pair<int, uint64_t> &p) const {
        size_t h1 = hash<int>()(p.first);
        size_t h2 = hash<uint64_t>()(p.second);
        return h1 ^ (h2 * 1000000007ULL);
    }
};

typedef pair<int, uint64_t> Estado;
typedef tuple<int, int, uint64_t> PQEntry;

bool hasGold(uint64_t boardMask, int r, int c) {
    return (boardMask & cellBit[r][c]) != 0;
}

bool faceHasGold(int face, int goldMask) {
    return (goldMask & (1 << face)) != 0;
}

void solve(int sr, int sc, int R, int C, int A, int B, uint64_t initialBoardMask) {
    unordered_map<Estado, int, PairHash> dist;

    priority_queue<PQEntry, vector<PQEntry>, greater<PQEntry> > pq;

    int s1 = encodeState1(sr, sc, 0);
    Estado estadoInicial = make_pair(s1, initialBoardMask);

    dist[estadoInicial] = 0;
    pq.push(make_tuple(0, s1, initialBoardMask));

    bool found = false;
    int ans = 0;

    while (!pq.empty() && !found) {
        int cost = get<0>(pq.top());
        int estado1 = get<1>(pq.top());
        uint64_t boardMask = get<2>(pq.top());
        pq.pop();

        Estado claveActual = make_pair(estado1, boardMask);
        unordered_map<Estado, int, PairHash>::iterator itActual = dist.find(claveActual);

        int er = estado1 & 7;
        int ec = (estado1 >> 3) & 7;
        int goldFaces = (estado1 >> 6) & 63;

        if (boardMask == 0) {
            found = true;
            ans = cost;
        }

        if (!found && itActual->second == cost) 
        {
            int d = 0;
            while (d < 4) {
                int nr = er + DR[d];
                int nc = ec + DC[d];

                if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                    int newGoldFaces = rotateMask(goldFaces, d);
                    uint64_t newBoardMask = boardMask;
                    int newCost = cost;

                    bool cellHasGoldB = hasGold(newBoardMask, nr, nc);
                    bool bottomHasGold = faceHasGold(DOWN, newGoldFaces);

                    if (cellHasGoldB && !bottomHasGold) {
                        // Recoger oro
                        newBoardMask &= ~cellBit[nr][nc];
                        newGoldFaces |= (1 << DOWN);
                        newCost += B;
                    }
                    else if (cellHasGoldB && bottomHasGold) {
                        // Hay oro en la celda y también abajo
                        newCost += A;
                    }
                    else if (!cellHasGoldB && bottomHasGold) {
                        // Dejar oro: costo A
                        newBoardMask |= cellBit[nr][nc];
                        newGoldFaces &= ~(1 << DOWN);
                        newCost += A;
                    }
                    else {
                        // Celda vacía y cara abajo vacía
                        newCost += A;
                    }

                    int newEstado1 = encodeState1(nr, nc, newGoldFaces);
                    Estado nuevaClave = make_pair(newEstado1, newBoardMask);

                    unordered_map<Estado, int, PairHash>::iterator it = dist.find(nuevaClave);

                    if (it == dist.end() || newCost < it->second) {
                        dist[nuevaClave] = newCost;
                        pq.emplace(make_tuple(newCost, newEstado1, newBoardMask));
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

    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            cellBit[i][j] = 1ULL << (i * 8 + j);
            j++;
        }
        i++;
    }

    int T, R, C, A, B;
    cin >> T;

    while (T-- > 0) {
        uint64_t boardMask = 0;
        int sr = -1, sc = -1;
        char aux0;

        cin >> R >> C >> A >> B;

        i = 0;
        while (i < R) {
            int j = 0;
            while (j < C) {
                cin >> aux0;

                if (aux0 == 'G') {
                    boardMask |= cellBit[i][j];
                }
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
