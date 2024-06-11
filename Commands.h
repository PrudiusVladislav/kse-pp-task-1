
#ifndef COMMANDS_H
#define COMMANDS_H
#include "GapBuffer.h"

struct ICommand {
    ICommand(GapBuffer *gapBuffer, bool *isRunning) : _gapBuffer(gapBuffer), _isRunning(isRunning) {}
    [[nodiscard]] virtual const char *getName() const = 0;
    virtual void execute() = 0;
    virtual void printHelp() const = 0;
    [[nodiscard]] virtual bool canundo() { return false; }
    [[nodiscard]] virtual bool breakUndoChain() const { return false; }
    virtual ~ICommand() = default;

protected:
    GapBuffer *_gapBuffer;
    bool *_isRunning;
};

struct IReversibleCommand : ICommand {
    IReversibleCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    virtual void undo() = 0;
    virtual void redo() = 0;
    [[nodiscard]] bool canundo() override { return true; }
};


class ExitCommand final : public ICommand {
public:
    ExitCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "exit"; }
    void execute() override;
    void printHelp() const override;
};

class AppendCommand final : public IReversibleCommand {
public:
    AppendCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "append"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ~AppendCommand() override;
private:
    char _appendedText[256];
};

class NewlineCommand final : public IReversibleCommand {
public:
    NewlineCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "nl"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
};

class SaveCommand final : public ICommand {
public:
    SaveCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "save"; }
    void execute() override;
    void printHelp() const override;
    [[nodiscard]] bool breakUndoChain() const override { return true; }
};

class LoadCommand final : public ICommand {
public:
    LoadCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "load"; }
    void execute() override;
    void printHelp() const override;
    [[nodiscard]] bool breakUndoChain() const override { return true; }
};

class PrintCommand final : public ICommand {
public:
    PrintCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "print"; }
    void execute() override;
    void printHelp() const override;
};

class InsertCommand final : public IReversibleCommand {
public:
    InsertCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "insert"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ~InsertCommand() override;
private:
    Position _position;
    char _text[256];
};

class SearchCommand final : public ICommand {
public:
    SearchCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "search"; }
    void execute() override;
    void printHelp() const override;
};

class DeleteAtCommand final : public IReversibleCommand {
public:
    DeleteAtCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "delete"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ~DeleteAtCommand() override;
private:
    Position _position;
    int _length;
    char *_deletedText;
};

class CutCommand final : public IReversibleCommand {
public:
    CutCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "cut"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ~CutCommand() override;
private:
    Position _position;
    char *_cutText;
};

class CopyCommand final : public ICommand {
public:
    CopyCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "copy"; }
    void execute() override;
    void printHelp() const override;
private:
    Position _position;
};

class PasteCommand final : public IReversibleCommand {
public:
    PasteCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "paste"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ~PasteCommand() override;
private:
    Position _position;
    char *_pastedText;
};

class InsertWithReplaceCommand final : public IReversibleCommand {
public:
    InsertWithReplaceCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "insert-r"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ~InsertWithReplaceCommand() override;
private:
    Position _position;
    char *_replacedText;
    char *_insertedText;
};

#endif //COMMANDS_H
