﻿// homework_mp_3.1.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// задача к теме 3 из модуля "многопоточность"

//Сортировка выбором

//Реализуйте алгоритм сортировки выбором.
//Сортировка выбором состоит в том, что в неупорядоченном списке находится наименьший элемент.
// Выбранный в исходном списке минимальный элемент записывается на i - е место исходного списка(i = 1, 2, …, п), 
// а элемент с i - го места — на место выбранного.При этом очевидно, что уже упорядоченные элементы, 
// а они будут расположены, начиная с первого места, исключаются из дальнейшей сортировки, 
// поэтому длина списка, участвующего в каждом последующем просмотре, должна быть на один элемент меньше предыдущего.
//Поиск минимального элемента реализуйте в виде асинхронной задачи.
//Результат должен возвращаться в основной поток через связку std::promise - std::future.


#include <iostream>
#include <thread>
#include <future>

void find_min(int min, int i, int N, int* arr, std::promise<int> prom)
{
    // получаем номер потока
    std::cout << "th2: " << std::this_thread::get_id() << std::endl;
    // находим номер минимального элемента
    for (int j = i + 1; j < N; j++)
        min = (arr[j] < arr[min]) ? j : min;
    // передаём номер минимального элемента в общую область памяти
    prom.set_value(min);
}

int main()
{
    std::cout << "Hello World!\n";

    const int N = 8;
    int* arr = new int[N] {9, 6, 3, 8, 4, 2, 4, 8};

    int min;
    int buf;


    // во внешнем цикле мы берём первый элемент массива, который имеет номер i = 0;
    // и во внутреннем цикле, который реализован в функции find_min,
    // ищем среди оставшихся семи элементов тот, который будет меньше первого (у которого i = 0)
    // если такой нашёлся, то меняем их местами. 
    // после берём второй элемент массива, у которого i = 1; и всё повторяется. 
    for (int i = 0; i < N; i++)
    {
        min = i;

        std::promise<int> prom;
        std::future<int> ft_res = prom.get_future();
        //почему-то выдаёт ошибку, когда я пытаюсь написать фразу ниже, чтобы получить номер потока
        std::cout << "th1: " << std::this_thread::get_id() << std::endl;
        auto ft = std::async(find_min, min, i, N, arr, std::move(prom));
        min = ft_res.get();

        // смена элементов местами
        if (i != min)
        {
            buf = arr[i];
            arr[i] = arr[min];
            arr[min] = buf;
        }
    }

    // вывод на печать от сортированной последовательности
    for (int i = 0; i < N; i++)
        std::cout << arr[i] << std::endl;
    std::cout << std::endl;

    return 0;
}