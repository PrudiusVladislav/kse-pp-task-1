#include "GapBuffer.h"
#include "StringUtils.h"
#include <cstring>
#include <iostream>
#include <bits/streambuf_iterator.h>
#include <format>


#define INITIAL_LINE_STARTS_CAPACITY 10
#define INITIAL_BUFFER_SIZE 100
#define DEFAULT_WINDOW_SIZE 10

GapBuffer::GapBuffer() {
    _buffer = new char[INITIAL_BUFFER_SIZE + 1];
    _length = INITIAL_BUFFER_SIZE;
    _buffer[_length] = '\0';

    _gap.from = 0;
    _gap.to = INITIAL_BUFFER_SIZE;

    _filepath = nullptr;
    _isSaved = true;

    _lineStarts = new LineStart[INITIAL_LINE_STARTS_CAPACITY];
    _lineStartsCapacity = INITIAL_LINE_STARTS_CAPACITY;
    _linesCount = 0;
}

int GapBuffer::getWindowLength() const {
    return _gap.to - _gap.from;
}

int GapBuffer::getIndexFromPosition(const Position position) const {
    return _lineStarts[position.line].position + position.index;
}

bool GapBuffer::isPositionValid(Position position) const {
    if (position.line >= 0 && position.line < _linesCount) {
        return true;
    }

    return false;
}

int GapBuffer::getContentLength() const {
    return _length - getWindowLength();
}

Position GapBuffer::getPositionFromIndex(const int index) const {
    if (index < 0 || index >= getContentLength()) {
        return { -1, -1 };
    }

    int line = 0;
    while (line < _linesCount && _lineStarts[line].position <= index) {
        line++;
    }

    return { line - 1, index - _lineStarts[line - 1].position };
}

void GapBuffer::expand(const int additionalLength) {
    const int newLength = _length + additionalLength + DEFAULT_WINDOW_SIZE;
    const auto newBuffer = new char[newLength + 1];
    newBuffer[newLength] = '\0';

    memcpy(newBuffer, _buffer, _gap.from * sizeof(char));

    const int lengthAfterGap = _length - _gap.to;
    memcpy(newBuffer + newLength - lengthAfterGap,
        _buffer + _gap.to,
        lengthAfterGap * sizeof(char));

    _gap.to = newLength - lengthAfterGap;
    _length = newLength;

    // delete[] _buffer;
    _buffer = newBuffer;
}

template <typename T>
T* reallocArray(T* oldArray, const int oldSize, const int newSize) {
    T* newArray = new T[newSize];
    std::copy(oldArray, oldArray + oldSize, newArray);
    delete[] oldArray;
    return newArray;
}

// invoked when a '\n' is inserted or deleted;
// int position - position of the new line start or the line start to delete
void GapBuffer::updateLineStarts(const int position, const bool isInsert) {
    if (isInsert) {
        if (_linesCount == _lineStartsCapacity) {
            _lineStartsCapacity *= 2;
            _lineStarts = reallocArray(_lineStarts, _linesCount, _lineStartsCapacity);
        }
        int i = _linesCount;
        while (i > 0 && _lineStarts[i - 1].position > position) {
            _lineStarts[i] = _lineStarts[i - 1];
            i--;
        }
        _lineStarts[i].position = position;
        _linesCount++;
    } else {
        int i = 0;
        while (i < _linesCount && _lineStarts[i].position < position) {
            i++;
        }
        for (int j = i; j < _linesCount - 1; j++) {
            _lineStarts[j] = _lineStarts[j + 1];
        }
        _linesCount--;
    }
}

void GapBuffer::moveGapTo(const int position) {
    if (position < _gap.from) {
        const int moveLength = _gap.from - position;

        memmove(
            _buffer + _gap.to - moveLength,
            _buffer + position,
            moveLength);

        _gap.to -= moveLength;
        _gap.from -= moveLength;

    } else if (position > _gap.from) {
        const int moveLength = position - _gap.from;

        memmove(
            _buffer + _gap.from,
            _buffer + _gap.to,
            moveLength);

        _gap.to += moveLength;
        _gap.from += moveLength;
    }
}

void GapBuffer::append(const char *text) {
    //move gap window to the end of the buffer
    moveGapTo(getContentLength());

    const int textLength = strlen(text);
    const int gapLength = getWindowLength();

    if (textLength > gapLength)
        expand(textLength - gapLength);

    memcpy(_buffer + _gap.from, text, textLength);
    _gap.from += textLength;
    _isSaved = false;

    if (_linesCount == 0) {
        _lineStarts[_linesCount].position = 0;
        _linesCount++;
    }

    if (strchr(text, '\n') != nullptr) {
        updateLineStarts(_gap.from, true);
    }
}


void GapBuffer::print() const{
    if (getContentLength() == 0) {
        std::cout << "Buffer is empty :(" << std::endl;
        return;
    }

    for (int i = 0; i < _gap.from; i++) {
        std::cout << _buffer[i] << std::flush;
    }
    for (int i = _gap.to; i < _length; i++) {
        std::cout << _buffer[i] << std::flush;
    }
    std::cout << std::endl;
}


void GapBuffer::setFilePath(const char *filename) {
    free(_filepath);
    _filepath = strdup(filename);
}

void GapBuffer::saveToFile(const char *filename) {
    if (FILE *file = fopen(filename, "w"); file != nullptr) {
        fwrite(_buffer, 1, _gap.from, file);
        fwrite(_buffer + _gap.to, 1, _length - _gap.to, file);
        fclose(file);
        _isSaved = true;

        std::cout << "Text has been saved successfully" << std::endl;
        setFilePath(filename);
    } else {
        std::cout << "Error opening file for writing" << std::endl;
    }
}

