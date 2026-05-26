/*

    cube.cpp
    Yura Hernandez - Juan Jose Bolivar

*/

#include <bits/stdc++.h>

using namespace std;

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
            boardMask
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
        int boardMask = 63;

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
        

    }
    

    return 0;
}
