#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <stdexcept>
 
using namespace std;
 
template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
 
   template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) {
            node_ = node;
        }
 
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
 
        BasicIterator() = default;
 
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }
 
        BasicIterator& operator=(const BasicIterator& rhs) = default;
 
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (node_ == rhs.node_ || (node_ == nullptr && rhs.node_ == nullptr));
        }
       
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }
      
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (node_ == rhs.node_ || (node_ == nullptr && rhs.node_ == nullptr));
        }
 
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }
 
        BasicIterator& operator++() {
           if(node_){ 
			node_ = node_->next_node;
            return *this;
            }else{
            throw std::logic_error("log"s);
            }
        }
 
        BasicIterator operator++(int)  {
			if(node_){            
				auto i = *this;
            	++(*this);
            	return i;
			}else{
            throw std::logic_error("log"s);
            }
        }
 
        [[nodiscard]] reference operator*() const  {
			if(node_){            
			return node_->value;
            }else{
			throw std::logic_error("log"s);
            }
        }
 
        [[nodiscard]] pointer operator->() const noexcept {
           return &node_->value;
        }
 
    private:
        Node* node_ = nullptr;
    };
 
public:
    SingleLinkedList<Type>  () {}
 
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
 
    [[nodiscard]] bool IsEmpty() const noexcept {
        bool is_empty = false;
        if (size_ == 0){
            is_empty = true;
        }
        return is_empty;
    }
 
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
 
    void Clear() noexcept {
        while (head_.next_node != nullptr){
                auto next = head_.next_node->next_node;
                delete head_.next_node;
                head_.next_node = next;
        }
        size_ = 0;
    }
 
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
 
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
 
    [[nodiscard]] Iterator begin() noexcept {
        if(IsEmpty()){
            return end();
        }else{
		    return Iterator(head_.next_node);
        }
    }
 
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }
 
    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }
 
    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }
 
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        if(IsEmpty()){
            return cend();
        }else{
            return ConstIterator(head_.next_node);
        }
    }
 
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        Node* before_begin = &head_;
        return Iterator(before_begin);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }
    
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        auto insert_obj = new Node(value, nullptr);
        insert_obj->next_node = pos.node_->next_node;
        pos.node_->next_node = insert_obj;
        ++size_;
        return Iterator(insert_obj);
    }

    void PopFront() noexcept {
        if (IsEmpty()) return;
        Node* it =  head_.next_node;
        head_.next_node = it -> next_node;
        delete it;
        size_ -= 1;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (IsEmpty()) return {};
        auto it_ = pos.node_ -> next_node;
        pos.node_->next_node = it_->next_node;
        delete it_;
        size_ -= 1;
        return Iterator(pos.node_->next_node);
    }
   
 
    SingleLinkedList(std::initializer_list<Type> values) {
        if(values.size() != 0){
            size_ = values.size();
            Node* node = new Node(*values.begin(), nullptr);
            head_.next_node = node;
            for (auto it = next(values.begin(),1) ; it != values.end(); ++it){
                  node->next_node = new Node(*it, nullptr);
                  node = node->next_node;
            }
        }
    }
 
    SingleLinkedList(const SingleLinkedList& other) {
        SingleLinkedList tmp;
        if (!other.IsEmpty()){
            tmp.size_ = other.size_;
            Node* node = new Node(*other.begin(), nullptr);
            tmp.head_.next_node = node;
            try{
                for (auto it = next(other.begin(), 1); it != other.end(); ++it){
                    node->next_node = new Node(*it, nullptr);
                    node = node->next_node;
                }
                node = nullptr;
                swap(tmp);
            }catch (const std::bad_alloc&){
                Clear();
                throw;
            }
        }
    }
 
 
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(this != &rhs){
            SingleLinkedList tmp (rhs);
            swap(tmp);
        }
        return *this;
    }
 
    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }
 
    ~ SingleLinkedList(){
       Clear();
    }
 
