//#include "json_builder.h"
//
//namespace json{
//
//    /// задаёт строковое значение ключа для очередной пары ключ-значение
//    KeyItemContext Builder::Key(std::string key){
//
////        if(nodes_stack_.back()->IsDict() && !current_key_){
////        if(!current_key_ || nodes_stack_.back()->IsDict() || !nodes_stack_.empty()){
//        if(!nodes_stack_.empty() || nodes_stack_.back()->IsDict()) {
//            /// если находимся в контексте словаря (последний элемент стека является словарем)
//            /// Задаем текущий ключ для пары ключ-значение
//            current_key_ = std::move(key);
//        }
//        else{
//            throw std::logic_error("Key method used in invalid context");
//        }
//
//        return {*this};
//    }
//
//    /// задает соответствующее этому ключу значение
//    Builder& Builder::Value(Node::Value value) {
//
//        /// Проверяем текущий контекст
//        Node* back_node = nodes_stack_.back();
//
//        /// Если объект еще не содержит значений, просто задаем значение для корневого узла
//        if(back_node->IsNull()){
////            root_.GetValue() = std::move(value);
//            back_node->GetValue() = std::move(value);
//        }
//        else if(back_node->IsDict()){
//            if (!current_key_) {
//                /// Если ключа нет, выбрасываем исключение, так как должен быть предыдущий вызов Key.
//                throw std::logic_error("Value method called without a corresponding Key method");
//            }
//
//            /// Получаем словарь из текущего узла
//            Dict& dict = std::get<Dict>(back_node->GetValue());
//            /// Вставляем или обновляем элемент в словаре
//            auto[item, _] = dict.emplace(std::move(current_key_.value()), Node{});
//            /// Сбрасываем текущий ключ
//            current_key_ = std::nullopt;
//            /// Переходим к узлу, который соответствует заданному ключу, чтобы задать ему значение
//            back_node = &item->second;
//            back_node->GetValue() = std::move(value);
//
////            /// Вставляем или обновляем элемент в словаре
////            auto ptr = dict.emplace(std::move(current_key_.value()), Node{});
////            /// Сбрасываем текущий ключ
////            current_key_ = std::nullopt;
////            /// Переходим к узлу, который соответствует заданному ключу, чтобы задать ему значение
////            back_node = &ptr.first->second;
////            back_node->GetValue() = std::move(value);
//
//        }
//        /// Если объект представляет массив, то добавляем значение в массив
//        else if(back_node->IsArray()){
//            Array& arr = std::get<Array>(back_node->GetValue());
//            arr.emplace_back(ReadValue(value));
//            back_node = &arr.back();
//        }
//        /// значение не может быть добавлено
//        else {
//            throw std::logic_error("Value can only be added inside a dictionary or an array");
//        }
//
//        return *this;
//    }
//
////    Builder& Builder::Value(Node::Value value) {
////
////        Node* back_node = nodes_stack_.back();
////        if(back_node->IsNull()){
////            root_.GetValue() = std::move(value);
////        }
////        else if(back_node->IsDict()){
////            if (!current_key_)
////                throw std::logic_error("Could not Value() for dict without key");
////
////            auto& dict = std::get<Dict>(back_node->GetValue());
////            auto [pos, _] = dict.emplace(std::move(current_key_.value()), Node{});
////            current_key_ = std::nullopt;
////            back_node = &pos->second;
////            back_node->GetValue() = std::move(value);
////        }
////        else if (back_node->IsArray()) {
////            auto& array = std::get<Array>(back_node->GetValue());
////            array.emplace_back(ReadValue(value));
////            back_node = &array.back();
////        }
////
////        else {
////            throw std::logic_error("Value can only be added inside a dictionary or an array");
////        }
////
////        return *this;
////    }
//
//    /// Начинает определение сложного значения-словаря
//    /// Вызывается в тех же контекстах, что и Value
//    DictItemContext Builder::StartDict(){
//
//        Node *back_node = nodes_stack_.back();
//
//        if(back_node->IsNull()){
//            back_node->GetValue() = Dict();
//        }
//        else if(back_node->IsDict()){
//            if(!current_key_){
//                throw std::logic_error("StartDict method used in invalid context");
//            }
//
//            Dict& dict = std::get<Dict>(back_node->GetValue());
//            auto[item, _] = dict.emplace(std::move(current_key_.value()), Dict());
//            current_key_ = std::nullopt;
//            nodes_stack_.emplace_back(&item->second);
//        }
//        else if(back_node->IsArray()){
//            Array& arr = std::get<Array>(back_node->GetValue());
//            arr.emplace_back(Dict());
//            nodes_stack_.emplace_back(&arr.back());
//        }
//        else {
//            throw std::logic_error("StartDict method used in invalid context");
//        }
//
//        return {*this};
//    }
//
//    ///  Начинает определение сложного значения-массива.
//    /// Вызывается в тех же контекстах, что и Value
//    ArrayItemContext Builder::StartArray(){
//
//        Node *back_node = nodes_stack_.back();
//
//        if (back_node->IsNull()) {
//            back_node->GetValue() = Array();
//
//        }
//        else if (back_node->IsDict()) {
//            if (!current_key_) {
//                throw std::logic_error("StartArray method used in invalid context");
//            }
//
//            Dict& dict = std::get<Dict>(back_node->GetValue());
//            auto[item, _] = dict.emplace(std::move(current_key_.value()), Array());
//            current_key_ = std::nullopt;
//
//            nodes_stack_.emplace_back(&item->second);
//        }
//        else if (back_node->IsArray()) {
//            Array& arr = std::get<Array>(back_node->GetValue());
//            arr.emplace_back(Array());
//            nodes_stack_.emplace_back(&arr.back());
//        }
//        else {
//            throw std::logic_error("StartArray method used in invalid context");
//        }
//
//        return {*this};
//    }
//
//    /// Завершает определение сложного значения-словаря
//    Builder& Builder::EndDict(){
//
//        /// Метод может вызываться только в контексте Dict
//        if(!nodes_stack_.back()->IsDict()){
//            throw std::logic_error("EndDict method used in invalid context");
//        }
//        nodes_stack_.pop_back();
//        return *this;
//    }
//
//    /// Завершает определение сложного значения-массива
//    Builder& Builder::EndArray(){
//
//        ///  Метод может вызываться только в контексте массива Array
//        if(!nodes_stack_.back()->IsArray()){
//            throw std::logic_error("EndDict method used in invalid context");
//        }
//
//        nodes_stack_.pop_back();
//        return *this;
//    }
//
//    /// Возвращает объект json::Node, содержащий JSON,
//    /// описанный предыдущими вызовами методов
//    json::Node Builder::Build(){
//
//        if(root_.IsNull() || nodes_stack_.size() > 1){
//            throw std::logic_error("Build method called on an incomplete JSON object");
//        }
//
//        return root_;
//    }
//
//
////    Node* Builder::AddElement(Node::Value value) {
////        using namespace std::string_literals;
////        if (nodes_stack_.back()->IsNull();) {
////            nodes_stack_.back()->GetValue() = std::move(value);
////            return nodes_stack_.back();
////        }
////        else if (nodes_stack_.back()->IsArray()) {
////            Array& link_arr = std::get<Array>(nodes_stack_.back()->GetValue());
////            return &link_arr.emplace_back(value);
////        }
////        else if (nodes_stack_.back()->IsDict() && current_key_) {
////            Dict& link_dict = std::get<Dict>(nodes_stack_.back()->GetValue());
////            auto ptr = link_dict.emplace(make_pair(current_key_.value(), value));
////            current_key_ = std::nullopt;
////            return &ptr.first->second;
////        }
////        else {
////            throw std::logic_error("The command \"Value\" was called in wrong place"s);
////        }
////    }
//
//
//    json::Node Builder::ReadValue(Node::Value& value) {
//        if (std::holds_alternative<bool>(value)) {
//            return Node{std::get<bool>(value)};
//        }
//        else if (std::holds_alternative<int>(value)) {
//            return Node{std::get<int>(value)};
//        }
//        else if (std::holds_alternative<double>(value)) {
//            return Node{std::get<double>(value)};
//        }
//        else if (std::holds_alternative<std::string>(value)) {
//            return Node{std::get<std::string>(value)};
//        }
//        else if (std::holds_alternative<Array>(value)) {
//            return Node{std::get<Array>(value)};
//        }
//        else if (std::holds_alternative<Dict>(value)) {
//            return Node{std::get<Dict>(value)};
//        }
//        else {
//            return Node{std::get<std::nullptr_t>(value)};
//        }
//    }
//
//
//    /// BaseContext
//
//    KeyItemContext BaseContext::Key(std::string key) {
//        return builder_.Key(std::move(key));
//    }
//
//    DictItemContext BaseContext::StartDict() {
//        return DictItemContext(builder_.StartDict());
//    }
//
//    ArrayItemContext BaseContext::StartArray() {
//        return ArrayItemContext(builder_.StartArray());
//    }
//
//    Builder& BaseContext::EndDict() {
//        return builder_.EndDict();
//    }
//
//    Builder& BaseContext::EndArray() {
//        return builder_.EndArray();
//    }
//
//    Builder& BaseContext::Value(Node::Value value) {
//        return builder_.Value(std::move(value));
//    }
//
//    json::Node BaseContext::Build(){
//        return builder_.Build();
//    }
//
//
//    /// ArrayItemContext
//
//    ArrayItemContext ArrayItemContext::Value(Node::Value value) {
////        auto result = BaseContext::Value(std::move(value));
////        return ArrayItemContext(result);
////        return BaseContext::Value(std::move(value));
//        return ArrayItemContext( BaseContext::Value(value));
//    }
//
//    /// KeyItemContext
//    DictItemContext KeyItemContext::Value(Node::Value value) {
////        return BaseContext::Value(std::move(value));
////        auto result = BaseContext::Value(std::move(value));
////        return DictItemContext(result);
//        return {BaseContext::Value(std::move(value))};
//    }
//
//
//} // namespace json
//


