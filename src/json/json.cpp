#include "json.h"

using namespace std;

namespace json {

namespace {

using Number = std::variant< int, double >;

Node LoadNode(istream& input);

Node LoadNumber(std::istream& input) {
    using namespace std::literals;

    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast< char >(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    } else {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                return Node{std::stoi(parsed_num)};
            } catch (...) {
                // В случае неудачи, например, при переполнении,
                // код ниже попробует преобразовать строку в double
            }
        }
        return Node{std::stod(parsed_num)};
    } catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

// Считывает содержимое строкового литерала JSON-документа
// Функцию следует использовать после считывания открывающего символа ":
std::string LoadString(std::istream& input) {
    using namespace std::literals;

    auto it = std::istreambuf_iterator< char >(input);
    auto end = std::istreambuf_iterator< char >();
    std::string s;
    while (true) {
        if (it == end) {
            // Поток закончился до того, как встретили закрывающую кавычку?
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            // Встретили закрывающую кавычку
            ++it;
            break;
        } else if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
                case 'n':
                    s.push_back('\n');
                    break;
                case 't':
                    s.push_back('\t');
                    break;
                case 'r':
                    s.push_back('\r');
                    break;
                case '"':
                    s.push_back('"');
                    break;
                case '\\':
                    s.push_back('\\');
                    break;
                default:
                    // Встретили неизвестную escape-последовательность
                    throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        } else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        } else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }
    return s;
}

Node LoadArray(istream& input) {
    Array result;

    if (input.peek() == -1) {
        throw ParsingError("Failed to parse array");
    }

    for (char c; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(std::move(result));
}

Node LoadDict(istream& input) {
    Dict result;

    if (input.peek() == -1) {
        throw ParsingError("Failed to parse dictionary.");
    }

    for (char c; input >> c && c != '}';) {
        if (c == ',') {
            input >> c;
        }

        string key = LoadString(input);
        input >> c;
        result.insert({ std::move(key), LoadNode(input) });
    }

    return Node(std::move(result));
}

Node LoadBool(istream& input) {
    std::string bool_str;

    while (std::isalpha(input.peek())) {
        bool_str.push_back(static_cast< char >(input.get()));
    }

    if (bool_str == "true"sv) {
        return Node{ true };
    } else if (bool_str == "false"sv) {
        return Node{ false };
    } else {
        throw ParsingError("Invalid bool value"s);
    }
}

Node LoadNull(std::istream& input) {
    std::string null_str;

    while (std::isalpha(input.peek())) {
        null_str.push_back(static_cast< char >(input.get()));
    }

    if (null_str != "null") {
        throw ParsingError("Invalid null.");
    }
    return Node(nullptr);
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

//            if (c == 'n') {
//                input.putback(c);
//                return LoadNull(input);
//            }
//            else if (c == '"') {
//                return LoadString(input);
//            }
//            else if (c == 't' || c == 'f') {
//                input.putback(c);
//                return LoadBool(input);
//            }
//            else if (c == '[') {
//                return LoadArray(input);
//            }
//            else if (c == '{') {
//                return LoadDict(input);
//            }
    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '\"') {
        return LoadString(input);
    } else if (c == 't' || c == 'f') {
        input.putback(c);
        return LoadBool(input);
    }
    if (c == 'n') {
        input.putback(c);
        return LoadNull(input);
    } else {
        input.putback(c);
        return LoadNumber(input);
    }
}


}  // namespace


/// ---------- Node -------------

Node::Node(std::nullptr_t) :
        value_(nullptr) {
}

Node::Node(Array array) :
        value_(std::move(array)) {
}

Node::Node(Dict map) :
        value_(std::move(map)) {
}

Node::Node(bool value) :
        value_(value) {
}

Node::Node(int value) :
        value_(value) {
}

Node::Node(double value) :
        value_(value) {
}

Node::Node(std::string value) :
        value_(std::move(value)) {
}



/// сообщают, хранится ли внутри значение некоторого типа
bool Node::IsInt() const {
    return std::holds_alternative< int >(value_);
}