private:
    Node head_;
    size_t size_ = 0;
};
 
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}
 
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() == rhs.GetSize() && equal(lhs.begin(),lhs.end(),rhs.cbegin(),rhs.end())){
    return true;
    }
    return false;
}
 
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs == rhs){
    return false;
    }
    return true;
}
 
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
   if (lexicographical_compare(lhs.cbegin(),lhs.cend(),rhs.cbegin(),rhs.cend())){
    return true;
    }
    return false;
}
 
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (rhs < lhs){
    return false;
    }
    return true;
}
 
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (rhs < lhs){
    return true;
    }
    return false;
}
 
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (rhs > lhs){
    return false;
    }
    return true;
} 
 


// Эта функция тестирует работу SingleLinkedList
void Test1() {
    // Шпион, следящий за своим удалением
    struct DeletionSpy {
        DeletionSpy() = default;
        explicit DeletionSpy(int& instance_counter) noexcept
            : instance_counter_ptr_(&instance_counter)  //
        {
            OnAddInstance();
        }
        DeletionSpy(const DeletionSpy& other) noexcept
            : instance_counter_ptr_(other.instance_counter_ptr_)  //
        {
            OnAddInstance();
        }
        DeletionSpy& operator=(const DeletionSpy& rhs) noexcept {
            if (this != &rhs) {
                auto rhs_copy(rhs);
                std::swap(instance_counter_ptr_, rhs_copy.instance_counter_ptr_);
            }
            return *this;
        }
        ~DeletionSpy() {
            OnDeleteInstance();
        }
 
    private:
        void OnAddInstance() noexcept {
            if (instance_counter_ptr_) {
                ++(*instance_counter_ptr_);
            }
        }
        void OnDeleteInstance() noexcept {
            if (instance_counter_ptr_) {
                assert(*instance_counter_ptr_ != 0);
                --(*instance_counter_ptr_);
            }
        }
 
        int* instance_counter_ptr_ = nullptr;
    };
 
    // Проверка вставки в начало
    {
        SingleLinkedList<int> l;
        assert(l.IsEmpty());
        assert(l.GetSize() == 0u);
 
        l.PushFront(0);
        l.PushFront(1);
        assert(l.GetSize() == 2);
        assert(!l.IsEmpty());
 
        l.Clear();
        assert(l.GetSize() == 0);
        assert(l.IsEmpty());
    }
 
    // Проверка фактического удаления элементов
    {
        int item0_counter = 0;
        int item1_counter = 0;
        int item2_counter = 0;
        {
            SingleLinkedList<DeletionSpy> list;
            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});
 
            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
            list.Clear();
            assert(item0_counter == 0);
            assert(item1_counter == 0);
            assert(item2_counter == 0);
 
            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});
            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
        }
        assert(item0_counter == 0);
        assert(item1_counter == 0);
        assert(item2_counter == 0);
    }
 
    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };
 
    {
        bool exception_was_thrown = false;
        // Последовательно уменьшаем счётчик копирований до нуля, пока не будет выброшено исключение
        for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
            // Создаём непустой список
            SingleLinkedList<ThrowOnCopy> list;
            list.PushFront(ThrowOnCopy{});
            try {
                int copy_counter = max_copy_counter;
                list.PushFront(ThrowOnCopy(copy_counter));
                // Если метод не выбросил исключение, список должен перейти в новое состояние
                assert(list.GetSize() == 2);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                // После выбрасывания исключения состояние списка должно остаться прежним
                assert(list.GetSize() == 1);
                break;
            }
        }
        assert(exception_was_thrown);
    }
    std::cout << "Tests 1 are OK"s << std::endl;
}
 
