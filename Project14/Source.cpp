#include<iostream>
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
int tileSize = 80; // size of each square on the board
// BASE CLASS
// encapsulation: row, col, color are protected (not directly accessible)
// abstraction: checkMove and getSymbol are pure virtual
class Piece
{
protected:
    int row;
    int col;
    bool white; // true = white piece, false = black piece
public:
    Piece(int r, int c, bool w)
    {
        row = r;
        col = c;
        white = w;
    }
    virtual ~Piece() {}
    int getRow() { return row; }
    int getCol() { return col; }
    bool isWhite() { return white; }
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    // every derived class must implement these two functions
    virtual bool checkMove(int toRow, int toCol, Piece* board[8][8]) = 0;
    virtual wstring getSymbol() = 0;
};

// ROOK CLASS
// inherits from Piece, overrides checkMove and getSymbol
class Rook : public Piece
{
public:
    Rook(int r, int c, bool w) : Piece(r, c, w) {}

    bool checkMove(int toRow, int toCol, Piece* board[8][8]) override
    {
        // rook only moves in straight lines
        if (toRow != row && toCol != col)
            return false;

        if (toRow == row && toCol == col)
            return false;

        // block if destination has same color piece
        if (board[toRow][toCol] != nullptr)
        {
            if (board[toRow][toCol]->isWhite() == white)
                return false;
        }

        // check squares between start and destination are empty
        if (toRow == row)
        {
            int s = col, e = toCol;
            if (s > e) { int t = s; s = e; e = t; }
            for (int i = s + 1; i < e; i++)
                if (board[row][i] != nullptr) return false;
        }
        else
        {
            int s = row, e = toRow;
            if (s > e) { int t = s; s = e; e = t; }
            for (int i = s + 1; i < e; i++)
                if (board[i][col] != nullptr) return false;
        }

        return true;
    }

    wstring getSymbol() override
    {
        if (white) return L"\u2656";
        else return L"\u265C";
    }
};

// KNIGHT CLASS
// inherits from Piece
class Knight : public Piece
{
public:
    Knight(int r, int c, bool w) : Piece(r, c, w) {}

    bool checkMove(int toRow, int toCol, Piece* board[8][8]) override
    {
        int rd = toRow - row;
        int cd = toCol - col;
        if (rd < 0) rd = -rd;
        if (cd < 0) cd = -cd;

        // knight moves in L shape, 2 squares one way and 1 the other
        bool lShape = (rd == 2 && cd == 1) || (rd == 1 && cd == 2);
        if (!lShape) return false;

        if (board[toRow][toCol] != nullptr)
            if (board[toRow][toCol]->isWhite() == white) return false;

        return true;
    }

    wstring getSymbol() override
    {
        if (white) return L"\u2658";
        else return L"\u265E";
    }
};

// BISHOP CLASS
// inherits from Piece
class Bishop : public Piece
{
public:
    Bishop(int r, int c, bool w) : Piece(r, c, w) {}

    bool checkMove(int toRow, int toCol, Piece* board[8][8]) override
    {
        int rd = toRow - row;
        int cd = toCol - col;
        if (rd < 0) rd = -rd;
        if (cd < 0) cd = -cd;

        // bishop only moves diagonally so row diff must equal col diff
        if (rd != cd || rd == 0) return false;

        if (board[toRow][toCol] != nullptr)
            if (board[toRow][toCol]->isWhite() == white) return false;

        // step direction
        int rStep = (toRow > row) ? 1 : -1;
        int cStep = (toCol > col) ? 1 : -1;

        int r = row + rStep;
        int c = col + cStep;
        while (r != toRow || c != toCol)
        {
            if (board[r][c] != nullptr) return false;
            r += rStep;
            c += cStep;
        }

        return true;
    }

    wstring getSymbol() override
    {
        if (white) return L"\u2657";
        else return L"\u265D";
    }
};

// QUEEN CLASS
// inherits from Piece
// queen combines rook and bishop movement
class Queen : public Piece
{
public:
    Queen(int r, int c, bool w) : Piece(r, c, w) {}

    bool checkMove(int toRow, int toCol, Piece* board[8][8]) override
    {
        if (toRow == row && toCol == col) return false;

        if (board[toRow][toCol] != nullptr)
            if (board[toRow][toCol]->isWhite() == white) return false;

        int rd = toRow - row;
        int cd = toCol - col;
        if (rd < 0) rd = -rd;
        if (cd < 0) cd = -cd;

        bool straight = (toRow == row || toCol == col);
        bool diagonal = (rd == cd);

        if (!straight && !diagonal) return false;

        if (straight)
        {
            if (toRow == row)
            {
                int s = col, e = toCol;
                if (s > e) { int t = s; s = e; e = t; }
                for (int i = s + 1; i < e; i++)
                    if (board[row][i] != nullptr) return false;
            }
            else
            {
                int s = row, e = toRow;
                if (s > e) { int t = s; s = e; e = t; }
                for (int i = s + 1; i < e; i++)
                    if (board[i][col] != nullptr) return false;
            }
        }
        else
        {
            int rStep = (toRow > row) ? 1 : -1;
            int cStep = (toCol > col) ? 1 : -1;
            int r = row + rStep;
            int c = col + cStep;
            while (r != toRow || c != toCol)
            {   
                if (board[r][c] != nullptr) return false;
                r += rStep;
                c += cStep;
            }
        }

        return true;
    }

    wstring getSymbol() override
    {
        if (white) return L"\u2655";
        else return L"\u265B";
    }
};

// KING CLASS
// inherits from Piece
class King : public Piece
{
public:
    King(int r, int c, bool w) : Piece(r, c, w) {}

