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
    tuple <int,int,int,int,int,int> cube;
    array<int, 6> faces;
    int goldMask;

    State() {

    }

    State(int _r_, int _c_, tuple <int,int,int,int,int,int> _cube_, array<int, 6> _faces_, int _goldMask_) {
        r = _r_;
        c = _c_;
        cube = _cube_;
        faces = _faces_;
        goldMask = _goldMask_;
    }

    bool operator<(const State &othr ) const {
        bool ans;
        if (r != othr.r) {
            ans = r < othr.r;
        }
        else if (c != othr.c) {
            ans = c < othr.c;
        }
        else if (cube != othr.cube) {
            ans = cube < othr.cube;
        }
        else if (faces != othr.faces) {
            ans = faces < othr.faces;
        }
        else {
            ans = goldMask < othr.goldMask;
        }

        return ans;
    }
};

// =========================== [ Cube functions ] ===========================

void moveNorth(tuple<int,int,int,int,int,int> &cube) {
    int temp = get<0>(cube);
    get<0>(cube) = get<3>(cube);
    get<3>(cube) = get<2>(cube);
    get<2>(cube) = get<1>(cube);
    get<1>(cube) = temp;
}

void moveSouth(tuple<int,int,int,int,int,int> &cube) {
    int temp = get<0>(cube);
    get<0>(cube) = get<1>(cube);
    get<1>(cube) = get<2>(cube);
    get<2>(cube) = get<3>(cube);
    get<3>(cube) = temp;
}

void moveWest(tuple<int,int,int,int,int,int> &cube) {
    int temp = get<0>(cube);
    get<0>(cube) = get<5>(cube);
    get<5>(cube) = get<2>(cube);
    get<2>(cube) = get<4>(cube);
    get<4>(cube) = temp;
}

void moveEast(tuple<int,int,int,int,int,int> &cube) {
    int temp = get<0>(cube);
    get<0>(cube) = get<4>(cube);
    get<4>(cube) = get<2>(cube);
    get<2>(cube) = get<5>(cube);
    get<5>(cube) = temp;
}

// =========================== [ END cube functions ] ===========================

void solve(State &s, vector<vector<int>> &m, int A, int B) {
    
    int R = m.size();
    int C = m[0].size();

    int cost, tmp, extraCost, newCost, ans;
    State act;
    bool con = true;

    map<State, int> dist;
    dist[s] = 0;

    priority_queue<pair<int, State>, vector<pair<int, State>>, greater<pair<int,State>>> pq;
    pq.push(make_pair(0,s));


    while (!pq.empty() && con)
    {
        cost = pq.top().first; act = pq.top().second;
        pq.pop();

        if (act.goldMask == 0) {
            con = false;
            ans = dist[act];
        }
            

        if (cost == dist[act] && con) {

            for (int i = 0; i < 4; i++)
            {
                bool extra = false;
                bool valid = true;
                State newS = act;

                int dr = DR[i];
                int dc = DC[i];

                newS.r += dr;
                newS.c += dc;

                if (newS.r < 0 || newS.r >= R || newS.c < 0 || newS.c >= C)
                    valid = false;
                    
                if (valid) {
                    
                    if(i == 0) moveEast(newS.cube);
                    else if (i == 1) moveNorth(newS.cube);
                    else if (i == 2) moveWest(newS.cube);
                    else if (i == 3) moveSouth(newS.cube);


                    if (m[newS.r][newS.c] != -1) {
                        extra = true;
                        int bottom = get<0>(newS.cube);
                        int face = newS.faces[bottom];

                        // Encontre el oro ahora que?
                        // Modificamos el goldMask no el mapa general
                        
                    }

                    if (extra)
                        extraCost = B;
                    else
                        extraCost = A;

                    newCost = cost + extraCost;

                    if (!dist.count(newS) || newCost < dist[newS])
                    {
                        dist[newS] = newCost;
                        pq.push(make_pair(newCost, newS));
                    }
                }

            }
        }
    }
    
    if (!con)
    {
        cout << "Screw you guys, I got all the gold for " << ans << " cost!" << endl;
    }
    else {
        cout << "Oh my God, they killed Kenny!" << endl;
    }
}

int main()
{
    //       initial state cube
    //          0      1     2     3      4     5
    //       Bottom, south, top, north, weast, east
    // tuple <int,int,int,int,int,int> cube = {1,5,6,2,3,4};

    /*
    
        State = {
            r,
            c,
            cubeOrientation,
            faces,
            goldMask
        }
    
    */

    int T, R, C, A, B;
    int r,c;
    char aux0;
    cin >> T;

    while (T-- > 0)
    {

        int count = 0;
        array<int, 6> faces = {-1, -1, -1, -1, -1, -1};
        tuple<int,int,int,int,int,int> cube = {0,4,5,1,2,3};
        int goldMask = 63;

        cin >> R >> C >> A >> B;
        vector<vector<int>> m(R, vector<int>(C, -1));

        for (int i = 0; i < R; i++)
        {
            for (int j = 0; j < C; j++)
            {
                cin >> aux0;

                if (aux0 == 'S') {r = i; c = j;}
                else if (aux0 == 'G') {
                    m[i][j] = count++;
                }
            }
        }
        
        State s(r, c, cube, faces, goldMask);

        solve(s, m, A, B);
    }

    return 0;
}
