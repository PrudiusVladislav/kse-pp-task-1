
#ifndef COMMANDS_H
#define COMMANDS_H
#include "../utils/CaesarCipher.h"
#include "../core/GapBuffer.h"

struct ICommand {
    ICommand() = default;
    ICommand(GapBuffer *gapBuffer, bool *isRunning) : _gapBuffer(gapBuffer), _isRunning(isRunning) {}
    [[nodiscard]] virtual ICommand *create(GapBuffer *gapBuffer, bool *isRunning) = 0;
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
    IReversibleCommand() = default;
    IReversibleCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    virtual void undo() = 0;
    virtual void redo() = 0;
    [[nodiscard]] bool canundo() override { return true; }
};


class ExitCommand final : public ICommand {
public:
    ExitCommand() = default;
    ExitCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "exit"; }
    void execute() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new ExitCommand(gapBuffer, isRunning);
    }
};

class AppendCommand final : public IReversibleCommand {
public:
    AppendCommand() = default;
    AppendCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "append"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new AppendCommand(gapBuffer, isRunning);
    }
private:
    char _appendedText[256];
};

class NewlineCommand final : public IReversibleCommand {
public:
    NewlineCommand() = default;
    NewlineCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "nl"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new NewlineCommand(gapBuffer, isRunning);
    }
};

class SaveCommand final : public ICommand {
public:
    SaveCommand() = default;
    SaveCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "save"; }
    void execute() override;
    void printHelp() const override;
    [[nodiscard]] bool breakUndoChain() const override { return true; }
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new SaveCommand(gapBuffer, isRunning);
    }
};

class LoadCommand final : public ICommand {
public:
    LoadCommand() = default;
    LoadCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "load"; }
    void execute() override;
    void printHelp() const override;
    [[nodiscard]] bool breakUndoChain() const override { return true; }
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new LoadCommand(gapBuffer, isRunning);
    }
};

class PrintCommand final : public ICommand {
public:
    PrintCommand() = default;
    PrintCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "print"; }
    void execute() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new PrintCommand(gapBuffer, isRunning);
    }
};

class InsertCommand final : public IReversibleCommand {
public:
    InsertCommand() = default;
    InsertCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "insert"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new InsertCommand(gapBuffer, isRunning);
    }
private:
    int _insertIndex;
    char _text[256];
};

class SearchCommand final : public ICommand {
public:
    SearchCommand() = default;
    SearchCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "search"; }
    void execute() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new SearchCommand(gapBuffer, isRunning);
    }
};

class DeleteAtCommand final : public IReversibleCommand {
public:
    DeleteAtCommand() = default;
    DeleteAtCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "delete"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new DeleteAtCommand(gapBuffer, isRunning);
    }
    ~DeleteAtCommand() override;
private:
    int _deleteIndex;
    int _length;
    char *_deletedText;
};

class CutCommand final : public IReversibleCommand {
public:
    CutCommand() = default;
    CutCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "cut"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new CutCommand(gapBuffer, isRunning);
    }
    ~CutCommand() override;
private:
    int _cutIndex;
    char *_cutText;
};

class CopyCommand final : public ICommand {
public:
    CopyCommand() = default;
    CopyCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "copy"; }
    void execute() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new CopyCommand(gapBuffer, isRunning);
    }
private:
    int _copyIndex;
};

class PasteCommand final : public IReversibleCommand {
public:
    PasteCommand() = default;
    PasteCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "paste"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new PasteCommand(gapBuffer, isRunning);
    }
    ~PasteCommand() override;
private:
    int _pasteIndex;
    char *_pastedText;
};

class InsertWithReplaceCommand final : public IReversibleCommand {
public:
    InsertWithReplaceCommand() = default;
    InsertWithReplaceCommand(GapBuffer *gapBuffer, bool *isRunning) : IReversibleCommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "insert-r"; }
    void execute() override;
    void undo() override;
    void redo() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new InsertWithReplaceCommand(gapBuffer, isRunning);
    }
    ~InsertWithReplaceCommand() override;
private:
    int _insertIndex;
    char _insertedText[256];
    char *_replacedText;
};

class MoveCursonCommand final : public ICommand {
public:
    MoveCursonCommand() = default;
    MoveCursonCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "move"; }
    void execute() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new MoveCursonCommand(gapBuffer, isRunning);
    }
};

class CipherCommand {
public:
    void cipher();
protected:
    CaesarCipher *_cipher;
    virtual char *processChunk(char *chunk) = 0;
};

class EncryptFileCommand final : public ICommand, public CipherCommand {
public:
    EncryptFileCommand() = default;
    EncryptFileCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "encrypt"; }
    void execute() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new EncryptFileCommand(gapBuffer, isRunning);
    }
protected:
    char *processChunk(char *chunk) override;
};

class DecryptFileCommand final : public ICommand, public CipherCommand{
public:
    DecryptFileCommand() = default;
    DecryptFileCommand(GapBuffer *gapBuffer, bool *isRunning) : ICommand(gapBuffer, isRunning) {}
    [[nodiscard]] const char *getName() const override { return "decrypt"; }
    void execute() override;
    void printHelp() const override;
    ICommand *create(GapBuffer *gapBuffer, bool *isRunning) override {
        return new DecryptFileCommand(gapBuffer, isRunning);
    }
protected:
    char *processChunk(char *chunk) override;
};

#endif //COMMANDS_H
