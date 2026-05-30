<img src="../../Assets/PortalSolverLogo.png" width="96m" align="left" hspace="15" alt="Portal Solver Logo">

## Portal Solver Editor SDK > Functions

<br>

C-API Справочник функций

Экспортируемый плоский C-интерфейс (extern "C") для интеграции SDK с другими языками программирования и обеспечения бинарной совместимости (ABI).

## PseBufferData / Create

| Сигнатура                                            | Тип возвращаемого значения | Аргументы                                          | Описание                                     |
|------------------------------------------------------|----------------------------|----------------------------------------------------|----------------------------------------------|
| **pseBufferDataCreateCommand(pBufferData, command)** | `void`                     | `PseBufferData* pBufferData`, `PseCommand command` | Записывает тип команды в контейнер данных    |
| **pseBufferDataCreateResult(pBufferData, result)**   | `void`                     | `PseBufferData* pBufferData`, `PseResult result`   | Записывает тип результата в контейнер данных |
| **pseBufferDataCreateEvent(pBufferData, event)**     | `void`                     | `PseBufferData* pBufferData`, `PseEvent event`     | Записывает тип события в контейнер данных    |

## PseBufferData / Read

| Сигнатура                                            | Тип возвращаемого значения | Аргументы                          | Описание                                                        |
|------------------------------------------------------|----------------------------|------------------------------------|-----------------------------------------------------------------|
| **pseBufferDataReadCommand(pBufferData)**            | `PseCommand`               | `const PseBufferData* pBufferData` | Возвращает текущее значение из контейнера данных как PseCommand |
| **pseBufferDataReadResult(pBufferData)**             | `PseResult`                | `const PseBufferData* pBufferData` | Возвращает текущее значение из контейнера данных как PseResult  |
| **pseBufferDataReadEvent(pBufferData)**              | `PseEvent`                 | `const PseBufferData* pBufferData` | Возвращает текущее значение из контейнера данных как PseEvent   |

## PseBufferData / Setters

| Сигнатура                                                 | Тип возвращаемого значения | Аргументы                                                                   | Описание                                    |
|-----------------------------------------------------------|----------------------------|-----------------------------------------------------------------------------|---------------------------------------------|
| **pseBufferDataSetInt8(pBufferData, offset, value)**      | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `int8_t value`              | Запись 8-битного целого числа               |
| **pseBufferDataSetInt16(pBufferData, offset, value)**     | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `int16_t value`             | Запись 16-битного целого числа              |
| **pseBufferDataSetInt32(pBufferData, offset, value)**     | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `int32_t value`             | Запись 32-битного целого числа              |
| **pseBufferDataSetInt64(pBufferData, offset, value)**     | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `int64_t value`             | Запись 64-битного целого числа              |
| **pseBufferDataSetFloat(pBufferData, offset, value)**     | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `float value`               | Запись числа с плавающей точкой             |
| **pseBufferDataSetGUID(pBufferData, offset, value)**      | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `PseGUID value`             | Запись GUID (8 байт)                        |
| **pseBufferDataSetVector2(pBufferData, offset, value)**   | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `PseVector2 value`          | Запись 2D-вектора (8 байт)                  |
| **pseBufferDataSetVector3(pBufferData, offset, value)**   | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `PseVector3 value`          | Запись 3D-вектора (12 байт)                 |
| **pseBufferDataSetQuat(pBufferData, offset, value)**      | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `PseQuat value`             | Запись кватерниона вращения (16 байт)       |
| **pseBufferDataSetTransform(pBufferData, offset, value)** | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `const PseTransform* value` | Запись трансформации по указателю (40 байт) |
| **pseBufferDataSetColor(pBufferData, offset, value)**     | `void`                     | `PseBufferData* pBufferData`, `int32_t offset`, `PseColor value`            | Запись структуры цвета RGBA (4 байта)       |

## PseBufferData / Getters

| Сигнатура                                          | Тип возвращаемого значения | Аргументы                                            | Описание                        |
|----------------------------------------------------|----------------------------|------------------------------------------------------|---------------------------------|
| **pseBufferDataGetInt8(pBufferData, offset)**      | `int8_t`                   | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение 8-битного целого числа   |
| **pseBufferDataGetInt16(pBufferData, offset)**     | `int16_t`                  | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение 16-битного целого числа  |
| **pseBufferDataGetInt32(pBufferData, offset)**     | `int32_t`                  | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение 32-битного целого числа  |
| **pseBufferDataGetInt64(pBufferData, offset)**     | `int64_t`                  | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение 64-битного целого числа  |
| **pseBufferDataGetFloat(pBufferData, offset)**     | `float`                    | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение числа с плавающей точкой |
| **pseBufferDataGetGUID(pBufferData, offset)**      | `PseGUID`                  | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение GUID (8 байт)            |
| **pseBufferDataGetVector2(pBufferData, offset)**   | `PseVector2`               | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение 2D-вектора               |
| **pseBufferDataGetVector3(pBufferData, offset)**   | `PseVector3`               | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение 3D-вектора               |
| **pseBufferDataGetQuat(pBufferData, offset)**      | `PseQuat`                  | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение кватерниона              |
| **pseBufferDataGetTransform(pBufferData, offset)** | `PseTransform`             | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение трансформации            |
| **pseBufferDataGetColor(pBufferData, offset)**     | `PseColor`                 | `const PseBufferData* pBufferData`, `int32_t offset` | Чтение цвета RGBA               |

## Buffers / Basic

