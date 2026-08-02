/**
* ===============================================================================
 * Portal Solver Editor SDK - Пример 01: Веселая кнопка и обработка событий
* ===============================================================================
 * 
 * ОПИСАНИЕ:
 * 1. Инициализируется подключение к игре через Shared Memory
 * 2. Двумя циклами генерируется пол 5х5 из белых и черных динамических мешей
 * 3. На сцене создается кнопка (PSE_CLASS_BUTTON). При её создании в сырую память 
 *    по смещению 42 записывается ПРЯМОЙ указатель на C++ функцию 
 *    onButtonChanged. Игра сохраняет этот адрес у себя
 * 4. Рядом создается физический куб (PSE_CLASS_WEIGHT_CUBE)
 * 5. Запускается бесконечный цикл опроса буфера событий (Event Loop)
 * 
 * СУТЬ STATELESS-МАГИИ:
 * Когда игрок (или куб) нажимает кнопку внутри игры, игра генерирует событие
 * PSE_EVENT_ELEMENT_CHANGED, вкладывает туда переданный нами ранее адрес функции 
 * и присылает пакет в Event Buffer. Мы вытаскиваем этот адрес, превращаем его 
 * обратно в указатель на функцию через `reinterpret_cast` и вызываем
 * 
 * Внутри обработчика кнопка телепортируется в случайные координаты на карте
 */

#include <cstdio>
#include <random>

#include "pse/pse.h"
#include "pse/pse.hpp"

// Глобальный идентификатор для отслеживания нашей кнопки
PseGUID button;

/**
 * Кастомный обработчик (Callback), который будет вызываться при изменении состояния кнопки
 * Адрес этой функции увозится в игру, а при нажатии кнопки - возвращается нам в событии
 */
void onButtonChanged(PseState inState)
{
    printf("[Callback] Новое состояние кнопки! state = %i\n", inState);
    
    static std::minstd_rand gen(std::random_device{}());
    static std::uniform_int_distribution<int> dist(-400, 400);
    
    {
        PseData data{};
        
        // [Команда] Переместить элемент (в нашем случае кнопку)
        data.header = PSE_COMMAND_ELEMENT_SET_TRANSFORM;
        
        // [Смещение 0] Передаем GUID кнопки, которую мы хотим переместить
        pse::Data::set<0, PseGUID>(data, button);
        
        // [Смещение 8] Задаем новые случайные координаты X и Y (Z фиксируем на -100)
        pse::Data::set<8, PseTransform>(data, PseTransform{PseQuat{}, PseVector3{static_cast<float>(dist(gen)), static_cast<float>(dist(gen)), -100}, PseVector3{1, 1, 1}});
        
        // Синхронно отправляем команду в игру и ждем завершения операции
        pseCommandBufferSinglePushAndWait(&data);
    }
}

