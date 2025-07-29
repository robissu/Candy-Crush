#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

sf::RenderWindow window;
int janela_width = 800;
int janela_height = 800;
int colunasTab = 4;
int linhasTab = 4;

enum class Formato
{
    QUADRADO,
    TRIANGULO,
    CIRCULO
};

//---------------------------------------------------------------------------------
class Peca
{
private:
    int x, y, dim, space;
    Formato shape;

public:
    Peca() : x(0), y(0), dim(0), space(0), shape(formaRandom())
    {
        // Você pode inicializar com valores padrão ou deixar vazio se não precisar de lógica aqui
        // Para uma Peca "vazia", talvez definir um shape específico ou uma cor nula.
    }

    Peca(int x, int y)
    {
        this->x = x;
        this->y = y;
        this->shape = formaRandom();
        this->dim = 100;
        this->space = 50;
    }

    Formato formaRandom()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        Formato formato;
        std::uniform_int_distribution<> dist(
            static_cast<int>(Formato::QUADRADO), // Valor mínimo (0)
            static_cast<int>(Formato::CIRCULO)   // Valor máximo (2)
        );

        formato = static_cast<Formato>(dist(gen));
        return formato;
    }

    Formato getForma()
    {
        return this->shape;
    }

    void setForma()
    {
        this->shape = formaRandom();
    }

    int getX()
    {
        return this->x;
    }
    int getY()
    {
        return this->y;
    }

    int getDim()
    {
        return this->dim;
    }
    int getSpace()
    {
        return this->space;
    }
};

//---------------------------------------------------------------------------------

class Game
{
private:
    int linha, coluna, dist_width, dist_height, tam;
    sf::VertexArray game;
    std::vector<std::vector<Peca>> matrizPeca;

public:
    Game()
    {
        this->linha = linhasTab;
        this->coluna = colunasTab;
        dist_height = (janela_height) / linha;
        dist_width = (janela_width) / coluna;
        tam = linha * coluna;
        game = sf::VertexArray(sf::Lines, tam * 2);
        matrizMonta();
    }

    void desenhaMatriz()
    {
        for (int i = 0; i < linhasTab; i++)
        {
            for (int j = 0; j < colunasTab; j++)
            {
                desenhaPeca(matrizPeca[j][i]);
            }
        }
    }

    bool match(Peca p)
    {
        int currentY = p.getY(); // Linha da peça atual
        int currentX = p.getX(); // Coluna da peça atual
        Formato currentShape = p.getForma();

        // -- RETO --

        // Reto para baixo
        if (currentY + 2 < linhasTab && // Verifica se Peca(Y+2, X) está dentro dos limites da linha
            matrizPeca[currentY + 1][currentX].getForma() == currentShape &&
            matrizPeca[currentY + 2][currentX].getForma() == currentShape)
        {
            return true;
        }

        // Reto para cima
        if (currentY - 2 >= 0 && // Verifica se Peca(Y-2, X) está dentro dos limites da linha
            matrizPeca[currentY - 1][currentX].getForma() == currentShape &&
            matrizPeca[currentY - 2][currentX].getForma() == currentShape)
        {
            return true;
        }

        // Reto para a direita
        if (currentX + 2 < colunasTab && // Verifica se Peca(Y, X+2) está dentro dos limites da coluna
            matrizPeca[currentY][currentX + 1].getForma() == currentShape &&
            matrizPeca[currentY][currentX + 2].getForma() == currentShape)
        {
            return true;
        }

        // Reto para a esquerda
        if (currentX - 2 >= 0 && // Verifica se Peca(Y, X-2) está dentro dos limites da coluna
            matrizPeca[currentY][currentX - 1].getForma() == currentShape &&
            matrizPeca[currentY][currentX - 2].getForma() == currentShape)
        {
            return true;
        }

        // -- DIAGONAIS --

        // Diagonal direita-baixo
        if (currentY + 2 < linhasTab && currentX + 2 < colunasTab &&
            matrizPeca[currentY + 1][currentX + 1].getForma() == currentShape &&
            matrizPeca[currentY + 2][currentX + 2].getForma() == currentShape)
        {
            return true;
        }

        // Diagonal direita-cima
        if (currentY - 2 >= 0 && currentX + 2 < colunasTab &&
            matrizPeca[currentY - 1][currentX + 1].getForma() == currentShape &&
            matrizPeca[currentY - 2][currentX + 2].getForma() == currentShape)
        {
            return true;
        }

        // Diagonal esquerda-baixo
        if (currentY + 2 < linhasTab && currentX - 2 >= 0 &&
            matrizPeca[currentY + 1][currentX - 1].getForma() == currentShape &&
            matrizPeca[currentY + 2][currentX - 2].getForma() == currentShape)
        {
            return true;
        }

        // Diagonal esquerda-cima
        if (currentY - 2 >= 0 && currentX - 2 >= 0 &&
            matrizPeca[currentY - 1][currentX - 1].getForma() == currentShape &&
            matrizPeca[currentY - 2][currentX - 2].getForma() == currentShape)
        {
            return true;
        }

        return false;
    }

