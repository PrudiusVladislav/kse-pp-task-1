
#include "CommandHandler.h"

#include <functional>
#include <iostream>
#include <string.h>


CommandHandler::CommandHandler(GapBuffer *gb, bool *isRunning): _gb(gb), _isRunning(isRunning) {
    _commands = {
        {"help", std::bind(&CommandHandler::help, this)},
        {"exit", std::bind(&CommandHandler::exitEditor, this)},
        {"append", std::bind(&CommandHandler::append, this)},
        {"nl", std::bind(&CommandHandler::newline, this)},
        {"save", std::bind(&CommandHandler::save, this)},
        {"load", std::bind(&CommandHandler::load, this)},
        {"print", std::bind(&CommandHandler::print, this)},
        {"insert", std::bind(&CommandHandler::insert, this)},
        {"search", std::bind(&CommandHandler::search, this)},
        {"clear", std::bind(&CommandHandler::clear, this)}
    };
}

void CommandHandler::printHelp() {
    std::cout << "Available commands:\n";
    std::cout << "help - Print this help message\n";
    std::cout << "exit - Exit the program\n";
    std::cout << "append - Append text to the end of the buffer\n";
    std::cout << "nl - Start a new line\n";
    std::cout << "save - Save the buffer to a file\n";
    std::cout << "load - Load a file into the buffer\n";
    std::cout << "print - Print the contents of the buffer\n";
    std::cout << "insert - Insert text at a specific line and index\n";
    std::cout << "search - Search for text in the buffer\n";
    std::cout << "clear - Clear the file contents\n";
}

void CommandHandler::help() {
    printHelp();
}

void CommandHandler::unsavedChangesDialog() {
    if (_gb->isSaved()) {
        return;
    }

    printf("You have unsaved changes. Do you want to save them? (y/n) ");
    char answer;
    scanf(" %c", &answer);
    if (tolower(answer) == 'y') {
        if (const char *setFilePath = _gb->getFilePath(); setFilePath != nullptr) {
            _gb->saveToFile(setFilePath);
        } else {
            char filename[256];
            printf("Enter the file name for saving: ");
            scanf("%s", filename);
            _gb->saveToFile(filename);
        }
    }
}

void CommandHandler::exitEditor() {
    if (!_gb->isSaved()) {
        unsavedChangesDialog();
    }

    *_isRunning = false;
}

void CommandHandler::append() {
    char text[256];
    printf("Enter text to append: ");
    scanf(" %[^\n]", text);
    _gb->append(text);
}

void CommandHandler::newline() {
    printf("New line is started\n");
    _gb->append("\n");
}

void CommandHandler::save() {
    char filename[256];
    printf("Enter the file name for saving: ");
    scanf("%s", filename);
    _gb->saveToFile(filename);
}

void CommandHandler::load() {
    if (!_gb->isSaved()) {
        unsavedChangesDialog();
    }

    char filename[256];
    printf("Enter the file name for loading:");
    scanf("%s", filename);
    _gb->loadFromFile(filename);
}

void CommandHandler::print() {
    _gb->print();
}

void CommandHandler::insert() {
    char text[256];
    int line, index;
    printf("Choose line and column (e.g. 8 2): ");
    fflush(stdout);
    scanf("%d %d", &line, &index);
    printf("Enter text to insert: ");
    scanf(" %[^\n]", text);
    _gb->insertAt(line, index, text);
}

void CommandHandler::search() {
    char text[256];
    printf("Enter text to search: ");
    scanf(" %[^\n]", text);
    _gb->search(text);
}

void CommandHandler::clear() {
    _gb->clear();
}

CommandHandler::~CommandHandler() {
    _commands.clear();
}

void CommandHandler::handle(const char *commandName) {
    const int commands_count = _commands.size();

    for (int i = 0; i < commands_count; i++) {
        if (strcmp(commandName, _commands[i].name) == 0) {
            _commands[i].func();
            return;
        }
    }

    printf("The command is not implemented yet :(\n");
    printHelp();
}
