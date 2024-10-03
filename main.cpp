#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <chrono>
#include <thread>
#include <numeric>
#include <cstdio>  // For system commands

// Placeholder function for speech recognition (manual input simulation)
std::string listen_for_command() {
    std::string command;
    std::cout << "Listening for commands...\n";
    std::getline(std::cin, command);  // For now, manual input simulation
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);  // Convert to lowercase
    return command;
}

// Function to use espeak for text-to-speech
void respond(const std::string& response_text) {
    std::cout << response_text << std::endl;
    std::string tts_command = "espeak \"" + response_text + "\"";
    system(tts_command.c_str());  // Calls espeak for TTS
}

// Function to tell a joke
void tell_joke() {
    std::vector<std::string> jokes = {
        "Why don't scientists trust atoms? Because they make up everything!",
        "Why did the scarecrow win an award? Because he was outstanding in his field!"
    };
    respond(jokes[rand() % jokes.size()]);
}

// Function to extract numbers from a string
std::vector<int> extract_numbers(const std::string& command) {
    std::vector<int> numbers;
    std::regex num_regex("\\d+");
    auto numbers_begin = std::sregex_iterator(command.begin(), command.end(), num_regex);
    auto numbers_end = std::sregex_iterator();

    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
        std::smatch match = *i;
        numbers.push_back(std::stoi(match.str()));
    }

    return numbers;
}

// Function to perform basic arithmetic operations
void perform_calculation(const std::string& command) {
    std::vector<int> numbers = extract_numbers(command);
    int result = 0;

    if (command.find("+") != std::string::npos) {
        result = std::accumulate(numbers.begin(), numbers.end(), 0);
    } else if (command.find("-") != std::string::npos) {
        result = numbers[0];
        for (size_t i = 1; i < numbers.size(); ++i) {
            result -= numbers[i];
        }
    } else if (command.find("*") != std::string::npos) {
        result = 1;
        for (int num : numbers) {
            result *= num;
        }
    } else if (command.find("/") != std::string::npos) {
        result = numbers[0];
        for (size_t i = 1; i < numbers.size(); ++i) {
            result /= numbers[i];
        }
    } else {
        respond("Sorry, I can only perform addition, subtraction, multiplication, and division.");
        return;
    }

    respond("The result is " + std::to_string(result) + ".");
}

// Function to search a query on Google using xdg-open
void search_on_google(const std::string& query) {
    std::string search_url = "https://www.google.com/search?q=" + query;
    std::string command = "xdg-open " + search_url;
    system(command.c_str());  // Open web browser using xdg-open
}

// Function to take a screenshot using scrot
void take_screenshot() {
    std::string screenshot_command = "scrot screenshot.png";  // Take screenshot and save it
    system(screenshot_command.c_str());
    respond("Screenshot has been taken.");
}

// Main loop to handle commands
void main_loop() {
    bool listeningToTask = false;
    std::vector<std::string> tasks;
    std::string triggerKeyword = "ava";

    while (true) {
        std::string command = listen_for_command();
        std::cout << "Command recognized: " << command << std::endl;

        if (!command.empty() && command.find(triggerKeyword) != std::string::npos) {
            if (listeningToTask) {
                tasks.push_back(command);
                listeningToTask = false;
                respond("Adding task: " + command + ". You now have " + std::to_string(tasks.size()) + " tasks.");
            } else if (command.find("add a task") != std::string::npos) {
                listeningToTask = true;
                respond("Sure, what is the task?");
            } else if (command.find("list tasks") != std::string::npos) {
                if (!tasks.empty()) {
                    respond("Here are your tasks:");
                    for (const std::string& task : tasks) {
                        respond(task);
                    }
                } else {
                    respond("You have no tasks.");
                }
            } else if (command.find("take a screenshot") != std::string::npos) {
                take_screenshot();
            } else if (command.find("open browser") != std::string::npos || command.find("open internet") != std::string::npos) {
                respond("Opening browser.");
                search_on_google("http://www.google.com");
            } else if (command.find("open youtube") != std::string::npos) {
                respond("Opening YouTube.");
                search_on_google("https://www.youtube.com/");
            } else if (command.find("hello") != std::string::npos) {
                respond("Hello, how can I help you?");
            } else if (command.find("who is your creator") != std::string::npos) {
                respond("His name is Jacob, rest is classified.");
            } else if (command.find("terminate") != std::string::npos) {
                respond("Goodbye!");
                break;
            } else if (command.find("time") != std::string::npos) {
                auto now = std::chrono::system_clock::now();
                std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                respond("The current time is " + std::string(std::ctime(&currentTime)));
            } else if (command.find("what are you") != std::string::npos) {
                respond("I am Advanced Virtual Assistant, here to help you.");
            } else if (command.find("calculate") != std::string::npos) {
                perform_calculation(command);
            } else if (command.find("roll") != std::string::npos) {
                int number = rand() % 6 + 1;
                respond(std::to_string(number));
            } else if (command.find("joke") != std::string::npos) {
                tell_joke();
            } else {
                respond("I'm sorry, I do not understand.");
            }
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));  // Seed random number generator
    respond("AVA is initializing.");
    respond("AVA is ready.");
    respond("Hello Jacob");
    main_loop();
    return 0;
}