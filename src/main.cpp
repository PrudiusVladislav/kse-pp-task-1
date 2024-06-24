#include <cctype>
#include <cstdio>
#include <string>
#include <iostream>

#include "core/CommandHandler.h"
#include "core/GapBuffer.h"
#include "utils/StringUtils.h"


char *promptForInputCommand() {
    char *command = StringUtils::CreateString(256);
    std::cout << "[cpp-editor]---> " << std::flush;
    scanf("%s", command);
    return StringUtils::ToLower(StringUtils::Trim(command));
}

void multiLineTestCase(GapBuffer *gb) {
    gb->append("\n");
    gb->append("line2");
    gb->moveGapTo(0);
    gb->insertAt("line1");
    gb->append("\n");
    gb->moveGapTo(gb->getIndexFromPosition({2, 0}));
    gb->insertAt("line3");
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