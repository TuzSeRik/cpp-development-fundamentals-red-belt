#include <string>
#include <list>

#include "test_runner.h"

using namespace std;

class Editor {
public:
    // This task is simple. Only hard thing is cut operation
    // In other command use simple list methods
    Editor() = default;
    // Checking if cursor os on the border of the list
    void Left() {
        if (cursor == begin(text))
            return;
        cursor--;
    }

    void Right() {
        if (cursor == end(text))
            return;
        cursor++;
    }

    void Insert(char token) {
        text.insert(cursor, token);
    }

    void Cut(size_t tokens = 1) {
        Copy(tokens);
        // List iterators have no compare operator, so we'll iterate until cursor in the end of
        // needed range or list
        auto endIt = cursor;
        for (int i = tokens; i > 0; --i)
            if (++endIt == end(text))
                break;
        // To prevent invalidation, assigning new iterator to cursor
        cursor = text.erase(cursor, endIt);
    }

    void Copy(size_t tokens = 1) {
        buffer = {cursor, next(cursor, tokens)};
    }

    void Paste() {
        text.insert(cursor, begin(buffer), end(buffer));
    }
    // Constructing string from list iterators
    [[nodiscard]] string GetText() const {
        return {begin(text), end(text)};
    }
// Here we'll use efficient linear container list
private:
    list<char> text;
    list<char>::iterator cursor = text.begin();
    list<char> buffer;
};
// Tests, provided by authors
void TypeText(Editor& editor, const string& text) {
    for(char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for(size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for(size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello")
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint")
    }
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for(char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse")
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "")

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "")
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example")
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    return 0;
}
