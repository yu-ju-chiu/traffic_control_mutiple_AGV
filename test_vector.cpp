#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

class NODE{
    public:
        char symbol;
        int  count;
        int row;
        int col; 
};

int main()
{
    NODE temp;

    vector<NODE> gem_list;

    vector<NODE> row;
    row.assign(3,temp);//配置一個 row 的大小
    vector< vector<NODE> > array_2D;
    array_2D.assign(3,row);//配置2維

    gem_list.push_back(temp);

    // .. 經過幾次push_back

    // for(int i=0; i<row.size(); i++)
    // {
    //     cout<<"1D"<<endl;
    //     cout<<row[i].symbol<<" "<<row[i].count<<endl;
    // }

    for(int i=0; i<row.size(); i++)
    {
        for(int j=0; j<array_2D.size(); j++)
        {
            NODE temp;
            temp.row = i;
            temp.col = j;
            array_2D[i][j] = temp;
            cout<<"2D"<<endl;
            cout<<array_2D[i][j].row<<" "<<array_2D[i][j].col<<endl;

        }
    }


    system("pause");
    return 0;

}