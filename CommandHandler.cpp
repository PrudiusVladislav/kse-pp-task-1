
#include "CommandHandler.h"

#include <functional>
#include <iostream>
#include <cstring>


CommandHandler::CommandHandler(GapBuffer *gb, bool *isRunning) : _gb(gb), _isRunning(isRunning) {
    _commands.push_back(new ExitCommand(gb, isRunning));
    _commands.push_back(new AppendCommand(gb, isRunning));
    _commands.push_back(new NewlineCommand(gb, isRunning));
    _commands.push_back(new SaveCommand(gb, isRunning));
    _commands.push_back(new LoadCommand(gb, isRunning));
    _commands.push_back(new PrintCommand(gb, isRunning));
    _commands.push_back(new InsertCommand(gb, isRunning));
    _commands.push_back(new SearchCommand(gb, isRunning));
    _commands.push_back(new DeleteAtCommand(gb, isRunning));
    _commands.push_back(new CutCommand(gb, isRunning));
    _commands.push_back(new CopyCommand(gb, isRunning));
    _commands.push_back(new PasteCommand(gb, isRunning));
    _commands.push_back(new InsertWithReplaceCommand(gb, isRunning));
}

void CommandHandler::handleHelp() {
    printHelp();
}

void CommandHandler::handleUndo() {
    if (_undoStack.empty()) {
        std::cout << "Nothing to undo" << std::endl;
        return;
    }

    IReversibleCommand *command = _undoStack.top();
    _undoStack.pop();

    command->undo();
    _redoStack.push(command);
}

void CommandHandler::handleRedo() {
    if (_redoStack.empty()) {
        std::cout << "Nothing to redo" << std::endl;
        return;
    }

    IReversibleCommand *command = _redoStack.top();
    _redoStack.pop();

    command->redo();
    _undoStack.push(command);
}


void CommandHandler::handle(const char *commandName) {
    if (strcmp(commandName, "help") == 0) {
        handleHelp();
        return;
    }

    if (strcmp(commandName, "undo") == 0) {
        handleUndo();
        return;
    }

    if (strcmp(commandName, "redo") == 0) {
        handleRedo();
        return;
    }

    for (const auto &command : _commands) {
        if (strcmp(commandName, command->getName()) == 0) {
            command->execute();
            if (command->breakUndoChain()) {
                _undoStack = std::stack<IReversibleCommand *>();
                return;
            }
            if (command->canundo()) {
                _undoStack.push(dynamic_cast<IReversibleCommand *>(command));
            }
            return;
        }
    }
    std::cout << "ヽ༼⊙_⊙༽ﾉ Unknown command: " << commandName << std::endl;
    printHelp();
}

void CommandHandler::printHelp() const{
    std::cout << "Available commands:" << std::endl;
    std::cout << "help - print this help" << std::endl;
    std::cout << "undo - undo the last command" << std::endl;
    std::cout << "redo - redo the last undone command" << std::endl;

    for (auto &command: _commands) {
        std::cout << command->getName() << " - ";
        command->printHelp();
    }
}


CommandHandler::~CommandHandler() {
    for (auto &command : _commands) {
        delete command;
    }

    while (!_undoStack.empty()) {
        delete _undoStack.top();
        _undoStack.pop();
    }

    while (!_redoStack.empty()) {
        delete _redoStack.top();
        _redoStack.pop();
    }
}
