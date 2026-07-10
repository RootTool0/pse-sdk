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
 * обратно в указатель на функцию через reinterpret_cast и вызываем
 * 
 * Внутри обработчика кнопка телепортируется в случайные координаты на карте
 */

#include <cstdio>
#include <random>

#include "pse.h"
#include "pse.hpp"

// Глобальный идентификатор для отслеживания нашей кнопки
PseGUID button;

/**
 * Кастомный обработчик (Callback), который будет вызываться при изменении состояния кнопки
 * Адрес этой функции увозится в игру, а при нажатии кнопки - возвращается нам в событии
 */
void onButtonChanged(int8_t inState)
{
    printf("[Callback] Новое состояние кнопки! state = %i\n", inState);
    
    static std::minstd_rand gen(std::random_device{}());
    static std::uniform_int_distribution<int> dist(-400, 400);
    
    {
        PseBufferData bufferData{};
        
        // [Команда] Переместить элемент (в нашем случае кнопку)
        bufferData.header = PSE_COMMAND_ELEMENT_SET_TRANSFORM;
        
        // [Смещение 0] Передаем GUID кнопки, которую мы хотим переместить
        pse::BufferData::set<0, PseGUID>(bufferData, button);
        
        // [Смещение 8] Задаем новые случайные координаты X и Y (Z фиксируем на -100)
        pse::BufferData::set<8, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{static_cast<float>(dist(gen)), static_cast<float>(dist(gen)), -100}, PseVector3{1, 1, 1}});
        
        // Синхронно отправляем команду в игру и ждем завершения операции
        pseCommandBufferSinglePushAndWait(&bufferData);
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
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_GAME_INITIALIZE;
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        if(bufferData.header != PSE_RESULT_SUCCESS)
        {
            printf("[SDK] Failed to initialize game! Error: 0x%08X\n", bufferData.header);
            return -1;
        }
    }
    
    // 3. Асинхронно спамим в командный буфер команды создания шахматного пола 5х5
    printf("[SDK] Генерация напольной плитки...\n");
    for(int x = 0; x < 5; ++x)
        for(int y = 0; y < 5; ++y)
        {
            PseBufferData bufferData{};
            
            // [Команда] Создаем динамический меш
            bufferData.header = PSE_COMMAND_DYNAMIC_MESH_CREATE;
            
            // [Смещение  0] Высчитываем сетку координат трансформа для каждого полигона
            pse::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{static_cast<float>(x) * 200 - 400, static_cast<float>(y) * 200 - 400, -100}, PseVector3{1, 1, 1}});
            
            // [Смещение 40] Указываем тип меша
            pse::BufferData::set<40, PseMesh>(bufferData, PSE_MESH_FACE);
            
            // [Смещение 42] Чередуем материалы в шахматном порядке
            pse::BufferData::set<42, PseMaterial>(bufferData, (x + y) % 2 == 0 ? PSE_MATERIAL_FLOOR_WHITE : PSE_MATERIAL_FLOOR_BLACK);
            
            // [Смещение 44] Флаг видимости (Visibility)
            pse::BufferData::set<44, int8_t>(bufferData, 1);
            
            // Быстро проталкиваем в очередь без ожидания ответа (SinglePush)
            pseCommandBufferSinglePush(&bufferData);
        }
    
    // Блокируем поток и ждем, пока игра полностью обработает ВСЮ пачку из 25 созданных мешей. Только после этого наш код пойдет дальше
    pseCommandBufferSynchronize();
    
    // 4. Создаем кнопку на координатах (200, 200, -100)
    {
        PseBufferData bufferData{};
        
        // [Команда] Создать элемент (в нашем случае кнопку)
        bufferData.header = PSE_COMMAND_ELEMENT_CREATE;
        
        // [Смещение  0] Задаем координаты кнопки
        pse::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{200, 200, -100}, PseVector3{1, 1, 1}});
        
        // [Смещение 40] Выставляем класс элемента - кнопка
        pse::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_BUTTON);
        
        // [Смещение 42] ВАЖНО: Приводим адрес нашей функции-обработчика к uint64_t и пакуем по смещению 42. SDK слепо доставит это число в игру, а игра просто сохранит его как число
        pse::BufferData::set<42, uint64_t>(bufferData, (uint64_t)&onButtonChanged);
        
        // [Смещение 50] Начальное состояние кнопки - 0
        pse::BufferData::set<50, int8_t>(bufferData, 0);
        
        // [Смещение 51] Флаг видимости (Visibility)
        pse::BufferData::set<51, int8_t>(bufferData, 1);
        
        // Отправляем и жестко ждем ответа, так как нам жизненно необходим GUID созданной кнопки
        pseCommandBufferSinglePushAndWait(&bufferData);
        
        // Игра перезаписала буфер и положила сгенерированный GUID по смещению 0
        button = pse::BufferData::get<0, PseGUID>(bufferData);
    }
    printf("[SDK] Интерактивная кнопка успешно зарегистрирована в игре\n");
    
    {
        PseBufferData bufferData{};
        
        // [Команда] Создать элемент (в нашем случае кубик)
        bufferData.header = PSE_COMMAND_ELEMENT_CREATE;
        
        // [Смещение  0] Задаем координаты кубика
        pse::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{100, -100, 0}, PseVector3{1, 1, 1}});
        
        // [Смещение 40] Выставляем класс элемента - кубик
        pse::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_WEIGHT_CUBE);
        
        // [Смещение 42] Нет Callback (0)
        pse::BufferData::set<42, uint64_t>(bufferData, 0);
        
        // [Смещение 50] Начальное состояние кубика - 0
        pse::BufferData::set<50, int8_t>(bufferData, 0);
        
        // [Смещение 51] Флаг видимости (Visibility)
        pse::BufferData::set<51, int8_t>(bufferData, 1);
        
        // Отправляем в игру и ждем ответа
        pseCommandBufferSinglePushAndWait(&bufferData);
    }
    printf("[SDK] Кубик создан\n");
    
    // 6. Бесконечный игровой цикл опроса асинхронных событий от игры (Event Loop)
    printf("[SDK] Вход в цикл опроса событий. Ожидаем действий в Portal: Solver...\n");
    while(true)
    {
        PseBufferData bufferData{};
        
        // Неблокирующий TryGet: если игра выставила событие, забираем его, иначе идем дальше
        if(pseEventBufferTryGet(&bufferData) != 0)
        {
            // Нам интересно только событие изменения состояния элементов
            if(bufferData.header == PSE_EVENT_ELEMENT_CHANGED)
            {
                // Извлекаем адрес коллбэка, прилетевший от игры
                uint64_t callback = pse::BufferData::get<8, uint64_t>(bufferData);
                
                // Извлекаем новое состояние элемента (например нажат/отпущен)
                int8_t state = pse::BufferData::get<16, int8_t>(bufferData);
                
                // Если адрес не нулевой, восстанавливаем сигнатуру функции и вызываем её
                if(callback != 0) 
                {
                    reinterpret_cast<void(*)(int8_t)>(callback)(state);
                }
            }
        }
    }
    
    // Корректное закрытие дескрипторов памяти (в данном примере недостижимо из-за while(true))
    pseDeinitializeBuffers();
    return 0;
}
