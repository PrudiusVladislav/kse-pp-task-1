
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <functional>
#include <stack>

#include "GapBuffer.h"
#include "../commands/Commands.h"

class CommandHandler {
public:
    CommandHandler(GapBuffer *gb, bool *isRunning);
    void handle(const char *commandName);
    void printHelp() const;
    ~CommandHandler();

private:
    GapBuffer *_gb;
    bool *_isRunning;
    std::vector<ICommand *> _commands;
    std::stack<IReversibleCommand *> _undoStack;
    std::stack<IReversibleCommand *> _redoStack;

    void handleHelp();
    void handleUndo();
    void handleRedo();
};

#endif //COMMANDHANDLER_H
