#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Player {
public:
    static int nextId;
    int id;
    int clubId;
    string name;
    int number;
    string position;

    Player(int clubID, const string& name, int num, const string& pos)
        : id(nextId++), clubId(clubID), name(name), number(num), position(pos) {}

    Player(int id_, int clubID, const string& name, int num, const string& pos)
        : id(id_), clubId(clubID), name(name), number(num), position(pos) {}

    int getId() const {
        return id;
    }

    int getClubId() const {
        return clubId;
    }

    string getName() const {
        return name;
    }

    int getNumber() const {
        return number;
    }

    string getPosition() const {
        return position;
    }

    void print() const {
        cout << "Player ID: " << id << endl;
        cout << "Club ID: " << clubId << endl;
        cout << "Name: " << name << endl;
        cout << "Number: " << number << endl;
        cout << "Position: " << position;
    }
};

int Player::nextId = 0;

class Club {
public:
    static int nextId;
    int id;
    string name;
    string city;
    string country;
    vector<Player> players;

    Club(const string& clubName, const string& clubCity, const string& clubCountry)
        : id(nextId++), name(clubName), city(clubCity), country(clubCountry) {}

    Club(int id_, const string& clubName, const string& clubCity, const string& clubCountry)
        : id(id_), name(clubName), city(clubCity), country(clubCountry) {}

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getCity() const {
        return city;
    }

    string getCountry() const {
        return country;
    }

    const vector<Player>& getPlayers() const {
        return players;
    }

    void addPlayer(const Player& player) {
        players.push_back(player);
    }

    void print() const {
        cout << "Club ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "City: " << city << endl;
        cout << "Country: " << country << endl;
        cout << "Players:" << endl << endl;
        for (const auto& player : players) {
            player.print();
            cout << endl;
        }
    }
};

int Club::nextId = 0;

int findClubIndexById(const vector<Club>& clubs, int index) {
    int foundIndex = -1;
    for (size_t i = 0; i < clubs.size(); ++i) {
        if (clubs[i].getId() == index) {
            foundIndex = i;
            break;
        }
    }
    return foundIndex;
}

int findPlayerIndexById(const vector<Player>& players, int playerId) {
    int foundIndex = -1;
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i].getId() == playerId) {
            foundIndex = i;
            break;
        }
    }
    return foundIndex;
}

vector<Club> readFile() {
    vector<Club> clubs;
    ifstream masterFile("clubs.txt");
    ifstream slaveFile("players.txt");

    int maxClubId = 0;
    int maxPlayerId = 0;

    if (masterFile.is_open() && slaveFile.is_open()) {
        string line;
        while (getline(masterFile, line)) {
            stringstream ss(line);
            string token;

            getline(ss, token, ';');
            int clubId = stoi(token);
            maxClubId = max(maxClubId, clubId);

            getline(ss, token, ';');
            string name = token;

            getline(ss, token, ';');
            string city = token;

            getline(ss, token, ';');
            string country = token;

            Club club(clubId, name, city, country);

            clubs.push_back(club);
        }

        while (getline(slaveFile, line)) {
            stringstream ss(line);
            string token;

            getline(ss, token, ';');
            int playerId = stoi(token);
            maxPlayerId = max(maxPlayerId, playerId);

            getline(ss, token, ';');
            int clientId = stoi(token);

            getline(ss, token, ';');
            string name = token;
            
            getline(ss, token, ';');
            int number = stoi(token);

            getline(ss, token, ';');
            string position = token;

            for (auto& club : clubs) {
                if (club.getId() == clientId) {
                    Player player(playerId, clientId, name, number, position);
                    club.addPlayer(player);
                    break;
                }
            }
        }

        masterFile.close();
        slaveFile.close();

        Club::nextId = maxClubId + 1;
        Player::nextId = maxPlayerId + 1;
    }
    else {
        cerr << "Error: Unable to open file for reading" << endl;
    }

    return clubs;
}

