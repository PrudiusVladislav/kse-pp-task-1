#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

struct GapWindow {
    int from;
    int to;
};

struct LineStart {
    int position;
};

class GapBuffer {
public:
    GapBuffer();
    void append(const char *text);
    void print() const;
    void saveToFile(const char *filename);
    void loadFromFile(const char *filename);
    void insertAt(int line, int index, const char *text);
    void search(const char *text) const;
    void clear();
    ~GapBuffer();

    [[nodiscard]] bool isSaved() const;
    [[nodiscard]] char *getFilePath() const;

private:
    int _length;
    GapWindow _gap;
    char *_buffer;
    char *_filepath;
    bool _isSaved;
    LineStart *_lineStarts;
    int _linesCount;
    int _lineStartsCapacity;

    [[nodiscard]] int getWindowLength() const;
    [[nodiscard]] int getContentLength() const;
    void expand(int additionalLength);
    void updateLineStarts(int position, bool isInsert);
    void moveGapTo(int position);
    void setFilePath(const char *filename);
    void populateLineStarts();
};

#endif //GAP_BUFFER_H