    bool checkMove(int toRow, int toCol, Piece* board[8][8]) override
    {
        int rd = toRow - row;
        int cd = toCol - col;
        if (rd < 0) rd = -rd;
        if (cd < 0) cd = -cd;

        // king moves only 1 square in any direction
        if (rd > 1 || cd > 1) return false;
        if (toRow == row && toCol == col) return false;

        if (board[toRow][toCol] != nullptr)
            if (board[toRow][toCol]->isWhite() == white) return false;

        return true;
    }

    wstring getSymbol() override
    {
        if (white) return L"\u2654";
        else return L"\u265A";
    }
};

// PAWN CLASS
// inherits from Piece
class Pawn : public Piece
{
public:
    Pawn(int r, int c, bool w) : Piece(r, c, w) {}

    bool checkMove(int toRow, int toCol, Piece* board[8][8]) override
    {
        // white goes up (row decreases), black goes down (row increases)
        int dir = white ? -1 : 1;
        int startRow = white ? 6 : 1;

        // one step forward into empty square
        if (toCol == col && toRow == row + dir && board[toRow][toCol] == nullptr)
            return true;

        // two steps forward from starting row if both squares are empty
        if (toCol == col && row == startRow && toRow == row + 2 * dir)
        {
            if (board[row + dir][col] == nullptr && board[toRow][toCol] == nullptr)
                return true;
        }

        // capture diagonally
        int cd = toCol - col;
        if (cd < 0) cd = -cd;
        if (cd == 1 && toRow == row + dir && board[toRow][toCol] != nullptr)
        {
            if (board[toRow][toCol]->isWhite() != white)
                return true;
        }

        return false;
    }

    wstring getSymbol() override
    {
        if (white) return L"\u2659";
        else return L"\u265F";
    }
};

// BOARD CLASS
// composition: Board contains an array of Piece pointers
class Board
{
public:
    Piece* grid[8][8];

    Board()
    {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                grid[r][c] = nullptr;
        placePieces();
    }

    ~Board()
    {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                delete grid[r][c];
    }

    void placePieces()
    {
       //write
    }

    // tries to move piece, returns 0 = invalid, 1 = moved, 2 = king captured
    int movePiece(int fromRow, int fromCol, int toRow, int toCol)
    {
        Piece* p = grid[fromRow][fromCol];
        if (p == nullptr) return 0;

        if (!p->checkMove(toRow, toCol, grid)) return 0;

        bool kingGone = false;
        if (grid[toRow][toCol] != nullptr)
        {
            // using dynamic_cast to check if captured piece is a King
            King* k = dynamic_cast<King*>(grid[toRow][toCol]);
            if (k != nullptr) kingGone = true;
            delete grid[toRow][toCol];
        }       

        grid[toRow][toCol] = p;
        grid[fromRow][fromCol] = nullptr;
        p->setRow(toRow);
        p->setCol(toCol);

        if (kingGone) return 2;
        return 1;
    }
};

// GAME CLASS
// handles the window, mouse input, drawing, and turn management
class Game
{
    Board board;
    sf::Font font;
    bool whiteTurn;
    bool selected;    // whether a piece is currently selected
    int selRow;
    int selCol;
    bool over;        // true when game has ended
    wstring msg;      // message shown at bottom of screen

public:
    Game()
    {
        whiteTurn = true;
        selected = false;
        selRow = -1;
        selCol = -1;
        over = false;
        msg = L"White's Turn";
        font.loadFromFile("C:\\Windows\\Fonts\\seguisym.ttf");
    }

    void run()
    {
      //write
    }

    void handleClick(int mx, int my)
    {
       //write
    }

    void drawBoard(sf::RenderWindow& window)
    {
        sf::Color light(240, 217, 181);
        sf::Color dark(181, 136, 99);

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                sf::RectangleShape sq(sf::Vector2f(tileSize, tileSize));
                sq.setPosition(c * tileSize, r * tileSize);

                if ((r + c) % 2 == 1)
                    sq.setFillColor(light);
                else
                    sq.setFillColor(dark);

                window.draw(sq);
            }
        }
    }

    void drawPieces(sf::RenderWindow& window)
    {
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                Piece* p = board.grid[r][c];
                if (p == nullptr) continue;

                sf::Text txt;
                txt.setFont(font);
                txt.setString(p->getSymbol());
                txt.setCharacterSize(52);

                if (p->isWhite())
                {
                    txt.setFillColor(sf::Color::White);
                    txt.setOutlineColor(sf::Color::Black);
                }
                else
                {
                    txt.setFillColor(sf::Color::Black);
                    txt.setOutlineColor(sf::Color::White);
                }

                txt.setOutlineThickness(1.5f);

                // center symbol inside the square
                sf::FloatRect b = txt.getLocalBounds();
                float x = c * tileSize + (tileSize - b.width) / 2 - b.left;
                float y = r * tileSize + (tileSize - b.height) / 2 - b.top - 4;
                txt.setPosition(x, y);

                window.draw(txt);
            }
        }
    }

    void drawMsg(sf::RenderWindow& window)
    {
        // dark bar at the bottom
        sf::RectangleShape bar(sf::Vector2f(640, 50));
        bar.setPosition(0, 640);
        bar.setFillColor(sf::Color(50, 50, 50));
        window.draw(bar);

        sf::Text txt;
        txt.setFont(font);
        txt.setString(msg);
        txt.setCharacterSize(22);
        txt.setFillColor(sf::Color::White);
        txt.setPosition(10, 648);
        window.draw(txt);
    }
};

int main()
{
    Game g;
    g.run();
    return 0;
}