void writeFile(const vector<Club>& clubs) {
    ofstream masterFile("clubs.txt");
    ofstream slaveFile("players.txt");

    if (masterFile.is_open() && slaveFile.is_open()) {
        for (const auto& club : clubs) {
            masterFile << club.getId() << ";" << club.getName() << ";" << club.getCity() << ";" << club.getCountry() << endl;

            const vector<Player>& players = club.getPlayers();
            for (const auto& player : players) {
                slaveFile << player.getId() << ";" << player.getClubId() << ";" << player.getName() << ";" << player.getNumber() << ";" << player.getPosition() << endl;
            }
        }
        masterFile.close();
        slaveFile.close();
    }
    else {
        cerr << "Error: Unable to open file for writing" << endl;
    }
}

void deleteClub(vector<Club>& clubs, int index) {
    int foundIndex = findClubIndexById(clubs, index);
    if (foundIndex >= 0 && foundIndex < clubs.size()) {
        clubs.erase(clubs.begin() + foundIndex);
    }
    else {
        cerr << "Error: Invalid club id" << endl;
    }
}

void deletePlayer(vector<Club>& clubs, int clubId, int playerId) {
    int foundClubIndex = findClubIndexById(clubs, clubId);
    if (foundClubIndex >= 0 && foundClubIndex < clubs.size()) {
        std::vector<Player>& players = clubs[foundClubIndex].players;
        int foundPlayerIndex = findPlayerIndexById(players, playerId);
        if (foundPlayerIndex >= 0 && foundPlayerIndex < players.size()) {
            players.erase(players.begin() + foundPlayerIndex);
        }
        else {
            cerr << "Error: Invalid player id" << endl;
        }
    }
    else {
        cerr << "Error: Invalid club id" << endl;
    }
}

void updateClubField(Club& club, const string& field, const string& value) {
    if (field == "name") {
        club.name = value;
    }
    else if (field == "city") {
        club.city = value;
    }
    else if (field == "country") {
        club.country = value;
    }
    else {
        cerr << "Error: Invalid field name" << endl;
    }
}

Player updatePlayerField(Player player, const string& field, const string& value) {
    if (field == "name") {
        player.name = value;
    }
    else if (field == "number") {
        istringstream iss(value);
        int num;
        if (iss >> num) {
            player.number = num;
        }
        else {
            cerr << "Error: Invalid number value" << endl;
        }
    }
    else if (field == "position") {
        player.position = value;
    }
    else {
        cerr << "Error: Invalid field name" << endl;
    }
    return player;
}

void countRecords(const vector<Club>& clubs) {
    int totalClubs = clubs.size();
    int totalPlayers = 0;
    for (const auto& club : clubs) {
        totalPlayers += club.getPlayers().size();
    }
    cout << "Total clubs: " << totalClubs << endl;
    cout << "Total players: " << totalPlayers << endl;
}

void printFileContents(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cerr << "Error: Unable to open file for reading" << endl;
    }
}

