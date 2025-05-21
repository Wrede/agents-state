#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

enum class AgentState { Idle, Moving, Resting };

std::string stateToString(AgentState state) {
    switch (state) {
        case AgentState::Idle: return "Idle";
        case AgentState::Moving: return "Moving";
        case AgentState::Resting: return "Resting";
    }
    return "";
}

sf::Color stateToColor(AgentState state) {
    switch (state) {
        case AgentState::Idle: return sf::Color::Yellow;
        case AgentState::Moving: return sf::Color::Green;
        case AgentState::Resting: return sf::Color::Blue;
    }
    return sf::Color::White;
}

struct Agent {
    sf::Vector2f position;
    AgentState state;
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Agent(float x, float y) {
        position = {x, y};
        state = AgentState::Idle;
        shape.setRadius(10.f);
        shape.setOrigin(10.f, 10.f);
        shape.setPosition(position);
        shape.setFillColor(stateToColor(state));
        velocity = {0.f, 0.f};
    }

    void update(float dt, const sf::RenderWindow& window) {
        // Randomly change state
        if (rand() % 1000 < 1) {
            int r = rand() % 3;
            state = static_cast<AgentState>(r);
            shape.setFillColor(stateToColor(state));
        }

        // State-based behavior
        switch (state) {
            case AgentState::Moving:
                if (velocity.x == 0 && velocity.y == 0) {
                    velocity = {(float)(rand() % 100 - 50) / 100.f,
                                (float)(rand() % 100 - 50) / 100.f};
                }
                position += velocity * 100.f * dt;
                break;
            case AgentState::Idle:
            case AgentState::Resting:
                velocity = {0.f, 0.f};
                break;
        }

        // Keep agents in bounds
        if (position.x < 0 || position.x > window.getSize().x)
            velocity.x = -velocity.x;
        if (position.y < 0 || position.y > window.getSize().y)
            velocity.y = -velocity.y;

        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window, sf::Font& font) {
        window.draw(shape);

        // Draw text
        sf::Text text;
        text.setFont(font);
        text.setString(stateToString(state));
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(position.x + 12, position.y - 10);
        window.draw(text);
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Multi-Agent Environment");
    sf::Clock clock;

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Font file missing
    }

    // Create agents
    std::vector<Agent> agents;
    for (int i = 0; i < 10; ++i) {
        agents.emplace_back(rand() % 800, rand() % 600);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        float dt = clock.restart().asSeconds();

        // Update agents
        for (auto& agent : agents) {
            agent.update(dt, window);
        }

        window.clear();
        for (auto& agent : agents) {
            agent.draw(window, font);
        }
        window.display();
    }

    return 0;
}

