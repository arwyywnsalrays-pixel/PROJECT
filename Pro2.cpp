#include <iostream>
#include <vector>   // �������� ����� vector �����������
#include <string>   // �������� ���� string
#include <fstream>  // ������ ������� �� ������� (ifstream)
#include <chrono>   // ����� ��� �������
#include <iomanip>  //  ( setprecision)
#include <cctype>   //  ( isspace, tolower)
using namespace std;
// -------------------- Position --------------------
// ()
class Position {
public:
    int row, col; //
    Position() : row(0), col(0) {} // ����� �������
    Position(int r, int c) : row(r), col(c) {} // ����� ���� ��������
};
struct Placement
{
    int row;
    int col;
    int dr;
    int dc;

    Placement(int r,int c,int d1,int d2)
    {
        row=r;
        col=c;
        dr=d1;
        dc=d2;
    }
};


// -------------------- Board --------------------
// ���� ���� ���� ����� �� �������
class Board {
public:
    vector<string> grid; // ���� ������ ���� ������
    int rows, cols;      // ����� ������ (��� ������ ��������)
    string file;         // ��� ��� ������
    long long backtrack_count = 0;      // ���� ���� ���� ������� (backtrack)
    long long expanded = 0; // ���� ����� ���� �� �����
    Board() : rows(0), cols(0) {} // ����� �������

    // ���� ������ ������ �� ��� ���
    bool loadFile(const string& path) {
        file = path;
        ifstream file(path); // ��� ����� �������

        if (!file.is_open()) {
            cerr << "Error: Could not open board file: " << path << endl;
            return false;
        }

        grid.clear(); // ��� �� ������ ����� �� ������
        string line;
        // ����� ����� ����� ����
        while (getline(file, line)) {
            string processed_line;
            // ����� �������� �� ����� �������
            for (int i = 0; i < line.length(); ++i) {
                char c = line[i];
                if (!isspace(c)) {
                    processed_line += c;
                }
            }
            if (!processed_line.empty()) {
                grid.push_back(processed_line); // ����� ����� ������� ��� ������
            }
        }
        file.close(); // ����� �����

        // ����� ����� ������ ������� �� ���� ������� �����
        if (!grid.empty()) {
            rows = grid.size();
            cols = grid[0].size();
            for (size_t i = 1; i < grid.size(); ++i) {
                if (grid[i].size() != (size_t)cols) {
                    cerr << "Error: Board is not rectangular. Row " << i << " has " << grid[i].size() << " chars, expected " << cols << "." << endl;
                    return false;
                }
            }
        } else {
            rows = 0;
            cols = 0;
        }

        return true;
    }

    // ���� ������ ������ ��� ������
    void print() const {
        cout << "\n--- Board ---\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << grid[i][j] <<" " ;
            }
            cout << endl;
        }
    }

    // ���� ����� �� ��� ����� ������ �� ������ ������ ���� ���� ���
    vector<Position> search(const string& word) {
        if (word.empty()) {
            return {}; // ����� ���� ���� ��� ���� ������ �����
        }

        // ������ ��� �� ���� �� ������ ����� ����� ������
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                // ��� ���� ������ ����� ����� ����� �� �����ɡ ���� �����
                if (grid[r][c] == word[0]) {
                    vector<Position> path;
                    vector<vector<bool>> visited(rows, vector<bool>(cols, false)); // ������ ����� ������� ��������� �� ������ ������
                    // ��� ����� ������ (DFS) �� ������ڡ �� ����� ����� ���� ��� ����
                    if (dfs_backtrack(r, c, word, 0, path, visited, -2, -2)) { // -2: ����� ��� ���� ���
                        return path; // ����� ������ ��� ������ ����
                    }
                }
            }
        }
        return {}; // ����� ���� ���� ��� �� ��� ������ ��� ������
    }
     bool canPlaceWord(string word, int row, int col, int dr, int dc)
{
    for (int i = 0; i < word.length(); i++)
    {
        int r = row + i * dr;
        int c = col + i * dc;

        if (r < 0 || r >= rows || c < 0 || c >= cols)
            return false;

        if (grid[r][c] != word[i])
            return false;
    }

    return true;
}

