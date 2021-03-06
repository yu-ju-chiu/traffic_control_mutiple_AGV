#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>
#include <time.h>

#define METHOD 1

using namespace std;
// #define X_MAP 66
// #define Y_MAP 53
#define NODE_TIME 1
#define STAY_TIME 9999
// #define START_Y1 15
// #define START_X1 18
// #define END_Y1 10
// #define END_X1 31

// #define START_Y2 18
// #define START_X2 21
// #define END_Y2 12
// #define END_X2 33

int getFilerow(string fileName)
{
    string tmp;
    int count = 0;
    ifstream file{fileName};

    if (!file.is_open())
        return -1;

    while (getline(file, tmp, '\n'))
    {
        if (tmp.size() > 0)
            count++;
    }
    file.close();
    return count;
}
int getFilecol(string fileName)
{
    string tmp;
    ifstream file{fileName};
    int count = 0;
    char c;
    c = file.peek();
    
    if (!file.is_open())
        return -1;
    while (('\r' != c) && (!file.eof()))
    {
        file >> tmp;
        ++count;
        c = file.peek();
    }
    file.close();
    return count;
}
class NODE{
    public:
        NODE() : start_t(0), end_t(0), row(0), col(0), mark(0),agv(0){};
        ~NODE(){};
        double start_t;
        double  end_t;
        int row;
        int col; 
        int mark;
        int agv;
};

class CPoint
{
public:
    CPoint(int x, int y) : X(x), Y(y), G(0), H(0), F(0),T(0),n_start_t(-1),n_end_t(-1), m_parentPoint(NULL){};
    ~CPoint(){};
    void CalcF()
    {
        F = G + H + T;
        // cout << "F: " << F << endl;
        // cout << "G: " << G << endl;
        // cout << "H: " << H << endl;
        // cout << "T: " << T << endl;
    }
    int X;
    int Y;
    int G;
    int H;
    int F;
    int T;
    int n_start_t;
    int n_end_t;
    CPoint *m_parentPoint;
};

class CAStar
{
public:
    // 構造函數
    CAStar(vector< vector<NODE> > &map, int X,int Y)
    {
        NODE temp;
        vector<NODE> row;
        row.assign(1000,temp);//配置一個 row 的大小
        m_map.assign(1000,row);//配置2維

        for (int i = 0; i < Y; ++i)
            for (int j = 0; j < X; ++j)
            {
                m_map[i][j] = map[i][j];
                // cout<<"m_map.start_time:" << m_map[i][j].start_t<<endl;
            }
    }

    CPoint *GetMinFPoint()
    {
        int idx = 0, valueF = 99999;
        for (int i = 0; i < m_openVec.size(); i++)
        {
            if (m_openVec[i]->F < valueF)
            {
                valueF = m_openVec[i]->F;
                idx = i;
            }
        }
        return m_openVec[idx];
    }

    bool RemoveFromOpenVec(CPoint *point)
    {
        for (POINTVEC::iterator it = m_openVec.begin(); it != m_openVec.end(); ++it)
        {
            if ((*it)->X == point->X && (*it)->Y == point->Y)
            {
                m_openVec.erase(it);
                return true;
            }
        }
        return false;
    }

    bool canReach(int x, int y)
    {
        return 0 == m_map[x][y].mark;
    }

    bool IsAccessiblePoint(CPoint *point, int x, int y, bool isIgnoreCorner)
    {
        if (!canReach(x, y) || isInCloseVec(x, y))
            return false;
        else
        {
            //可達
            if (abs(x - point->X) + abs(y - point->Y) == 1) // 左右上下
                return true;
            else
            {
                if (canReach(abs(x - 1), y) && canReach(x, abs(y - 1))) // 對角
                    return true;
                else
                    return isIgnoreCorner; //牆角
            }
        }
    }

    std::vector<CPoint *> GetAdjacentPoints(CPoint *point, bool isIgnoreCorner)
    {
        POINTVEC adjacentPoints;
        for (int x = point->X - 1; x <= point->X + 1; ++x)
            for (int y = point->Y - 1; y <= point->Y + 1; ++y)
            {
                if (IsAccessiblePoint(point, x, y, isIgnoreCorner))
                {
                    CPoint *tmpPoint = new CPoint(x, y);
                    adjacentPoints.push_back(tmpPoint);
                }
            }

        return adjacentPoints;
    }

    bool isInOpenVec(int x, int y)
    {
        for (POINTVEC::iterator it = m_openVec.begin(); it != m_openVec.end(); ++it)
        {
            if ((*it)->X == x && (*it)->Y == y)
                return true;
        }
        return false;
    }

