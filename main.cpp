#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "CommandHandler.h"
#include "GapBuffer.h"
#include "StringUtils.h"


char *promptForInputCommand() {
    char *command = StringUtils::CreateString(256);
    printf("[cpp-editor]--->");
    scanf("%s", command);
    return StringUtils::ToLower(StringUtils::Trim(command));
}

int main(void) {
    auto *gb = new GapBuffer();
    bool isRunning = true;

    auto *commandHandler = new CommandHandler(gb, &isRunning);
    CommandHandler::printHelp();

    while (isRunning) {
        const char *command = promptForInputCommand();
        commandHandler->handle(command);
    }

    delete gb;
    delete commandHandler;
    return 0;
}

void multiLineTestCase(GapBuffer *gb) {
    gb->append("\n");
    gb->append("line2");
    gb->insertAt(0, 0, "line1");
    gb->append("\n");
    gb->insertAt(2, 0, "line3");
    gb->print();
}