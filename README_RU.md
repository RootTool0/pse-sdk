# Portal Solver Editor SDK

**[[EN]](README.md)** | **[RU]**

---

**Portal Solver Editor (PSE) SDK** - Официальный набор инструментов _(SDK)_ для программной работы с внутренним редактором в игре **Portal: Solver**

Библиотека предоставляет программный интерфейс _(C API)_ для двустороннего обмена данными в реальном времени, позволяя создавать **внешние редакторы уровней**, **инструменты автоматизации**, **плагины**, **необычные кастомные уровни** и **утилиты**, работающие синхронно с запущенной игрой в реальном времени

---

## Биндинги

Архитектура ядра _(C API)_ позволяет перенести **PSE SDK** на любую платформу и язык

Ниже представлены список существующих решений 

_(все они лично проверены мной - правильно используют внутреннюю архитектуру проекта, и рекомендуются к использованию)_

- **Python:** **[pse-sdk-python](https://github.com/RootTool0/pse-sdk-python)** by [RootTool](https://github.com/RootTool0)

---

## Структура проекта

- `include/` - Заголовочные файлы интерфейса _(pse.h, enums.h, structures.h, pse.hpp)_
- `src/` - Исходный код реализации **PSE SDK**
- `examples/` - Компилируемые примеры использования различных режимов **PSE SDK**
- `docs/` - Техническая и концептуальная документация проекта

---

## Документация

Подробное описание архитектуры, спецификация элементов и руководства по работе находятся в разделе **[Документация](docs/ru/pse_sdk.md)**

---

## Пример кода

Базовый сценарий инициализации сессии и отправки команды на создание динамического меша с блокированием потока до получения ответа от игры

```cpp
#include "pse.h
#include "pse.hpp"

int main() {
    // Выделение и инициализация буферов
    if (pseInitializeBuffers() != 0) {
        return -1;
    }
    
    // Создаем команду
    PseBufferData bufferData{};
    bufferData.header = PSE_COMMAND_DYNAMIC_MESH_CREATE; // Команда - создать динамический меш
    
    // Упаковка параметров по фиксированным байтовым смещениям
    PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{0, 0, 0}, PseVector3{1, 1, 1}});
    PseUtilities::BufferData::set<40, PseMesh>(bufferData, PSE_MESH_FACE);
    PseUtilities::BufferData::set<42, PseMaterial>(bufferData, PSE_MATERIAL_FLOOR_WHITE);
    PseUtilities::BufferData::set<44, int8_t>(bufferData, 1); // Visibility
    
    // Отправка пакета в кольцевой буфер и ожидание результата выполнения
    pseCommandBufferSinglePushAndWait(&bufferData);
    
    // Освобождение ресурсов разделяемой памяти
    pseDeinitializeBuffers();
    return 0;
}
```

---

## Сборка из исходников _(Получение DLL / Библиотеки)_

Внутренняя часть **SDK** реализована на **C++17**, а наружу предоставляется чистый **C API интерфейс** для обеспечения **максимальной совместимости**

Для сборки динамической библиотеки _(`.dll` / `.so`)_ и заголовочных файлов в единую папку дистрибутива `dist/`, выполните следующие команды в терминале из корня репозитория:
```bash
# 1. Генерируем файлы сборки и задаем папку для вывода в папку dist
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../dist"

# 2. Компилируем проект
cmake --build build --config Release

# 3. Собираем готовый дистрибутив (библиотека и инклюды улетят в папку dist)
cmake --install build --config Release
```

После выполнения этих команд в корне проекта появится папка `dist/`, готовая к интеграции в **ваши проекты** или **биндинги**

---

## Ссылки и обратная связь

- Страница **Portal: Solver** на **GameJolt**: [GameJolt](https://gamejolt.com/games/PortalSolver/898591)
- Скачать готовые релизы: [Releases](https://github.com/RootTool0/pse-sdk/releases)
- Сообщить об ошибке _(Bug Report)_: [Issues](https://github.com/RootTool0/pse-sdk/issues)
- Связь с **RootTool**:
  - **Telegram (канал/блог)**: [RootTool Blog](https://t.me/RootToolBlog)
  - **Telegram (ЛС)**: [@RootTool0](https://t.me/RootTool0)
  - **Discord**: `.roottool` (или `RootTool#5776`)
  - **YouTube**: [RootTool](https://www.youtube.com/@RTEdits0)
