// Nodira Kazakova
// Password strength tester

#include <iostream>
#include <string>
#include <cctype>
#include <stdio.h>
#include <tgbot/tgbot.h>

using namespace std;


/*------------------------------------------------------- Funcitons -------------------------------------------------------------------*/

// Function to calculate password strength and provide feedback
pair<string, string> getPasswordStrength(const string& password) {
    int length = password.size();
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;

    for (char c : password) {
        if (islower(c)) hasLower = true;
        if (isupper(c)) hasUpper = true;
        if (isdigit(c)) hasDigit = true;
        if (!isalnum(c)) hasSpecial = true;
    }

    int strength = 0;
    if (length >= 8) strength++;
    if (length >= 12) strength++;
    if (hasLower) strength++;
    if (hasUpper) strength++;
    if (hasDigit) strength++;
    if (hasSpecial) strength++;

    string result = "Password strength: ";
    string feedback;

    if (length < 8) feedback += "Increase the length to at least 8 characters. ";
    if (!hasLower) feedback += "Add at least one lowercase letter. ";
    if (!hasUpper) feedback += "Add at least one uppercase letter. ";
    if (!hasDigit) feedback += "Include at least one numeric digit. ";
    if (!hasSpecial) feedback += "Include at least one special character. ";

    switch (strength) {
    case 0:
    case 1:
    case 2:
        result += "Weak";
        break;
    case 3:
    case 4:
        result += "Moderate";
        break;
    case 5:
    case 6:
        result += "Strong";
        break;
    }

    return { result, feedback };
}

/*------------------------------------------------------- Main Funciton -------------------------------------------------------------------*/

int main() {
    TgBot::Bot bot("8159927544:AAFOM8Zj7nCEkY6rvfEmESzWqql790bymPA");

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Welcome! Send me a password, and I'll test its strength. Type /quit to end the session.");
        });

    bot.getEvents().onCommand("quit", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Goodbye! If you need me again, just type /start.");
        });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        if (message->text == "/start" || message->text == "/quit") {
            return;
        }

        string password = message->text;
        auto [strengthResult, feedback] = getPasswordStrength(password);

        bot.getApi().sendMessage(message->chat->id, strengthResult);
        if (!feedback.empty()) {
            bot.getApi().sendMessage(message->chat->id, "Suggestions to improve your password: " + feedback);
        }

        bot.getApi().sendMessage(message->chat->id, "Would you like to test another password? Type it or type /quit to end.");
        });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
