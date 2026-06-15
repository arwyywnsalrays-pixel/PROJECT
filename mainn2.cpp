#include <iostream>
#include <vector>   // áÇÓÊÎÏÇã ÍÇæíÉ vector ÇáÏíäÇãíßíÉ
#include <string>   // áÇÓÊÎÏÇã ßáÇÓ string
#include <fstream>  // áÅÏÇÑÉ ÇáÊÚÇãá ãÚ ÇáãáİÇÊ (ifstream)
#include <chrono>   // áŞíÇÓ æŞÊ ÇáÊäİíĞ
#include <iomanip>  // ááÊÍßã İí ÊäÓíŞ ÇáÅÎÑÇÌ (ãËá setprecision)
#include <cctype>   // áÏæÇá ÇáÊÚÇãá ãÚ ÇáÃÍÑİ (ãËá isspace, tolower)

using namespace std;
// -------------------- Position --------------------

// ßáÇÓ ÈÓíØ áÊÎÒíä ÅÍÏÇËíÇÊ (Õİ æÚãæÏ) ÎáíÉ İí ÇááæÍÉ
class Position {
public:
    int row, col; // ãÊÛíÑÇÊ áÊÎÒíä ÑŞã ÇáÕİ æÇáÚãæÏ

    Position() : row(0), col(0) {} // ãõäÔÆ ÇİÊÑÇÖí
    Position(int r, int c) : row(r), col(c) {} // ãõäÔÆ íÃÎĞ ÅÍÏÇËíÇÊ
};

// -------------------- Board --------------------
// ßáÇÓ íãËá áæÍÉ ÇáÈÍË Úä ÇáßáãÇÊ
class Board {
public:
    vector<string> grid; // ÔÈßÉ áÊÎÒíä ÃÍÑİ ÇááæÍÉ
    int rows, cols;      // ÃÈÚÇÏ ÇááæÍÉ (ÚÏÏ ÇáÕİæİ æÇáÃÚãÏÉ)
    string file;         // ÇÓã ãáİ ÇááæÍÉ
    long long backtrack_count = 0;      // ÚÏÇÏ áÚÏÏ ãÑÇÊ ÇáÊÑÇÌÚ (backtrack)
    long long expanded = 0; // ÚÏÇÏ ááÚŞÏ ÇáÊí Êã İÍÕåÇ

    Board() : rows(0), cols(0) {} // ãõäÔÆ ÇİÊÑÇÖí

    // ÏÇáÉ áÊÍãíá ÇááæÍÉ ãä ãáİ äÕí
    bool loadFile(const string& path) {
        file = path;
        ifstream file(path); // İÊÍ Çáãáİ ááŞÑÇÁÉ

        if (!file.is_open()) {
            cerr << "Error: Could not open board file: " << path << endl;
            return false;
        }

        grid.clear(); // ãÓÍ Ãí ÈíÇäÇÊ ÓÇÈŞÉ İí ÇáÔÈßÉ
        string line;
        // ŞÑÇÁÉ Çáãáİ ÓØÑÇğ ÈÓØÑ
        while (getline(file, line)) {
            string processed_line;
            // ÅÒÇáÉ ÇáãÓÇİÇÊ ãä ÇáÓØÑ ÇáãŞÑæÁ
            for (int i = 0; i < line.length(); ++i) {
                char c = line[i];
                if (!isspace(c)) {
                    processed_line += c;
                }
            }
            if (!processed_line.empty()) {
                grid.push_back(processed_line); // ÅÖÇİÉ ÇáÓØÑ ÇáãÚÇáÌ Åáì ÇáÔÈßÉ
            }
        }
        file.close(); // ÅÛáÇŞ Çáãáİ

        // ÊÍÏíÏ ÃÈÚÇÏ ÇááæÍÉ æÇáÊÍŞŞ ãä ÃäåÇ ãÓÊØíáÉ ÇáÔßá
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

    // ÏÇáÉ áØÈÇÚÉ ÇááæÍÉ Úáì ÇáÔÇÔÉ
    void print() const {
        cout << "\n--- Board ---\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << grid[i][j] <<" " ;
            }
            cout << endl;
        }
    }

    // ÏÇáÉ ááÈÍË Úä Ãæá ÊæÇÌÏ ááßáãÉ İí ÇááæÍÉ æÅÑÌÇÚ ãÓÇÑ æÇÍÏ İŞØ
    vector<Position> search(const string& word) {
        if (word.empty()) {
            return {}; // ÅÑÌÇÚ ãÓÇÑ İÇÑÛ ÅĞÇ ßÇäÊ ÇáßáãÉ İÇÑÛÉ
        }

        // ÇáãÑæÑ Úáì ßá ÎáíÉ İí ÇááæÍÉ ßäŞØÉ ÈÏÇíÉ ãÍÊãáÉ
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                // ÅĞÇ ßÇäÊ ÇáÎáíÉ ÊØÇÈŞ ÇáÍÑİ ÇáÃæá ãä ÇáßáãÉ¡ äÈÏÃ ÇáÈÍË
                if (grid[r][c] == word[0]) {
                    vector<Position> path;
                    vector<vector<bool>> visited(rows, vector<bool>(cols, false)); // ãÕİæİÉ áÊÊÈÚ ÇáÎáÇíÇ ÇáãÓÊÎÏãÉ İí ÇáãÓÇÑ ÇáÍÇáí
                    // ÈÏÁ ÇáÈÍË ÇáÚãíŞ (DFS) ãÚ ÇáÊÑÇÌÚ¡ ãÚ ÊãÑíÑ ÇÊÌÇå Ãæáí ÛíÑ ãÍÏÏ
                    if (dfs_backtrack(r, c, word, 0, path, visited, -2, -2)) { // -2: ÇÊÌÇå ÛíÑ ãÍÏÏ ÈÚÏ
                        return path; // ÅÑÌÇÚ ÇáãÓÇÑ İæÑ ÇáÚËæÑ Úáíå
                    }
                }
            }
        }
        return {}; // ÅÑÌÇÚ ãÓÇÑ İÇÑÛ ÅĞÇ áã íÊã ÇáÚËæÑ Úáì ÇáßáãÉ
    }