private:
    // ���� ����� ������ ��������� (DFS) �� ������� (Backtracking)
    bool dfs_backtrack(int r, int c, const string& word, int index, vector<Position>& path, vector<vector<bool>>& visited, int dr, int dc) {
        // ��� ������: ������ �� ���� �����ɡ �� ������ �� ������ǡ �� ����� �� ������
        if (r < 0 || r >= rows || c < 0 || c >= cols || visited[r][c] || grid[r][c] != word[index]) {
            // ����� ���� �������
            return false; // ������ ��� ����
        }

        //�������� (Choose): ����� ������ ������� ��� ������ ���� ����� "��� �������"
        path.push_back(Position(r, c));
        visited[r][c] = true;
        expanded++; // ����� ���� ����� ���� �� �����

        // ��� ������: ��� ����� ��� ��� ��� �� �����ɡ ��� �� ������ �����
        if (index == word.length() - 1) {
            return true;
        }

        // ��������� (Explore)
        if (dr == -2) { // ��� ��� ������� �� ����� ��� (��� ����� �����)
            // ��� �� ��������� �������� �������
            int all_dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int all_dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

            for (int i = 0; i < 8; i++) {
                // ������� ������ ������ ������� ������ �� ���� ������
                if (dfs_backtrack(r + all_dr[i], c + all_dc[i], word, index + 1, path, visited, all_dr[i], all_dc[i])) {
                    return true; // ��� ��� �����ѡ ���� �����
                }
            }
        } else { // ��� ��� ������� ������ ������
            // ����� �� ��� ������� ���
            if (dfs_backtrack(r + dr, c + dc, word, index + 1, path, visited, dr, dc)) {
                return true;
            }
        }

        // ������� (Backtrack): ��� ���� �� �������� �� ��� ������
        // ���� �������� �� ������ ������ ����� "��� �������" ����� �� ������ ����
        path.pop_back();
        visited[r][c] = false;
        backtrack_count++; // <-- ��� ����� ��� �� ����� ������ ��� ������� �� �������
        return false; // ����� ������ ��������� ���� ��� ������
    }

};

// -------------------- WordList --------------------
// ���� ���� ����� ������� ������� ����� ����
class WordList {
public:
    vector<string> words; // ����� ������ ����� �������
    string file;          // ��� ��� �������

    WordList() {} // ����� �������

    // ���� ������ ����� ������� �� ��� ���
    bool loadFile(const string& path) {
        file = path;
        ifstream file(path); // ��� ����� �������

        if (!file.is_open()) {
            cerr << "Error: Could not open word list file: " << path << endl;
            return false;
        }

        words.clear(); // ��� �� ����� �����
        string word;
        // ����� ������� �� ����� ���� �����
        while (file >> word) {
            string clean_word;
            for(size_t i = 0; i < word.length(); ++i) {
                char c = word[i];
                if(!isspace(c)) clean_word += c;
            }
            if (!clean_word.empty()) {
                words.push_back(clean_word); // ����� ������ ������� ��� �������
            }
        }
        file.close(); // ����� �����
        return true;
    }
    // ���� ������ ����� �������
    void print() const {
        cout << "\n--- Words to Find ---\n";
        for (int i = 0; i < words.size(); i++) {
            cout << "- " << words[i] << endl;
        }
        cout << "----------------------\n";
    }
};
class CSP
{
public:

    // الكلمات (Variables)
    vector<string> variables;

