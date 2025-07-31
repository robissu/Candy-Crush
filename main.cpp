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
    bool selecao;

public:
    Peca() : x(0), y(0), dim(0), space(0), shape(formaRandom()), selecao(false)
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
        this->selecao = false;
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

    void setSelecao(bool selecao)
    {
        this->selecao = selecao;
    }

    bool getSelecao()
    {
        return this->selecao;
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

    void setX(int x)
    {
        this->x = x;
    }

    void setY(int y)
    {
        this->y = y;
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
    int linha, coluna, dist_width, dist_height, tam, selecaoX, selecaoY;
    sf::VertexArray game;
    std::vector<std::vector<Peca>> matrizPeca;

public:
    Game()
    {
        this->linha = linhasTab;
        this->coluna = colunasTab;
        this->dist_height = (janela_height) / linha;
        this->dist_width = (janela_width) / coluna;
        this->tam = linha * coluna;
        this->game = sf::VertexArray(sf::Lines, tam * 2);
        this->selecaoX = -1;
        this->selecaoY = -1;
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
            if (p.getSelecao() == false)
            {
                retangulo.setFillColor(sf::Color::Red);
            }
            else
            {
                retangulo.setFillColor(sf::Color::White);
            }
            window.draw(retangulo);
        }
        else if (p.getForma() == Formato::CIRCULO)
        {
            sf::CircleShape circulo(p.getDim() / 2);
            if (p.getSelecao() == false)
            {
                circulo.setFillColor(sf::Color::Blue);
            }
            else
            {
                circulo.setFillColor(sf::Color::White);
            }

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
            if (p.getSelecao() == false)
            {
                triangulo.setFillColor(sf::Color::Yellow);
            }
            else
            {
                triangulo.setFillColor(sf::Color::White);
            }

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

    void swap(Peca &p1, Peca &p2)
    {
        int original_p1_x = p1.getX();
        int original_p1_y = p1.getY();
        int original_p2_x = p2.getX();
        int original_p2_y = p2.getY();

        std::swap(p1, p2);

        p1.setX(original_p1_x);
        p1.setY(original_p1_y);

        p2.setX(original_p2_x);
        p2.setY(original_p2_y);

        p1.setSelecao(false);
        p2.setSelecao(false);
    }

    void trocaPecas()
    {
        for (int i = 0; i < linhasTab; i++)
        {
            for (int j = 0; j < colunasTab; j++)
            {
                if (j + 1 < colunasTab && matrizPeca[j][i].getSelecao() == true && matrizPeca[j + 1][i].getSelecao() == true)
                {
                    // direita
                    swap(matrizPeca[j][i], matrizPeca[j + 1][i]);
                }
                else if (j - 1 >= 0 && matrizPeca[j][i].getSelecao() == true && matrizPeca[j - 1][i].getSelecao() == true)
                {
                    // esquerda
                    swap(matrizPeca[j][i], matrizPeca[j - 1][i]);
                }
                else if (i - 1 >= 0 && matrizPeca[j][i].getSelecao() == true && matrizPeca[j][i - 1].getSelecao() == true)
                {
                    // cima
                    swap(matrizPeca[j][i], matrizPeca[j][i - 1]);
                }
                else if (i + 1 < linhasTab && matrizPeca[j][i].getSelecao() == true && matrizPeca[j][i + 1].getSelecao() == true)
                {
                    // baixo
                    swap(matrizPeca[j][i], matrizPeca[j][i + 1]);
                }
                else if (j + 1 < colunasTab && i + 1 < linhasTab && matrizPeca[j][i].getSelecao() == true && matrizPeca[j + 1][i + 1].getSelecao() == true)
                {
                    // diagonal dir baixa
                    swap(matrizPeca[j][i], matrizPeca[j + 1][i + 1]);
                }
                else if (j + 1 < colunasTab && i - 1 >= 0 && matrizPeca[j][i].getSelecao() == true && matrizPeca[j + 1][i - 1].getSelecao() == true)
                {
                    // diagonal dir cima
                    swap(matrizPeca[j][i], matrizPeca[j + 1][i - 1]);
                }
                else if (j - 1 >= 0 && i + 1 < linhasTab && matrizPeca[j][i].getSelecao() == true && matrizPeca[j - 1][i + 1].getSelecao() == true)
                {
                    // diagonal esq baixa
                    swap(matrizPeca[j][i], matrizPeca[j - 1][i + 1]);
                }
                else if (i - 1 >= 0 && j - 1 >= 0 && matrizPeca[j][i].getSelecao() == true && matrizPeca[j - 1][i - 1].getSelecao() == true)
                {
                    // diagonal esq cima
                    swap(matrizPeca[j][i], matrizPeca[j - 1][i - 1]);
                }
            }
        }
    }

    void checaClique(sf::Event event)
    {

        // checar coluna
        if (event.mouseButton.x < (janela_width / colunasTab))
        {
            selecaoX = 0;
        }
        else if (event.mouseButton.x < (janela_width / colunasTab) * 2)
        {
            selecaoX = 1;
        }
        else if (event.mouseButton.x < (janela_width / colunasTab) * 3)
        {
            selecaoX = 2;
        }
        else if (event.mouseButton.x < (janela_width / colunasTab) * 4)
        {
            selecaoX = 3;
        }

        // checar linha
        if (event.mouseButton.y < (janela_height / linhasTab))
        {
            selecaoY = 0;
        }
        else if (event.mouseButton.y < (janela_height / linhasTab) * 2)
        {
            selecaoY = 1;
        }
        else if (event.mouseButton.y < (janela_height / linhasTab) * 3)
        {
            selecaoY = 2;
        }
        else if (event.mouseButton.y < (janela_height / linhasTab) * 4)
        {
            selecaoY = 3;
        }

        if (matrizPeca[selecaoY][selecaoX].getSelecao() == false)
        {
            matrizPeca[selecaoY][selecaoX].setSelecao(true);
        }
        else
            matrizPeca[selecaoY][selecaoX].setSelecao(false);
    }

    void destacaSelecao()
    {
        std::cout << "Y: " << selecaoY << " X: " << selecaoX << std::endl;
        std::cout << "Esta selecionada? " << matrizPeca[selecaoY][selecaoX].getSelecao() << std::endl;
    }
};

//---------------------------------------------------------------------------------
int main()
{
    window.create(sf::VideoMode(janela_width, janela_height), "Candy Crush");
    Game game;
    Peca p1(0, 1); // coluna, linha
    static int cont = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                cont++;
                game.checaClique(event);
                game.destacaSelecao();
            }
        }

        window.clear();
        if (cont == 2)
        {
            game.trocaPecas();
            cont = 0;
        }
        window.draw(game.criaGrade());
        // game.desenhaPeca(p1);
        game.desenhaMatriz();
        game.checaMatch();
        window.display();
    }
    return 0;
}
