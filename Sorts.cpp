#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>

sf::RenderWindow window(sf::VideoMode(500, 350), "SFML works!");
int my_time = 0;                                                                    //время сортировки
int swaps = 0;                                                                      //количество свапов


void draw_array(int* A,int N){
    /*проинициализировать шрифты нужно один раз, поэтому мы должны распознать самый первый вход в функцию
    для этого введем статический флаг*/
    static int flag = 0;
    static sf::Font font;                                                           //объявление шрифта
    if(flag == 0){
        font.loadFromFile("consola.ttf");                                           //инициализация шрифта
        flag++;
    }
    int weight = 500/N;                                                             //ширина столбца
    sf::RectangleShape* rectangles = new sf::RectangleShape[N];                     //объявление массва прямоугольников
    for (int i = 0; i < N; i++)
    {
        rectangles[i] = sf::RectangleShape(sf::Vector2f(weight,3*A[i]));            //инициализация прямоугольников
        rectangles[i].move(weight*i,350-3*A[i]);                                    //позиционирование прямоугольников
        window.draw(rectangles[i]);                                                 //прорисовка
    }
    sf::Text text("", font, 20);                                                    //задание параметров текста
    std::ostringstream swaps_text;                                                  //потог для ввода текста
    swaps_text<<swaps;
    std::ostringstream time_text; 
    time_text<<swaps;
    text.setString("Swaps:" + swaps_text.str() + " " + "Time:" + time_text.str());  //сам текст
    text.setPosition(10,10);                                                        //позиционирование текста
    window.draw(text);                                                              //прорисовка текста
    window.display();                                                               //отображение всего нарисованного на экране
    sleep(1);                                                                       //задержка
    window.clear();                                                                 //очищение экрана
    my_time++;
    swaps++;
}

void swap(int &A, int &B,int *D,int N){                                             //Функция для свапанья
    int C = A;
    A = B;
    B = C;
    draw_array(D,N);                                                                //Рисуем каждый сват
}

void Bubble_sort(int *A,int N){                                                     //пузырьковая сортировка
    for (int i = 0; i < N; i++)
    {
        for (int j = 1; j < N-i; j++)
        {
            if(A[j-1]>A[j]){
                swap(A[j-1],A[j],A,N);
            }
        }
    }
}

void Insertion_sort(int *A,int N){                                                  //Сортировка вставками
    for (int i = 1; i < N; i++)
    {
        for (int j = i; j > 0; j--)
        {
            if(A[j-1]>A[j]){
                swap(A[j-1],A[j],A,N);
            }
        }
    }  
}

void print_array(int *A,int N){                                                     //Вывод массива
    for (int i = 0; i < N; printf("%d ",A[i++]));  
    printf("\n");
}

void counter_sort(int*A,int N){                                                     //Сортировка подсчетом
    int min = A[0];
    int max = A[0];
    for (int i = 0; i < N; i++)
    {
        if (min>A[i])
        {
            min = A[i];
        }
        if (max<A[i])
        {
            max = A[i];
        }
    }
    int denomenator=min;
    for (int i = 0; i < N; i++)
    {
        A[i] -= denomenator;
    }
    max-=denomenator;
    int *B = (int*)malloc(sizeof(int)*max);
    for (int i = 0; i < max;B[i++] = 0);

    
    for (int i = 0; i < N; i++)
    {
        B[A[i]]++;
    }
    int summ = 0;
    for (int i = 0; i <= max; i++){
        for (int j = 0; j <B[i]; j++,summ++){
             A[summ] = i+denomenator;
        }
    }
    delete[] B;
}

/*Быструю сортировку обычно пишут так, что бы её можно было вызвать рекурсивно
Однако тогда эта сортировка будет отличаться по выхову от других функций,
что не позволит вызывать её из массива указателей на функции. Поэтому придется
создать промежуточную функцию  quick_sort, сигнатура которой совпадает с остальными
функциями с сортировками. Эта функция будет вызывать quick_req_sort в которой уже и 
находится текст основной сортировки*/

void quick_req_sort(int *A, int first, int last,int N)                              //сама быстрая сортировка                
{
    if (first < last)
    {
        int left = first, right = last, middle = A[(left + right) / 2];
        do
        {
            while (A[left] < middle) left++;
            while (A[right] > middle) right--;
            if (left <= right)
            {
                swap(A[left],A[right],A,N);
                left++;
                right--;
            }
        } while (left <= right);
        quick_req_sort(A, first, right,N);
        quick_req_sort(A, left, last,N);
    }
}

void quick_sort(int*A,int N){                                                       //Промежуточное звено
    quick_req_sort(A,0,N-1,N);
}

void break_array(int*A,int N){                                                      //Самая главная сортировка
    for (int i = 0; i < 10; i++)
    {
        A[i] = rand()%100;
    }
}

int main()
{
    
    typedef void(f)(int*A,int N);                                                   //массив, хранящий вызываемые сортировки
    f* Mass[3] = {Bubble_sort,Insertion_sort,quick_sort};
    int A[10] = {83,86,77,17,93,35,86,92,49,21};
    while (window.isOpen())                                                         //Открытие окна с грачикой
    {
        window.clear();                                                             //Очистка экрана
        for (int i = 0; i < 3; i++)                                                 //Рисование всех сортировок (которые можно нарисовать)
        {
            break_array(A,10);                                                      
            sleep(1);
            Mass[i](A,10);
            sleep(1);
            window.clear();
            window.display();
            swaps = 0;
            my_time = 0; 
        }
        window.close();                                                              //Закрырие окна с графикой
    }

    return 0;
}