    bool isInCloseVec(int x, int y)
    {
        for (POINTVEC::iterator it = m_closeVec.begin(); it != m_closeVec.end(); ++it)
        {
            if ((*it)->X == x && (*it)->Y == y)
                return true;
        }
        return false;
    }

    void RefreshPoint(CPoint *tmpStart, CPoint *point, int start_time)
    {
        int valueG = CalcG(tmpStart, point);
        // start_time = start_time+NODE_TIME;
        if (valueG < point->G)
        {
            point->m_parentPoint = tmpStart;
            // point->n_start_t = start_time;
            // point->n_end_t = start_time+NODE_TIME;
            point->G = valueG;
            point->CalcF();
        }
    }

    void NotFoundPoint(vector< vector<NODE> > &map,CPoint *tmpStart, CPoint *end, CPoint *point,int start_time)
    {
        start_time = start_time+NODE_TIME;
        point->m_parentPoint = tmpStart;
        point->n_start_t = start_time;
        point->n_end_t = start_time+NODE_TIME;
        point->G = CalcG(tmpStart, point);
        point->T = CalcT(map,point);
        point->H = CalcH(end, point);
        point->CalcF();
        m_openVec.push_back(point);
    }

    int CalcG(CPoint *start, CPoint *point)
    {

        int G = (abs(point->X - start->X) + abs(point->Y - start->Y)) == 1 ? STEP : OBLIQUE;
        int parentG = point->m_parentPoint != NULL ? point->m_parentPoint->G : 0;
        // cout << "G + parentG: " << G + parentG << endl;
        return G + parentG;
    }

    int CalcT(vector< vector<NODE> > &map,CPoint *point)
    {
        int T = 0;
        if (abs(point->n_start_t - map[point->X][point->Y].start_t) < 1)
        {
            T = 9999; 
        }
        else if(abs(point->n_end_t - map[point->X][point->Y].end_t) < 1)
        {
            T = 9999;
        }
        // cout << "CalcT_point_start: " <<"("<< point->X <<","<< point->Y<<")"<< point->n_start_t << endl;
        // cout << "CalcT_map_start: " <<"("<< point->X <<","<< point->Y<<")"<< map[point->X][point->Y].start_t << endl;
        return T;
    }
    int CalcH(CPoint *end, CPoint *point)
    {
        double step = sqrt(pow(abs(point->X - end->X), 2) + pow(abs(point->Y - end->Y), 2));
        // cout << "step * STEP " << step * STEP << endl;
        return step * STEP;
    }

    // 搜索路徑
    CPoint *FindPath(vector< vector<NODE> > &map,CPoint *start, CPoint *end, bool isIgnoreCorner,int start_time)
    {
        m_openVec.push_back(start);
        while (0 != m_openVec.size())
        {
            CPoint *tmpStart = GetMinFPoint(); // 取F最小值
            // cout<<"F cor: "<<tmpStart->X<<","<<tmpStart->Y<<endl;
            // cout<<"F the smallest"<<tmpStart->F<<endl;

            RemoveFromOpenVec(tmpStart);
            m_closeVec.push_back(tmpStart);
            if (tmpStart->n_start_t != -1)
            {
                start_time = tmpStart->n_start_t;
            }
            
            map[tmpStart->X][tmpStart->Y].start_t = start_time;
            map[tmpStart->X][tmpStart->Y].end_t = start_time + NODE_TIME;


            POINTVEC adjacentPoints = GetAdjacentPoints(tmpStart, isIgnoreCorner);
            for (POINTVEC::iterator it = adjacentPoints.begin(); it != adjacentPoints.end(); ++it)
            {
                CPoint *point = *it;
                if (isInOpenVec(point->X, point->Y)) // 在開啟列表
                    RefreshPoint(tmpStart, point, start_time);
                //else if(inCloseVec(point))
                //{
                // 檢查節點的g值，如果新計算得到的路徑開銷比該g值低，那麼要重新打開該節點（即重新放入OPEN集）
                //}
                else
                    NotFoundPoint(map,tmpStart, end, point,start_time);
            }
            if (isInOpenVec(end->X, end->Y)) // 目標點已在開啟列表中
            {
                for (int i = 0; i < m_openVec.size(); ++i)
                {
                    if (end->X == m_openVec[i]->X && end->Y == m_openVec[i]->Y)
                    {
                        start_time = start_time + NODE_TIME;
                        map[end->X][end->Y].start_t = start_time;
                        map[end->X][end->Y].end_t = STAY_TIME;
                        return m_openVec[i];
                    }

                }
            }
            start_time = start_time + NODE_TIME;

        }
        map[end->X][end->Y].start_t = start_time;
        map[end->X][end->Y].end_t = STAY_TIME;

        return end;
    }

private:
    vector< vector<NODE> > m_map;
    static const int STEP = 10;
    static const int OBLIQUE = 14;

