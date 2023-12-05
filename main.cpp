#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <unordered_map>
#include <set>
#include <map>
#include "wine_reader.h"
#include <random>

enum class AppState { Input, Results };
enum class ActiveInputField { Country, Variety, Price };
ActiveInputField activeInputField = ActiveInputField::Country;

int main()
{
    sf::RenderWindow window(sf::VideoMode(720, 560), "SFML Application");

    std::vector<Wine> wines = ReadWinesFromFile("/path_to_modified_file1.csv");

    std::unordered_map<std::string, Wine> selectionMap;

    auto countryTable = hashtable_func(wines);


    std::vector<Wine> priceFilteredWines;

    sf::Font font;
    if (!font.loadFromFile("AlexBrushRegular.ttf")) {
        return 1;
    }

    sf::Font font2;
    if (!font2.loadFromFile("/CafeFrancoise-ywMWq.ttf")) {
        std::cout<<"error";
    }

    AppState state = AppState::Input;

    sf::Text mapText;
    mapText.setFont(font2);
    mapText.setCharacterSize(20);
    mapText.setFillColor(sf::Color::White);
    mapText.setPosition(100, 250);


    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Welcome to the WineWhisperer");
    titleText.setCharacterSize(45);
    titleText.setFillColor(sf::Color::Red);
    float titleWidth = titleText.getLocalBounds().width;
    titleText.setPosition((720 / 2) - (titleWidth / 2), 560 * 1 / 8);

    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setString("Enter a Country: ");
    instructionsText.setCharacterSize(30);
    instructionsText.setFillColor(sf::Color::Red);
    float instructionsWidth = instructionsText.getLocalBounds().width;
    instructionsText.setPosition((720 / 2) - (instructionsWidth / 2), 200);

    sf::Text userInputText;
    userInputText.setFont(font);
    userInputText.setCharacterSize(28);
    userInputText.setFillColor(sf::Color::Black);
    userInputText.setPosition(160, 250);
    std::string userInput;

    sf::RectangleShape inputFieldBg;
    inputFieldBg.setSize(sf::Vector2f(400, 50));
    inputFieldBg.setFillColor(sf::Color::White);
    inputFieldBg.setPosition(160, 250);

    sf::RectangleShape cursor;
    cursor.setSize(sf::Vector2f(2, 30));
    cursor.setFillColor(sf::Color::Black);
    bool cursorVisible = true;
    sf::Clock cursorTimer;
    cursorTimer.restart();

    sf::Text resultsText;
    resultsText.setFont(font);
    resultsText.setCharacterSize(45);
    resultsText.setFillColor(sf::Color::White);
    resultsText.setPosition(100, 300);

    sf::Text varietyInstructionsText;
    std::string varietyInput;
    sf::Text varietyInputText;
    sf::RectangleShape varietyInputFieldBg;

    varietyInstructionsText.setFont(font);
    varietyInstructionsText.setString("Enter a variety: ");
    varietyInstructionsText.setCharacterSize(30);
    varietyInstructionsText.setFillColor(sf::Color::Red);
    float varietyInstructionsWidth = varietyInstructionsText.getLocalBounds().width;
    varietyInstructionsText.setPosition((720 / 2) - (varietyInstructionsWidth / 2), 300);

    varietyInputFieldBg.setSize(sf::Vector2f(400, 50));
    varietyInputFieldBg.setFillColor(sf::Color::White);
    varietyInputFieldBg.setPosition(160, 350);

    varietyInputText.setFont(font);
    varietyInputText.setCharacterSize(28);
    varietyInputText.setFillColor(sf::Color::Black);
    varietyInputText.setPosition(160, 350);

    sf::Text priceInstructionsText;
    std::string priceInput;
    sf::Text priceInputText;
    sf::RectangleShape priceInputFieldBg;

    priceInstructionsText.setFont(font);
    priceInstructionsText.setString("Enter the Max Price: ");
    priceInstructionsText.setCharacterSize(30);
    priceInstructionsText.setFillColor(sf::Color::Red);
    float priceInstructionsWidth = priceInstructionsText.getLocalBounds().width;
    priceInstructionsText.setPosition((720 / 2) - (priceInstructionsWidth / 2), 400);

    priceInputFieldBg.setSize(sf::Vector2f(400, 50));
    priceInputFieldBg.setFillColor(sf::Color::White);
    priceInputFieldBg.setPosition(160, 450);

    priceInputText.setFont(font);
    priceInputText.setCharacterSize(28);
    priceInputText.setFillColor(sf::Color::Black);
    priceInputText.setPosition(160, 450);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (event.text.unicode == '\b')
                    {
                        if (activeInputField == ActiveInputField::Country && !userInput.empty())
                            userInput.pop_back();
                        else if (activeInputField == ActiveInputField::Variety && !varietyInput.empty())
                            varietyInput.pop_back();
                        else if (activeInputField == ActiveInputField::Price && !priceInput.empty())
                            priceInput.pop_back();
                    }
                    else if (event.text.unicode != '\b')
                    {
                        if (activeInputField == ActiveInputField::Country)
                            userInput += static_cast<char>(event.text.unicode);
                        else if (activeInputField == ActiveInputField::Variety)
                            varietyInput += static_cast<char>(event.text.unicode);
                        else if (activeInputField == ActiveInputField::Price)
                            priceInput += static_cast<char>(event.text.unicode);
                    }

                    userInputText.setString(userInput);
                    varietyInputText.setString(varietyInput);
                    priceInputText.setString(priceInput);
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    // Update resultsText based on user inputs
                    resultsText.setString("Country: " + userInput + "\nVariety: " + varietyInput + "\nMax Price: " + priceInput);

                    resultsText.setFillColor(sf::Color::Red);

                    float resultsWidth = resultsText.getLocalBounds().width;

                    resultsText.setPosition((720 / 2) - (resultsWidth / 2), 10);




                    std::vector<Wine> selection = GetWinesByCountry(countryTable, userInput);

                    auto varietyTable = hashtable_var(selection);

                    std::vector<Wine> selection2 = GetWinesByVariety(varietyTable, varietyInput);

                    std::map<std::string, Wine> filteredWines = GetWinesLessThanValue(selection2,
                                                                                      std::stod(priceInput));

                    std::string mapContent;

                    std::vector<std::pair<std::string, Wine>> filteredWinesVector(filteredWines.begin(), filteredWines.end());

                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::shuffle(filteredWinesVector.begin(), filteredWinesVector.end(), gen);

                    for (size_t i = 0; i < 5 && i < filteredWinesVector.size(); ++i)
                    {
                        const auto& pair = filteredWinesVector[i];
                        mapContent += pair.first;
                        mapContent += "\n";
                        mapContent += "\n";
                    }





                    for (const auto& pair : filteredWines)
                    {
                        std::cout << "Name: " << pair.first << ", ";
                        std::cout << "Value: " << pair.second.points << std::endl;
                    }

                    mapText.setString(mapContent);


                    state = AppState::Results;
                }
                else if (event.key.code == sf::Keyboard::Tab)
                {
                    if (activeInputField == ActiveInputField::Country)
                        activeInputField = ActiveInputField::Variety;
                    else if (activeInputField == ActiveInputField::Variety)
                        activeInputField = ActiveInputField::Price;
                    else
                        activeInputField = ActiveInputField::Country;
                }
            }
        }

        window.clear();

        if (state == AppState::Input) {
            window.draw(titleText);
            window.draw(instructionsText);
            window.draw(inputFieldBg);
            window.draw(userInputText);

            window.draw(varietyInstructionsText);
            window.draw(varietyInputFieldBg);
            window.draw(varietyInputText);

            window.draw(priceInstructionsText);
            window.draw(priceInputFieldBg);
            window.draw(priceInputText);

            if (cursorVisible) {
                if (activeInputField == ActiveInputField::Country) {
                    cursor.setPosition(userInputText.getPosition().x + userInputText.getLocalBounds().width + 5, 260);
                }
                else if (activeInputField == ActiveInputField::Variety) {
                    cursor.setPosition(varietyInputText.getPosition().x + varietyInputText.getLocalBounds().width + 5, 360);
                }
                else if (activeInputField == ActiveInputField::Price) {
                    cursor.setPosition(priceInputText.getPosition().x + priceInputText.getLocalBounds().width + 5, 460);
                }
                window.draw(cursor);
            }
        }
        else if (state == AppState::Results) {
            window.draw(resultsText);

            window.draw(mapText);



        }

        if (cursorTimer.getElapsedTime().asSeconds() >= 0.5) {
            cursorVisible = !cursorVisible;
            cursorTimer.restart();
        }

        window.display();
    }

    return 0;
}
