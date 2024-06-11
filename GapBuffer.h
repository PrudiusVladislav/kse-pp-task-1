#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H
#include <tuple>


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
    void insertAt(Position position, const char *text);
    void search(const char *text) const;
    char *remove(Position position, int length);
    char *cut(Position position, int length);
    char *copy(Position position, int length);
    char *paste(Position position);
    char *insertWithReplace(Position position, const char *text);
    ~GapBuffer();

    [[nodiscard]] bool isSaved() const;
    [[nodiscard]] char *getFilePath() const;
    [[nodiscard]] int getContentLength() const;
    [[nodiscard]] Position getPositionFromIndex(int index) const;

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

    int getWindowLength() const;
    int getIndexFromPosition(Position position) const;
    bool isPositionValid(Position position) const;
    void expand(int additionalLength);
    void updateLineStarts(int position, bool isInsert);
    void moveGapTo(int position);
    void setFilePath(const char *filename);
    void populateLineStarts();
};

#endif //GAP_BUFFER_H


