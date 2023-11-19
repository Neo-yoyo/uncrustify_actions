//#include "json.h"
//
//#include <optional>
//
//namespace json{
//
//    class DictItemContext;
//    class ArrayItemContext;
//    class KeyItemContext;
//    class BaseContext;
//
//class Builder{
//public:
//    /// При создании объекта класса инициализируем стек указателей на Node
//    /// и добавляем корневой узел в стек. Это готовый для построения JSON-объект.
////    Builder(){
////        Node* ptr = &root_;
////        nodes_stack_.emplace_back(ptr);
////    }
//    Builder()
//            : root_(nullptr)
//            , nodes_stack_{ &root_ }
//            , current_key_(std::nullopt) {}
//
//    KeyItemContext Key(std::string);
//    DictItemContext StartDict();
//    ArrayItemContext StartArray();
//
//    Builder& Value(Node::Value);
//    Builder& EndDict();
//    Builder& EndArray();
//
//    json::Node Build();
//
//    json::Node ReadValue(Node::Value& value);
////    Node* AddElement(Node::Value value);
//
//private:
//    /// сам конструируемый объект
//    Node root_{nullptr};
//    /// стек указателей на те вершины JSON, которые ещё не построены
//    std::vector<Node*> nodes_stack_;
//
//    /// Текущий ключ для определения пары ключ-значение в словаре
////    std::string current_key_;
//    std::optional<std::string> current_key_ {std::nullopt};
//
//}; // class Builder
//
//class BaseContext {
//public:
//    BaseContext(Builder& builder)
//        : builder_(builder) {}
//
//    KeyItemContext Key(std::string key);
//    DictItemContext StartDict();
//    ArrayItemContext StartArray();
//
//    Builder& EndDict();
//    Builder& EndArray();
//    Builder& Value(Node::Value value);
//
//    json::Node Build();
//
//private:
//    Builder &builder_;
//}; // class BaseContext
//
//
//
//class DictItemContext : public BaseContext {
//public:
//    DictItemContext(BaseContext base) :
//        BaseContext(base) {}
//
////    DictItemContext Key(std::string key);
////    Builder& EndDict();
//
//    /// Правило 3
//    DictItemContext StartDict() = delete;
//    ArrayItemContext StartArray() = delete;
//    Builder& Value(Node::Value value) = delete;
//    Builder& EndArray() = delete;
//    json::Node Build() = delete;
//};
//
//
//
//class ArrayItemContext : public BaseContext {
//
//public:
//    ArrayItemContext(BaseContext base) :
//        BaseContext(base) {}
//
////    DictItemContext StartDict();
////    ArrayItemContext StartArray();
////    Builder& EndArray();
////    Builder& Value(Node::Value value);
//
//    /// Правило 4
//    KeyItemContext Key(std::string key) = delete;
//    Builder& EndDict() = delete;
//    json::Node Build() = delete;
//
//    /// правило 5
//    ArrayItemContext Value(Node::Value value);
////    ArrayItemContext Value(Node node){
////        return {BaseContext::Value(std::move(node.GetValue()))};
////    }
//};
//
//
//class KeyItemContext : public BaseContext {
//
//public:
//    KeyItemContext(BaseContext base) :
//        BaseContext(base) {}
//
////    DictItemContext StartDict();
////    ArrayItemContext StartArray();
////    Builder& Value(Node::Value value);
//
//    /// Правило 1
//    Builder& EndDict() = delete;
//    Builder& EndArray() = delete;
//    json::Node Build() = delete;
//
//    KeyItemContext Key(std::string key) = delete;
//
//    /// правило 2
//    DictItemContext Value(Node::Value value);
//};
//
//
//} // namespace json



#include "json.h"
#include <optional>

namespace json{
    class Builder{
    public:
        /// При создании объекта класса инициализируем стек указателей на Node
        /// и добавляем корневой узел в стек. Это готовый для построения JSON-объект.
        Builder(){
            Node* ptr = &root_;
            nodes_stack_.emplace_back(ptr);
        }
        Builder& Key(std::string);
        Builder& Value(Node::Value);
        Builder& StartDict();
        Builder& StartArray();
        Builder& EndDict();
        Builder& EndArray();
        json::Node Build();

    private:
        /// сам конструируемый объект
        Node root_{nullptr};
        /// стек указателей на те вершины JSON, которые ещё не построены
        std::vector<Node*> nodes_stack_;

        Node ReadValue(Node::Value& value);
        Node* Insert(Node node);
        /// Текущий ключ для определения пары ключ-значение в словаре
//    std::string current_key_;
//        std::optional<std::string> current_key_ {std::nullopt};

    };

} // namespace json