#include "json_builder.h"

namespace json{

    /// задаёт строковое значение ключа для очередной пары ключ-значение
    Builder& Builder::Key(std::string key){
        auto* back_node = nodes_stack_.back();
        if(!back_node->IsDict() && nodes_stack_.empty()){
            throw std::logic_error("Key method used in invalid context");

        }

        Dict& dict = get<Dict>(back_node->GetValue());
        nodes_stack_.emplace_back(&dict[std::move(key)]);
//        nodes_stack_.emplace_back(&const_cast<Dict&>(back_node->AsDict())[key]);
        return *this;

//        if(back_node->IsDict() && !current_key_){
//            /// если находимся в контексте словаря (последний элемент стека является словарем)
//            /// Задаем текущий ключ для пары ключ-значение
//            current_key_ = std::move(key);
//        }
//        else{
//            throw std::logic_error("Key method used in invalid context");
//        }
//        return *this;
    }

    /// задает соответствующее этому ключу значение
    Builder& Builder::Value(Node::Value value) {
        /// Проверяем текущий контекст
        auto* back_node = nodes_stack_.back();

        /// Если объект еще не содержит значений, просто задаем значение для корневого узла
        if(back_node->IsNull()){
            root_.GetValue() = std::move(value);
        }
        else if(back_node->IsDict()){
//            if (!current_key_) {
//                /// Если ключа нет, выбрасываем исключение, так как должен быть предыдущий вызов Key.
//                throw std::logic_error("Value method called without a corresponding Key method");
//            }
//            /// Получаем словарь из текущего узла
//            auto& dict = std::get<Dict>(back_node->GetValue());
//            /// Вставляем или обновляем элемент в словаре
//            auto[item, _] = dict.emplace(std::move(current_key_.value()), Node{});
//            /// Сбрасываем текущий ключ
//            current_key_ = std::nullopt;
//            /// Переходим к узлу, который соответствует заданному ключу, чтобы задать ему значение
//            back_node = &item->second;
//            back_node->GetValue() = std::move(value);
        }
            /// Если объект представляет массив, то добавляем значение в массив
        else if(back_node->IsArray()){
            auto& array = std::get<Array>(nodes_stack_.back()->GetValue());
            array.emplace_back(ReadValue(value));
            back_node = &array.back();
        }
            /// значение не может быть добавлено
        else {
            throw std::logic_error("Value can only be added inside a dictionary or an array");
        }
        return *this;
    }

