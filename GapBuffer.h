#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H


struct GapWindow {
    int from;
    int to;
};

struct LineStart {
    int position;
};

struct Position {
    int line;
    int index;
};

class GapBuffer {
public:
    GapBuffer();
    void append(const char *text);
    void print() const;
    void saveToFile(const char *filename);
    void loadFromFile(const char *filename);
    void insertAt(const char *text);
    void search(const char *text) const;
    char *remove(int length);
    char *cut(int length);
    char *copy(int length);
    char *paste();
    char *insertWithReplace(const char *text);
    void moveGapTo(int position);
    ~GapBuffer();

    [[nodiscard]] bool isSaved() const;
    [[nodiscard]] char *getFilePath() const;
    [[nodiscard]] int getContentLength() const;
    [[nodiscard]] Position getPositionFromIndex(int index) const;
    [[nodiscard]] int getIndexFromPosition(Position position) const;
    [[nodiscard]] Position getCursorPosition() const;

private:
    int _length;
    GapWindow _gap;
    char *_buffer;
    char *_filepath;
    bool _isSaved;
    LineStart *_lineStarts;
    int _linesCount;
    int _lineStartsCapacity;
    char *_copiedText;
    Position _cursorPosition;

    int getWindowLength() const;
    bool isPositionValid(Position position) const;
    void expand(int additionalLength);
    void updateLineStarts(int position, bool isInsert);
    void setFilePath(const char *filename);
    void populateLineStarts();
};

#endif //GAP_BUFFER_H


