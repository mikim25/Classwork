//
//  cyborgs.cpp
//  Project 7
//
//  Created by Michelle Kim on 6/2/22.
//

/*
 A position in the arena may have a wall segment. If the player or a cyborg moving of its own free will chooses a direction such that moving would be to a position occupied by a wall or to a point out of the arena, then the player or cyborg doesn't move that turn. If a cyborg moving under the command of the player attempts to move into a wall or to a position out of the arena, it doesn't move that turn, but suffers 1 unit of damage. A cyborg that has suffered 3 units of damage is dead and is removed from the game.
 
 Each turn
 - x: stand (or attempt to move onto wall/out of arena)
 - n/e/s/w: move one step
 - enter: move chosen by computer
 - death if player and cyborg overlap
 -------
 - attempt broadcast (1/2/3 then n/e/s/w)
 
 Cyborg turn
 - 1/2 probability that broadcast succeeds (all cyborgs listening on that channel move one step in the direction commanded if they can; if they would hit a wall or go out of bounds, stay put and take 1 damage)
 - 1/2 chance broadcast fails: cyborgs move normally (each cyborg picks a random direction (north, east, south, west) with equal probability, moves one step in that direction if it can; if the cyborg attempts to move into a wall or out of the arena, however, it does not move and suffers no damage)
 - More than one cyborg may occupy the same position; the display will show a digit character indicating the channel number of one of the cyborgs at that point; it doesn't matter which one.
 */

/* We've provided some incorrect return statements (indicated) to allow for compilation. First, implement the TRIVIAL functions.  Then get Arena::display going.
 
 You may add or remove private data members or private member functions, or change their types. Do not do this for public members or functions. Global variables aren't allowed, global constants are fine.
 
 Any member functions you implement must never put an object into an invalid state.
 Any function that has a reasonable way of indicating failure through its return value should do so.
 
 */

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;              // max number of rows in the arena
const int MAXCOLS = 20;              // max number of columns in the arena
const int MAXCYBORGS = 100;          // max number of cyborgs allowed
const int MAXCHANNELS = 3;           // max number of channels
const int INITIAL_CYBORG_HEALTH = 3; // initial cyborg health
const double WALL_DENSITY = 0.11;    // density of walls

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;
const int BADDIR = -1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Cyborg declaration.

class Cyborg
{
  public:
      // Constructor
    Cyborg(Arena* ap, int r, int c, int channel);

      // Accessors
    int  row() const;
    int  col() const;
    int  channel() const;
    bool isDead() const;

      // Mutators
    void forceMove(int dir);
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_channel;
    int    m_health;
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string stand();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     cyborgCount() const;
    bool    hasWallAt(int r, int c) const;
    int     numberOfCyborgsAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void   placeWallAt(int r, int c);
    bool   addCyborg(int r, int c, int channel);
    bool   addPlayer(int r, int c);
    string moveCyborgs(int channel, int dir);

  private:
    bool    m_wallGrid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Cyborg* m_cyborgs[MAXCYBORGS];
    int     m_nCyborgs;

      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nCyborgs);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
    string takeCyborgsTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char ch);
int randInt(int lowest, int highest);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Cyborg implementation
///////////////////////////////////////////////////////////////////////////