| Сигнатура                        | Тип возвращаемого значения | Аргументы | Описание                                                       |
|----------------------------------|----------------------------|-----------|----------------------------------------------------------------|
| **pseInitializeBuffers()**       | `int32_t`                  | **-**     | Инициализирует и аллоцирует **Shared Memory**                  |
| **pseDeinitializeBuffers()**     | `void`                     | **-**     | Освобождает дескрипторы и закрывает сегмент разделяемой памяти |

## Buffers / Address Getters

| Сигнатура                        | Тип возвращаемого значения | Аргументы | Описание                                                                |
|----------------------------------|----------------------------|-----------|-------------------------------------------------------------------------|
| **pseGetBuffersAddress()**       | `PseBuffers*`              | **-**     | Возвращает указатель на корень структуры **PseBuffers**                 |
| **pseGetCommandBufferAddress()** | `PseCommandBuffer*`        | **-**     | Возвращает прямой указатель на **командный буфер**                      |
| **pseGetEventBufferAddress()**   | `PseEventBuffer*`          | **-**     | Возвращает прямой указатель на **событийный буфер**                     |
| **pseGetSharedMemoryHandle()**   | `void*`                    | **-**     | Возвращает системный дескриптор (**Handle**) созданного сегмента **ОС** |

## Command Buffer / Methods

| Сигнатура                         | Тип возвращаемого значения | Аргументы | Описание                                                                        |
|-----------------------------------|----------------------------|-----------|---------------------------------------------------------------------------------|
| **pseCommandBufferSynchronize()** | `void`                     | **-**     | Блокирует поток (**Spin-wait**), пока игра не выполнит все отправленные команды |

| Сигнатура                                             | Тип возвращаемого значения | Аргументы                                         | Описание                                                                                                 |
|-------------------------------------------------------|----------------------------|---------------------------------------------------|----------------------------------------------------------------------------------------------------------|
| **pseCommandBufferSinglePush(pBufferData)**           | `uint32_t`                 | `const PseBufferData* pBufferData`                | Запись одной команды. Возвращает абсолютный индекс слота                                                 |
| **pseCommandBufferSingleWait(index, pOutBufferData)** | `void`                     | `uint32_t index`, `PseBufferData* pOutBufferData` | Ожидает завершения на индексе и копирует результат в **pOutBufferData**                                  |
| **pseCommandBufferSinglePushAndWait(pBufferData)**    | `void`                     | `const PseBufferData* pBufferData`                | Синхронный вызов: отправляет команду и блокирует поток, а результат записывает обратно в **pBufferData** |

| Сигнатура                                                | Тип возвращаемого значения | Аргументы                                                              | Описание                                                                                  |
|----------------------------------------------------------|----------------------------|------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|
| **pseCommandBufferBatchPush(pBufferData, count)**        | `uint32_t`                 | `const PseBufferData* pBufferData`, `uint32_t count`                   | Копирует пакет из count-количество команд в кольцо. Возвращает стартовый индекс           |
| **pseCommandBufferBatchWait(index, pBufferData, count)** | `void`                     | `uint32_t index`, `const PseBufferData* pBufferData`, `uint32_t count` | Блокирует поток до выполнения пакета игрой и заменяет массив **pBufferData** результатами |
| **pseCommandBufferBatchPushAndWait(pBufferData, count)** | `void`                     | `const PseBufferData* pBufferData`, `uint32_t count`                   | Синхронно отправляет пакет и ожидает перезаписи pBufferData выходными результатами игры   |

| Сигнатура                                               | Тип возвращаемого значения | Аргументы                               | Описание                                                                                    |
|---------------------------------------------------------|----------------------------|-----------------------------------------|---------------------------------------------------------------------------------------------|
| **pseCommandBufferRawReserve(count)**                   | `uint32_t`                 | `uint32_t count`                        | Резервирует count-количество слотов под Zero-Copy запись. Возвращает стартовый индекс       |
| **pseCommandBufferRawCommit(startIndex, count)**        | `void`                     | `uint32_t startIndex`, `uint32_t count` | Публикует (передает игре) зарезервированные in-place команды                                |
| **pseCommandBufferRawWait(index, count)**               | `void`                     | `uint32_t index`, `uint32_t count`      | Ожидает, пока игра отработает count-количество зарезервированных команд и вернет результаты |
| **pseCommandBufferRawCommitAndWait(startIndex, count)** | `void`                     | `uint32_t startIndex`, `uint32_t count` | Записывает RAW-команды и сразу переходит в режим ожидания ответов игры в этих слотах        |

## Event Buffer / Methods

| Сигнатура                                | Тип возвращаемого значения | Аргументы                       | Описание                                                                                                                                   |
|------------------------------------------|----------------------------|---------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------|
| **pseEventBufferTryGet(pOutBufferData)** | `int32_t`                  | `PseBufferData* pOutBufferData` | Неблокирующая попытка забрать событие. Если событие есть, копирует его в **pOutBufferData** и возвращает 1 (**true**), иначе 0 (**false**) |

## Архитектурные особенности

### Коды возврата `pseInitializeBuffers()`

| Значение | Статус              | Причина                                      | Действия системы                                                                                                                 |
|:--------:|---------------------|----------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------|
|  **0**   | **Успех**           | Подключение выполнено штатно                 | Память спроецирована, вызвана **C++** инициализация, **SDK** готов к работе                                                      |
|  **-1**  | **Ошибка открытия** | Не удалось открыть объект File Mapping       | `OpenFileMappingA` вернул `NULL`. Скорее всего, хост-процесс (игра) ещё не запущен или не создал сегмент                         |
|  **-2**  | **Ошибка маппинга** | Не удалось спроецировать представление файла | `MapViewOfFile` вернул `NULL`. Выделенный `HANDLE` автоматически закрывается (`CloseHandle`), указатель сбрасывается в `nullptr` |