    /// Начинает определение сложного значения-словаря
    /// Вызывается в тех же контекстах, что и Value
    Builder& Builder::StartDict(){
        auto* back_node = nodes_stack_.back();
        if(back_node->IsNull()){
            back_node->GetValue() = Dict();
        }
        else if(nodes_stack_.back()->IsDict()){
            auto& dict = std::get<Dict>(back_node->GetValue());
            dict[std::to_string(dict.size())] = Dict(); // Добавляем новую пустую пару ключ-значение в словарь
            nodes_stack_.emplace_back(&dict[std::to_string(dict.size())]);

//            if(!current_key_){
//                throw std::logic_error("StartDict method used in invalid context");
//            }
//            auto& dict = std::get<Dict>(back_node->GetValue());
//            auto[item, _] = dict.emplace(std::move(current_key_.value()), Dict());
//            current_key_ = std::nullopt;
//            nodes_stack_.emplace_back(&item->second);
        }
        else if(back_node->IsArray()){
            auto& arr = std::get<Array>(back_node->GetValue());
            arr.emplace_back(Dict());
            nodes_stack_.emplace_back(&arr.back());
        }
        else {
            throw std::logic_error("StartDict method used in invalid context");
        }
        return *this;
    }


    ///  Начинает определение сложного значения-массива.
    /// Вызывается в тех же контекстах, что и Value
    Builder& Builder::StartArray(){
        auto* back_node = nodes_stack_.back();
        if (back_node->IsNull()) {
            back_node->GetValue() = Array();
        }
        else if (back_node->IsDict()) {
//            if (!current_key_) {
//                throw std::logic_error("StartArray method used in invalid context");
//            }
//            auto& dict = std::get<Dict>(back_node->GetValue());
//            auto[item, _] = dict.emplace(std::move(current_key_.value()), Array());
//            current_key_ = std::nullopt;
//            nodes_stack_.emplace_back(&item->second);
        }
        else if (back_node->IsArray()) {
            auto& arr = std::get<Array>(back_node->GetValue());
            arr.emplace_back(Array());
            nodes_stack_.emplace_back(&arr.back());
        }
        else {
            throw std::logic_error("StartArray method used in invalid context");
        }
        return *this;
    }

