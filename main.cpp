#include <SFML/Graphics.hpp>
#include <vector>
int janela_width = 800;
int janela_height = 800;

class Grade
{
private:
    int linha, coluna, dist_width, dist_height, tam;
    sf::VertexArray grade;

public:
    Grade(int linha, int coluna)
    {
        this->linha = linha;
        this->coluna = coluna;
        dist_height = (janela_height / 2) / linha;
        dist_width = (janela_width / 2) / coluna;
        tam = linha*coluna;
        grade = sf::VertexArray(sf::Lines, tam * 2);
    }

    sf::VertexArray criaGrade()
    {
        int i,j, vertex = 0;
        // linhas
        for (i = 0; i < linha * 2; i = i + 2)
        {
            grade[vertex].position = sf::Vector2f(0, dist_height * i);
            grade[vertex].color = sf::Color::Red;
            vertex++;
            grade[vertex].position = sf::Vector2f(janela_width, dist_height * i);
            grade[vertex].color = sf::Color::Red;
            vertex++;
        }
        // colunas
        for (j = 0; j < coluna * 2; j = j + 2)
        {
            grade[vertex].position = sf::Vector2f(dist_width * j, 0);
            grade[vertex].color = sf::Color::Red;
            vertex++;
            grade[vertex].position = sf::Vector2f(dist_width * j, janela_height);
            grade[vertex].color = sf::Color::Red;
            vertex++;
        }
        return grade;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(janela_width, janela_height), "Candy Crush");
    Grade grade(8, 4);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.draw(grade.criaGrade());
        window.display();
    }
    return 0;
}
