/**
* ===============================================================================
 * Portal Solver Editor SDK - Пример 02: Laser Halo (Лазерный Нимб)
* ===============================================================================
 * 
 * ОПИСАНИЕ:
 * 1. Процедурно спавнится кольцо из лазерных излучателей (PSE_CLASS_LASER_TX)
 * 2. Каждый лазер динамически красится в свой уникальный цвет из палитры HSV (Радуга)
 * 3. Цвет упаковывается побитово через хелпер pse::Register::setInt() в Регистр [1]
 *    (Laser Color в регистре [1] в элементе LaserTx)
 * 4. Запускается математический цикл симуляции спирографа (эпициклов)
 * 
 * СУТЬ АССИНХРОННЫХ БУФЕРОВ:
 * Это демонстрация полной асинхронности командного буфера
 * Нам НЕ НУЖНО ждать игру после отправки каждой команды изменения трансформации элементов
 * Командный буфер огромный (2048 команд, PSE_COMMAND_BUFFER_SIZE)
 * Поэтому мы можем в один миг закинуть туда всю пачку команд через `pseCommandBufferSinglePush`
 * 
 * Мы просто кидаем команды в память одна за другой, не дожидаясь ответа от игры.
 * Единственное, что нужно помнить: игра за один кадр физически успевает переварить 
 * максимум 128 команд (PSE_MAX_COMMANDS_PER_TICK). Мы спокойно укладываемся в этот лимит,
 * а в конце цикла вызываем `pseCommandBufferSynchronize()`, чтобы игра завершила этот кадр
 */

#include <chrono>
#include <cstdio>
#include <random>

#include "pse.h"
#include "pse.hpp"

// Константы
#define LASER_COUNT 24
#define R1 800.0f
#define R2 10.0f
#define SPEED_1 0.6f
#define SPEED_2 1.5f

