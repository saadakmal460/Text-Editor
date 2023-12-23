#include <iostream>
#include<stack>
#include<deque>
#include<list>
#include<fstream>
using namespace std;

list<string> files;


struct state
{
    int row;
    int column;
    list<list<char>> text;
    list<list<char>>::iterator rowIter;
    list<char>::iterator columnIter;
};

class TextEditor
{
public:
    int currentRow;
    int currentColumn;
    list<list<char>> text;
    list<list<char>>::iterator rowIter;
    list<char>::iterator columnIter;
    deque<state> undo;
    stack<state> redo;

    TextEditor()
    {
        text.push_back(list<char>());
        rowIter = text.begin();
        (*rowIter).push_back(' ');
        columnIter = (*rowIter).begin();
        currentRow = 0;
        currentColumn = 0;
    }

    state SaveState()
    {
        state* s = new state();
        s->text.push_back(list<char>());
        auto iterRow = s->text.begin();
        for (auto row = text.begin(); row != text.end(); row++, iterRow++)
        {
            for (auto col = (*row).begin(); col != (*row).end(); col++)
            {
                char ch = *col;
                (*iterRow).push_back(ch);
            }
        }
        s->rowIter = s->text.begin();
        s->columnIter = (*s->rowIter).begin();
        
        for (int i = 0; i < currentRow; i++)
        {
            s->rowIter++;
        }
        
        for (int i = 0; i < currentColumn; i++)
        {
            columnIter++;
        }
        
        s->column = currentColumn;
        s->row = currentRow;
      
        return *s;
    }

    void LoadState(state s)
    {
        text = s.text;
        rowIter = text.begin();
        currentColumn = s.column;
        currentRow = s.row;

        for (int i = 0; i < s.row; i++)
        {
            rowIter++;
        }

        for (int i = 0; i < s.column; i++)
        {
            columnIter++;
        }

    }

    void Input(ifstream& rdr)
    {
        string fileName;
        rdr >> fileName;

        while (!rdr.eof())
        {
            files.push_back(fileName);
            rdr >> fileName;
        }
    }
    
}; 

int main()
{
    system("color 09");
    int row = 0;
    int column = 0;

    TextEditor e;
    ifstream reader;
    ofstream writer;

    reader.open("SaveFile.txt");
    writer.open("RecentFile.txt");

    e.Input(reader);

    while (true)
    {

    }

    reader.close();
}