    // المجالات (Domains)
    vector<vector<Placement>> domains;
    void generateDomains(Board &board, WordList &list);
    int selectMRV();
    vector<string> getMRVOrder();
    void applyAC3(Board &board);
    // إعادة تهيئة البيانات
    void clear()
    {
        variables.clear();
        domains.clear();
    }
};
void CSP::generateDomains(Board &board, WordList &list)
{
    clear();

    // الاتجاهات الثمانية
    int dr[8] = {-1,-1,-1,0,0,1,1,1};
    int dc[8] = {-1,0,1,-1,1,-1,0,1};

    // المرور على جميع الكلمات
    for (int i = 0; i < list.words.size(); i++)
    {
        variables.push_back(list.words[i]);

        vector<Placement> domain;

        // المرور على جميع خلايا البورد
        for (int r = 0; r < board.rows; r++)
        {
            for (int c = 0; c < board.cols; c++)
            {
                // تجربة جميع الاتجاهات
                for (int d = 0; d < 8; d++)
                {
                    if (board.canPlaceWord(list.words[i], r, c, dr[d], dc[d]))
                    {
                        domain.push_back(
                            Placement(r, c, dr[d], dc[d])
                        );
                    }
                }
            }
        }

        // حفظ الـ Domain الخاص بهذه الكلمة
        domains.push_back(domain);
    }
}
int CSP::selectMRV()
{
    int index = -1;
    int minimum = INT_MAX;

    for (int i = 0; i < domains.size(); i++)
    {
        if (!domains[i].empty() &&
            domains[i].size() < minimum)
        {
            minimum = domains[i].size();
            index = i;
        }
    }

    return index;
}vector<string> CSP::getMRVOrder()
{
    vector<string> order;

    vector<bool> used(variables.size(), false);

    for (int k = 0; k < variables.size(); k++)
    {
        int best = -1;
        int minimum = INT_MAX;

        for (int i = 0; i < variables.size(); i++)
        {
            if (!used[i] &&
                domains[i].size() < minimum)
            {
                minimum = domains[i].size();
                best = i;
            }
        }

        if (best != -1)
        {
            order.push_back(variables[best]);
            used[best] = true;
        }
    }

    return order;
}
void CSP::applyAC3(Board &board)
{
    for(int i=0;i<domains.size();i++)
    {
        vector<Placement> newDomain;

        for(int j=0;j<domains[i].size();j++)
        {
            Placement p = domains[i][j];

            if(board.canPlaceWord(
                    variables[i],
                    p.row,
                    p.col,
                    p.dr,
                    p.dc))
            {
                newDomain.push_back(p);
            }
        }

        domains[i]=newDomain;
    }
}


// ���� ������ ����� ������ ����� ��� ���� ������� ��������
string getDirection(const Position& start, const Position& end, size_t word_length) {
    // ��� ���� ������ �� ��� ���ϡ ��� ���� �����
    if (word_length <= 1) return "N/A";

    int dr = end.row - start.row;
    int dc = end.col - start.col;

    // ������ �� ��������� ���������
    if (dr == 0 && dc != 0) return (dc > 0) ? "Horizontal (Right)" : "Horizontal (Left)";
    if (dc == 0 && dr != 0) return (dr > 0) ? "Vertical (Down)" : "Vertical (Up)";

    // ������ �� ��������� �������
    if (abs(dr) == abs(dc) && dr != 0) {
        if (dr > 0) {
            return (dc > 0) ? "Diagonal (Down-Right)" : "Diagonal (Down-Left)";
        } else {
            return (dc > 0) ? "Diagonal (Up-Right)" : "Diagonal (Up-Left)";
        }
    }

    // ��� �� ��� ������ �������� (���� �������)� ��� ����� "�����"
    return "Changing";
}