/// Возвращает true, если в Node хранится int либо double
bool Node::IsDouble() const {
    return std::holds_alternative< int >(value_) ||
           std::holds_alternative< double >(value_);
}

/// Возвращает true, если в Node хранится double
bool Node::IsPureDouble() const {
    return std::holds_alternative< double >(value_);
}

bool Node::IsBool() const {
    return std::holds_alternative< bool >(value_);
}

bool Node::IsString() const {
    return std::holds_alternative< std::string >(value_);
}

bool Node::IsNull() const {
    return std::holds_alternative< std::nullptr_t >(value_);
}

bool Node::IsArray() const {
    return std::holds_alternative< Array >(value_);
}

bool Node::IsMap() const {
    return std::holds_alternative< Dict >(value_);
}

int Node::AsInt() const {
    if (!IsInt()) {
        throw std::logic_error("Cannot convert Node to int");
    }
    return std::get< int >(value_);
}

bool Node::AsBool() const {
    if (!IsBool()) {
        throw std::logic_error("Cannot convert Node to bool");
    }
    return std::get< bool >(value_);
}

double Node::AsDouble() const {
    if (IsInt()) {
        return static_cast< double >(std::get< int >(value_));
    }
    if (IsPureDouble()) {
        return std::get< double >(value_);
    }
    throw std::logic_error("Cannot convert Node to double");
}

const std::string& Node::AsString() const {
    if (!IsString()) {
        throw std::logic_error("Cannot convert Node to string");
    }
    return std::get< std::string >(value_);
}

const Array& Node::AsArray() const {
    if (!IsArray()) {
        throw std::logic_error("Cannot convert Node to Array");
    }
    return std::get< Array >(value_);
}

const Dict& Node::AsMap() const {
    if (!IsMap()) {
        throw std::logic_error("Cannot convert Node to Dict");
    }
    return std::get< Dict >(value_);
}

bool Node::operator== (const Node& rhs) const {
    return value_ == rhs.value_;
}

bool Node::operator!= (const Node& rhs) const {
    return value_ != rhs.value_;
}


/// ---------- Document -------------
Document::Document(Node root)
        : root_(move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

bool Document::operator== (const Document& rhs) const {
    return root_ == rhs.root_;
}

bool Document::operator!= (const Document& rhs) const {
    return root_ != rhs.root_;
}


Document Load(istream& input) {
    return Document{LoadNode(input)};
}

void Print(const Document& doc, std::ostream& output) {
//    const Node& root = doc.GetRoot();
    std::visit(PrintValueVisitor{output}, doc.GetRoot().GetValue());
}


void PrintValueVisitor::operator() (std::nullptr_t) {
    out << "null"sv;
}

void PrintValueVisitor::operator() (const std::string value) {
    out << "\""sv;
    for (const char& c : value) {
        if (c == '\n') {
            out << "\\n"sv;
            continue;
        }
        if (c == '\r') {
            out << "\\r"sv;
            continue;
        }
        if (c == '\"') {
            out << "\\"sv;
        }
        if (c == '\t') {
            out << "\t"sv;
            continue;
        }
        if (c == '\\') {
            out << "\\"sv;
        }

        out << c;
    }
    out << "\""sv;
}

void PrintValueVisitor::operator() (int value) {
    out << value;
}

void PrintValueVisitor::operator() (double value) {
    out << value;
}

void PrintValueVisitor::operator() (bool value) {
    out << std::boolalpha << value;
}

void PrintValueVisitor::operator() (const Array array) {
    out << "["sv;
    for (size_t i = 0; i < array.size(); ++i) {
        std::visit(PrintValueVisitor{out}, array[i].GetValue());
        if (i != array.size() - 1) {
            out << ", ";
        }
    }
    out << "]"sv;
}

void PrintValueVisitor::operator() (const Dict dict) {
    out << "{ "sv;
    size_t count = 0;
    for (const auto& [key, value] : dict) {
        out << "\"" << key << "\": ";
        std::visit(PrintValueVisitor{out}, value.GetValue());

        if (count != dict.size() - 1) {
            out << ", ";
        }
        count++;
    }
    out << " }"sv;
}



}  // namespace json