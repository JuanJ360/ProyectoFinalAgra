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

struct State {
    int r;
    int c;
    array<int,3> cube = {0, 4, 3};
    int goldFaces = 0;
    uint64_t boardMask;

    State() {

    }

    State(int _r_, int _c_, uint64_t _boardMask_) {
        r = _r_;
        c = _c_;
        boardMask = _boardMask_;
    }

    bool operator<(const State &othr ) const {
        bool ans;
        if (r != othr.r)
            ans = r < othr.r;
        else if (c != othr.c)
            ans = c < othr.c;
        else if (cube != othr.cube)
            ans = cube < othr.cube;
        else if (goldFaces != othr.goldFaces)
            ans = goldFaces < othr.goldFaces;
        else
            ans = boardMask < othr.boardMask;

        return ans;
    }
};

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

const int INVERSE[6] = {5, 4, 3, 2, 1, 0};

void moveSouth(array<int, 3> &cube) {
    int tmp = INVERSE[cube[0]];
    cube[0] = cube[1];
    cube[1] = tmp;
}

void moveNorth(array<int, 3> &cube) {
    int tmp = INVERSE[cube[1]];
    cube[1] = cube[0];
    cube[0] = tmp;
}

void moveEast(array<int, 3> &cube) {
    int tmp = INVERSE[cube[0]];
    cube[0] = cube[2];
    cube[2] = tmp;
}

void moveWeast(array<int, 3> &cube) {
    int tmp = INVERSE[cube[2]];
    cube[2] = cube[0];
    cube[0] = tmp;
}

// =========================== [ END cube functions ] ===========================

bool hasGold(uint64_t b, int r, int c) {
    return b & (1ULL << (r*8+c));
}

bool faceHasGold(int cubeFace, int goldMask) {
    return goldMask & (1 << cubeFace);
}

void solve(State &s, int R, int C, int A, int B) {
    State act;
    State newS;
    int cost, newCost, ans;
    bool found = false;
    map<State, int> dist;
    priority_queue<pair<int,State>, vector<pair<int, State>>, greater<pair<int, State>>> pq;

    dist[s] = 0;
    pq.push(make_pair(0, s));

    while (!pq.empty() && !found)
    {
        
        act = pq.top().second; cost = pq.top().first;
        pq.pop();

        if (!act.boardMask) {
            found = true;
            ans = cost;
        }

        if (cost == dist[act] && !found)
        {
            
            for (int i = 0; i < 4; i++)
            {
                newS = act;
                newCost = cost;

                newS.r += DR[i];
                newS.c += DC[i];

                if (newS.r >= 0 && newS.r < R && newS.c >= 0 && newS.c < C ) {

                    if (i == 0) moveEast(newS.cube);
                    else if (i == 1) moveSouth(newS.cube);
                    else if (i == 2) moveWeast(newS.cube);
                    else if (i == 3) moveNorth(newS.cube);

                    if (hasGold(newS.boardMask, newS.r, newS.c)) {

                        // No tiene oro y hay oro
                        if (!faceHasGold(newS.cube[0], newS.goldFaces))
                        {
                            newS.boardMask &= ~(1ULL << (newS.r*8+newS.c));
                            newS.goldFaces |= (1 << newS.cube[0]);
                            newCost += B;
                        }
                        else
                        {
                            // Tiene oro y hay oro
                            newCost += A;    
                        }

                    }
                    else {
                        // Tiene oro y no hay oro
                        if (faceHasGold(newS.cube[0], newS.goldFaces)) {
                            newCost += A;
                            newS.boardMask |= (1ULL << (newS.r * 8 + newS.c));
                            newS.goldFaces &= ~(1 << newS.cube[0]);
                        }
                        else
                        {
                            newCost += A;
                        }
                    }

                    if (!dist.count(newS) || newCost < dist[newS]) {
                        dist[newS] = newCost;
                        pq.push(make_pair(newCost, newS));
                    }
                        
                }

            }
        }
    }
    
    if (found)
        cout << "Screw you guys, I got all the gold for " << ans << " cost!" << endl;
    else
        cout << "Oh my God, they killed Kenny!" << endl;
}

int main() {

    int T, R, C, A, B;
    int r,c;
    char aux0;
    cin >> T;

    while (T-- > 0)
    {

        int count = 0;
        uint64_t boardMask = 0;

        cin >> R >> C >> A >> B;
        for (int i = 0; i < R; i++)
        {
            for (int j = 0; j < C; j++)
            {
                cin >> aux0;
                if (aux0 == 'G')
                    boardMask |= (1ULL << (i*8+ j));
                else if (aux0 == 'S') {
                    r = i;
                    c = j;
                }
            }   
        }
        
        State s(r, c, boardMask);

        solve(s, R, C, A, B);

    }

    return 0;
}
