struct SidePrinter : stringstream {
    int cursor;
    int width;
    vector<stringstream> streams;
    SidePrinter(): cursor(0), width(0) {
        streams.push_back(stringstream());
    }
    ~SidePrinter() {
        flush();
    }

    void endLine() {
        cursor++;
        while (cursor >= streams.size()) {
            streams.push_back(stringstream());
            streams.back() << string(width, ' ');
        }
    }

    void flush() {
        for (auto &ss : streams) {
            cout << ss.str() << endl;
        }
        streams.clear();
        width = 0;
        cursor = -1;
        endLine();
    }

    void reset() {
        cursor = 0;
        for (auto &ss : streams) {
            width = max(width, int(ss.str().size()));
        }
        for (auto &&ss : streams) {
            int rem = width - int(ss.str().size());
            ss << string(rem, ' ');
        }
    }

    SidePrinter& operator<<(bool s) { streams[cursor] << s; return *this; }
    SidePrinter& operator<<(int s) { streams[cursor] << s; return *this; }
    SidePrinter& operator<<(long long s) { streams[cursor] << s; return *this; }
    SidePrinter& operator<<(double s) { streams[cursor] << s; return *this; }
    SidePrinter& operator<<(char s) {
        if (s == '\n') endLine();
        else streams[cursor] << s;
        return *this;
    }
    SidePrinter& operator<<(const char* s) {
        while (*s) {
            *this << *s;
            s++;
        }
        return *this;
    }
    SidePrinter& operator<<(const string& s) {
        for (char c : s) {
            *this << c;
        }
        return *this;
    }
};
