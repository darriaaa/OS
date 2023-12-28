// lab2.2_os.cpp : Алгоритм зміщення по колу сторінок
//

#include <iostream>
#include <unordered_map>
#include <list>

class LRUCache {
private:
    // Хэш-таблица для отображения страницы на итератор в двусвязном списке
    std::unordered_map<int, std::list<int>::iterator> cache_map;
    // Двусвязный список для хранения порядка доступа к страницам
    std::list<int> cache_list;
    // Максимальная вместимость кеша
    int capacity;

public:
    // Конструктор класса LRUCache
    LRUCache(int cap) : capacity(cap) {}

    // Метод для доступа к странице в кеше
    void access_page(int page) {
        if (cache_map.find(page) != cache_map.end()) {
            // Если страница уже в кеше, перемещаем ее в начало списка
            cache_list.erase(cache_map[page]);
        }
        else {
            // Если страницы нет в кеше и кеш заполнен, удаляем наименее используемую страницу
            if (cache_list.size() >= capacity) {
                int lru_page = cache_list.back();
                cache_list.pop_back();
                cache_map.erase(lru_page);
            }
        }

        // Добавляем текущую страницу в начало кеша
        cache_list.push_front(page);
        cache_map[page] = cache_list.begin();
    }

    // Метод для вывода содержимого кеша
    void print_cache() {
        for (int page : cache_list) {
            std::cout << page << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // Пример использования
    LRUCache lru_cache(3); //Створюємо кеш з розміром 3

    lru_cache.access_page(1);
    lru_cache.print_cache(); // Вывод: 1

    lru_cache.access_page(2);
    lru_cache.print_cache(); // Вывод: 2 1

    lru_cache.access_page(3);
    lru_cache.print_cache(); // Вывод: 3 2 1

    lru_cache.access_page(1); // 1 вже є в кеші, тому його видаляємо з поточного положення та додаємо його в кінець
    lru_cache.print_cache(); // Вывод: 1 3 2, 

    lru_cache.access_page(4);
    lru_cache.print_cache(); // Вывод: 4 1 3 (2 была удалена, так как кеш был полон)

    return 0;
}
