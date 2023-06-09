#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

bool gameover;
const int width = 25;
const int height = 35;
const int maxSnakeSize = width * height;
int a[maxSnakeSize], b[maxSnakeSize];
int fruitA, fruitB, score;
enum eD { stop = 0, Left, Right, up, down };
eD wsda;
int snakeSize;

void setup() {// �������� �������� �������� ��� ���
    gameover = false;
    wsda = stop;
    a[0] = width / 2;
    b[0] = height / 2;
    fruitA = rand() % width;
    fruitB = rand() % height;
    score = 0;
    snakeSize = 1;
}

void draw(sf::RenderWindow& window) {// ³������ �� ����������� ��� �� ���
    window.clear(sf::Color(35, 148, 30));

    // ���� ������� ������� �������� ����
    sf::Vector2u windowSize = window.getSize();
    float cellWidth = windowSize.x / static_cast<float>(width);
    float cellHeight = windowSize.y / static_cast<float>(height);

    // ����� ����
    sf::RectangleShape gridCell(sf::Vector2f(cellWidth, cellHeight));
    gridCell.setFillColor(sf::Color(35, 148, 30));
    gridCell.setOutlineThickness(2.0f);  // ������� ����
    gridCell.setOutlineColor(sf::Color(16, 77, 14));  // ���� ����
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            gridCell.setPosition(i * cellWidth, j * cellHeight);
            window.draw(gridCell);
        }
    }

    // ����� �����
    sf::RectangleShape snakeSegment(sf::Vector2f(cellWidth, cellHeight));
    snakeSegment.setFillColor(sf::Color(240, 199, 27));
    snakeSegment.setOutlineThickness(3.0f);  // ������� ������� �����
    snakeSegment.setOutlineColor(sf::Color(99, 75, 12));  // ���� ������� �����
    for (int i = 0; i < snakeSize; i++) {
        snakeSegment.setPosition(a[i] * cellWidth, b[i] * cellHeight);
        window.draw(snakeSegment);
    }

    // ����� �����
    sf::RectangleShape fruit(sf::Vector2f(cellWidth, cellHeight));
    fruit.setFillColor(sf::Color(232, 19, 68));
    fruit.setOutlineThickness(5.0f);  // ������� ������� ������
    fruit.setOutlineColor(sf::Color(99, 12, 27));  // ���� ������� ������
    fruit.setPosition(fruitA * cellWidth, fruitB * cellHeight);
    window.draw(fruit);

    // ����� �������
    sf::Font font;
    if (!font.loadFromFile("ArialNarrow.ttf")) {
        cout << "Font not found." << endl;
        return;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineThickness(3.0f);  // ������� �������
    scoreText.setOutlineColor(sf::Color(75, 75, 75));  // ���� ������� �������
    scoreText.setString("Score: " + to_string(score));
    scoreText.setPosition(10, 10);
    window.draw(scoreText);

    window.display();
}

void input(sf::RenderWindow& window) {// �������� �������� �����������
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::A:
                wsda = Left;
                break;
            case sf::Keyboard::D:
                wsda = Right;
                break;
            case sf::Keyboard::W:
                wsda = up;
                break;
            case sf::Keyboard::S:
                wsda = down;
                break;
            case sf::Keyboard::Escape:
                gameover = true;
                break;
            }
        }
    }
}

bool isSnakeCollision() { // �������� �� ��������� � ����� �����
    for (int i = 1; i < snakeSize; i++) {
        if (a[i] == a[0] && b[i] == b[0]) {
            return true;
        }
    }
    return false;
}

void logic(sf::RenderWindow& window) {// ³������ �� ����� ���
    int prevA = a[snakeSize - 1];
    int prevB = b[snakeSize - 1];
    for (int i = snakeSize - 1; i > 0; i--) {
        a[i] = a[i - 1];
        b[i] = b[i - 1];
    }
    switch (wsda) {
    case Left:
        a[0]--;
        break;
    case Right:
        a[0]++;
        break;
    case up:
        b[0]--;
        break;
    case down:
        b[0]++;
        break;
    }
    if (a[0] >= width || a[0] < 0 || b[0] >= height || b[0] < 0 || isSnakeCollision()) {
        gameover = true;
        cout << "GameOver" << endl;
        return;
    }
    if (a[0] == fruitA && b[0] == fruitB) {
        score += 10;
        fruitA = rand() % width;
        fruitB = rand() % height;
        if (snakeSize < maxSnakeSize) {
            snakeSize++;
            a[snakeSize - 1] = prevA;
            b[snakeSize - 1] = prevB;
        }
    }
    sf::sleep(sf::milliseconds(175));

    draw(window);
}

void startGame(sf::RenderWindow& window) {//������ ���
    setup();
    while (window.isOpen() && !gameover) {
        input(window);
        logic(window);
    }
}

void setting() {// ������������
    cout << "under development" << endl;
}

void userManual() {// ���������� �����������
    cout << "Control keys: WSDA. The main thing is not to hit the boundaries of the field and the body of the snake" << endl;
}

void exitGame() {// ����� � ���
    cout << "Exiting the game..." << endl;
    exit(0);
}

void displayMenu() {//  �������� ���� ��� �� �������
    cout << "1. Start Game" << endl;
    cout << "2. Settings" << endl;
    cout << "3. Instruction" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {// ������� ���
    int choice;
    do {
        displayMenu();
        cin >> choice;
        switch (choice) {
        case 1: {
            sf::RenderWindow window(sf::VideoMode(width * 20, height * 20), "Snake Game");
            startGame(window);
            break;
        }
        case 2:
            setting();
            break;
        case 3:
            userManual();
            break;
        case 4:
            exitGame();
            break;
        default:
            cout << "Problem." << endl;
        }
    } while (choice != 4);

    return 0;
}
//�� ��� ������� ���������� ����� 24 �����
//�� ������ ������� ������� ������ 27 ����� 
