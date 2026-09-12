#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
int main() {
    sf::RenderWindow window(sf::VideoMode({800,600}),"Ping Pong em SFML");
    window.setFramerateLimit(30);

    //ball
    sf::CircleShape ball(12.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition({400.f,300.f});

    //velocidade da bola
    sf::Vector2f ballVelocity({7.f,7.f});

    //raquete player1
    sf::RectangleShape raquetePlayer1(sf::Vector2f({20.f,100.f}));
    raquetePlayer1.setFillColor(sf::Color::Blue);
    raquetePlayer1.setPosition({30.f,250.f});

    while(window.isOpen()) {

        // 1. PROCESSAMENTO DE EVENTOS (Apenas inputs e ações do sistema)
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 2. ATUALIZAÇÃO DO JOGO (Movimentação, Física e Colisões rodam soltas aqui)

        // Movimentação da raquete do player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && raquetePlayer1.getPosition().y > 0) {
            raquetePlayer1.move({0.f, -8.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && raquetePlayer1.getPosition().y < 500) {
            raquetePlayer1.move({0.f, 8.f});
        }

        // Movimentar a bola
        ball.move(ballVelocity);

        // Colisão da bola com teto e chão
        if (ball.getPosition().y <= 0 || ball.getPosition().y >= 580) {
            ballVelocity.y = -ballVelocity.y;
        }

        // Colisão com a raquete do player 1 (SFML 3 usa findIntersection)
        if (ball.getGlobalBounds().findIntersection(raquetePlayer1.getGlobalBounds())) {
            ballVelocity.x = -ballVelocity.x;
        }

        // Sistema de pontos
        if (ball.getPosition().x < 0) {
            ball.setPosition({400.f, 300.f});
            ballVelocity.x = -ballVelocity.x;
        }

        // Rebate na parede direita
        if (ball.getPosition().x > 780) {
            ballVelocity.x = -ballVelocity.x;
        }

        // 3. RENDERIZAÇÃO (Limpa, desenha e exibe tudo de uma vez por frame)
        window.clear(sf::Color::Black);
        window.draw(raquetePlayer1);
        window.draw(ball);
        window.display();
    }

}