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

    // gap_buffer_append(gb, "\n");
    // gap_buffer_append(gb, "line2");
    // gap_buffer_insert_at(gb, 0, 0, "line1");
    // gap_buffer_append(gb, "\n");
    // gap_buffer_insert_at(gb, 2, 0, "line3");
    // gap_buffer_print(gb);

    gb->clear();
    delete gb;
    delete commandHandler;
    return 0;
}