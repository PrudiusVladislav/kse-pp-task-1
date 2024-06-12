
#include "Commands.h"

#include <iostream>
#include <cstring>


void unsavedChangesDialog(GapBuffer *gb) {
    if (gb->isSaved()) {
        return;
    }

    std::cout << "You have unsaved changes. Do you want to save them? (y/n): " << std::flush;
    char answer;
    scanf(" %c", &answer);
    if (tolower(answer) == 'y') {
        if (const char *setFilePath = gb->getFilePath(); setFilePath != nullptr) {
            gb->saveToFile(setFilePath);
        } else {
            char filename[256];
            std::cout << "Enter the file name for saving: " << std::flush;
            scanf("%s", filename);
            gb->saveToFile(filename);
        }
    }
}

void ExitCommand::execute() {
    if (!_gapBuffer->isSaved()) {
        unsavedChangesDialog(_gapBuffer);
    }

    *_isRunning = false;
}

void ExitCommand::printHelp() const {
    std::cout << "exit the editor" << std::endl;
}

void AppendCommand::execute() {
    std::cout << "Enter text to append: " << std::flush;
    scanf(" %[^\n]", _appendedText);
    _gapBuffer->append(_appendedText);
}

void AppendCommand::undo() {
    const int appendedLength = strlen(_appendedText);
    const Position position = _gapBuffer->getPositionFromIndex(
        _gapBuffer->getContentLength() - appendedLength);

    _gapBuffer->remove(position, appendedLength);
}

void AppendCommand::redo() {
    _gapBuffer->append(_appendedText);
}

void AppendCommand::printHelp() const {
    std::cout << "append text to the end of the buffer" << std::endl;
}

void NewlineCommand::execute() {
    _gapBuffer->append("\n");
    std::cout << "New line is started\n" << std::endl;
}

void NewlineCommand::undo() {
    const Position position = _gapBuffer->getPositionFromIndex(_gapBuffer->getContentLength() - 1 - 1);
    _gapBuffer->remove(position, 1);
}

void NewlineCommand::redo() {
    _gapBuffer->append("\n");
}

void NewlineCommand::printHelp() const {
    std::cout << "start a new line" << std::endl;
}

void SaveCommand::execute() {
    if (_gapBuffer->isSaved()) {
        return;
    }

    if (const char *setFilePath = _gapBuffer->getFilePath(); setFilePath != nullptr) {
        _gapBuffer->saveToFile(setFilePath);
        return;
    }

    char filename[256];
    std::cout << "Enter the file name for saving: " << std::flush;
    scanf("%s", filename);
    _gapBuffer->saveToFile(filename);
}

void SaveCommand::printHelp() const {
    std::cout << "save the buffer to a file" << std::endl;
}

void LoadCommand::execute() {
    if (!_gapBuffer->isSaved()) {
        unsavedChangesDialog(_gapBuffer);
    }

    char filename[256];
    std::cout << "Enter the file name for loading: " << std::flush;
    scanf("%s", filename);
    _gapBuffer->loadFromFile(filename);
}

void LoadCommand::printHelp() const {
    std::cout << "load a file into the buffer" << std::endl;
}

void PrintCommand::execute() {
    _gapBuffer->print();
}

void PrintCommand::printHelp() const {
    std::cout << "print the contents of the buffer" << std::endl;
}

void InsertCommand::execute() {
    std::cout << "Enter line and index to insert text (e.g. 8 2): " << std::flush;
    scanf("%d %d", &_position.line, &_position.index);
    std::cout << "Enter text to insert: " << std::flush;
    scanf(" %[^\n]", _text);
    _gapBuffer->insertAt(_position, _text);
}

void InsertCommand::undo() {
    _gapBuffer->remove(_position, strlen(_text));
}

void InsertCommand::redo() {
    _gapBuffer->insertAt(_position, _text);
}

void InsertCommand::printHelp() const {
    std::cout << "insert text at a specific line and index" << std::endl;
}

void SearchCommand::execute() {
    char text[256];
    std::cout << "Enter text to search: " << std::flush;
    scanf(" %[^\n]", text);
    _gapBuffer->search(text);
}

void SearchCommand::printHelp() const {
    std::cout << "search for text in the buffer" << std::endl;
}

void DeleteAtCommand::execute() {
    std::cout << "Enter line, index and length to delete text (e.g. 8 2 5): " << std::flush;
    scanf("%d %d %d", &_position.line, &_position.index, &_length);
    _deletedText = _gapBuffer->remove(_position, _length);
}

void DeleteAtCommand::undo() {
    _gapBuffer->insertAt(_position, _deletedText);
    delete[] _deletedText;
}

void DeleteAtCommand::redo() {
    _deletedText = _gapBuffer->remove(_position, _length);
}

void DeleteAtCommand::printHelp() const {
    std::cout << "delete text at a specific line and index" << std::endl;
}

DeleteAtCommand::~DeleteAtCommand() {
    delete[] _deletedText;
}

void CutCommand::execute() {
    int length;
    std::cout << "Enter line, index and length to cut text (e.g. 8 2 5): " << std::flush;
    scanf("%d %d %d", &_position.line, &_position.index, &length);
    _cutText = _gapBuffer->cut(_position, length);
}

void CutCommand::undo() {
    _gapBuffer->insertAt(_position, _cutText);
    delete[] _cutText;
}

void CutCommand::redo() {
    _cutText = _gapBuffer->cut(_position, strlen(_cutText));
}

void CutCommand::printHelp() const {
    std::cout << "cut text at a specific line and index" << std::endl;
}

CutCommand::~CutCommand() {
    delete[] _cutText;
}

void CopyCommand::execute() {
    int length;
    std::cout << "Enter line, index and length to copy text (e.g. 8 2 5): " << std::flush;
    scanf("%d %d %d", &_position.line, &_position.index, &length);
    _gapBuffer->copy(_position, length);
}

void CopyCommand::printHelp() const {
    std::cout << "copy text at a specific line and index" << std::endl;
}

void PasteCommand::execute() {
    std::cout << "Choose line and column to paste text (e.g. 8 2): " << std::flush;
    scanf("%d %d", &_position.line, &_position.index);
    _pastedText = _gapBuffer->paste(_position);
}

void PasteCommand::undo() {
    _gapBuffer->remove(_position, strlen(_pastedText));
}

void PasteCommand::redo() {
    _gapBuffer->insertAt(_position, _pastedText);
}

void PasteCommand::printHelp() const {
    std::cout << "paste copied text at a specific line and index" << std::endl;
}

PasteCommand::~PasteCommand() {
    delete[] _pastedText;
}

void InsertWithReplaceCommand::execute() {
    std::cout << "Choose line and column to insert text (e.g. 8 2): " << std::flush;
    scanf("%d %d", &_position.line, &_position.index);
    std::cout << "Enter text to insert: " << std::endl;
    scanf(" %[^\n]", _insertedText);
    _replacedText = _gapBuffer->insertWithReplace(_position, _insertedText);
}

void InsertWithReplaceCommand::undo() {
    _gapBuffer->remove(_position, strlen(_insertedText));
    _gapBuffer->insertAt(_position, _replacedText);
}

void InsertWithReplaceCommand::redo() {
    _gapBuffer->insertWithReplace(_position, _insertedText);
}

void InsertWithReplaceCommand::printHelp() const {
    std::cout << "insert text at a specific line and index, replacing the existing text" << std::endl;
}

InsertWithReplaceCommand::~InsertWithReplaceCommand() {
    delete[] _replacedText;
    delete[] _insertedText;
}












