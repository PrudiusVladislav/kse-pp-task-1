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

    // const char *text = new char[10];
    // delete[] text;
    // text = new char[10];
    // strcpy(const_cast<char *>(text), "hello!");
    // std::cout << text << std::endl;

    // gb->append("test");
    // const char *copiedText = gb->copy({0 ,0}, 2);
    // std::cout << "Copied text: " << copiedText << std::endl;

    // gb->append("new test!");
    // gb->print();
    // // gb->insertWithReplace({0, 0}, "the");
    // gb->cut({ 0, 0 }, 3);
    // gb->print();

    // gb->append("new");
    // gb->cut({0,0}, 1);
    // gb->paste({0,0});
    // gb->print();


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