private:
    // ÏÇáÉ ÇáÈÍË ÇáÚãíŞ ÇáÊßÑÇÑíÉ (DFS) ãÚ ÇáÊÑÇÌÚ (Backtracking)
    bool dfs_backtrack(int r, int c, const string& word, int index, vector<Position>& path, vector<vector<bool>>& visited, int dr, int dc) {
        // ÔÑØ ÇáÊæŞİ: ÇáÎÑæÌ ãä ÍÏæÏ ÇááæÍÉ¡ Ãæ ÇáÎáíÉ Êã ÒíÇÑÊåÇ¡ Ãæ ÇáÍÑİ áÇ íÊØÇÈŞ
        if (r < 0 || r >= rows || c < 0 || c >= cols || visited[r][c] || grid[r][c] != word[index]) {
            // ÒíÇÏÉ ÚÏÇÏ ÇáÊÑÇÌÚ
            return false; // ÇáãÓÇÑ ÛíÑ ÕÇáÍ
        }

        //ÇáÇÎÊíÇÑ (Choose): ÅÖÇİÉ ÇáÎáíÉ ÇáÍÇáíÉ Åáì ÇáãÓÇÑ ææÖÚ ÚáÇãÉ "ÊãÊ ÒíÇÑÊåÇ"
        path.push_back(Position(r, c));
        visited[r][c] = true;
        expanded++; // ÒíÇÏÉ ÚÏÇÏ ÇáÚŞÏ ÇáÊí Êã İÍÕåÇ

        // ÔÑØ ÇáäÌÇÍ: ÅĞÇ æÕáäÇ Åáì ÂÎÑ ÍÑİ İí ÇáßáãÉ¡ İŞÏ Êã ÇáÚËæÑ ÚáíåÇ
        if (index == word.length() - 1) {
            return true;
        }

        // ÇáÇÓÊßÔÇİ (Explore)
        if (dr == -2) { // ÅĞÇ ßÇä ÇáÇÊÌÇå áã íÊÍÏÏ ÈÚÏ (ÚäÏ ÇáÍÑİ ÇáÃæá)
            // ÌÑÈ ßá ÇáÇÊÌÇåÇÊ ÇáËãÇäíÉ ÇáããßäÉ
            int all_dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int all_dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

            for (int i = 0; i < 8; i++) {
                // ÇÓÊÏÚÇÁ ÊßÑÇÑí áÊÍÏíÏ ÇáÇÊÌÇå æÇáÈÍË Úä ÈŞíÉ ÇáßáãÉ
                if (dfs_backtrack(r + all_dr[i], c + all_dc[i], word, index + 1, path, visited, all_dr[i], all_dc[i])) {
                    return true; // ÅĞÇ äÌÍ ÇáãÓÇÑ¡ äÚæÏ ÈäÌÇÍ
                }
            }
        } else { // ÅĞÇ ßÇä ÇáÇÊÌÇå ãÍÏÏÇğ ÈÇáİÚá
            // ÇÓÊãÑ İí äİÓ ÇáÇÊÌÇå İŞØ
            if (dfs_backtrack(r + dr, c + dc, word, index + 1, path, visited, dr, dc)) {
                return true;
            }
        }

        // ÇáÊÑÇÌÚ (Backtrack): ÅĞÇ İÔáÊ ßá ÇáãÓÇÑÇÊ ãä åĞå ÇáÎáíÉ
        // äŞæã ÈÅÒÇáÊåÇ ãä ÇáãÓÇÑ æÅáÛÇÁ ÚáÇãÉ "ÊãÊ ÒíÇÑÊåÇ" ááÈÍË İí ãÓÇÑÇÊ ÃÎÑì
        path.pop_back();
        visited[r][c] = false;
        backtrack_count++; // <-- åĞÇ ÇáÓØÑ íÌÈ Ãä íõäİĞ ÏÇÆãğÇ ÈÚÏ ÇÓÊßÔÇİ ßá ÇáÌíÑÇä
        return false; // ÅÚáÇã ÇáÏÇáÉ ÇáãÓÊÏÚíÉ ÈİÔá åĞÇ ÇáãÓÇÑ
    }
};

