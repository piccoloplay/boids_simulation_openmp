#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>

// Classe Boid (stessa classe)
class Boid {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;

    Boid(float x, float y) {
        position = sf::Vector2f(x, y);
        velocity = sf::Vector2f(((float)rand() / RAND_MAX - 0.5f) * 1.0f, ((float)rand() / RAND_MAX - 0.5f) * 1.0f);
    }

    void updatePosition() {
        position += velocity;
        if (position.x < 0) position.x = 800;
        if (position.x > 800) position.x = 0;
        if (position.y < 0) position.y = 600;
        if (position.y > 600) position.y = 0;
    }
};

// Calcola la distanza tra due boid (funzione non parallelizzata con nuovo nome)
float distanceNonParallel(const Boid& b1, const Boid& b2) {
    return std::sqrt((b1.position.x - b2.position.x) * (b1.position.x - b2.position.x) +
        (b1.position.y - b2.position.y) * (b1.position.y - b2.position.y));
}

// Applica le regole dei Boid (versione non parallelizzata con nuovo nome)
double applyRulesNonParallel(std::vector<Boid>& boids, int choice, std::vector<sf::VertexArray>& vectors) {
    auto start = std::chrono::high_resolution_clock::now();

    // Ciclo non parallelizzato
    for (int i = 0; i < boids.size(); ++i) {
        sf::Vector2f separation(0.0f, 0.0f);
        sf::Vector2f alignment(0.0f, 0.0f);
        sf::Vector2f cohesion(0.0f, 0.0f);
        int neighborCount = 0;

        // Ciclo interno
        for (int j = 0; j < boids.size(); ++j) {
            if (i != j) {
                float dist = distanceNonParallel(boids[i], boids[j]);
                if (dist < 100.0f) {
                    separation += boids[i].position - boids[j].position;
                    alignment += boids[j].velocity;
                    cohesion += boids[j].position;
                    neighborCount++;
                }
            }
        }

        if (neighborCount > 0) {
            separation.x /= neighborCount;
            separation.y /= neighborCount;
            alignment.x /= neighborCount;
            alignment.y /= neighborCount;
            cohesion.x /= neighborCount;
            cohesion.y /= neighborCount;

            boids[i].velocity += separation * 0.03f;
            boids[i].velocity += (alignment - boids[i].velocity) * 0.05f;
            boids[i].velocity += (cohesion - boids[i].position) * 0.01f;
        }

        sf::Vector2f chosenVector;
        sf::Color vectorColor;
        if (choice == 1) {
            chosenVector = separation * 1.5f;
            vectorColor = sf::Color::Red;
            if (std::sqrt(chosenVector.x * chosenVector.x + chosenVector.y * chosenVector.y) < 5.0f) {
                continue;
            }
        }
        else if (choice == 2) {
            chosenVector = (alignment - boids[i].velocity) * 10.0f;
            vectorColor = sf::Color::Blue;
        }
        else if (choice == 3) {
            chosenVector = (cohesion - boids[i].position) * 0.5f;
            vectorColor = sf::Color::White;
        }

        sf::VertexArray line(sf::Lines, 2);
        line[0].position = boids[i].position;
        line[0].color = vectorColor;
        line[1].position = boids[i].position + chosenVector;
        line[1].color = vectorColor;

        vectors.push_back(line);

        float speed = std::sqrt(boids[i].velocity.x * boids[i].velocity.x + boids[i].velocity.y * boids[i].velocity.y);
        if (speed > 1.5f) {
            boids[i].velocity = (boids[i].velocity / speed) * 1.5f;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

// Calcola la velocità media dei Boid (versione non parallelizzata con nuovo nome)
float calculateAverageSpeedNonParallel(const std::vector<Boid>& boids) {
    float totalSpeed = 0.0f;
    for (const auto& boid : boids) {
        totalSpeed += std::sqrt(boid.velocity.x * boid.velocity.x + boid.velocity.y * boid.velocity.y);
    }
    return totalSpeed / boids.size();
}

int mainZ() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boids Simulation No OpenMP");

    sf::Font font;
    if (!font.loadFromFile("C:/Users/imnot/Documents/UNIFI/Parallel Computing/"
        "ConsoleApplication1/x64/Debug/arial.ttf")) {
        std::cerr << "Errore nel caricamento del font" << std::endl;
        return -1;
    }

    std::vector<Boid> boids;
    for (int i = 0; i < 1000; ++i) {
        boids.push_back(Boid(rand() % 800, rand() % 600));
    }

    int choice;
    std::cout << "Quale vettore vuoi visualizzare?\n1. Separation\n2. Alignment\n3. Cohesion\nInserisci il numero: ";
    std::cin >> choice;

    std::string ruleName = (choice == 1) ? "Separation" : (choice == 2) ? "Alignment" : "Cohesion";

    // Misura del tempo singolo fuori dal ciclo principale
    std::vector<sf::VertexArray> vectors;
    double applyRulesTimeSingolo = applyRulesNonParallel(boids, choice, vectors);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        vectors.clear();
        double applyRulesTime = applyRulesNonParallel(boids, choice, vectors);

        for (int i = 0; i < boids.size(); ++i) {
            boids[i].updatePosition();
        }

        window.clear();
        for (auto& boid : boids) {
            sf::CircleShape shape(5.0f);
            shape.setPosition(boid.position);
            shape.setFillColor(sf::Color::Green);
            window.draw(shape);
        }

        for (auto& vector : vectors) {
            window.draw(vector);
        }

        sf::Text textRule("Regola: " + ruleName, font, 20);
        textRule.setPosition(10, 10);
        textRule.setFillColor(sf::Color::White);

        sf::Text textBoidCount("Numero di Boid: " + std::to_string(boids.size()), font, 20);
        textBoidCount.setPosition(10, 40);
        textBoidCount.setFillColor(sf::Color::White);

        sf::Text textAvgSpeed("Velocità Media: " + std::to_string(calculateAverageSpeedNonParallel(boids)), font, 20);
        textAvgSpeed.setPosition(10, 70);
        textAvgSpeed.setFillColor(sf::Color::White);

        sf::Text textSimTime("Tempo applyRules singolo: " + std::to_string(applyRulesTimeSingolo) + "s", font, 20);
        textSimTime.setPosition(10, 100);
        textSimTime.setFillColor(sf::Color::White);

        window.draw(textRule);
        window.draw(textBoidCount);
        window.draw(textAvgSpeed);
        window.draw(textSimTime);

        window.display();

        sf::sleep(sf::milliseconds(70)); // Rallenta leggermente la simulazione
    }

    return 0;
}
