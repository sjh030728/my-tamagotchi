#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

class Tamagotchi {
private:
    string name;
    int hunger;      // 0 ~ 100 (Higher means hungrier)
    int happiness;   // 0 ~ 100 (Higher means happier)
    int energy;      // 0 ~ 100 (Higher means more energetic)
    int age;         // Age (Turns passed)
    bool alive;

    // Stat clamping helper
    void clampStats() {
        hunger = max(0, min(100, hunger));
        happiness = max(0, min(100, happiness));
        energy = max(0, min(100, energy));

        if (hunger >= 100 || energy <= 0) {
            alive = false;
        }
    }

public:
    // Constructor
    Tamagotchi(string petName) {
        name = petName;
        hunger = 30;
        happiness = 50;
        energy = 70;
        age = 1;
        alive = true;
    }

    // Default constructor for loading
    Tamagotchi() : name(""), hunger(0), happiness(0), energy(0), age(0), alive(false) {}

    // Time passes
    void passTime() {
        if (!alive) return;
        age++;
        hunger += 15;
        happiness -= 10;
        energy -= 5;
        clampStats();
    }

    // Action A: Feed
    void feed() {
        cout << "\n[*] You fed delicious snacks to " << name << "!\n";
        hunger -= 30;
        energy += 5;
        passTime();
    }

    // Action B: Play
    void play() {
        if (energy < 20) {
            cout << "\n[!] " << name << " is too exhausted to play! Please let them sleep.\n";
            return;
        }
        cout << "\n[*] You played a fun game with " << name << "!\n";
        happiness += 30;
        energy -= 20;
        hunger += 10;
        passTime();
    }

    // Action C: Sleep
    void sleep() {
        cout << "\n[zZz] " << name << " took a deep sleep and recovered energy.\n";
        energy = 100;
        hunger += 20;
        passTime();
    }

    // Status Display with ASCII Art
    void displayStatus() {
        cout << "\n========================================\n";
        cout << " [NAME] " << name << " (Age: " << age << ")\n";
        cout << "========================================\n";

        if (!alive) {
            cout << "      ( X _ X ) \n";
            cout << "   " << name << " has passed away...\n";
        }
        else if (hunger >= 70 || energy <= 30) {
            cout << "      ( ; n ; ) dynamic error...\n";
            cout << "   [STATUS: DANGER] Hungry or Exhausted!\n";
        }
        else if (happiness >= 75) {
            cout << "      ( ^ Q ^ )/ *~<3 \n";
            cout << "   [STATUS: VERY HAPPY] Feeling awesome!\n";
        }
        else {
            cout << "      ( o v o ) \n";
            cout << "   [STATUS: NORMAL] Safe and peaceful.\n";
        }

        cout << "----------------------------------------\n";
        cout << " [H] Hunger    : " << hunger << " / 100  (100 is Danger)\n";
        cout << " [P] Happiness : " << happiness << " / 100\n";
        cout << " [E] Energy    : " << energy << " / 100\n";
        cout << "========================================\n";
    }

    bool isAlive() { return alive; }
    string getName() { return name; }
    int getAge() { return age; }

    // Save Game Data
    void saveToFile(const string& filename) {
        ofstream saveFile(filename);
        if (saveFile.is_open()) {
            saveFile << name << "\n" << hunger << "\n" << happiness << "\n" << energy << "\n" << age << "\n" << alive;
            saveFile.close();
            cout << "\n[SAVE] Game data successfully saved to '" << filename << "'!\n";
        }
        else {
            cout << "\n[ERROR] Failed to save game data.\n";
        }
    }

    // Load Game Data
    bool loadFromFile(const string& filename) {
        ifstream saveFile(filename);
        if (saveFile.is_open()) {
            saveFile >> name >> hunger >> happiness >> energy >> age >> alive;
            saveFile.close();
            return true;
        }
        return false;
    }
};

int main() {
    cout << "=== Welcome to CLI Tamagotchi Simulator ===\n";
    cout << "1. New Game\n2. Load Game (Continue)\nSelect: ";

    int startChoice;
    cin >> startChoice;

    Tamagotchi pet("");
    const string saveFilename = "tamagotchi_save.txt";

    if (startChoice == 2) {
        if (pet.loadFromFile(saveFilename)) {
            cout << "\n[SUCCESS] Game data loaded successfully!\n";
        }
        else {
            cout << "\n[ERROR] No save file found. Starting a New Game instead.\n";
            startChoice = 1;
        }
    }

    if (startChoice == 1) {
        cout << "\nEnter your Tamagotchi's name: ";
        string name;
        cin >> name;
        pet = Tamagotchi(name);
    }

    // Main Game Loop
    while (pet.isAlive()) {
        pet.displayStatus();

        // 30턴 생존 시 클리어 (이모지 제거 버전)
        if (pet.getAge() >= 30) {
            cout << "\n========================================\n";
            cout << " *** CONGRATULATIONS! ***\n";
            cout << " " << pet.getName() << " has successfully grown into an adult!\n";
            cout << " You officially CLEARED the game! Excellent job!\n";
            cout << "========================================\n";
            return 0;
        }

        cout << "1. Feed | 2. Play | 3. Sleep | 4. Save & Exit\nSelect: ";

        int action;
        cin >> action;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[WARNING] Invalid input! Please enter a number.\n";
            continue;
        }

        if (action == 1) {
            pet.feed();
        }
        else if (action == 2) {
            pet.play();
        }
        else if (action == 3) {
            pet.sleep();
        }
        else if (action == 4) {
            pet.saveToFile(saveFilename);
            cout << "Goodbye! See you again soon.\n";
            return 0;
        }
        else {
            cout << "\n[WARNING] Invalid choice. Please choose between 1 and 4.\n";
        }
    }

    // Final death state print
    pet.displayStatus();
    cout << "[GAME OVER] " << pet.getName() << " has left the game. Try better next time!\n";

    return 0;
}