// -------------------- WordList --------------------
// ßáÇÓ íãËá ŞÇÆãÉ ÇáßáãÇÊ ÇáãØáæÈ ÇáÈÍË ÚäåÇ
class WordList {
public:
    vector<string> words; // ÍÇæíÉ áÊÎÒíä ŞÇÆãÉ ÇáßáãÇÊ
    string file;          // ÇÓã ãáİ ÇáßáãÇÊ

    WordList() {} // ãõäÔÆ ÇİÊÑÇÖí

    // ÏÇáÉ áÊÍãíá ŞÇÆãÉ ÇáßáãÇÊ ãä ãáİ äÕí
    bool loadFile(const string& path) {
        file = path;
        ifstream file(path); // İÊÍ Çáãáİ ááŞÑÇÁÉ

        if (!file.is_open()) {
            cerr << "Error: Could not open word list file: " << path << endl;
            return false;
        }

        words.clear(); // ãÓÍ Ãí ßáãÇÊ ÓÇÈŞÉ
        string word;
        // ŞÑÇÁÉ ÇáßáãÇÊ ãä Çáãáİ ßáãÉ ÈßáãÉ
        while (file >> word) {
            string clean_word;
            for(size_t i = 0; i < word.length(); ++i) {
                char c = word[i];
                if(!isspace(c)) clean_word += c;
            }
            if (!clean_word.empty()) {
                words.push_back(clean_word); // ÅÖÇİÉ ÇáßáãÉ ÇáäÙíİÉ Åáì ÇáŞÇÆãÉ
            }
        }
        file.close(); // ÅÛáÇŞ Çáãáİ
        return true;
    }
    // ÏÇáÉ áØÈÇÚÉ ŞÇÆãÉ ÇáßáãÇÊ
    void print() const {
        cout << "\n--- Words to Find ---\n";
        for (int i = 0; i < words.size(); i++) {
            cout << "- " << words[i] << endl;
        }
        cout << "----------------------\n";
    }
};



// ÏÇáÉ áÊÍÏíÏ ÇÊÌÇå ÇáßáãÉ ÈäÇÁğ Úáì äŞØÉ ÇáÈÏÇíÉ æÇáäåÇíÉ
string getDirection(const Position& start, const Position& end, size_t word_length) {
    // ÅĞÇ ßÇäÊ ÇáßáãÉ ãä ÍÑİ æÇÍÏ¡ İáÇ íæÌÏ ÇÊÌÇå
    if (word_length <= 1) return "N/A";

    int dr = end.row - start.row;
    int dc = end.col - start.col;

    // ÇáÊÍŞŞ ãä ÇáÇÊÌÇåÇÊ ÇáãÓÊŞíãÉ
    if (dr == 0 && dc != 0) return (dc > 0) ? "Horizontal (Right)" : "Horizontal (Left)";
    if (dc == 0 && dr != 0) return (dr > 0) ? "Vertical (Down)" : "Vertical (Up)";

    // ÇáÊÍŞŞ ãä ÇáÇÊÌÇåÇÊ ÇáŞØÑíÉ
    if (abs(dr) == abs(dc) && dr != 0) {
        if (dr > 0) {
            return (dc > 0) ? "Diagonal (Down-Right)" : "Diagonal (Down-Left)";
        } else {
            return (dc > 0) ? "Diagonal (Up-Right)" : "Diagonal (Up-Left)";
        }
    }

    // ÅĞÇ áã íßä ÇáãÓÇÑ ãÓÊŞíãÇğ (ÈÓÈÈ ÇáÊÑÇÌÚ)¡ íÊã ÅÑÌÇÚ "ãÊÛíÑ"
    return "Changing";
}