// // invoked when loading a file
void GapBuffer::populateLineStarts() {
    _linesCount = 0;
    for (int i = 0; i < _gap.from; i++) {
        if (_buffer[i] != '\n') {
            continue;
        }
        if (_linesCount == _lineStartsCapacity) {
            _lineStartsCapacity *= 2;
            _lineStarts = reallocArray(
                _lineStarts, _linesCount, _lineStartsCapacity);
        }
        _lineStarts[_linesCount].position = i + 1;
        _linesCount++;
    }
}

long GetFileSize(FILE *file) {
    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void GapBuffer::loadFromFile(const char *filename) {
    if (FILE *file = fopen(filename, "r"); file != nullptr) {
        const long fileSize = GetFileSize(file);
        const int newSize = fileSize + DEFAULT_WINDOW_SIZE;
        _buffer = reallocArray(_buffer, _length, newSize + 1);
        _length = newSize;

        fread(_buffer, 1, fileSize, file);
        fclose(file);
        StringUtils::TerminateString(_buffer, _length);

        _gap.from = fileSize;
        _gap.to = _length;
        _isSaved = true;

        populateLineStarts();

        std::cout << "Text has been loaded successfully" << std::endl;
        setFilePath(filename);
    } else {
        std::cout << "Error opening file for reading" << std::endl;
    }
}

void GapBuffer::insertAt(Position position, const char *text) {
    if (position.line < 0 || position.line >= _linesCount) {
        std::cout << "Invalid line number ヽ༼⊙_⊙༽ﾉ" << std::endl;
        return;
    }

    const int positionIndex = _lineStarts[position.line].position + position.index;
    moveGapTo(positionIndex);

    const int textLength = strlen(text);
    const int gapLength = getWindowLength();

    if (textLength > gapLength)
        expand(textLength - gapLength);

    memcpy(_buffer + _gap.from, text, textLength);
    _gap.from += textLength;
    _isSaved = false;

    // Update positions of subsequent lines
    for (int i = position.line + 1; i < _linesCount; i++) {
        _lineStarts[i].position += textLength;
    }

    if (strchr(text, '\n') != nullptr) {
        updateLineStarts(_gap.from, true);
    }
}

void GapBuffer::search(const char *text) const {
    const int textLength = strlen(text);
    for (int i = 0; i <= _gap.from - textLength; i++) {
        if (strncmp(_buffer + i, text, textLength) != 0) {
            continue;
        }

        const auto [line, index] = getPositionFromIndex(i);
        std::cout << "Text is present in this position (line, index): "
        << line << ", " << index << std::endl;
    }
    for (int i = _gap.to; i <= _length - textLength; i++) {
        if (strncmp(_buffer + i, text, textLength) != 0) {
            continue;
        }

        const auto [line, index] = getPositionFromIndex(i);
        std::cout << "Text is present in this position (line, index): "
        << line << ", " << index << std::endl;
    }
}

char *GapBuffer::remove(Position position, const int length) {
    if (position.line < 0 || position.line >= _linesCount) {
        std::cout << "Invalid line number ヽ༼⊙_⊙༽ﾉ" << std::endl;
        return nullptr;
    }

    const int positionIndex = _lineStarts[position.line].position + position.index;
    moveGapTo(positionIndex);

    const int deleteLength = std::min(length, _length - _gap.to);

    for (int i = _gap.to; i < _gap.to + deleteLength; i++) {
        if (_buffer[i] == '\n') {
            updateLineStarts(i, false);
        }
    }

    char * deletedText = new char[deleteLength + 1];
    strncpy(deletedText, _buffer + _gap.to, deleteLength);
    deletedText[deleteLength] = '\0';
    _gap.to += deleteLength;
    for (int i = position.line + 1; i < _linesCount; i++) {
        _lineStarts[i].position -= deleteLength;
    }

    return deletedText;
}

char *GapBuffer::cut(Position position, int length) {
    if (!isPositionValid(position)) {
        std::cout << "Invalid line number ヽ༼⊙_⊙༽ﾉ" << std::endl;
        return nullptr;
    }

    _copiedText = remove(position, length);
    return _copiedText;
}

char *GapBuffer::copy(Position position, int length) {
    if (!isPositionValid(position)) {
        std::cout << "Invalid line number ヽ༼⊙_⊙༽ﾉ" << std::endl;
        return nullptr;
    }

    const int positionIndex = getIndexFromPosition(position);
    moveGapTo(positionIndex);
    const int copiedLength = std::min(length, _length - _gap.to);

    _copiedText = StringUtils::CreateString(length);
    strncpy(_copiedText, _buffer + positionIndex, copiedLength);
    return _copiedText;
}

char *GapBuffer::paste(Position position) {
    if (!isPositionValid(position)) {
        std::cout << "Invalid line number ヽ༼⊙_⊙༽ﾉ" << std::endl;
        return nullptr;
    }

    if (_copiedText == nullptr) {
        std::cout << "No text to paste ヽ༼⊙_⊙༽ﾉ" << std::endl;
        return nullptr;
    }

    insertAt(position, _copiedText);
    return _copiedText;
}

char *GapBuffer::insertWithReplace(Position position, const char *text) {
    if (!isPositionValid(position)) {
        std::cout << "Invalid line number ヽ༼⊙_⊙༽ﾉ" << std::endl;
        return nullptr;
    }

    const int positionIndex = getIndexFromPosition(position);
    moveGapTo(positionIndex);
    const int textLength = strlen(text);
    char *replacedText = remove(position, textLength);
    insertAt(position, text);

    return replacedText;
}

GapBuffer::~GapBuffer() {
    delete[] _buffer;
    delete[] _lineStarts;
    free(_filepath);
}

bool GapBuffer::isSaved() const {
    return _isSaved;
}

char * GapBuffer::getFilePath() const {
    return _filepath;
}
