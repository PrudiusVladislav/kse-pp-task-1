#include <cctype>
#include <cstdio>
#include <string>
#include <iostream>

#include "CommandHandler.h"
#include "GapBuffer.h"
#include "StringUtils.h"


char *promptForInputCommand() {
    char *command = StringUtils::CreateString(256);
    std::cout << "[cpp-editor]---> " << std::flush;
    scanf("%s", command);
    return StringUtils::ToLower(StringUtils::Trim(command));
}

void multiLineTestCase(GapBuffer *gb) {
    gb->append("\n");
    gb->append("line2");
    gb->insertAt({0, 0}, "line1");
    gb->append("\n");
    gb->insertAt({2, 0}, "line3");
    gb->print();
}

int main() {
    auto *gb = new GapBuffer();
    bool isRunning = true;
    auto *commandHandler = new CommandHandler(gb, &isRunning);

    commandHandler->printHelp();

    while (isRunning) {
        const char *command = promptForInputCommand();
        commandHandler->handle(command);
        delete[] command;
    }

    delete commandHandler;
    delete gb;
    return 0;
}