// ÇáÏÇáÉ ÇáÑÆíÓíÉ ááÈÑäÇãÌ
int main() {

    // ØÈÇÚÉ ÑÓÇáÉ áÊæÖíÍ ãÇ ÇáßáãÇÊ ÇáÊì ÊÍÊæíåÇ ÇáÈæÑÏ
    cout << "\n------------------------------------------------\n";
   cout << "HINT: Search for Computer Science specializations and concepts!\n";
    cout << "-------------------------------------------------\n";

    // ÅäÔÇÁ ßÇÆä ááæÍÉ æÊÍãíá Çáãáİ ÇáÎÇÕ ÈåÇ
    Board board;
    if (!board.loadFile("C:/Users/arwyy/Downloads/proj/board.txt")) {
        return 1; // ÇáÎÑæÌ ÅĞÇ İÔá ÊÍãíá Çáãáİ
    }
    board.print(); // ØÈÇÚÉ ÇááæÍÉ

    // ØÈÇÚÉ ÃÈÚÇÏ ÇááæÍÉ
    cout << "Board size: " << board.rows << "x" << board.cols << "\n";

    // ÅäÔÇÁ ßÇÆä áŞÇÆãÉ ÇáßáãÇÊ æÊÍãíá Çáãáİ ÇáÎÇÕ ÈåÇ
    WordList wl;
    if (!wl.loadFile("C:/Users/arwyy/Downloads/proj/words.txt")) {
        return 1; // ÇáÎÑæÌ ÅĞÇ İÔá ÊÍãíá Çáãáİ
    }
    if (wl.words.empty()) {
        cout << "Word list is empty. Exiting.\n";
        return 0;
    }

    while (true) {
        string answer;
        cout << "\nDo you want to start searching for ALL words? (yes/no)\n> ";
        getline(cin, answer);

        // ÊÍæíá ÇáÅÌÇÈÉ Åáì ÃÍÑİ ÕÛíÑÉ áÅÌÑÇÁ ãŞÇÑäÉ ÛíÑ ÍÓÇÓÉ áÍÇáÉ ÇáÃÍÑİ
        string lower_answer;
        for (char ch : answer) {
            lower_answer += tolower(ch);
        }

        if (lower_answer == "yes" || lower_answer == "y") {
            // ÈÏÁ ÚãáíÉ ÇáÈÍË
            cout << "\n Starting search...\n\n";
            board.backtrack_count = 0; // ÅÚÇÏÉ ÊÕİíÑ ÇáÚÏÇÏÇÊ
            board.expanded = 0;
            chrono::time_point<std::chrono::high_resolution_clock> start_time = chrono::high_resolution_clock::now(); // ÊÓÌíá æŞÊ ÇáÈÏÁ

            // ÍáŞÉ ááÈÍË Úä ßá ßáãÉ İí ÇáŞÇÆãÉ
            for (size_t i = 0; i < wl.words.size(); i++) {
                    long long word_backtracks_before = board.backtrack_count;
long long word_expanded_before = board.expanded;
                vector<Position> found_path = board.search(wl.words[i]); // ÇÓÊÏÚÇÁ ÏÇáÉ ÇáÈÍË
                long long word_backtracks =
    board.backtrack_count - word_backtracks_before;

long long word_expanded =
    board.expanded - word_expanded_before;

                if (!found_path.empty()) {
                    // ÅĞÇ Êã ÇáÚËæÑ Úáì ÇáßáãÉ¡ ØÈÇÚÉ ÊİÇÕíá ÇáãÓÇÑ
                   cout << "FOUND: " << wl.words[i]
     << " | Length: "
     << wl.words[i].length();
                    Position start = found_path.front(); // äŞØÉ ÇáÈÏÇíÉ
                    Position end = found_path.back();   // äŞØÉ ÇáäåÇíÉ
                    cout << " | Path: (" << start.row << "," << start.col << ") -> (" << end.row << "," << end.col << ")";
                    cout << " | Direction: " << getDirection(start, end, found_path.size());
                    cout << " | Expanded Nodes: "
     << word_expanded;

cout << " | Backtracks: "
     << word_backtracks;

cout << "\n";
                } else {
                    // ÅĞÇ áã íÊã ÇáÚËæÑ Úáì ÇáßáãÉ
                   cout << "NOT FOUND: "
     << wl.words[i]
     << " | Expanded Nodes: "
     << word_expanded
     << " | Backtracks: "
     << word_backtracks
     << "\n";
                }
            }

            chrono::time_point<std::chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now(); // ÊÓÌíá æŞÊ ÇáÇäÊåÇÁ
            chrono::duration<double> duration = end_time - start_time; // ÍÓÇÈ ÇáãÏÉ

            // ØÈÇÚÉ ÅÍÕÇÆíÇÊ ÇáÈÍË
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
            // ÅĞÇ ÇÎÊÇÑ ÇáãÓÊÎÏã ÇáÎÑæÌ
            cout << "Stopping search. Goodbye!\n";
            break;
        } else {
            // ÅĞÇ ßÇäÊ ÇáÅÌÇÈÉ ÛíÑ ÕÇáÍÉ
            cout << "Invalid input. Please enter 'yes' or 'no'.\n";
        }
    }


    return 0;
}