Cyborg::Cyborg(Arena* ap, int r, int c, int channel)
{
    if (ap == nullptr)
    {
        cout << "***** A cyborg must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Cyborg created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    if (channel < 1  ||  channel > MAXCHANNELS)
    {
        cout << "***** Cyborg created with invalid channel " << channel << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_channel = channel;
    m_health = INITIAL_CYBORG_HEALTH;
}

int Cyborg::row() const
{
    return m_row;
}

int Cyborg::col() const
{
      // DONE: TRIVIAL:  Return what column the cyborg is at.
    return m_col;
}

int Cyborg::channel() const
{
      // DONE: TRIVIAL:  Return what channel the cyborg is listening on.
    return m_channel;
}

bool Cyborg::isDead() const
{
      // DONE: TRIVIAL:  Return whether the cyborg is dead.
    return (m_health <= 0);
}

void Cyborg::forceMove(int dir)
{
      // DONE:  Move the cyborg in the indicated direction, if possible.
    if (!isDead())
    {
        // Suffer one unit of damage if moving fails.
        if (attemptMove(*m_arena, dir, m_row, m_col) == false)
            m_health--;
    }
}

void Cyborg::move()
{
      // Attempt to move in a random direction; if cannot move, don't move
    if (!isDead())
        attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col);
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
      // DONE: TRIVIAL:  Return what row the player is at.
    return m_row;
}

int Player::col() const
{
      // DONE: TRIVIAL:  Return what column the player is at.
    return m_col;
}

string Player::stand()
{
    return "Player stands.";
}

string Player::move(int dir)
{
    // DONE:  Attempt to move the player one step in the indicated direction.
    if(attemptMove(*m_arena, dir, m_row, m_col) == false)
        return "Player couldn't move; player stands.";
    // Set player to dead if necessary.
    if(m_arena->numberOfCyborgsAt(m_row, m_col) >= 1)
        return "Player walked into a cyborg and died.";
    if(dir == NORTH)
        return "Player moved north.";
    if(dir == EAST)
        return "Player moved east.";
    if(dir == SOUTH)
        return "Player moved south.";
    if(dir == WEST)
        return "Player moved west.";
    return "Fix line 304!";
}

bool Player::isDead() const
{
      // DONE: TRIVIAL:  Return whether the player is dead.
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nCyborgs = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_wallGrid[r-1][c-1] = false;
}

Arena::~Arena()
{
    // DONE:  Release the player and all remaining dynamically allocated cyborgs.
    delete m_player;
    for (int k = 0; k < m_nCyborgs; k++)
        delete m_cyborgs[k];
}

int Arena::rows() const
{
      // DONE: TRIVIAL:  Return the number of rows in the arena.
    return m_rows;
}

int Arena::cols() const
{
      // DONE: TRIVIAL:  return the number of columns in the arena
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::cyborgCount() const
{
      // DONE: TRIVIAL:  return the number of cyborgs in the arena
    return m_nCyborgs;
}

bool Arena::hasWallAt(int r, int c) const
{
    checkPos(r, c, "Arena::hasWallAt");
    return m_wallGrid[r-1][c-1];
}

int Arena::numberOfCyborgsAt(int r, int c) const
{
    int count = 0;
      // DONE:  Return the number of cyborgs at row r, column c.
    for (int k = 0; k < m_nCyborgs; k++)
    {
        if (m_cyborgs[k]->row() == r && m_cyborgs[k]->col() == c)
            count++;
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (wall)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (hasWallAt(r, c) ? '*' : '.');

      // Indicate cyborg positions by their channels.  If more than one cyborg
      // occupies a cell, show just one (any one will do).

      // DONE:  For each cyborg, set the grid cell to the digit character
      //        representing its channel number.
    
    for (int k = 0; k < m_nCyborgs; k++)
    {
        displayGrid[m_cyborgs[k]->row()-1][m_cyborgs[k]->col()-1] = m_cyborgs[k]->channel() + '0';
    }

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, cyborg, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << cyborgCount() << " cyborgs remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
}

void Arena::placeWallAt(int r, int c)
{
    checkPos(r, c, "Arena::placeWallAt");
    m_wallGrid[r-1][c-1] = true;
}

bool Arena::addCyborg(int r, int c, int channel)
{
    if (! isPosInBounds(r, c)  ||  hasWallAt(r, c))
        return false;
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    if (channel < 1  ||  channel > MAXCHANNELS)
        return false;
    if (m_nCyborgs == MAXCYBORGS)
        return false;
    m_cyborgs[m_nCyborgs] = new Cyborg(this, r, c, channel);
    m_nCyborgs++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (m_player != nullptr  ||  ! isPosInBounds(r, c)  ||  hasWallAt(r, c))
        return false;
    if (numberOfCyborgsAt(r, c) > 0)
        return false;
    m_player = new Player(this, r, c);
    return true;
}

string Arena::moveCyborgs(int channel, int dir)
{
      // Cyborgs on the channel will respond with probability 1/2
    bool willRespond = (randInt(0, 1) == 0);

    // DONE:  Move all cyborgs.
    int nCyborgsOriginally = m_nCyborgs;
    
    for (int k = 0; k < m_nCyborgs; k++)
    {
        // Force cyborgs listening on the channel to move in the indicated direction.
        if (willRespond == true && m_cyborgs[k]->channel() == channel)
            m_cyborgs[k]->forceMove(dir);
        // If willRespond is false, or if the cyborg is listening on a different channel, it just moves.
        else
            m_cyborgs[k]->move();
    }
    
    // Mark the player dead if necessary.
    for (int j = 0; j < m_nCyborgs; j++)
    {
        if (m_cyborgs[j]->row() == m_player->row() && m_cyborgs[j]->col() == m_player->col())
        {
            m_player->setDead();
            break;
        }
    }
    
    // Release dead dynamically allocated cyborgs.
    for (int i = 0; i < m_nCyborgs; i++)
    {
        if (m_cyborgs[i]->isDead())
        {
            delete m_cyborgs[i];
            // Move all cyborg pointers 1 position to the left.
            for (int h = i; h+1 < m_nCyborgs; h++)
                m_cyborgs[h] = m_cyborgs[h+1];
            m_nCyborgs--;
        }
    }

    if (m_nCyborgs < nCyborgsOriginally)
        return "Some cyborgs have been destroyed.";
    else
        return "No cyborgs were destroyed.";
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nCyborgs)
{
    if (nCyborgs < 0  ||  nCyborgs > MAXCYBORGS)
    {
        cout << "***** Game created with invalid number of cyborgs:  "
             << nCyborgs << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nCyborgs - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nCyborgs << " cyborgs!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add some walls in WALL_DENSITY of the empty spots
    assert(WALL_DENSITY >= 0  &&  WALL_DENSITY <= 1);
    int nWalls = static_cast<int>(WALL_DENSITY * nEmpty);
    while (nWalls > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->hasWallAt(r, c))
            continue;
        m_arena->placeWallAt(r, c);
        nWalls--;
    }

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->hasWallAt(rPlayer, cPlayer));
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with cyborgs
    while (nCyborgs > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->hasWallAt(r, c)  ||  (r == rPlayer && c == cPlayer))
            continue;
        m_arena->addCyborg(r, c, randInt(1, MAXCHANNELS));
        nCyborgs--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->stand();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->stand();
            else
            {
                dir = decodeDirection(tolower(playerMove[0]));
                if (dir != BADDIR)
                    return player->move(dir);
            }
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

string Game::takeCyborgsTurn()
{
    for (;;)
    {
        cout << "Broadcast (e.g., 2n): ";
        string broadcast;
        getline(cin, broadcast);
        if (broadcast.size() != 2)
        {
            cout << "You must specify a channel followed by a direction." << endl;
            continue;
        }
        else if (broadcast[0] < '1'  ||  broadcast[0] > '0'+MAXCHANNELS)
            cout << "Channel must be a digit in the range 1 through "
                 << MAXCHANNELS << "." << endl;
        else
        {
            int dir = decodeDirection(tolower(broadcast[1]));
            if (dir == BADDIR)
                cout << "Direction must be n, e, s, or w." << endl;
            else
                return m_arena->moveCyborgs(broadcast[0]-'0', dir);
        }
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->cyborgCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        msg = takeCyborgsTurn();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'n':  return NORTH;
      case 'e':  return EAST;
      case 's':  return SOUTH;
      case 'w':  return WEST;
    }
    return BADDIR;  // bad argument passed in!
}

  // Return a random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

  // DONE:  Implement this function.
  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would hit a wall or run off the edge of the
  // arena.  Otherwise, update r and c to the position resulting from the
  // move and return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    // If moving would go into wall or out of bounds, don't move
    if (
    (dir == 0 && r == 1)        || (dir == 0 && a.hasWallAt(r-1,c)) ||
    (dir == 1 && c == a.cols()) || (dir == 1 && a.hasWallAt(r,c+1)) ||
    (dir == 2 && r == a.rows()) || (dir == 2 && a.hasWallAt(r+1,c)) ||
    (dir == 3 && c == 1)        || (dir == 3 && a.hasWallAt(r,c-1))
       )
    {
        return false;
    }
    else // Move
    {
        switch(dir)
        {
            case 0: // N
                r--;
                break;
            case 1: // E
                c++;
                break;
            case 2: // S
                r++;
                break;
            case 3: // W
                c--;
                break;
        }
        return true;
    }
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should stand; otherwise, bestDir is
  // set to the recommended direction to move.

  // Your replacement implementation should do something intelligent.
  // For example, if you're standing next to four cyborgs, and moving
  // north would put you next to two cyborgs, but moving east would put
  // you next to none, moving east is a safer choice than standing or
  // moving north.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    // DONE:  Replace this implementation:
    int closeCyborgsNow = 0;
    int closeCyborgsNorth = 0;
    int closeCyborgsEast = 0;
    int closeCyborgsSouth = 0;
    int closeCyborgsWest = 0;
    bestDir = 0;
    
    // Sum up cyborgs near player currently
    if (r != 1)
        closeCyborgsNow += a.numberOfCyborgsAt(r-1,c);
    if (c != a.cols())
        closeCyborgsNow += a.numberOfCyborgsAt(r,c+1);
    if (r != a.rows())
        closeCyborgsNow += a.numberOfCyborgsAt(r+1,c);
    if (c != 1)
        closeCyborgsNow += a.numberOfCyborgsAt(r,c-1);
    
    // Sum up cyborgs near player if next move is n
    if (r != 1)
    {
        if (r-1 != 1)
            closeCyborgsNorth += a.numberOfCyborgsAt(r-2,c);
        if (c != a.cols())
            closeCyborgsNorth += a.numberOfCyborgsAt(r-1,c+1);
        if (r-1 != a.rows())
            closeCyborgsNorth += a.numberOfCyborgsAt(r,c);
        if (c != 1)
            closeCyborgsNorth += a.numberOfCyborgsAt(r-1,c-1);
    }
    
    // Sum up cyborgs near player if next move is e
    if (c != a.cols())
    {
        if (r != 1)
            closeCyborgsEast += a.numberOfCyborgsAt(r-1,c+1);
        if (c+1 != a.cols())
            closeCyborgsEast += a.numberOfCyborgsAt(r,c+2);
        if (r != a.rows())
            closeCyborgsEast += a.numberOfCyborgsAt(r+1,c+1);
        if (c+1 != 1)
            closeCyborgsEast += a.numberOfCyborgsAt(r,c);
    }
    
    // Sum up cyborgs near player if next move is s
    if (r != a.rows())
    {
        if (r+1 != 1)
            closeCyborgsSouth += a.numberOfCyborgsAt(r,c);
        if (c != a.cols())
            closeCyborgsSouth += a.numberOfCyborgsAt(r+1,c+1);
        if (r+1 != a.rows())
            closeCyborgsSouth += a.numberOfCyborgsAt(r+2,c);
        if (c != 1)
            closeCyborgsSouth += a.numberOfCyborgsAt(r+1,c-1);
    }
    
    // Sum up cyborgs near player if next move is w
    if (c != 1)
    {
        if (r != 1)
            closeCyborgsWest += a.numberOfCyborgsAt(r-1,c-1);
        if (c-1 != a.cols())
            closeCyborgsWest += a.numberOfCyborgsAt(r,c);
        if (r != a.rows())
            closeCyborgsWest += a.numberOfCyborgsAt(r+1,c-1);
        if (c-1 != 1)
            closeCyborgsWest += a.numberOfCyborgsAt(r,c-2);
    }

    if (closeCyborgsNow <= closeCyborgsNorth &&
        closeCyborgsNow <= closeCyborgsEast &&
        closeCyborgsNow <= closeCyborgsSouth &&
        closeCyborgsNow <= closeCyborgsWest)
        return false;  // Recommend standing

    if (r != 1 && a.numberOfCyborgsAt(r-1,c) == 0 &&
        closeCyborgsNorth <= closeCyborgsNow &&
        closeCyborgsNorth <= closeCyborgsEast &&
        closeCyborgsNorth <= closeCyborgsSouth &&
        closeCyborgsNorth <= closeCyborgsWest)
    {
        bestDir = 0;
        return true;  // Recommend north
    }
    
    if (c != a.cols() && a.numberOfCyborgsAt(r,c+1) == 0 &&
        closeCyborgsEast <= closeCyborgsNorth &&
        closeCyborgsEast <= closeCyborgsNow &&
        closeCyborgsEast <= closeCyborgsSouth &&
        closeCyborgsEast <= closeCyborgsWest)
    {
        bestDir = 1;
        return true;  // Recommend east
    }
    
    if (r != a.rows() && a.hasWallAt(r+1,c) &&
        closeCyborgsSouth <= closeCyborgsNorth &&
        closeCyborgsSouth <= closeCyborgsEast &&
        closeCyborgsSouth <= closeCyborgsNow &&
        closeCyborgsSouth <= closeCyborgsWest)
    {
        bestDir = 2;
        return true;  // Recommend south
    }
    
    if (c != 1 && a.hasWallAt(r,c-1) &&
        closeCyborgsWest <= closeCyborgsNorth &&
        closeCyborgsWest <= closeCyborgsEast &&
        closeCyborgsWest <= closeCyborgsSouth &&
        closeCyborgsWest <= closeCyborgsNow)
    {
        bestDir = 3;
        return true;  // Recommend west
    }
    
    return false;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 50);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
