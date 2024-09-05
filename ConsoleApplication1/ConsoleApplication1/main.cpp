#include <SFML/Graphics.hpp>

int mainZZ() {
    // Crea una finestra di 800x600 pixel
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Crea un cerchio di 50 pixel di raggio
    sf::CircleShape shape(50.0f);
    shape.setFillColor(sf::Color::Green);

    // Ciclo principale della finestra
    while (window.isOpen()) {
        // Gestione degli eventi della finestra
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Pulisce la finestra
        window.clear();

        // Disegna il cerchio
        window.draw(shape);

        // Aggiorna la finestra
        window.display();
    }

    return 0;
}
