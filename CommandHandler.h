
#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <functional>

#include "GapBuffer.h"

typedef std::function<void()> CommandFunc;

struct Command {
    const char *name;
    CommandFunc func;
};

class CommandHandler {
public:
    CommandHandler(GapBuffer *gb, bool *isRunning);
    void handle(const char *commandName);
    static void printHelp() ;

private:
    GapBuffer *_gb;
    bool *_isRunning;
    std::vector<Command> _commands;

    void help();
    void exitEditor();
    void unsavedChangesDialog();
    void append();
    void newline();
    void save();
    void load();
    void print();
    void insert();
    void search();
    void clearСonsole();
};

#endif //COMMANDHANDLER_H
