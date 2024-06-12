
#include "CommandHandler.h"

#include <functional>
#include <iostream>
#include <cstring>


CommandHandler::CommandHandler(GapBuffer *gb, bool *isRunning) : _gb(gb), _isRunning(isRunning) {
    _commands.push_back(new ExitCommand());
    _commands.push_back(new AppendCommand());
    _commands.push_back(new NewlineCommand());
    _commands.push_back(new SaveCommand());
    _commands.push_back(new LoadCommand());
    _commands.push_back(new PrintCommand());
    _commands.push_back(new InsertCommand());
    _commands.push_back(new SearchCommand());
    _commands.push_back(new DeleteAtCommand());
    _commands.push_back(new CutCommand());
    _commands.push_back(new CopyCommand());
    _commands.push_back(new PasteCommand());
    _commands.push_back(new InsertWithReplaceCommand());
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
            ICommand *cmd = command->create(_gb, _isRunning);
            cmd->execute();
            if (cmd->breakUndoChain()) {
                _undoStack = std::stack<IReversibleCommand *>();
                return;
            }
            if (cmd->canundo()) {
                _undoStack.push(dynamic_cast<IReversibleCommand *>(cmd));
                return;
            }
            delete cmd;
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
    while (!_undoStack.empty()) {
        delete _undoStack.top();
        _undoStack.pop();
    }

    while (!_redoStack.empty()) {
        delete _redoStack.top();
        _redoStack.pop();
    }

    for (auto &command : _commands) {
        delete command;
    }
}
