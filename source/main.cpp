#include <cstdio>

// Заголовочный файл с PSE SDK
#include "pse.h"

int main()
{
    // Инициализируем буферы в SDK
    if(pseInitializeBuffers() != 0)
    {
        printf("Failed to initialize buffers!\n");
        return -1;
    }
    
    // Попробуем сегодня создать дверь, выставить ей кастомный цвет и открыть!
    PseGUID doorGUID;
    
    // 1. Создадим дверь
    {
        // Для начала создадим пустой буфер, который будем заполнять полезными данными
        PseBufferData bufferData{};
        
        bufferData.command = PSE_COMMAND_ELEMENT_CREATE; // Выбираем команду - создать элемент
        bufferData.set< 0, PseTransform>(PseTransform{PseQuat{}, PseVector3{0, 0, 0}, PseVector3{1, 1, 1}});
        bufferData.set<40, PseClass>(PSE_CLASS_DOOR); // Класс - Дверь
        bufferData.set<42, uint64_t>(0); // Callback - нуль, так как двери не нужен калбек
        bufferData.set<50, int8_t>(0); // State - нуль, изначально дверь закрыта
        bufferData.set<51, int8_t>(1); // Видимость - единица, да. Нам нужна видимая дверь
        
        pseCommandBufferSinglePushAndWait(&bufferData); // Отправляем команду и ждем ответ (ответ запишется в этот же буфер)
        if(bufferData.result != PSE_RESULT_SUCCESS) // Если ответ НЕ успешный, то это ошибка...
        {
            printf("Failed to create door!\n");
            return -1;
        }
        
        // В случае успеха, в ответе читаем выданный игрой GUID двери (через который мы сможем обращаться к самой двери в игре)
        doorGUID = bufferData.get<0, PseGUID>();
    }
    
    // 2. Поставим двери красный цвет при активации
    {
        PseColor colorActivated = PseColor{ 255, 0, 0}; // Красный цвет
        
        // Также создадим пустой буфер, который будем заполнять полезными данными
        PseBufferData bufferData{};
        
        pseBufferDataCreateCommand(&bufferData, PSE_COMMAND_ELEMENT_SET_REGISTER); // Выбираем команду - изменить регистр элемента (по индексу)
        pseBufferDataSetGUID      (&bufferData, 0 , doorGUID); // GUID элемента, который хотим изменить - GUID двери
        pseBufferDataSetColor     (&bufferData, 8 , colorActivated); // Значение регистра - наш с вами красный цвет 
        pseBufferDataSetInt8      (&bufferData, 12, 1); // Индекс регистра - 1
        
        pseCommandBufferSinglePushAndWait(&bufferData); // Также отправляем команду и ждем ответа (ответ запишется в этот же буфер)
        if(pseBufferDataReadResult(&bufferData) != PSE_RESULT_SUCCESS) // Если ответ НЕ успешный, то это ошибка...
        {
            printf("Failed to set color activated to door!\n");
            return -1;
        }

        // Profit! Теперь при активации двери, у нее будет красный цвет!
    }

    // 3. Откроем саму дверь!
    {
        int8_t state = 1; // Состояние, которое мы поставим двери - 1
        
        // Снова создадим пустой буфер, который будем заполнять полезными данными...
        PseBufferData bufferData{};
        
        pseBufferDataCreateCommand(&bufferData, PSE_COMMAND_ELEMENT_SET_STATE); // Выбираем команду - задать состояние элемента
        pseBufferDataSetGUID      (&bufferData, 0 , doorGUID); // GUID элемента, который хотим изменить - GUID двери
        pseBufferDataSetInt8      (&bufferData, 8 , state); // Новое состояние двери - 1
        
        pseCommandBufferSinglePushAndWait(&bufferData); // Также отправляем команду и ждем ответа (ответ запишется в этот же буфер)...
        if(pseBufferDataReadResult(&bufferData) != PSE_RESULT_SUCCESS) // Если ответ НЕ успешный, то это ошибка...
        {
            printf("Failed to set state to door!\n");
            return -1;
        }

        // Profit! Теперь дверь откроется, так как сработает условие State != 0 => true (т.е. открытие двери)
        // А так как ранее мы ей выставили красный цвет при активации (пункт 2), то открыв ее, цвет сменится подсветки сменится на красный
    }

    // Ну и не забываем прибраться за собой, закрыв все буферы
    pseDeinitializeBuffers();

    return 0;
}