// Функция для перевода индекса лазера в сочный RGB цвет по цветовому кругу HSV
// Возвращает упакованный PseRegister с правильной разметкой байт для игры
PseRegister calculateLaserColorHSV(const int index)
{
    const float h = (6.0f * static_cast<float>(index)) / static_cast<float>(LASER_COUNT);
    const float s = 1.0f;
    const float v = 1.0f;
    
    const int i = static_cast<int>(std::floor(h));
    const float f = h - static_cast<float>(i);
    const float p = v * (1.0f - s);
    const float q = v * (1.0f - s * f);
    const float t = v * (1.0f - s * (1.0f - f));
    
    float r = 0, g = 0, b = 0;
    switch (i % 6)
    {
    default:
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
    }
    
    // Переводим дробные значения в классический диапазон 0-255
    const uint8_t red   = static_cast<uint8_t>(r * 255.0f);
    const uint8_t green = static_cast<uint8_t>(g * 255.0f);
    const uint8_t blue  = static_cast<uint8_t>(b * 255.0f);
    
    // Собираем регистр побитово. Разметка игры: ________RRRRRRRRGGGGGGGGBBBBBBBB
    PseRegister reg = 0;
    pse::Register::setInt<uint8_t>(reg, 0 , red);
    pse::Register::setInt<uint8_t>(reg, 8 , green);
    pse::Register::setInt<uint8_t>(reg, 16, blue);
    
    return reg;
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
    
    // 3. Спавним огромный черный пол-подложку
    {
        PseData data{};
        
        // [Команда] Создать динамический меш
        data.header = PSE_COMMAND_DYNAMIC_MESH_CREATE;
        
        // [Смещение 0] Задаем координаты и огромный масштаб меша (24х24)
        pse::Data::set< 0, PseTransform>(data, PseTransform{PseQuat{}, PseVector3{0.f, 0.f, -1600.f}, PseVector3{24, 24, 1}});
        
        // [Смещение 40] Указываем тип меша (плоскость)
        pse::Data::set<40, PseMesh>(data, PSE_MESH_FACE);
        
        // [Смещение 42] Задаем черный материал пола
        pse::Data::set<42, PseMaterial>(data, PSE_MATERIAL_FLOOR_BLACK);
        
        // [Смещение 44] Делаем меш видимым
        pse::Data::set<44, int8_t>(data, 1);
        
        // Отправляет команду в игру
        pseCommandBufferSinglePush(&data);
    }
    
    // 4. Спавним невидимый пол под игроком
    {
        PseData data{};
        
        // [Команда] Создать динамический меш
        data.header = PSE_COMMAND_DYNAMIC_MESH_CREATE;
        
        // [Смещение 0] Задаем координаты и небольшой масштаб
        pse::Data::set< 0, PseTransform>(data, PseTransform{PseQuat{}, PseVector3{0.f, 0.f, -100.f}, PseVector3{2, 2, 1}});
        
        // [Смещение 40] Указываем тип меша (плоскость)
        pse::Data::set<40, PseMesh>(data, PSE_MESH_FACE);
        
        // [Смещение 42] Задаем белый материал пола
        pse::Data::set<42, PseMaterial>(data, PSE_MATERIAL_FLOOR_BLACK);
        
        // [Смещение 44] Делаем меш НЕ видимым
        pse::Data::set<44, int8_t>(data, 0);
        
        // Отправляет команду в игру
        pseCommandBufferSinglePush(&data);
    }
    
    // 5. Отключаем игроку Солвер Ган, чтобы не мешал на экране
    {
        PseData data{};
        
        // [Команда] Установить Солвер Ган (Enabled означает есть ли в руках Солвер Ган или нет)
        data.header = PSE_COMMAND_SOLVER_GUN_SET_ENABLED;
        
        // [Смещение 0] Флаг доступности = 0 (Нет в руках Солвер Гана)
        pse::Data::set< 0, int8_t>(data, 0);
        
        pseCommandBufferSinglePush(&data);
    }
    
    std::vector<PseGUID> lasers(LASER_COUNT);
    printf("[SDK] Спавним лазерное кольцо из %i излучателей...\n", LASER_COUNT);
    
    for(int i = 0; i < LASER_COUNT; ++i)
    {
        // Создаем сам элемент лазера в игре
        {
            PseData data{};
            
            // [Команда] Создать игровой элемент
            data.header = PSE_COMMAND_ELEMENT_CREATE;
            
            // [Смещение 40] Выставляем класс - лазерный излучатель
            pse::Data::set<40, PseClass>(data, PSE_CLASS_LASER_TX);
            
            // [Смещение 50] Включаем лазер (State = 1)
            pse::Data::set<50, PseState>(data, 1);
            
            // [Смещение 51] Делаем модельку видимой
            pse::Data::set<51, int8_t>(data, 1);
            
            // Нам ЖИЗНЕННО необходим GUID, поэтому жестко ждем ответа от игры
            pseCommandBufferSinglePushAndWait(&data);
            
            // Забираем сгенерированный игрой GUID лазера из смещения 0
            lasers[i] = pse::Data::get<0, PseGUID>(data);
        }
        
        // Красим лазерный нашим кастомный HSV-цветом
        {
            // Генерируем упакованный цвет радуги для регистра
            PseRegister reg = calculateLaserColorHSV(i);
            
            PseData data{};
            
            // [Команда] Изменить значение регистра в элементе
            data.header = PSE_COMMAND_ELEMENT_SET_REGISTER;
            
            // [Смещение 0] Передаем GUID лазера, регистр которого хотим изменить
            pse::Data::set<0, PseGUID>(data, lasers[i]);
            
            // [Смещение 8] Передаем сам упакованный регистр цвета
            pse::Data::set<8, PseRegister>(data, reg);
            
            // [Смещение 16] Указываем индекс целевого регистра (Регистр 1 = Laser Color)
            pse::Data::set<16, int8_t>(data, 1);
            
            // Синхронно отправляем цвет в игру и ждем применения
            pseCommandBufferSinglePushAndWait(&data);
        }
    }
    
    printf("[SDK] Запуск Event Loop. Наслаждайтесь шоу\n");
    
    // Засекаем стартовую точку времени для плавных тригонометрических расчетов
    const auto startTime = std::chrono::high_resolution_clock::now();
    while(true)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        const float t = std::chrono::duration<float>(currentTime - startTime).count();
        
        // Быстро генерируем пачку из 24 команд перемещения в один кадр
        for(int i = 0; i < LASER_COUNT; ++i)
        {
            // Равномерно распределяем лазеры по кругу (шаг угла alpha)
            const float alpha = 2.0f * 3.14159265358979323846f * static_cast<float>(i) / static_cast<float>(LASER_COUNT);
            
            // Рассчитываем угол поворота лазера вокруг своей оси (в ридианах)
            const float lookAngle = SPEED_1 * t + alpha + SPEED_2 * t;
            
            // Создаем кватернион вращения через локальный хелпер
            const PseQuat rotation = pse::Quat::fromRadians(0.f, lookAngle, 0.f);
            
            // Рассчитываем сложную позицию спирографа (сложение двух вращающихся радиусов R1 и R2)
            const PseVector3 location = PseVector3{
                R1 * std::cos(SPEED_1 * t + alpha) + R2 * std::cos(SPEED_2 * t),
                R1 * std::sin(SPEED_1 * t + alpha) + R2 * std::sin(SPEED_2 * t),
                -1575.0f
            };
            
            {
                PseData data{};
                
                // [Команда] Обновить трансформ (координаты/поворот) элемента
                data.header = PSE_COMMAND_ELEMENT_SET_TRANSFORM;
                
                // [Смещение 0] Передаем GUID перемещаемого лазера
                pse::Data::set<0, PseGUID>(data, lasers[i]);
                
                // [Смещение 8] Передаем собранную структуру трансформа (поворот, позиция, масштаб 1:1:1)
                pse::Data::set<8, PseTransform>(data, PseTransform{rotation, location, PseVector3{1, 1, 1}});
                
                // Стреляем командой в буфер АСИНХРОННО! Не ждем игру для каждого лазера!
                pseCommandBufferSinglePush(&data);
            }
        }
        
        // Синхронизируем буферы, чтобы игра в текущем кадре (тике) закончила выполнять все команды
        // Всего за один кадр может быть обработано максимум 128 команд
        // Они закреплены в макросе PSE_MAX_COMMANDS_PER_TICK
        pseCommandBufferSynchronize();
    }
    
    // Штатное освобождение Shared Memory ресурсов (недостижимо из-за while(true))
    pseDeinitializeBuffers();
    return 0;
}
