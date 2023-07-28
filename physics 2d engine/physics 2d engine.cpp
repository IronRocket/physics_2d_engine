#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

const int WIDTH = 1600;
const int HEIGHT = 1200;

const float SCALE = 1;
const float G = 9.8;
const float air_density = 1.204;

int white[3] = { 255,255,255 }; 

class Rectangle {
public:
    sf::RectangleShape shape;
    std::string name;
    int* identifier;
    int width;
    int height;

    float x;
    float y;
    float mass;
    float x_vel;
    float y_vel;
    int force_x_direction;
    float kinetic_energy_x;
    float kinetic_energy_y;
    float static_coefficient;
    float kinetic_coefficient;

    bool grounded = true;
    bool static_state = false;
    bool rect = true;


    void init(std::string name, float x, float y, int width, int height, int color[], float mass, float static_coefficient, float kinetic_coefficient) {
        this->identifier = &width;
        this->name = name;
        this->width = width;
        this->height = height;

        this->x = x;
        this->y = y;
        this->mass = mass;
        this->x_vel = 0;
        this->y_vel = 0;
        this->static_coefficient = static_coefficient;
        this->kinetic_coefficient = kinetic_coefficient;
        shape.setPosition(sf::Vector2f(x, y));
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::White);
    }
    void move(const float x, const float  y) {
        shape.setPosition((float)x, (float)y);
    }
};

class Circle {
public:
    sf::CircleShape shape;
    std::string name;
    int width;
    int height;

    float x;
    float y;
    float mass;
    float x_vel;
    float y_vel;
    int force_x_direction;
    float kinetic_energy_x;
    float kinetic_energy_y;
    float static_coefficient;
    float kinetic_coefficient;

    bool grounded = true;
    bool static_state = false;
    bool rect = true;


    void init(std::string name, float x, float y, int radius, int color[], float mass, float static_coefficient, float kinetic_coefficient) {
        this->name = name;
        this->width = radius;
        this->height = radius;

        this->x = x;
        this->y = y;
        this->mass = mass;
        this->x_vel = 0;
        this->y_vel = 0;
        this->static_coefficient = static_coefficient;
        this->kinetic_coefficient = kinetic_coefficient;
        shape.setPosition(sf::Vector2f(x, y));
        shape.setRadius(radius);
        shape.setFillColor(sf::Color::White);
    }
    void move(const float x, const float  y) {
        shape.setPosition((float)x, (float)y);
    }
    void update_physics(Rectangle floor) {
        y_vel = G + y_vel;
        if (y + height >= floor.y-floor.height) {
            if (y_vel > 0) {
                y_vel = 0;
                y = floor.y - floor.height * 2;
            }
            x_vel = ( (mass*x_vel) - kinetic_coefficient * mass * x_vel )/mass;
        }
        else {
            if (x_vel < 0) {
                float temp;
                temp = -x_vel;
                x_vel = -( ((mass * temp) - (0.5 * air_density * pow(temp, 2) * 0.6 * 1.25)) / mass );
            }
            else {
                x_vel = ((mass * x_vel) - (0.5 * air_density * pow(x_vel, 2) * 0.6 * 1.25)) / mass;
            }
            if (y_vel < 0) {
                float temp;
                temp = -y_vel;
                y_vel = -( ((mass * y_vel) - (0.5 * air_density * pow(temp, 2) * 0.6 * 1.25)) / mass );
            }
            else {
                y_vel = ((mass * y_vel) - (0.5 * air_density * pow(y_vel, 2) * 0.6 * 1.25)) / mass;
            }
            kinetic_energy_x = mass * x_vel;
            kinetic_energy_y = mass * y_vel;

        }
    }
    void render() {
        x += x_vel;
        y += y_vel;
        move(x, y);
    }
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Physics Engine");
    window.setFramerateLimit(60);

    Rectangle rect_1;
    rect_1.init("player_1", 100, HEIGHT - 50, 10 / SCALE, 10 / SCALE, white, 0.1496855, 0.4, 0.3);

    Rectangle floor;
    floor.init("floor", 0, HEIGHT - 10, WIDTH/SCALE, 10/SCALE, white, 0, 0, 0);

    Circle ball;
    ball.init("ball", WIDTH / 2, HEIGHT / 2, 10 / SCALE, white, 10, 0.4, 0.1);

    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                ball.y_vel = -100;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                ball.x_vel = 10;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                ball.x_vel = -10;
            }

        }
        window.clear();

        ball.update_physics(floor);
        ball.render();
        window.draw(ball.shape);
        window.draw(floor.shape);
        window.display();
        std::cout << ball.x << ":" << ball.y <<"    "<< ball.x_vel << ":" << ball.y_vel << "\n";
    }

    return 0;
}