int main() {
    vector<Club> clubs = readFile();

    int choice;
    do {
        cout << "1) get-m" << endl;
        cout << "2) get-s" << endl;
        cout << "3) del-m" << endl;
        cout << "4) del-s" << endl;
        cout << "5) update-m" << endl;
        cout << "6) update-s" << endl;
        cout << "7) insert-m" << endl;
        cout << "8) insert-s" << endl;
        cout << "9) calc-m" << endl;
        cout << "10) calc-s" << endl;
        cout << "11) ut-m" << endl;
        cout << "12) ut-s" << endl;
        cout << "Enter your choice (1-12): ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int id;
            cout << "Enter club id: ";
            cin >> id;
            int foundIndex = findClubIndexById(clubs, id);
            if (foundIndex >= 0 && foundIndex < clubs.size()) {
                clubs[foundIndex].print();
                writeFile(clubs);
            }
            else {
                cerr << "Error: Invalid id" << endl;
            }
            break;
        }
        case 2: {
            int playerId;
            cout << "Enter player id: ";
            cin >> playerId;
            int foundPlayerIndex = -1;
            for (const auto& club : clubs) {
                foundPlayerIndex = findPlayerIndexById(club.players, playerId);
                if (foundPlayerIndex != -1) {
                    club.players[foundPlayerIndex].print();
                    writeFile(clubs);
                    break;
                }
            }

            cerr << "Error: Invalid player id" << endl;
            break;
        }
        case 3: {
            int id;
            cout << "Enter club id to delete: ";
            cin >> id;
            deleteClub(clubs, id);
            writeFile(clubs);
            break;
        }
        case 4: {
            int clubId, playerId;
            cout << "Enter club id: ";
            cin >> clubId;
            cout << "Enter player id to delete: ";
            cin >> playerId;
            deletePlayer(clubs, clubId, playerId);
            writeFile(clubs);
            break;
        }
        case 5: {
            int id;
            string field, value;
            cout << "Enter club id to update: ";
            cin >> id;
            int foundIndex = -1;
            foundIndex = findClubIndexById(clubs, id);
            if (foundIndex >= 0 && foundIndex < clubs.size()) {
                cout << "Enter field to update (name/city/country): ";
                cin >> field;
                if (field == "name" || field == "city" || field == "country") {
                    cout << "Enter new value: ";
                    cin >> value;
                    updateClubField(clubs[foundIndex], field, value);
                    writeFile(clubs);
                }
                else {
                    cerr << "Error: Invalid field name" << endl;
                }
            }
            else {
                cerr << "Error: Invalid club id" << endl;
            }
            break;
        }
        case 6: {
            int clubId, playerId;
            string field, value;
            cout << "Enter club id: ";
            cin >> clubId;
            int foundClubIndex = -1;
            int foundPlayerIndex = -1;
            foundClubIndex = findClubIndexById(clubs, clubId);
            if (foundClubIndex >= 0 && foundClubIndex < clubs.size()) {
                cout << "Enter player id to update: ";
                cin >> playerId;
                const vector<Player>& players = clubs[foundClubIndex].getPlayers();
                foundPlayerIndex = findPlayerIndexById(players, playerId);
                if (foundPlayerIndex >= 0 && foundPlayerIndex < players.size()) {
                    cout << "Enter field to update (name/number/position): ";
                    cin >> field;
                    if (field == "name" || field == "number" || field == "position") {
                        cout << "Enter new value: ";
                        cin >> value;
                        clubs[foundClubIndex].players[foundPlayerIndex] = updatePlayerField(players[foundPlayerIndex], field, value);
                        writeFile(clubs);
                    }
                    else {
                        cerr << "Error: Invalid field name" << endl;
                    }
                }
                else {
                    cerr << "Error: Invalid player id" << endl;
                }
            }
            else {
                cerr << "Error: Invalid club id" << endl;
            }
            break;
        }
        case 7: {
            string name, city, country;
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter city: ";
            cin >> city;
            cout << "Enter country: ";
            cin >> country;
            clubs.emplace_back(name, city, country);
            cout << "Inserted a club " << name;
            writeFile(clubs);
            break;
        }
        case 8: {
            int clubId;
            string name, position;
            int number;
            cout << "Enter club id to add player: ";
            cin >> clubId;
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter number: ";
            cin >> number;
            cout << "Enter position: ";
            cin >> position;
            int foundIndex = -1;
            foundIndex = findClubIndexById(clubs, clubId);
            if (foundIndex >= 0 && foundIndex < clubs.size()) {
                cout << "Inserted a player " << name;
                clubs[foundIndex].addPlayer(Player(clubs[foundIndex].getId(), name, number, position));
            }
            else {
                cerr << "Error: Invalid club id" << endl;
            }
            writeFile(clubs);
            break;
        }
        case 9: {
            countRecords(clubs);
            writeFile(clubs);
            break;
        }
        case 10: {
            int clubId;
            cout << "Enter club id to count players: ";
            cin >> clubId;
            int foundIndex = -1;
            foundIndex = findClubIndexById(clubs, clubId);
            if (foundIndex >= 0 && foundIndex < clubs.size()) {
                cout << "Number of players in club " << clubs[foundIndex].name << ": " << clubs[foundIndex].getPlayers().size() << endl;
                writeFile(clubs);
            }
            else {
                cerr << "Error: Invalid club id" << endl;
            }
            break;
        }
        case 11: {
            printFileContents("clubs.txt");
            writeFile(clubs);
            break;
        }
        case 12: {
            printFileContents("players.txt");
            writeFile(clubs);
            break;
        }
        case 0:
            cout << "Exiting..." << endl;
            writeFile(clubs);
            break;
        default:
            cerr << "Invalid choice. Enter a number between 1 and 12" << endl;
            writeFile(clubs);
        }
        cout << "<--------------------------------------------------------->" << endl;
    } while (choice != 0);

    writeFile(clubs);

    return 0;
}