// Эта функция тестирует работу SingleLinkedList
void Test2() {
    // Итерирование по пустому списку
    {
        SingleLinkedList<int> list;
        // Константная ссылка для доступа к константным версиям begin()/end()
        const auto& const_list = list;
 
        // Итераторы begin и end у пустого диапазона равны друг другу
        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
    }
 
    // Итерирование по непустому списку
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;
 
        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());
 
        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());
 
        assert(const_list.begin() == const_list.cbegin());
 
        assert(*list.cbegin() == 1);
        *list.begin() = -1;
        assert(*list.cbegin() == -1);
 
        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);
 
        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);
        // Проверка прединкремента
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // Проверка постинкремента
        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }
        // Итератор, указывающий на позицию после последнего элемента, равен итератору end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }
    }
    // Преобразование итераторов
    {
        SingleLinkedList<int> list;
        list.PushFront(1);
        // Конструирование ConstIterator из Iterator
        SingleLinkedList<int>::ConstIterator const_it(list.begin());
        assert(const_it == list.cbegin());
        assert(*const_it == *list.cbegin());
 
        SingleLinkedList<int>::ConstIterator const_it1;
        // Присваивание ConstIterator'у значения Iterator
        const_it1 = list.begin();
        assert(const_it1 == const_it);
    }
    // Проверка оператора ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;
 
        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }
    cout << "Test 2 is ok"s << endl;
}
 
