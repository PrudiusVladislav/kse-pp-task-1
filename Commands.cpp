
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
    const int appendedTextIndex = _gapBuffer->getContentLength() - appendedLength;

    _gapBuffer->moveGapTo(appendedTextIndex);
    _gapBuffer->remove(appendedLength);
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
    const int lastCharIndex = _gapBuffer->getContentLength() - 1 - 1;

    _gapBuffer->moveGapTo(lastCharIndex);
    _gapBuffer->remove(1);
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
    std::cout << "Enter text to insert: " << std::flush;
    scanf(" %[^\n]", _text);

    _insertIndex = _gapBuffer->getIndexFromPosition(_gapBuffer->getCursorPosition());
    _gapBuffer->insertAt(_text);
}

void InsertCommand::undo() {
    _gapBuffer->moveGapTo(_insertIndex);
    _gapBuffer->remove(strlen(_text));
}

void InsertCommand::redo() {
    _gapBuffer->moveGapTo(_insertIndex);
    _gapBuffer->insertAt(_text);
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
    std::cout << "Enter length of the text to delete: " << std::flush;
    scanf("%d", &_length);

    _deleteIndex = _gapBuffer->getIndexFromPosition(_gapBuffer->getCursorPosition());
    _deletedText = _gapBuffer->remove(_length);
}

void DeleteAtCommand::undo() {
    _gapBuffer->moveGapTo(_deleteIndex);
    _gapBuffer->insertAt(_deletedText);

    delete[] _deletedText;
}

void DeleteAtCommand::redo() {
    _gapBuffer->moveGapTo(_deleteIndex);
    _deletedText = _gapBuffer->remove(_length);
}

void DeleteAtCommand::printHelp() const {
    std::cout << "delete text at a specific line and index" << std::endl;
}

DeleteAtCommand::~DeleteAtCommand() {
    delete[] _deletedText;
}

void CutCommand::execute() {
    int length;
    std::cout << "Enter the length of the text to cut: " << std::flush;
    scanf("%d", &length);

    _cutIndex = _gapBuffer->getIndexFromPosition(_gapBuffer->getCursorPosition());
    _cutText = _gapBuffer->cut(length);
}

void CutCommand::undo() {
    _gapBuffer->moveGapTo(_cutIndex);
    _gapBuffer->insertAt(_cutText);
    delete[] _cutText;
}

void CutCommand::redo() {
    _gapBuffer->moveGapTo(_cutIndex);
    _cutText = _gapBuffer->cut(strlen(_cutText));
}

void CutCommand::printHelp() const {
    std::cout << "cut text at a specific line and index" << std::endl;
}

CutCommand::~CutCommand() {
    delete[] _cutText;
}

void CopyCommand::execute() {
    int length;
    std::cout << "Enter the length of the text to copy: " << std::flush;
    scanf("%d", &length);

    _copyIndex = _gapBuffer->getIndexFromPosition(_gapBuffer->getCursorPosition());
    _gapBuffer->copy(length);
}

void CopyCommand::printHelp() const {
    std::cout << "copy text at a specific line and index" << std::endl;
}

void PasteCommand::execute() {
    _pasteIndex = _gapBuffer->getIndexFromPosition(_gapBuffer->getCursorPosition());
    _pastedText = _gapBuffer->paste();
}

void PasteCommand::undo() {
    _gapBuffer->moveGapTo(_pasteIndex);
    _gapBuffer->remove(strlen(_pastedText));
}

void PasteCommand::redo() {
    _gapBuffer->moveGapTo(_pasteIndex);
    _gapBuffer->insertAt(_pastedText);
}

void PasteCommand::printHelp() const {
    std::cout << "paste copied text at a specific line and index" << std::endl;
}

PasteCommand::~PasteCommand() {
    delete[] _pastedText;
}

void InsertWithReplaceCommand::execute() {
    std::cout << "Enter text to insert: " << std::endl;
    scanf(" %[^\n]", _insertedText);

    _insertIndex = _gapBuffer->getIndexFromPosition(_gapBuffer->getCursorPosition());
    _replacedText = _gapBuffer->insertWithReplace(_insertedText);
}

void InsertWithReplaceCommand::undo() {
    _gapBuffer->moveGapTo(_insertIndex);
    _gapBuffer->remove(strlen(_insertedText));
    _gapBuffer->insertAt(_replacedText);
}

void InsertWithReplaceCommand::redo() {
    _gapBuffer->moveGapTo(_insertIndex);
    _gapBuffer->insertWithReplace(_insertedText);
}

void InsertWithReplaceCommand::printHelp() const {
    std::cout << "insert text at a specific line and index, replacing the existing text" << std::endl;
}

InsertWithReplaceCommand::~InsertWithReplaceCommand() {
    delete[] _replacedText;
}

void MoveCursonCommand::execute() {
    int line, index;
    std::cout << "Choose line and column to move the cursor (e.g. 8 2): " << std::flush;
    scanf("%d %d", &line, &index);

    int position = _gapBuffer->getIndexFromPosition({line, index});
    if (position == -1) {
        std::cout << "Invalid position" << std::endl;
        return;
    }

    _gapBuffer->moveGapTo(position);
}

void MoveCursonCommand::printHelp() const {
    std::cout << "move curson to a line and index. later, it will be printed as: |_|" << std::endl;
}














