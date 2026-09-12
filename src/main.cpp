#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Enum atualizado com o estado do Menu
enum class GameState {
    Menu,
    Playing,
    GameOver
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800,600}), "Ping Pong em SFML");
    window.setFramerateLimit(30);

    // --- ELEMENTOS DO JOGO ---
    sf::CircleShape ball(12.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition({400.f,300.f});

    sf::Vector2f ballVelocity({10.f,10.f});

    sf::RectangleShape raquetePlayer1(sf::Vector2f({20.f,100.f}));
    raquetePlayer1.setFillColor(sf::Color::Blue);
    raquetePlayer1.setPosition({30.f,250.f});

    sf::RectangleShape raquetePlayer2(sf::Vector2f({20.f,100.f}));
    raquetePlayer2.setFillColor(sf::Color::Blue);
    raquetePlayer2.setPosition({750.f,250.f});

    int ptsPlayer1 = 0;
    int ptsPlayer2 = 0;
    const int MAX_SCORE = 5;
    std::string msgVitoria = "";

    // FONTE E TEXTOS
    sf::Font font;
    if (!font.openFromFile("assets/fonts/Montserrat-Regular.ttf")) {
        std::cerr << "Could not load font" << std::endl;
    }

    sf::Text textPlacar(font);
    textPlacar.setFillColor(sf::Color::White);
    textPlacar.setCharacterSize(30);
    textPlacar.setStyle(sf::Text::Bold);
    textPlacar.setPosition({320.f ,0});

    sf::Text textGameOver(font);
    textGameOver.setFillColor(sf::Color::Green);
    textGameOver.setCharacterSize(20);
    textGameOver.setStyle(sf::Text::Bold);
    textGameOver.setPosition({150.f, 250.f});

    // --- TEXTOS DO MENU ---
    sf::Text menuTitulo(font);
    menuTitulo.setString("PING PONG");
    menuTitulo.setFillColor(sf::Color::White);
    menuTitulo.setCharacterSize(60);
    menuTitulo.setStyle(sf::Text::Bold);
    menuTitulo.setPosition({240.f, 100.f});

    sf::Text menuJogar(font);
    menuJogar.setString("JOGAR");
    menuJogar.setCharacterSize(35);
    menuJogar.setPosition({350.f, 300.f});

    sf::Text menuSair(font);
    menuSair.setString("SAIR");
    menuSair.setCharacterSize(35);
    menuSair.setPosition({362.f, 380.f});

    // CONTROLE DO MENU (0 = Jogar, 1 = Sair)
    int menuSelecao = 0;

    // ESTADO INICIAL: Começa no Menu!
    GameState estadoAtual = GameState::Menu;

    while(window.isOpen()) {

        // 1. PROCESSAMENTO DE EVENTOS
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // CONTROLES DO MENU
            if (estadoAtual == GameState::Menu && event->is<sf::Event::KeyPressed>()) {
                auto keyCode = event->getIf<sf::Event::KeyPressed>()->code;

                if (keyCode == sf::Keyboard::Key::Up || keyCode == sf::Keyboard::Key::Down) {
                    // Alterna entre 0 e 1 de forma simples
                    menuSelecao = (menuSelecao == 0) ? 1 : 0;
                }
                if (keyCode == sf::Keyboard::Key::Enter) {
                    if (menuSelecao == 0) {
                        estadoAtual = GameState::Playing; // Inicia o jogo
                    } else {
                        window.close(); // Fecha o jogo
                    }
                }
            }

            // CONTROLES DE REINÍCIO (FIM DE JOGO)
            if (estadoAtual == GameState::GameOver && event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                    ptsPlayer1 = 0;
                    ptsPlayer2 = 0;
                    ball.setPosition({400.f, 300.f});
                    raquetePlayer1.setPosition({30.f, 250.f});
                    raquetePlayer2.setPosition({750.f, 250.f});
                    estadoAtual = GameState::Playing;
                } else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S) {
                    estadoAtual = GameState::Menu;
                }
            }
        }

        // 2. ATUALIZAÇÃO DA LÓGICA DO JOGO
        if (estadoAtual == GameState::Menu) {
            // Destaca visualmente a opção selecionada piscando ou mudando de cor
            if (menuSelecao == 0) {
                menuJogar.setFillColor(sf::Color::Yellow);
                menuSair.setFillColor(sf::Color::White);
            } else {
                menuJogar.setFillColor(sf::Color::White);
                menuSair.setFillColor(sf::Color::Yellow);
            }
        }
        else if (estadoAtual == GameState::Playing) {
            // Movimentação Player 1
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && raquetePlayer1.getPosition().y > 0) {
                raquetePlayer1.move({0.f, -20.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && raquetePlayer1.getPosition().y < 500) {
                raquetePlayer1.move({0.f, 20.f});
            }

            // Movimentação Player 2
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && raquetePlayer2.getPosition().y > 0) {
                raquetePlayer2.move({0.f, -20.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && raquetePlayer2.getPosition().y < 500) {
                raquetePlayer2.move({0.f, 20.f});
            }

            // Física da bola
            ball.move(ballVelocity);

            if (ball.getPosition().y <= 0 || ball.getPosition().y >= 580) {
                ballVelocity.y = -ballVelocity.y;
            }

            if (ball.getGlobalBounds().findIntersection(raquetePlayer1.getGlobalBounds()) ||
                ball.getGlobalBounds().findIntersection(raquetePlayer2.getGlobalBounds())) {
                ballVelocity.x ++;
                ballVelocity.y ++;
                ballVelocity.x = -ballVelocity.x;
            }

            // Pontuação
            if (ball.getPosition().x < 0) {
                ball.setPosition({400.f, 300.f});
                ballVelocity.x = -ballVelocity.x;
                ptsPlayer2++;
            } else if (ball.getPosition().x >= 780) {
                ball.setPosition({400.f, 300.f});
                ballVelocity.x = -ballVelocity.x;
                ptsPlayer1++;
            }

            textPlacar.setString("Pontos: " + std::to_string(ptsPlayer1) + " X " + std::to_string(ptsPlayer2));

            // Condição de vitória
            if (ptsPlayer1 >= MAX_SCORE) {
                msgVitoria = "O Jogador 1 VENCEU!\nPressione R para reiniciar\nou S para voltar ao menu principal.";
                estadoAtual = GameState::GameOver;
            } else if (ptsPlayer2 >= MAX_SCORE) {
                msgVitoria = "O Jogador 2 VENCEU!\nPressione R para reiniciar\nou S para voltar ao menu principal.";
                estadoAtual = GameState::GameOver;
            }
        }
        else if (estadoAtual == GameState::GameOver) {
            textGameOver.setString(msgVitoria);
        }

        // 3. RENDERIZAÇÃO SELETIVA
        window.clear(sf::Color::Black);

        if (estadoAtual == GameState::Menu) {
            // Desenha as coisas do Menu
            window.draw(menuTitulo);
            window.draw(menuJogar);
            window.draw(menuSair);
        }
        else if (estadoAtual == GameState::Playing) {
            // Desenha a partida rodando
            window.draw(textPlacar);
            window.draw(raquetePlayer1);
            window.draw(raquetePlayer2);
            window.draw(ball);
        }
        else if (estadoAtual == GameState::GameOver) {
            // Desenha a tela final
            window.draw(textPlacar);
            window.draw(textGameOver);
        }

        window.display();
    }
}
