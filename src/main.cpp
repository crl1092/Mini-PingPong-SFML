#include <iostream>
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

    //raquete player 2
    sf::RectangleShape raquetePlayer2(sf::Vector2f({20.f,100.f}));
    raquetePlayer2.setFillColor(sf::Color::Blue);
    raquetePlayer2.setPosition({750.f,250.f});

    //pontuação
    int ptsPlayer1 = 0;
    int ptsPlayer2 = 0;
    sf::Font font;
    //carregar fonte
    if (!font.openFromFile("assets/fonts/Montserrat-Regular.ttf")) {
        std::cerr << "Could not load font" << std::endl;
    }
    sf::Text text(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFont(font);
    text.setPosition({320.f ,0});

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
            raquetePlayer1.move({0.f, -10.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && raquetePlayer1.getPosition().y < 500) {
            raquetePlayer1.move({0.f, 10.f});
        }

        //movinemtação da raquete do player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && raquetePlayer2.getPosition().y > 0) {
            raquetePlayer2.move({0.f, -10.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && raquetePlayer2.getPosition().y < 500) {
            raquetePlayer2.move({0.f, 10.f});
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
        //Player1
        if (ball.getPosition().x < 0) {
            ball.setPosition({400.f, 300.f});
            ballVelocity.x = -ballVelocity.x;
            ptsPlayer2++;
        } else if (ball.getPosition().x >= 780) {
            ball.setPosition({400.f, 300.f});
            ballVelocity.x = -ballVelocity.x;
            ptsPlayer1++;
        }
        //Mostra os pontos
        text.setString("Pontos: " + std::to_string(ptsPlayer1) + " X " + std::to_string(ptsPlayer2));


        // Rebate na raquete do player 2
        if (ball.getGlobalBounds().findIntersection(raquetePlayer2.getGlobalBounds())) {
            ballVelocity.x = -ballVelocity.x;
        }

        // 3. RENDERIZAÇÃO (Limpa, desenha e exibe tudo de uma vez por frame)
        window.clear(sf::Color::Black);
        window.draw(text);
        window.draw(raquetePlayer1);
        window.draw(raquetePlayer2);
        window.draw(ball);
        window.display();
    }

}