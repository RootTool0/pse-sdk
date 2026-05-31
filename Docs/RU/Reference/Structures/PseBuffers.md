<img src="../../../Assets/PortalSolverLogo.png" width="96m" align="left" hspace="15" alt="Portal Solver Logo">

## [Portal Solver Editor SDK](../../PortalSolverEditorSDK.md) > [Reference](../Index.md) > [Structures](Index.md) > [PseBuffers]()

<br>

Для организации единого сегмента разделяемой памяти (**Shared Memory**) оба кольцевых буфера упакованы в монолитную структуру `PseBuffers`

Упаковка структуры гарантирует жесткое фиксирование адресов в памяти без скрытых вставок (**padding**) со стороны компилятора

## Поля

| Поле                | Тип                | Смещение<br>_(в байтах)_ |  Размер<br>_(в байтах)_   | Описание / Документация                                                                     |
|---------------------|--------------------|:------------------------:|:-------------------------:|---------------------------------------------------------------------------------------------|
| **`commandBuffer`** | `PseCommandBuffer` |           `0`            | **131 140**<br>_(128 КБ)_ | Конвейер команд от SDK к игре. Подробнее в [PseCommandBuffer.md](PseCommandBuffer)          |
| **`eventBuffer`**   | `PseEventBuffer`   |        `131 140`         |  **16 388**<br>_(16 КБ)_  | Конвейер асинхронных событий от игры к SDK. Подробнее в [PseEventBuffer.md](PseEventBuffer) |

## Константы

```c
#define PSE_COMMAND_BUFFER_SIZE 2048
#define PSE_EVENT_BUFFER_SIZE 256
```

Размеры буферов строго являются **степенью двойки**, что позволяет использовать быструю битовую маску вместо медленной операции деления (`%`) на процессоре

## Связанная документация

Подробное описание внутреннего устройства каждого компонента, логики работы **Lock-free** алгоритмов, **C++ методов** и **базовых типов данных** находится в соответствующих файлах спецификации:
- Базовые типы данных: [Common](Common.md)
- Контейнер данных _(PseBufferData)_: [PseBufferData](PseBufferData.md)
- Командный буфер _(PseCommandBuffer)_: [PseCommandBuffer](PseCommandBuffer.md)
- Событийный буфер _(PseEventBuffer)_: [PseEventBuffer](PseEventBuffer.md)