    /// Завершает определение сложного значения-словаря
    Builder& Builder::EndDict(){
        auto* back_node = nodes_stack_.back();
        /// Метод может вызываться только в контексте Dict
        if(!back_node->IsDict()){
            throw std::logic_error("EndDict method used in invalid context");
        }
        nodes_stack_.pop_back();
        return *this;
    }

    /// Завершает определение сложного значения-массива
    Builder& Builder::EndArray(){
        auto* back_node = nodes_stack_.back();
        ///  Метод может вызываться только в контексте массива Array
        if(!back_node->IsArray()){
            throw std::logic_error("EndDict method used in invalid context");
        }
        nodes_stack_.pop_back();
        return *this;
    }

    /// Возвращает объект json::Node, содержащий JSON,
    /// описанный предыдущими вызовами методов
    json::Node Builder::Build(){
        if(root_.IsNull() || nodes_stack_.size() > 1){
            throw std::logic_error("Build method called on an incomplete JSON object");
        }
        return root_;
    }



    Node Builder::ReadValue(Node::Value& value) {
        if (std::holds_alternative<bool>(value)) {
            return Node{std::get<bool>(value)};
        }
        else if (std::holds_alternative<int>(value)) {
            return Node{std::get<int>(value)};
        }
        else if (std::holds_alternative<double>(value)) {
            return Node{std::get<double>(value)};
        }
        else if (std::holds_alternative<std::string>(value)) {
            return Node{std::get<std::string>(value)};
        }
        else if (std::holds_alternative<Array>(value)) {
            return Node{std::get<Array>(value)};
        }
        else if (std::holds_alternative<Dict>(value)) {
            return Node{std::get<Dict>(value)};
        }
        else {
            return Node{std::get<std::nullptr_t>(value)};
        }
    }


    Node* Builder::Insert(Node node){



    }


} // namespace json