// Эта функция проверяет работу класса SingleLinkedList
void Test3() {
    // Проверка списков на равенство и неравенство
    {
        SingleLinkedList<int> list_1;
        list_1.PushFront(1);
        list_1.PushFront(2);
 
        SingleLinkedList<int> list_2;
        list_2.PushFront(1);
        list_2.PushFront(2);
        list_2.PushFront(3);
 
        SingleLinkedList<int> list_1_copy;
        list_1_copy.PushFront(1);
        list_1_copy.PushFront(2);
 
        SingleLinkedList<int> empty_list;
        SingleLinkedList<int> another_empty_list;
 
        // Список равен самому себе
        assert(list_1 == list_1);
        assert(empty_list == empty_list);
 
        // Списки с одинаковым содержимым равны, а с разным - не равны
        assert(list_1 == list_1_copy);
        assert(list_1 != list_2);
        assert(list_2 != list_1);
        assert(empty_list == another_empty_list);
    }
 
    // Обмен содержимого списков
    {
        SingleLinkedList<int> first;
        first.PushFront(1);
        first.PushFront(2);
 
        SingleLinkedList<int> second;
        second.PushFront(10);
        second.PushFront(11);
        second.PushFront(15);
 
        const auto old_first_begin = first.begin();
        const auto old_second_begin = second.begin();
        const auto old_first_size = first.GetSize();
        const auto old_second_size = second.GetSize();
 
        first.swap(second);
 
        assert(second.begin() == old_first_begin);
        assert(first.begin() == old_second_begin);
        assert(second.GetSize() == old_first_size);
        assert(first.GetSize() == old_second_size);
 
        // Обмен при помощи функции swap
        {
            using std::swap;
 
            // В отсутствие пользовательской перегрузки будет вызвана функция std::swap, которая
            // выполнит обмен через создание временной копии
            swap(first, second);
 
            // Убеждаемся, что используется не std::swap, а пользовательская перегрузка
 
            // Если бы обмен был выполнен с созданием временной копии,
            // то итератор first.begin() не будет равен ранее сохранённому значению,
            // так как копия будет хранить свои узлы по иным адресам
            assert(first.begin() == old_first_begin);
            assert(second.begin() == old_second_begin);
            assert(first.GetSize() == old_first_size);
            assert(second.GetSize() == old_second_size);
        }
    }
 
    // Инициализация списка при помощи std::initializer_list
    {
        SingleLinkedList<int> list{1, 2, 3, 4, 5};
        assert(list.GetSize() == 5);
        assert(!list.IsEmpty());
        assert(std::equal(list.begin(), list.end(), std::begin({1, 2, 3, 4, 5})));
    }
 
    // Лексикографическое сравнение списков
    {
        using IntList = SingleLinkedList<int>;
 
        assert((IntList{1, 2, 3} < IntList{1, 2, 3, 1}));
        assert((IntList{1, 2, 3} <= IntList{1, 2, 3}));
        assert((IntList{1, 2, 4} > IntList{1, 2, 3}));
        assert((IntList{1, 2, 3} >= IntList{1, 2, 3}));
    }
 
    // Копирование списков
    {
        const SingleLinkedList<int> empty_list{};
        // Копирование пустого списка
        {
            auto list_copy(empty_list);
            assert(list_copy.IsEmpty());
        }
 
        SingleLinkedList<int> non_empty_list{1, 2, 3, 4};
        // Копирование непустого списка
        {
            auto list_copy(non_empty_list);
 
            assert(non_empty_list.begin() != list_copy.begin());
            assert(list_copy == non_empty_list);
        }
    }
 
    // Присваивание списков
    {
        const SingleLinkedList<int> source_list{1, 2, 3, 4};
 
        SingleLinkedList<int> receiver{5, 4, 3, 2, 1};
        receiver = source_list;
        assert(receiver.begin() != source_list.begin());
       
        assert(receiver == source_list);
        
    }
 
    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
 
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };
 
    // Безопасное присваивание списков
    {
        SingleLinkedList<ThrowOnCopy> src_list;
        src_list.PushFront(ThrowOnCopy{});
        src_list.PushFront(ThrowOnCopy{});
        auto thrower = src_list.begin();
        src_list.PushFront(ThrowOnCopy{});
 
        int copy_counter = 0;  // при первом же копировании будет выброшего исключение
        thrower->countdown_ptr = &copy_counter;
 
        SingleLinkedList<ThrowOnCopy> dst_list;
        dst_list.PushFront(ThrowOnCopy{});
        int dst_counter = 10;
        dst_list.begin()->countdown_ptr = &dst_counter;
        dst_list.PushFront(ThrowOnCopy{});
 
        try {
            dst_list = src_list;
            // Ожидается исключение при присваивании
            assert(false);
            
        } catch (const std::bad_alloc&) {
            // Проверяем, что состояние списка-приёмника не изменилось
            // при выбрасывании исключений
            assert(dst_list.GetSize() == 2);
           
            auto it = dst_list.begin();
            assert(it != dst_list.end());
            
            assert(it->countdown_ptr == nullptr);
            
            ++it;
            assert(it != dst_list.end());
           
            assert(it->countdown_ptr == &dst_counter);
            
            assert(dst_counter == 10);
            
        } catch (...) {
            // Других типов исключений не ожидается
            assert(false);
        }
    }
    cout << "Test 3 is OK"s << endl;
}

// Эта функция проверяет работу класса SingleLinkedList
void Test4() {
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    // Проверка PopFront
    {
        SingleLinkedList<int> numbers{3, 14, 15, 92, 6};
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));
       

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }

    // Доступ к позиции, предшествующей begin
    {
        SingleLinkedList<int> empty_list;
        const auto& const_empty_list = empty_list;
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{1, 2, 3, 4};
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());
    }

    // Вставка элемента после указанной позиции
    {  // Вставка в пустой список
        {
            SingleLinkedList<int> lst;
            const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }

        // Вставка в непустой список
        {
            SingleLinkedList<int> lst{1, 2, 3};
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };
    }

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };

    // Проверка обеспечения строгой гарантии безопасности исключений
    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{}};
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
        }
        assert(exception_was_thrown);
    }

    // Удаление элементов после указанной позиции
    {
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto& const_lst = lst;
            const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{DeletionSpy{}, DeletionSpy{}, DeletionSpy{}};
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }
    cout << "Test 4 is OK"s << endl;
}
 
int main() {
    Test1();
    Test2();
    Test3();
    Test4();
} 
