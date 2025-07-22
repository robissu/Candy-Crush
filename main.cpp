#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

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
    enum class Cor
    {
        VERMELHO,
        AZUL,
        AMARELO
    };

//---------------------------------------------------------------------------------
class Peca
{

private:
    int x, y, dim, space;
    Formato shape;
    Cor color;

public:
    Peca(int x, int y)
    {
        this->x = x;
        this->y = y;
        this->shape = formaRandom();
        this->color = associaCor(shape);
        this->dim = 100;
        this->space = 50;
    }

    Cor associaCor(Formato shape)
    {
        if (shape == Formato::QUADRADO)
        {
            return Cor::VERMELHO;
        }
        else if (shape == Formato::TRIANGULO)
        {
            return Cor::AMARELO;
        }
        else if (shape == Formato::CIRCULO)
        {
            return Cor::AZUL;
        }
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

    Formato getForma(){
        return this->shape;
    }

    int getX(){
        return this->x;
    }
    int getY(){
        return this->y;
    }

    int getDim(){
        return this->dim;
    }
    int getSpace(){
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
    }

    std::vector<std::vector<Peca>> criarMatrizDePecas()
    {
        std::vector<std::vector<Peca>> matrizPeca;
        matrizPeca.resize(linha);
        for (int i = 0; i < linha; ++i)
        {
            matrizPeca[i].resize(coluna);
            
            for (int j = 0; j < coluna; ++j)
            {
                matrizPeca[i][j] = Peca(i, j);

            }
        }
        return matrizPeca;
    }

    void renderizaFiguras(){
        for(int i = 0; i <linhasTab; i++){
            for(int j = 0; j<colunasTab; j++){
                if(matrizPeca[i][j].getForma() == Formato::CIRCULO){
                    sf::CircleShape circulo(matrizPeca[i][j].getDim());
                    circulo.setFillColor(sf::Color::Blue);
                    circulo.setPosition(i*matrizPeca[i][j].getSpace(), j*matrizPeca[i][j].getSpace());
                }
            }
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
    sf::RenderWindow window(sf::VideoMode(janela_width, janela_height), "Candy Crush");
    Game Game;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.draw(Game.criaGrade());
        window.display();
    }
    return 0;
}