    void checaMatch()
    {
        for (int i = 0; i < linhasTab; i++)
        {
            for (int j = 0; j < colunasTab; j++)
            {
                if (match(matrizPeca[j][i]))
                {
                    matrizPeca[j][i].setForma();
                }
            }
        }
    }

    void matrizMonta()
    {
        matrizPeca.resize(this->linha);
        for (int i = 0; i < linhasTab; i++)
        {
            matrizPeca[i].resize(this->coluna);
            for (int j = 0; j < colunasTab; j++)
            {
                matrizPeca[i][j] = Peca(j, i);
            }
        }
    }

    void desenhaPeca(Peca p)
    {

        int valX = ((janela_width / colunasTab) / 2) - (p.getDim() / 2);
        int valY = ((janela_height / linhasTab) / 2) - (p.getDim() / 2);

        int posX = valX + (p.getX() * (janela_width / colunasTab));
        int posY = valY + (p.getY() * (janela_height / linhasTab));

        if (p.getForma() == Formato::QUADRADO)
        {
            sf::RectangleShape retangulo(sf::Vector2f(p.getDim(), p.getDim()));
            retangulo.setPosition(posX, posY);
            retangulo.setFillColor(sf::Color::Red);
            window.draw(retangulo);
        }
        else if (p.getForma() == Formato::CIRCULO)
        {
            sf::CircleShape circulo(p.getDim() / 2);
            circulo.setFillColor(sf::Color::Blue);
            circulo.setPosition(posX, posY);
            circulo.setPointCount(100); // Mais pontos para um círculo mais suave (padrão é 30)
            window.draw(circulo);
        }
        else if (p.getForma() == Formato::TRIANGULO)
        {
            // 1. Criar o objeto sf::ConvexShape e definir o número de pontos
            sf::ConvexShape triangulo;
            triangulo.setPointCount(3); // Um triângulo tem 3 pontos
            // 2. Definir as coordenadas de cada ponto
            // Os pontos são definidos em relação à origem local da forma (0,0),
            // antes que setPosition() seja aplicado.
            triangulo.setPoint(0, sf::Vector2f(50.f, 0.f));    // Ponto superior
            triangulo.setPoint(1, sf::Vector2f(100.f, 100.f)); // Ponto inferior direito
            triangulo.setPoint(2, sf::Vector2f(0.f, 100.f));   // Ponto inferior esquerdo
            triangulo.setFillColor(sf::Color::Yellow);
            triangulo.setPosition(posX, posY);
            window.draw(triangulo);
        }
    }

    sf::VertexArray criaGrade()
    {
        int i, j, vertex = 0;
        // linhas
        for (i = 0; i < linha * 2; i++)
        {
            game[vertex].position = sf::Vector2f(0, dist_height * i);
            game[vertex].color = sf::Color::Red;
            vertex++;
            game[vertex].position = sf::Vector2f(janela_width, dist_height * i);
            game[vertex].color = sf::Color::Red;
            vertex++;
        }
        // colunas
        for (j = 0; j < coluna * 2; j++)
        {
            game[vertex].position = sf::Vector2f(dist_width * j, 0);
            game[vertex].color = sf::Color::Red;
            vertex++;
            game[vertex].position = sf::Vector2f(dist_width * j, janela_height);
            game[vertex].color = sf::Color::Red;
            vertex++;
        }
        return game;
    }
};

//---------------------------------------------------------------------------------
int main()
{
    window.create(sf::VideoMode(janela_width, janela_height), "Candy Crush");
    Game game;
    Peca p1(0, 1); // coluna, linha
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.draw(game.criaGrade());
        // game.desenhaPeca(p1);
        game.desenhaMatriz();
        game.checaMatch();
        window.display();
    }
    return 0;
}