int main()
{
    // 1. Инициализируем буферы разделяемой памяти (Shared Memory) игры
    if(pseInitializeBuffers() != 0)
    {
        printf("[SDK] Failed to initialize buffers!\n");
        return -1;
    }
    
    // 2. Инициализируем игру
    {
        PseData data{};
        data.header = PSE_COMMAND_GAME_INITIALIZE;
        
        pseCommandBufferSinglePushAndWait(&data);
        if(data.header != PSE_RESULT_SUCCESS)
        {
            printf("[SDK] Failed to initialize game! Error: 0x%08X\n", data.header);
            return -1;
        }
    }
    
    // 3. Асинхронно спамим в командный буфер команды создания шахматного пола 5х5
    printf("[SDK] Генерация напольной плитки...\n");
    for(int x = 0; x < 5; ++x)
        for(int y = 0; y < 5; ++y)
        {
            PseData data{};
            
            // [Команда] Создаем динамический меш
            data.header = PSE_COMMAND_DYNAMIC_MESH_CREATE;
            
            // [Смещение  0] Высчитываем сетку координат трансформа для каждого полигона
            pse::Data::set< 0, PseTransform>(data, PseTransform{PseQuat{}, PseVector3{static_cast<float>(x) * 200 - 400, static_cast<float>(y) * 200 - 400, -100}, PseVector3{1, 1, 1}});
            
            // [Смещение 40] Указываем тип меша
            pse::Data::set<40, PseMesh>(data, PSE_MESH_FACE);
            
            // [Смещение 42] Чередуем материалы в шахматном порядке
            pse::Data::set<42, PseMaterial>(data, (x + y) % 2 == 0 ? PSE_MATERIAL_FLOOR_WHITE : PSE_MATERIAL_FLOOR_BLACK);
            
            // [Смещение 44] Флаг видимости (Visibility)
            pse::Data::set<44, int8_t>(data, 1);
            
            // Быстро проталкиваем в очередь без ожидания ответа (SinglePush)
            pseCommandBufferSinglePush(&data);
        }
    
    // Блокируем поток и ждем, пока игра полностью обработает ВСЮ пачку из 25 созданных мешей. Только после этого наш код пойдет дальше
    pseCommandBufferSynchronize();
    
    // 4. Создаем кнопку на координатах (200, 200, -100)
    {
        PseData data{};
        
        // [Команда] Создать элемент (в нашем случае кнопку)
        data.header = PSE_COMMAND_ELEMENT_CREATE;
        
        // [Смещение  0] Задаем координаты кнопки
        pse::Data::set< 0, PseTransform>(data, PseTransform{PseQuat{}, PseVector3{200, 200, -100}, PseVector3{1, 1, 1}});
        
        // [Смещение 40] Выставляем класс элемента - кнопка
        pse::Data::set<40, PseClass>(data, PSE_CLASS_BUTTON);
        
        // [Смещение 42] ВАЖНО: Приводим адрес нашей функции-обработчика к PseCallback и пакуем по смещению 42. SDK слепо доставит это число в игру, а игра просто сохранит его как число
        pse::Data::set<42, PseCallback>(data, (PseCallback)&onButtonChanged);
        
        // [Смещение 50] Начальное состояние кнопки - 0
        pse::Data::set<50, PseState>(data, 0);
        
        // [Смещение 51] Флаг видимости (Visibility)
        pse::Data::set<51, int8_t>(data, 1);
        
        // Отправляем и жестко ждем ответа, так как нам жизненно необходим GUID созданной кнопки
        pseCommandBufferSinglePushAndWait(&data);
        
        // Игра перезаписала буфер и положила сгенерированный GUID по смещению 0
        button = pse::Data::get<0, PseGUID>(data);
    }
    printf("[SDK] Интерактивная кнопка успешно зарегистрирована в игре\n");
    
    {
        PseData data{};
        
        // [Команда] Создать элемент (в нашем случае кубик)
        data.header = PSE_COMMAND_ELEMENT_CREATE;
        
        // [Смещение  0] Задаем координаты кубика
        pse::Data::set< 0, PseTransform>(data, PseTransform{PseQuat{}, PseVector3{100, -100, 0}, PseVector3{1, 1, 1}});
        
        // [Смещение 40] Выставляем класс элемента - кубик
        pse::Data::set<40, PseClass>(data, PSE_CLASS_WEIGHT_CUBE);
        
        // [Смещение 42] Нет Callback (0)
        pse::Data::set<42, PseCallback>(data, 0);
        
        // [Смещение 50] Начальное состояние кубика - 0
        pse::Data::set<50, PseState>(data, 0);
        
        // [Смещение 51] Флаг видимости (Visibility)
        pse::Data::set<51, int8_t>(data, 1);
        
        // Отправляем в игру и ждем ответа
        pseCommandBufferSinglePushAndWait(&data);
    }
    printf("[SDK] Кубик создан\n");
    
    // 6. Бесконечный игровой цикл опроса асинхронных событий от игры (Event Loop)
    printf("[SDK] Вход в цикл опроса событий. Ожидаем действий в Portal: Solver...\n");
    while(true)
    {
        PseData data{};
        
        // Неблокирующий TryGet: если игра выставила событие, забираем его, иначе идем дальше
        if(pseEventBufferTryGet(&data) != 0)
        {
            // Нам интересно только событие изменения состояния элементов
            if(data.header == PSE_EVENT_ELEMENT_CHANGED)
            {
                // Извлекаем адрес коллбэка, прилетевший от игры
                PseCallback callback = pse::Data::get<8, PseCallback>(data);
                
                // Извлекаем новое состояние элемента (например нажат/отпущен)
                PseState state = pse::Data::get<16, PseState>(data);
                
                // Если адрес не нулевой, восстанавливаем сигнатуру функции и вызываем её
                if(callback != 0) 
                {
                    reinterpret_cast<void(*)(PseState)>(callback)(state);
                }
            }
        }
    }
    
    // Корректное закрытие дескрипторов памяти (в данном примере недостижимо из-за while(true))
    pseDeinitializeBuffers();
    return 0;
}
