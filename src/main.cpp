#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Enum para gerenciar os estados do jogo
enum class GameState {
    Playing,
    GameOver
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Ping Pong em SFML");
    window.setFramerateLimit(30);

    //ball
    sf::CircleShape ball(12.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition({400.f, 300.f});

    //velocidade da bola
    sf::Vector2f ballVelocity({7.f, 7.f});

    //raquete player1
    sf::RectangleShape raquetePlayer1(sf::Vector2f({20.f, 100.f}));
    raquetePlayer1.setFillColor(sf::Color::Blue);
    raquetePlayer1.setPosition({30.f, 250.f});

    //raquete player 2
    sf::RectangleShape raquetePlayer2(sf::Vector2f({20.f, 100.f}));
    raquetePlayer2.setFillColor(sf::Color::Blue);
    raquetePlayer2.setPosition({750.f, 250.f});

    //pontuação
    int ptsPlayer1 = 0;
    int ptsPlayer2 = 0;
    const int MAX_SCORE = 5; // Condição de vitória: vence quem fizer 5 pontos
    std::string msgVitoria = "";

    // Estado inicial do jogo
    GameState estadoAtual = GameState::Playing;

    sf::Font font;
    //carregar fonte
    if (!font.openFromFile("assets/fonts/Montserrat-Regular.ttf")) {
        std::cerr << "Could not load font" << std::endl;
    }

    // Texto do Placar
    sf::Text text(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setPosition({320.f, 0});

    // Texto de Fim de Jogo
    sf::Text textGameOver(font);
    textGameOver.setFillColor(sf::Color::Green);
    textGameOver.setCharacterSize(40);
    textGameOver.setStyle(sf::Text::Bold);
    textGameOver.setPosition({150.f, 250.f}); // Centralizado na tela

    while (window.isOpen()) {
        // 1. PROCESSAMENTO DE EVENTOS
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Se o jogo acabou e apertar R, reseta tudo para jogar de novo
            if (estadoAtual == GameState::GameOver && event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                    ptsPlayer1 = 0;
                    ptsPlayer2 = 0;
                    ball.setPosition({400.f, 300.f});
                    raquetePlayer1.setPosition({30.f, 250.f});
                    raquetePlayer2.setPosition({750.f, 250.f});
                    estadoAtual = GameState::Playing;
                }
            }
        }

        // 2. ATUALIZAÇÃO DO JOGO (Só roda se o jogo estiver ativo)
        if (estadoAtual == GameState::Playing) {
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

            // Colisão com a raquete do player 1
            if (ball.getGlobalBounds().findIntersection(raquetePlayer1.getGlobalBounds())) {
                ballVelocity.x = -ballVelocity.x;
            }

            // Rebate na raquete do player 2
            if (ball.getGlobalBounds().findIntersection(raquetePlayer2.getGlobalBounds())) {
                ballVelocity.x = -ballVelocity.x;
            }

            // Sistema de pontos
            if (ball.getPosition().x < 0) {
                ball.setPosition({400.f, 300.f});
                ballVelocity.x = -ballVelocity.x;
                ptsPlayer2++;
            } else if (ball.getPosition().x >= 780) {
                ball.setPosition({400.f, 300.f});
                ballVelocity.x = -ballVelocity.x;
                ptsPlayer1++;
            }

            // Mostra os pontos
            text.setString("Pontos: " + std::to_string(ptsPlayer1) + " X " + std::to_string(ptsPlayer2));

            // VERIFICAÇÃO DA CONDIÇÃO DE VITÓRIA
            if (ptsPlayer1 >= MAX_SCORE) {
                msgVitoria = "O JOGADOR 1 VENCEU!\nPressione R para reiniciar.";
                estadoAtual = GameState::GameOver;
            } else if (ptsPlayer2 >= MAX_SCORE) {
                msgVitoria = " O JOGADOR 2 VENCEU!\nPressione R para reiniciar.";
                estadoAtual = GameState::GameOver;
            }
        } else if (estadoAtual == GameState::GameOver) {
            // Atualiza a mensagem final com base em quem ganhou
            textGameOver.setString(msgVitoria);
        }

        // 3. RENDERIZAÇÃO
        window.clear(sf::Color::Black);

        if (estadoAtual == GameState::Playing) {
            // Desenha os elementos normais do jogo acontecendo
            window.draw(text);
            window.draw(raquetePlayer1);
            window.draw(raquetePlayer2);
            window.draw(ball);
        } else if (estadoAtual == GameState::GameOver) {
            // Tela de fim de jogo: mostra apenas o placar final e quem ganhou
            window.draw(text);
            window.draw(textGameOver);
        }

        window.display();
    }
}