// ������ �������� ��������
int main() {

    // ����� ����� ������ �� ������� ���� ������� ������
    cout << "\n------------------------------------------------\n";
   cout << "HINT: Search for Computer Science specializations and concepts!\n";
    cout << "-------------------------------------------------\n";

    // ����� ���� ����� ������ ����� ����� ���
    Board board;
    if (!board.loadFile("C:/Users/arwyy/Downloads/proj/board.txt")) {
        return 1; // ������ ��� ��� ����� �����
    }
    board.print(); // ����� ������

    // ����� ����� ������
    cout << "Board size: " << board.rows << "x" << board.cols << "\n";

    // ����� ���� ������ ������� ������ ����� ����� ���
    WordList wl;
    CSP csp;
    if (!wl.loadFile("C:/Users/arwyy/Downloads/proj/words.txt")) {
        return 1; // ������ ��� ��� ����� �����
    }
    csp.generateDomains(board, wl);
    csp.applyAC3(board);
    vector<string> orderedWords = csp.getMRVOrder();
    cout << "\n========== MRV Search Order ==========\n";
    for(int i = 0; i < orderedWords.size(); i++)
{
    for(int j = 0; j < csp.variables.size(); j++)
    {
        if(orderedWords[i] == csp.variables[j])
        {
            cout << i + 1
                 << "- "
                 << orderedWords[i]
                 << " (Domain = "
                 << csp.domains[j].size()
                 << ")"
                 << endl;
            break;
        }
    }
}
    cout << "\n========== CSP Variables ==========\n";

for (int i = 0; i < csp.variables.size(); i++)
{
    cout << "Variable " << i + 1
         << " : "
         << csp.variables[i]
         << endl;
}
cout << "\n========== CSP Domains ==========\n";

for(int i=0;i<csp.domains.size();i++)
{
    cout << csp.variables[i]
         << " -> "
         << csp.domains[i].size()
         << " possible placements"
         << endl;
}
    if (wl.words.empty()) {
        cout << "Word list is empty. Exiting.\n";
        return 0;
    }

    while (true) {
        string answer;
        cout << "\nDo you want to start searching for ALL words? (yes/no)\n> ";
        getline(cin, answer);

        // ����� ������� ��� ���� ����� ������ ������ ��� ����� ����� ������
        string lower_answer;
        for (char ch : answer) {
            lower_answer += tolower(ch);
        }

        if (lower_answer == "yes" || lower_answer == "y") {
            // ��� ����� �����
            cout << "\n Starting search...\n\n";
            board.backtrack_count = 0; // ����� ����� ��������
            board.expanded = 0;
            chrono::time_point<std::chrono::high_resolution_clock> start_time = chrono::high_resolution_clock::now(); // ����� ��� �����

            // ���� ����� �� �� ���� �� �������
            for(size_t i=0;i<orderedWords.size();i++) {
                    long long word_backtracks_before = board.backtrack_count;
long long word_expanded_before = board.expanded;
                vector<Position> found_path = board.search(orderedWords[i]); // ������� ���� �����
                long long word_backtracks =
    board.backtrack_count - word_backtracks_before;

long long word_expanded =
    board.expanded - word_expanded_before;

                if (!found_path.empty()) {
                    // ��� �� ������ ��� �����ɡ ����� ������ ������
                   cout << "FOUND: " <<orderedWords[i]
     << " | Length: "
     << orderedWords[i].length();
                    Position start = found_path.front(); // ���� �������
                    Position end = found_path.back();   // ���� �������
                    cout << " | Path: (" << start.row << "," << start.col << ") -> (" << end.row << "," << end.col << ")";
                    cout << " | Direction: " << getDirection(start, end, found_path.size());
                    cout << " | Expanded Nodes: "
     << word_expanded;

cout << " | Backtracks: "
     << word_backtracks;

cout << "\n";
                } else {
                    // ��� �� ��� ������ ��� ������
                   cout << "NOT FOUND: "
     << orderedWords[i]
     << " | Expanded Nodes: "
     << word_expanded
     << " | Backtracks: "
     << word_backtracks
     << "\n";
                }
            }

            chrono::time_point<std::chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now(); // ����� ��� ��������
            chrono::duration<double> duration = end_time - start_time; // ���� �����

            // ����� �������� �����
            cout << "\n----------------------------------\n";
            cout << "Search completed.\n";
            cout << "Total backtracks: " << board.backtrack_count << "\n";
            cout << "Total expanded nodes: " << board.expanded<< "\n";
           cout << "Execution time: "
     << fixed << setprecision(6)
     << duration.count() << " s\n";
            cout << "----------------------------------\n";
            break;
        } else if (lower_answer == "no" || lower_answer == "n") {
            // ��� ����� �������� ������
            cout << "Stopping search. Goodbye!\n";
            break;
        } else {
            // ��� ���� ������� ��� �����
            cout << "Invalid input. Please enter 'yes' or 'no'.\n";
        }
    }


    return 0;
}

