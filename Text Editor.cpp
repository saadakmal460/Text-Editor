#include <iostream>
#include<stack>
#include<deque>
#include<list>
#include<algorithm>
#include<Windows.h>
#include<conio.h>
#include<fstream>
#include <vector>
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
            s->text.push_back(list<char>());
            for (auto col = (*row).begin(); col != (*row).end(); col++)
            {
                char ch = *col;
                (*iterRow).push_back(ch);
            }
        }
        s->rowIter = s->text.begin();
        s->columnIter = (*s->rowIter).begin();
        //s->column = currentColumn;
        
        /*for (int i = 0; i < currentRow; i++)
        {
            s->rowIter++;
        }
        
        for (int i = 0; i < currentColumn; i++)
        {
            columnIter++;
        }*/
        
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
    

    void EditFile(ofstream& wrt)
    {
        //system("color F0");
        char c;
        c = _getch();
        gotoxy(currentColumn, currentRow);
        cout << c;

        (*columnIter) = c;
        currentColumn++;

        while (true)
        {
            if (currentRow == 0)
            {
                gotoxy(currentColumn, currentRow);
            }
            else
            {
                gotoxy(currentColumn+1, currentRow);
            }

            c = _getch();

            if (c == -32)
            {
            A:
                c = _getch();

                if (c == 72) // up arrow key
                {
                    if (currentRow == 0)
                    {
                        continue;
                    }

                    rowIter--;
                    columnIter = (*rowIter).begin();
                    currentRow--;
                    currentColumn = 0;
                }

                if (c == 80) // down arrow key
                {
                    
                        rowIter++;
                        columnIter = (*rowIter).begin();
                        currentRow++;
                        currentColumn = 0;
                    
                }

                if (c == 75)//Left arrow key
                {
                    if (currentColumn - 1 > 0)
                    {
                        columnIter--;
                        currentColumn--;
                    }
                }

                if (c == 77)//Right arrow key
                {
                    /*if (currentColumn == 159);
                    {
                        continue;
                    }*/

                    columnIter++;
                    currentColumn++;
                }

                else if (c == 83)//Delete key
                {
                    auto temp = columnIter;
                    columnIter++;
                    (*rowIter).erase(columnIter);
                    columnIter = temp;
                    system("cls");
                    print();
                    UpdateUndo();
                }
                if (currentColumn == 0)
                {
                    gotoxy(0, currentRow);
                    c = _getch();
                    if (c == -32)
                    {
                        goto A;
                    }
                    (*rowIter).push_front(c);
                    columnIter = (*rowIter).begin();
                    currentColumn = 1;
                    system("cls");
                    print();
                    UpdateUndo();
                }
                continue;

            }

            else if (c == 13) // Enter
            {
                auto temp = rowIter;
                rowIter++;
                text.insert(rowIter, list<char>());
                rowIter = ++temp;
                currentRow++;
                currentColumn = 0;
                gotoxy(currentColumn, currentRow);
                c = _getch();
                if (c == -32)
                {
                    goto A;
                }
                (*rowIter).push_back(c);
                columnIter = (*rowIter).begin();
                system("cls");
                print();
                UpdateUndo();
                continue;
            }

            else if (c == 8)
            {
                if (currentColumn == 0)
                {
                    gotoxy(0, currentRow);
                    c = _getch();
                    if (c == -32)
                    {
                        goto A;
                    }

                    if (c == 8)
                    {
                        continue;
                    }

                    (*columnIter) = c;
                    columnIter = (*rowIter).begin();
                    currentColumn = 1;
                    continue;
                }
                auto temp = --columnIter;
                columnIter++;
                (*rowIter).erase(columnIter);
                columnIter = temp;
                currentColumn--;
                system("cls");
                print();
                UpdateUndo();
                continue;
            }

            else if (c == 26)//Undo
            {
                if (!undo.empty())
                {
                    state s = undo.back();
                    LoadState(s);
                    redo.push(undo.back());
                    undo.pop_back();
                    system("cls");
                    print();
                }
                continue;
            }

            else if (c == 25)//redo
            {
                if (!redo.empty())
                {
                    undo.push_back(redo.top());
                    state s = redo.top();
                    redo.pop();
                    LoadState(s);

                    system("cls");
                    print();
                }
                continue;

            }

            else if (c == 27)//esc to exit
            {
                SaveInFile(wrt);
                break;

            }

            if (currentColumn == 100)
            {
                text.push_back(list<char>());
                rowIter++;
                currentColumn = 0;
                currentRow++;
                (*rowIter).push_back(c);
                columnIter = (*rowIter).begin();
            }
            else
            {
                auto temp = columnIter;
                columnIter++;
                (*rowIter).insert(columnIter, c);
                columnIter = ++temp;
                currentColumn++;
            }

            system("cls");
            print();
            UpdateUndo();
        }

    }

    void SaveInFile(ofstream& wrt)
    {
        for (auto r = text.begin(); r != text.end(); r++)
        {
            for (auto c = (*r).begin(); c != (*r).end(); c++)
            {
                wrt << *c;
            }
            wrt << '\n';
        }
    }

    void OpenFile()
    {
        system("cls");
        string name;
        cout << "Enter file name: ";
        cin >> name;
        if (find(files.begin(), files.end(), name) == files.end())
        {
            cout << "File does not exist\n";
            return;
        }
        ifstream read(name.c_str());
        OpenSavedFile(read);
        read.close();

        ofstream write(name.c_str());
        system("cls");
        print();
        gotoxy(currentColumn, currentRow);
        EditFile(write);
        system("cls");
        system("color 09");
    }


    void OpenSavedFile(ifstream& read)
    {
        char ch;

        while (!read.eof())
        {
            read.get(ch);
            if (ch != '\n')
            {
                (*rowIter).push_back(ch);
            }
            else
            {
                text.push_back(list<char>());
                rowIter++;
            }
        }
        rowIter = text.begin();
        columnIter = (*rowIter).begin();
        currentRow = 0;
        currentColumn = 0;
        gotoxy(currentColumn, currentRow);
    }

    void print()
    {
        for (auto r = text.begin(); r != text.end(); r++)
        {
            for (auto c = (*r).begin(); c != (*r).end(); c++)
            {
                cout << *c;
            }
            cout << endl;
        }
    }


    void UpdateUndo()
    {
        if (undo.size() > 5)
        {
            undo.erase(undo.begin());
        }

        state s = SaveState();
        undo.push_back(s);
    }

    void CreateNewFile()
    {
        system("cls");
        string fileName;
        cout << "Enter file name: ";
        cin >> fileName;

        if (find(files.begin(), files.end(), fileName) != files.end())
        {
            cout << "File already exist";
            return;
        }

        files.push_back(fileName);


        ofstream wrt(fileName.c_str());


        system("cls");
        EditFile(wrt);
        system("cls");
        system("color 09");
        wrt.close();

    }

    int choice(ifstream& read)
    {
        system("cls");
        int choice = -1;
        int r = 0;
        int c = 0;

        gotoxy(80,6 );
        cout << "New File" << endl;

        gotoxy(80,9);
        cout << "Open File" << endl;

        gotoxy(80,12);
        cout << "Exit" << endl;

        cin >> choice;

        if (choice == 1)
        {
            CreateNewFile();
        }
        if (choice == 2)
        {
            OpenFile();
        }
        if (choice == 3)
        {
            Closing(read);
        }
        return choice;
    }


    void Closing(ifstream& read)
    {
        
        read.close();
        ofstream write;
        write.open("SaveFile.txt");

        for (auto i = files.begin(); i != files.end(); i++)
        {
            write << (*i)<<endl;
        }
        write.close();   
    }

    void gotoxy(int x, int y)
    {
        COORD coordinates;
        coordinates.X = x;
        coordinates.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
    }
}; 

int main()
{
    system("color 09");

    
    int row = 0;
    int column = 0;
    int choice = 0;

    TextEditor e;
    ifstream reader;
    ofstream writer;

    reader.open("SaveFile.txt");
    writer.open("RecentFile.txt");

    e.Input(reader);

    while (choice!=3)
    {
        choice = e.choice(reader);
    }

    reader.close();
}