    typedef std::vector<CPoint *> POINTVEC;
    POINTVEC m_openVec;
    POINTVEC m_closeVec;
};

int main()
{
    cout<<"auto find the size of txt"<<endl;
    int num_node = 0;
    int method = 0;
    int num = 0;
    int sx_1, sy_1, ex_1, ey_1;
    int sx_2, sy_2, ex_2, ey_2;
    // input the map
    int X = 0;
    int Y = 0;
    string filename{"factory_map.txt"};
    double start_t, end_t;

    //right now time
    time_t timep; 
    time (&timep); 
    printf("%s",ctime(&timep)); 
    //open csv
    std::ofstream myfile;
    myfile.open ("path.csv");
    myfile << ctime(&timep);
    myfile << "\n";
    

    X = getFilecol(filename);
    Y = getFilerow(filename);
    cout << "map size (X,Y):" <<"("<< X <<","<<Y<<")"<< endl;

    int array[1000][1000]{};
    //input the file
    ifstream file{"factory_map.txt"};
    if (!file.is_open())
        cout << "can't open file" << endl;

    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            file >> array[i][j];
        }
    }
    file.close();
    // show the map
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (array[i][j] == 1)
                cout << "X";
            else
                cout << " ";
        }
        cout << " " << i << endl;
    }
    for (int j = 0; j < X; j++)
    {
        if (j % 5 == 0)
        {
            cout << j;
            cout << "   ";
        }
    }
    //array convert to map
    NODE temp;
    vector<NODE> row;
    row.assign(X,temp);//配置一個 row 的大小
    vector< vector<NODE> > map;
    map.assign(Y,row);//配置2維
    for(int i=0; i<Y; i++)
    {
        for(int j=0; j<X; j++)
        {
            NODE temp;
            temp.start_t = 0;
            temp.end_t = 0;
            temp.row = i;
            temp.col = j;
            temp.mark = array[i][j];
            map[i][j] = temp;
        }
    }
    cout << endl;
    cout << "the number of the car" << endl;
    cin >> num;
    int start_time = 0;
    int wait_time = 0;

    myfile << "AGV_path; ;AGV_time \n";

    for(int k = 1 ; k<=num;k++)
    {
        cout << "enter AGV"<<k<<"start X" << endl;
        cin >> sx_1;
        cout << "enter AGV"<<k<<"start Y" << endl;
        cin >> sy_1;
        cout << "enter AGV"<<k<<"end X" << endl;
        cin >> ex_1;
        cout << "enter AGV"<<k<<"end Y" << endl;
        cin >> ey_1;
        myfile << "AGV_"<<k<<"\n";
        if (k>1)
        {
            start_time = 0 + wait_time;
            cout << "start time ?(s))"<< endl;
            cin >> wait_time;    
            start_time = start_time + wait_time;   
        }

        CAStar *pAStar = new CAStar( map, X, Y);
        CPoint *start = new CPoint(sy_1, sx_1);
        CPoint *end = new CPoint(ey_1, ex_1);
        CPoint *point = pAStar->FindPath(map,start, end, false, start_time);
        CPoint *head = new CPoint(sy_1, sx_1);
        head->m_parentPoint = point;
        point->n_start_t = start_time;
        point->n_end_t = start_time+NODE_TIME;

        num_node = 0;
        
        while (point != NULL)
        {
            cout << "AGV:"<< k 
                 << "(" << point->Y << "," << point->X << ");" << "//";
            
            map[point->X][point->Y].agv = k;
            cout << "time:"
                 << "(" << map[point->X][point->Y].start_t << "," << map[point->X][point->Y].end_t << ");" << endl;
            myfile <<point->Y<<";"<<point->X<<";"<<map[point->X][point->Y].start_t<<";"<< map[point->X][point->Y].end_t<<"\n";
            point = point->m_parentPoint;
            num_node++;
        }
        map[ey_1][ex_1].agv = -2;
        map[sy_1][sx_1].agv = -1;
    }

    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (map[i][j].mark == 1)
                cout << "X";
            else if (map[i][j].agv == -2)
                cout << "E";
            else if (map[i][j].agv == -1)
                cout << "S";
            else if (map[i][j].agv != 0)
                cout << map[i][j].agv;
            else
                cout << " ";
        }
        cout << endl;
    }

    myfile.close();
    // system("pause");
    return 0;
}